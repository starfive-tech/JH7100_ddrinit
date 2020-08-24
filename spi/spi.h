/*
 * Common SPI Interface: Controller-specific definitions
 *
 * (C) Copyright 2001
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SPI_H_
#define _SPI_H_

#include <comdef.h>

#define SPI_DATEMODE_32		32
#define SPI_DATAMODE_16		16
#define	SPI_DATAMODE_8		8

/* SPI mode flags */
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* CS active high */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */
#define	SPI_LOOP	0x20			/* loopback mode */
#define	SPI_SLAVE	0x40			/* slave mode */
#define	SPI_PREAMBLE	0x80			/* Skip preamble bytes */

/* SPI transfer flags */
#define SPI_XFER_BEGIN		0x01	/* Assert CS before transfer */
#define SPI_XFER_END		0x02	/* Deassert CS after transfer */
#define SPI_XFER_MMAP		0x08	/* Memory Mapped start */
#define SPI_XFER_MMAP_END	0x10	/* Memory Mapped End */
#define SPI_XFER_ONCE		(SPI_XFER_BEGIN | SPI_XFER_END)
#define SPI_XFER_U_PAGE		(1 << 5)

/* SPI TX operation modes */
#define SPI_OPM_TX_QPP		1 << 0

/* SPI RX operation modes */
#define SPI_OPM_RX_AS		1 << 0
#define SPI_OPM_RX_DOUT		1 << 1
#define SPI_OPM_RX_DIO		1 << 2
#define SPI_OPM_RX_QOF		1 << 3
#define SPI_OPM_RX_QIOF		1 << 4
#define SPI_OPM_RX_EXTN		SPI_OPM_RX_AS | SPI_OPM_RX_DOUT | \
				SPI_OPM_RX_DIO | SPI_OPM_RX_QOF | \
				SPI_OPM_RX_QIOF

/* SPI bus connection options */
#define SPI_CONN_DUAL_SHARED	1 << 0
#define SPI_CONN_DUAL_SEPARATED	1 << 1

/* Header byte that marks the start of the message */
#define SPI_PREAMBLE_END_BYTE	0xec

#define SPI_DEFAULT_WORDLEN 8

/*-----------------------------------------------------------------------
 * Representation of a SPI slave, i.e. what we're communicating with.
 *
 * Drivers are expected to extend this with controller-specific data.
 *
 *   bus:	ID of the bus that the slave is attached to.
 *   cs:	ID of the chip select connected to the slave.
 */

struct spi_slave {
	unsigned int	bus;
	unsigned int	bus_width;
#define NOT_SWAP_MODE	0
#define SWAP_MODE	1
	unsigned int	bus_order;
	unsigned int	cs;
};

struct spi_operation{
	struct spi_slave* (*setup_slave)(unsigned int bus, unsigned int cs,
		unsigned int max_hz, u32 mode, u32 fifo_width);
	int  (*spi_xfer)(struct spi_slave *slave, unsigned int bitlen, void *dout,
		void *din, unsigned long flags,int bit_mode);
};


/**
 * Initialization, must be called once on start up.
 *
 */
void spi_init(void);

/**
 * spi_do_alloc_slave - Allocate a new SPI slave (internal)
 *
 * Allocate and zero all fields in the spi slave, and set the bus/chip
 * select. Use the helper macro spi_alloc_slave() to call this.
 *
 * @offset:	Offset of struct spi_slave within slave structure.
 * @size:	Size of slave structure.
 * @bus:	Bus ID of the slave chip.
 * @cs:		Chip select ID of the slave chip on the specified bus.
 */
void *spi_do_alloc_slave(int offset, int size, unsigned int bus,
			 unsigned int cs);

/**
 * spi_alloc_slave - Allocate a new SPI slave
 *
 * Allocate and zero all fields in the spi slave, and set the bus/chip
 * select.
 *
 * @_struct:	Name of structure to allocate (e.g. struct tegra_spi).
 *		This structure must contain a member 'struct spi_slave *slave'.
 * @bus:	Bus ID of the slave chip.
 * @cs:		Chip select ID of the slave chip on the specified bus.
 */
#define spi_alloc_slave(_struct, bus, cs) \
	spi_do_alloc_slave(offsetof(_struct, slave), \
			    sizeof(_struct), bus, cs)

/**
 * spi_alloc_slave_base - Allocate a new SPI slave with no private data
 *
 * Allocate and zero all fields in the spi slave, and set the bus/chip
 * select.
 *
 * @bus:	Bus ID of the slave chip.
 * @cs:		Chip select ID of the slave chip on the specified bus.
 */
#define spi_alloc_slave_base(bus, cs) \
	spi_do_alloc_slave(0, sizeof(struct spi_slave), bus, cs)

int spi_register(unsigned int bus, struct spi_operation *operation);

/**
 * Set up communications parameters for a SPI slave.
 *
 * This must be called once for each slave. Note that this function
 * usually doesn't touch any actual hardware, it only initializes the
 * contents of spi_slave so that the hardware can be easily
 * initialized later.
 *
 * @bus:	Bus ID of the slave chip.
 * @cs:		Chip select ID of the slave chip on the specified bus.
 * @max_hz:	Maximum SCK rate in Hz.
 * @mode:	Clock polarity, clock phase and other parameters.
 *
 * Returns: A spi_slave reference that can be used in subsequent SPI
 * calls, or NULL if one or more of the parameters are not supported.
 */
struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode, unsigned int bus_width);

/**
 * Free any memory associated with a SPI slave.
 *
 * @slave:	The SPI slave
 */


/**
 * SPI transfer
 *
 * This writes "bitlen" bits out the SPI MOSI port and simultaneously clocks
 * "bitlen" bits in the SPI MISO port.  That's just the way SPI works.
 *
 * The source of the outgoing bits is the "dout" parameter and the
 * destination of the input bits is the "din" parameter.  Note that "dout"
 * and "din" can point to the same memory location, in which case the
 * input data overwrites the output data (since both are buffered by
 * temporary variables, this is OK).
 *
 * spi_xfer() interface:
 * @slave:	The SPI slave which will be sending/receiving the data.
 * @bitlen:	How many bits to write and read.
 * @dout:	Pointer to a string of bits to send out.  The bits are
 *		held in a byte array and are sent MSB first.
 * @din:	Pointer to a string of bits that will be filled in.
 * @flags:	A bitwise combination of SPI_XFER_* flags.
 *
 * Returns: 0 on success, not 0 on failure
 */
int  spi_xfer(struct spi_slave *slave, unsigned int bitlen, void *dout,
		void *din, unsigned long flags, int bit_mode);


#endif	/* _SPI_H_ */
