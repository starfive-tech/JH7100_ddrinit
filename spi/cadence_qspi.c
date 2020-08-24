/*
 * Copyright (C) 2012
 * Altera Corporation <www.altera.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <comdef.h>
#include <spi.h>
#include <sys.h>
#include <cadence_qspi.h>
#include <ezGPIO_fullMux_ctrl_macro.h>
#include <clkgen_ctrl_macro.h>
#include <rstgen_ctrl_macro.h>

struct cadence_spi_slave
{
	struct spi_slave slave;
	u32 base;
	int irq;
};

struct spi_operation cadence_spi4x_func;
struct cadence_spi_platdata cadence_plat;
struct cadence_spi_priv spi_priv;
struct cadence_spi_slave vic_spi_slave;

#define CQSPI_STIG_READ			0
#define CQSPI_STIG_WRITE		1
#define CQSPI_INDIRECT_READ		2
#define CQSPI_INDIRECT_WRITE	3
#define CADENCE_QSPI_MAX_HZ		QSPI_CLK
#define CONFIG_CQSPI_REF_CLK	QSPI_CLK
#define CONFIG_CQSPI_DECODER	0
static int cadence_spi_write_speed(unsigned int hz)
{
	struct cadence_spi_platdata *plat = &cadence_plat;
	struct cadence_spi_priv *priv = &spi_priv;

	cadence_qspi_apb_config_baudrate_div(priv->regbase,
					     CONFIG_CQSPI_REF_CLK, hz);

	/* Reconfigure delay timing if speed is changed. */
	cadence_qspi_apb_delay(priv->regbase, CONFIG_CQSPI_REF_CLK, hz,
			       plat->tshsl_ns, plat->tsd2d_ns,
			       plat->tchsh_ns, plat->tslch_ns);

	return 0;
}

/* Calibration sequence to determine the read data capture delay register */
static int spi_calibration(unsigned int hz, unsigned int cs)
{
	struct cadence_spi_priv *priv = &spi_priv;
	void * base = priv->regbase;
	u8 opcode_rdid = 0x9F;
	unsigned int idcode = 0, temp = 0;
	int err = 0, i, range_lo = -1, range_hi = -1;

	/* start with slowest clock (1 MHz) */
	cadence_spi_write_speed(500000);

	/* configure the read data capture delay register to 0 */
	cadence_qspi_apb_readdata_capture(base, 1, 0);
		/* Enable QSPI */
	cadence_qspi_apb_controller_enable(base);

	/* read the ID which will be our golden value */
	err = cadence_qspi_apb_command_read(base, 1, &opcode_rdid,
		3, (u8 *)&idcode);
	if (err) {
		//uart_printf("SF: Calibration failed (read)\n");
		return err;
	}

	/* use back the intended clock and find low range */
	cadence_spi_write_speed(hz);
	for (i = 0; i < CQSPI_READ_CAPTURE_MAX_DELAY; i++) {
				/* Disable QSPI */
		cadence_qspi_apb_controller_disable(base);

		/* reconfigure the read data capture delay register */
		cadence_qspi_apb_readdata_capture(base, 1, i);
			/* Enable back QSPI */
		cadence_qspi_apb_controller_enable(base);

		/* issue a RDID to get the ID value */
		err = cadence_qspi_apb_command_read(base, 1, &opcode_rdid,
			3, (u8 *)&temp);
		if (err) {
			//uart_printf("SF: Calibration failed (read)\n");
			return err;
		}

		/* search for range lo */
		if (range_lo == -1 && temp == idcode) {
			range_lo = i;
			continue;
		}

		/* search for range hi */
		if (range_lo != -1 && temp != idcode) {
			range_hi = i - 1;
			break;
		}
		range_hi = i;
	}

	if (range_lo == -1) {
		//uart_printf("SF: Calibration failed (low range)\n");
		return err;
	}
		/* Disable QSPI for subsequent initialization */
	cadence_qspi_apb_controller_disable(base);

	/* configure the final value for read data capture delay register */
	cadence_qspi_apb_readdata_capture(base, 1, (range_hi + range_lo) / 2);
	//uart_printf("SF: Read data capture delay calibrated to %i (%i - %i)\n",
	      //(range_hi + range_lo) / 2, range_lo, range_hi);

	/* just to ensure we do once only when speed or chip select change */
	priv->qspi_calibrated_hz = hz;
	priv->qspi_calibrated_cs = cs;

	return 0;
}

struct spi_slave *cadence_spi4x_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, u32 mode, u32 fifo_width)
{

	struct cadence_spi_slave *spi4slave;
	struct cadence_spi_platdata *plat = &cadence_plat;
	struct cadence_spi_priv *priv = &spi_priv;
	u32 base = 0;
	int err = 0;
	u32 clk_pol;
	u32 clk_pha;

	spi4slave = &vic_spi_slave;
	
	spi4slave->base = (void *)QSPI_BASE_ADDR;
	clk_pol = (mode & SPI_CPOL) ? 1 : 0;
	clk_pha = (mode & SPI_CPHA) ? 1 : 0;


	plat->regbase = (void *)QSPI_BASE_ADDR;
	plat->ahbbase = (void *)QSPI_BASE_AHB_ADDR;
	plat->max_hz = CADENCE_QSPI_MAX_HZ;


/****default set, may change******/
	plat->tshsl_ns =  200;
	plat->tsd2d_ns =  255;
	plat->tchsh_ns =  2;
	plat->tslch_ns =  20;
	plat->sram_size = 256;

	plat->block_size = 16;
	plat->page_size = 256;

