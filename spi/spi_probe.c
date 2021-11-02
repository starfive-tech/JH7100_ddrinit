/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  spi_probe.c
  * @author  StarFive Technology
  * @version  V1.0
  * @date  07/24/2020
  * @brief
  ******************************************************************************
  * @copy
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STARFIVE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  *  COPYRIGHT 2020 Shanghai StarFive Technology Co., Ltd.
  */

#include <comdef.h>
#include <spi_flash.h>
#include <spi_flash_internal.h>
#include <spi.h>
#include <uart.h>

#define IDCODE_CONT_LEN 0
#define IDCODE_PART_LEN 3
#define IDCODE_LEN (IDCODE_CONT_LEN + IDCODE_PART_LEN)

#define CMD_READ_ID			0x9f

#define NOR 			0
#define GIGANAND 		1

//#define CONFIG_SPI_FLASH_ATMEL
//#define CONFIG_SPI_FLASH_GIGADEVICE
//#define CONFIG_SPI_FLASH_EON
//#define CONFIG_SPI_FLASH_MACRONIX
//#define CONFIG_SPI_FLASH_SPANSION
//#define CONFIG_SPI_FLASH_STMICRO
//#define CONFIG_SPI_FLASH_SST
//#define CONFIG_SPI_FLASH_WINBOND
//#define TEST_GD25Q64B
//#define TEST_GD25Q64C
//#define TEST_GD25LB64C

static struct spi_flash g_spi_flash[1];