	priv->regbase = plat->regbase;
	priv->ahbbase = plat->ahbbase;

	/* Disable QSPI */
	cadence_qspi_apb_controller_disable(priv->regbase);

	/* Set SPI mode */
	cadence_qspi_apb_set_clk_mode(priv->regbase, clk_pol, clk_pha);
#if 0
	if (!priv->qspi_is_init) {
		cadence_qspi_apb_controller_init(plat);
		priv->qspi_is_init = 1;
	}
#endif

	cadence_qspi_apb_controller_init(plat);

	if (max_hz > plat->max_hz)
		max_hz = plat->max_hz;

	/*
	 * Calibration required for different current SCLK speed, requested
	 * SCLK speed or chip select
	 */
	if (priv->previous_hz != max_hz ||
	    priv->qspi_calibrated_hz != max_hz ||
	    priv->qspi_calibrated_cs != cs) {
		err = spi_calibration(max_hz, cs);
		if (err)
			return NULL;

		/* prevent calibration run when same as previous request */
		priv->previous_hz = max_hz;
	}

	/* Enable QSPI */
	cadence_qspi_apb_controller_enable(priv->regbase);

	spi4slave->slave.bus = bus;
	spi4slave->slave.cs = cs;
	spi4slave->slave.bus_width= fifo_width;

	return &spi4slave->slave;

	
}


static int cadence_spi_xfer(struct spi_slave *slave, unsigned int bitlen,
			    const void *dout, void *din, unsigned long flags)
{
	struct cadence_spi_platdata *plat = &cadence_plat;
	struct cadence_spi_priv *priv = &spi_priv;
	void * base = priv->regbase;
	u8 *cmd_buf = priv->cmd_buf;
	unsigned int data_bytes = 0;
	int err = 0;
	u32 mode = CQSPI_STIG_WRITE;

	if (flags & SPI_XFER_BEGIN) {
		/* copy command to local buffer */
		priv->cmd_len = bitlen / 8;
		sys_memcpy(cmd_buf, dout, priv->cmd_len);
	}

	if (flags == (SPI_XFER_BEGIN | SPI_XFER_END)) 
	{
		/* if start and end bit are set, the data bytes is 0. */
		data_bytes = 0;
	} 
	else 
	{
		data_bytes = bitlen / 8;
	}
	//uart_printf("%s: len=%d [bytes]\n", __func__, data_bytes);

	/* Set Chip select */
	cadence_qspi_apb_chipselect(base, slave->cs,
				    CONFIG_CQSPI_DECODER);

	if ((flags & SPI_XFER_END) || (flags == 0)) {
		if (priv->cmd_len == 0) {
			//uart_printf("QSPI: Error, command is empty.\n");
			return -1;
		}

		if (din && data_bytes) {
			/* read */
			/* Use STIG if no address. */
			if (!CQSPI_IS_ADDR(priv->cmd_len))
				mode = CQSPI_STIG_READ;
			else
				mode = CQSPI_INDIRECT_READ;
		} else if (dout && !(flags & SPI_XFER_BEGIN)) {
			/* write */
			if (!CQSPI_IS_ADDR(priv->cmd_len))
				mode = CQSPI_STIG_WRITE;
			else
				mode = CQSPI_INDIRECT_WRITE;
		}
		switch (mode) {
		case CQSPI_STIG_READ:
			err = cadence_qspi_apb_command_read(
				base, priv->cmd_len, cmd_buf,
				data_bytes, din);

		break;
		case CQSPI_STIG_WRITE:
			err = cadence_qspi_apb_command_write(base,
				priv->cmd_len, cmd_buf,
				data_bytes, dout);
		break;
		case CQSPI_INDIRECT_READ:
			err = cadence_qspi_apb_indirect_read_setup(plat,
				priv->cmd_len, cmd_buf);
			if (!err) {
				err = cadence_qspi_apb_indirect_read_execute
				(plat, data_bytes, din);
			}
		break;
		case CQSPI_INDIRECT_WRITE:
			err = cadence_qspi_apb_indirect_write_setup
				(plat, priv->cmd_len, cmd_buf);
			if (!err) {
				err = cadence_qspi_apb_indirect_write_execute
				(plat, data_bytes, dout);
			}
		break;
		default:
			err = -1;
			break;
		}

		if (flags & SPI_XFER_END) {
			/* clear command buffer */
			sys_memset(cmd_buf, 0, sizeof(priv->cmd_buf));
			priv->cmd_len = 0;
		}
	}

	return err;
}

void cadence_qspi_init(unsigned int bus, u32 mode)
{
	struct spi_operation *func;
    struct cadence_spi_platdata *plat = &cadence_plat;

	/******************* reset ****************/
	_ENABLE_CLOCK_clk_qspi_refclk_;
    _ENABLE_CLOCK_clk_qspi_apb_;
    _ENABLE_CLOCK_clk_qspi_ahb_;
    _ASSERT_RESET_rstgen_rstn_qspi_ahb_;
    _ASSERT_RESET_rstgen_rstn_qspi_core_;
    _ASSERT_RESET_rstgen_rstn_qspi_apb_;
    _CLEAR_RESET_rstgen_rstn_qspi_ahb_;
    _CLEAR_RESET_rstgen_rstn_qspi_core_;
    _CLEAR_RESET_rstgen_rstn_qspi_apb_;
	plat->bit_mode = mode;
	
	func = &cadence_spi4x_func;
	func->setup_slave = cadence_spi4x_setup_slave;
	func->spi_xfer = cadence_spi_xfer;

	spi_register(bus, func);

	return;
}