static const struct spi_flash_params spi_flash_table[] =
{
#ifdef CONFIG_SPI_FLASH_ATMEL		/* ATMEL */
	{"AT45DB011D",		0x00221f,8,16,16,4,NOR},
	{"AT45DB021D",		0x00231f,8,16,16,8,NOR},
	{"AT45DB041D",		0x00241f,8,16,16,8,NOR},
	{"AT45DB081D",		0x00251f,8,16,16,16,NOR},
	{"AT45DB161D",		0x00261f,8,16,16,32,NOR},
	{"AT45DB321D",		0x00271f,8,16,16,64,NOR},
	{"AT45DB641D",		0x00281f,8,16,16,128,NOR},
	{"AT25DF161", 		0x01461f,8,16,16,32,NOR},
	{"AT25DF321", 		0x01471f,8,16,16,64,NOR},
#endif
#ifdef CONFIG_SPI_FLASH_EON		/* EON */
	{"EN25Q64",	 		0x17301c,8,16,16,128,NOR},
#endif
#ifdef CONFIG_SPI_FLASH_GIGADEVICE	/* GIGADEVICE */
#ifdef TEST_GD25Q64B
	{"GD25Q64B",		0x1740c8,8,16,16,128,NOR},
#endif
#ifdef TEST_GD25Q64C
	{"GD25Q64C",		0x1740c8,8,16,16,128,NOR},
	{"GD25Q64C---",		0x1740D8,8,16,16,128,NOR},
#endif	
	{"GD25LQ32",		0x1660c8,8,16,16,64,NOR},
	{"GD5F1GQ4UAYIG",	0xc8f1c8,11,64,0,1024,GIGANAND},
	
#endif
#ifdef CONFIG_SPI_FLASH_MACRONIX	/* MACRONIX */
	{"MX25L2006E",	   	0x1220c2,8,16,16,4,NOR},
	{"MX25L4005",	   	0x1320c2,8,16,16,8,NOR},
	{"MX25L8005",	   	0x1420c2,8,16,16,16,NOR},
	{"MX25L1605D",	   	0x1520c2,8,16,16,32,NOR},
	{"MX25L3205D",	   	0x1620c2,8,16,16,64,NOR},
	{"MX25L6405D",	   	0x1720c2,8,16,16,128,NOR},
	{"MX25L12805",	   	0x1820c2,8,16,16,256,NOR},
	{"MX25L25635F",	   	0x1920c2,8,16,16,512,NOR},
	{"MX25L51235F",	   	0x1a20c2,8,16,16,1024,NOR},
	{"MX25L12855E",	   	0x1826c2,8,16,16,256,NOR},
#endif
#ifdef CONFIG_SPI_FLASH_SPANSION	/* SPANSION */
	{"S25FL008A",	   	0x130201, 8,16,16,    16,NOR},
	{"S25FL016A",	   	0x140201, 8,16,16,    32,NOR},
	{"S25FL032A",	   	0x150201, 8,16,16,    64,NOR},
	{"S25FL064A",	   	0x160201, 8,16,16,   128,NOR},
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO		/* STMICRO */
	{"M25P10",			0x112020, 8,16,8,	  4,NOR},
	{"M25P20",	   		0x122020, 8,16,16,     4,NOR},
	{"M25P40",	   		0x132020, 8,16,16,     8,NOR},
	{"M25P80",	  		0x142020, 8,16,16,    16,NOR},
	{"M25P16",	   		0x152020, 8,16,16,    32,NOR},
	{"M25P32",	   		0x162020, 8,16,16,    64,NOR},
	{"M25P64",	   		0x172020, 8,16,16,   128,NOR},
	{"M25P128",	   		0x182020, 8,16,32,     64,NOR},
	{"N25Q32",	   		0x16ba20, 8,16,16,    64,NOR},
	{"N25Q32A",	   		0x16bb20, 8,16,16,    64,NOR},
	{"N25Q64",	   		0x17ba20, 8,16,16,   128,NOR},
	{"N25Q64A",	   		0x17bb20, 8,16,16,   128,NOR},
	{"N25Q128",	  		0x18ba20, 8,16,16,   256,NOR},
	{"N25Q128A",   		0x18bb20, 8,16,16,   256,NOR},
	{"N25Q256",	   		0x19ba20, 8,16,16,   512,NOR},
	{"N25Q256A",   		0x19bb20, 8,16,16,   512,NOR},
	{"N25Q512",	   		0x20ba20, 8,16,16,  1024,NOR},
	{"N25Q512A",   		0x20bb20, 8,16,16,  1024,NOR},
	{"N25Q1024",   		0x21ba20, 8,16,16,  2048,NOR},
	{"N25Q1024A",		0x21bb20, 8,16,16,2048,NOR},
#endif
#ifdef CONFIG_SPI_FLASH_SST		/* SST */
	{"SST25VF040B",	   	0x8d25bf, 8,16,16,     8,NOR},
	{"SST25VF080B",	   	0x8e25bf, 8,16,16,    16,NOR},
	{"SST25VF016B",	   	0x4125bf, 8,16,16,    32,NOR},
	{"SST25VF032B",	  	0x4a25bf, 8,16,16,    64,NOR},
	{"SST25VF064C",	   	0x4b25bf, 8,16,16,   128,NOR},
	{"SST25WF512",	   	0x0125bf, 8,16,16,     1,NOR},
	{"SST25WF010",	   	0x0225bf, 8,16,16,     2,NOR},
	{"SST25WF020",	   	0x0325bf, 8,16,16,     4,NOR},
	{"SST25WF040",	   	0x0425bf, 8,16,16,     8,NOR},
	{"SST25WF080",	   	0x0525bf, 8,16,16,    16,NOR},
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND		/* WINBOND */
	{"W25P80",	   		0x1420ef, 8,16,16,    16,NOR},
	{"W25P16",	   		0x1520ef, 8,16,16,    32,NOR},
	{"W25P32",	   		0x1620ef, 8,16,16,    64,NOR},
	{"W25X40",	   		0x1330ef, 8,16,16,     8,NOR},
	{"W25X16",	   		0x1530ef, 8,16,16,    32,NOR},
	{"W25X32",	   		0x1630ef, 8,16,16,    64,NOR},
	{"W25X64",	   		0x1730ef, 8,16,16,   128,NOR},
	{"W25Q80BL",   		0x1440ef, 8,16,16,    16,NOR},
	{"W25Q16CL",   		0x1540ef, 8,16,16,    32,NOR},
	{"W25Q32BV",   		0x1640ef, 8,16,16,    64,NOR},
	{"W25Q64CV",   		0x1740ef, 8,16,16,   128,NOR},
	{"W25Q128BV",  		0x1840ef, 8,16,16,   256,NOR},
	{"W25Q256",	   		0x1940ef, 8,16,16,   512,NOR},
	{"W25Q80BW",   		0x1450ef, 8,16,16,    16,NOR},
	{"W25Q16DW",   		0x1560ef, 8,16,16,    32,NOR},
	{"W25Q32DW",   		0x1660ef, 8,16,16,    64,NOR},
	{"W25Q64DW",   		0x1760ef, 8,16,16,   128,NOR},
	{"W25Q128FW",  		0x1860ef, 8,16,16,   256,NOR},
	{"W25Q256FW",  		0x1940ef, 8,16,16,   256,NOR},
	
#endif
	//{"EN25QH16",		0x15701c,   8,16,16,32,  NOR},
	//{"IS25WP256D",		0x19609d,   8,16,16,256, NOR},
	//{"MT25QL256ABA",    0x19ba20,   8,16,16,256, NOR},
	 //{"GD25LB64C",	   	0x1760C8,   8,16,16,256, NOR},
	{"S25FL512S_512K", 0x200201, 8,16,64,256, NOR},
	{"GD25LQ256D", 0x1960C8, 8,16,16,512, NOR},
	{"GD25LQ128", 0x1960C8, 8,16,16,256, NOR},
	//{"GD25LB64B",	   	0xC86017,   8,16,16,256, NOR},
	//{"NORMAL",			0x000000,	8,16,16,	1,	NOR},

	
};

struct spi_flash *spi_flash_probe_nor(struct spi_slave *spi, u8 *idcode)
{
	struct spi_flash_params *params;
	struct spi_flash *flash;
	u32 id = 0;
	static int i = 0;

	id = ((idcode[2] << 16) | (idcode[1] << 8) | idcode[0]);
    if(id == 0x0)
    {
        return NULL;
    }
	params = spi_flash_table;
	for (i = 0; spi_flash_table[i].name != NULL; i++)
	{
		if ((spi_flash_table[i].id & 0xFFFFFF) == id)
		{
			break;
		}
	}

	flash = &g_spi_flash[spi->bus];
	if (!flash)
	{
		//uart_printf("SF: Failed to allocate memory\r\n");
		return NULL;
	}

	flash->name = spi_flash_table[i].name;
	if(spi_flash_table[i].flags == NOR)
	{
		/* Assuming power-of-two page size initially. */
		flash->write = spi_flash_cmd_write_mode;
		flash->erase = spi_flash_erase_mode;
		flash->read = spi_flash_read_mode;
		flash->page_size = 1 << spi_flash_table[i].l2_page_size;
		flash->sector_size = flash->page_size * spi_flash_table[i].pages_per_sector;
		flash->block_size = flash->sector_size * spi_flash_table[i].sectors_per_block;
		flash->size = flash->page_size * spi_flash_table[i].pages_per_sector
						* spi_flash_table[i].sectors_per_block
						* spi_flash_table[i].nr_blocks;
	}

	//uart_printf("spi probe complete\r\n");

	return flash;
}
#if 0
static int print_id(u8 *idcode, int len)
{
	int i;

	printk("idcode:0x");
	for (i=len-1; i>=0; i--)
		print_ubyte_hex(idcode[i]);
	printk("\r\n");
	return 0;
}
#endif
static int spi_read_id(struct spi_slave *spi, unsigned char cmd, void *response, u32 len)
{
	int ret1 = -1,ret2 = -1;
	unsigned char buf[4] = {0};// = {(u8)cmd, 0x00, 0x00, 0x00};

	buf[0] = cmd;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;

	ret1 = spi_xfer(spi, 1*8, &buf[0], NULL, SPI_XFER_BEGIN, 8);
	ret2 = spi_xfer(spi, len*8, NULL, response, SPI_XFER_END, 8);
    return 0;
}

static struct spi_flash aic_flash;

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode, unsigned int bus_width)
{
	struct spi_slave *spi;
	struct spi_flash *flash = &aic_flash;
	int ret = 0;
	u8 idcode[IDCODE_LEN];

	spi = spi_setup_slave(bus, cs, max_hz, mode, bus_width);
	if (!spi) {
		//uart_printf("SF: Failed to set up slave\n");
		return NULL;
	}

	/* Read the ID codes */
 	ret = spi_read_id(spi, CMD_READ_ID, idcode, sizeof(idcode));
	if (ret)
	{
		//uart_printf("SF: Failed to read ID : %d\n", ret);
		goto err_read_id;
	}

//	print_id(idcode, sizeof(idcode));

	flash = spi_flash_probe_nor(spi,idcode);
	if (!flash)
	{
		goto err_manufacturer_probe;
	}

	flash->spi = spi;
	return flash;

err_manufacturer_probe:
err_read_id:

	return NULL;
}
