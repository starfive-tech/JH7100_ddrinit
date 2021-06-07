/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  spi_flash.h
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

#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include <comdef.h>


//#define SPI_1LINE

struct spi_flash_params {
	const char	*name;
	u32	id;
	/* Log2 of page size in power-of-two mode */
	u8		l2_page_size;
	u16	pages_per_sector;
	u16	sectors_per_block;
	u16	nr_blocks;
	int flags;
};

struct spi_flash
{
	struct spi_slave *spi;
	//void *regs;
	const char	*name;
	u32 size; /* Total flash size */
	u32 page_size; /* Write (page) size */
	u32	sector_size; /* Erase (sector) size */
	u32	block_size; /* Erase (sector) size */
	int	(*read )(struct spi_flash *flash, u32 offset,u32 len, void *data, u32 mode);
	int	(*write)(struct spi_flash *flash, u32 offset,u32 len, void *data, u32 mode);
	int	(*erase)(struct spi_flash *flash, u32 offset,u32 len, u32 mode);
};

/*---------------------------------------------------
 * spi_flash_probe:
 * This function initialize module hardware and some software structures ,
 * setup slave and read id codes , search the table and call probe
 *
 * spi_flash_probe() interface:
 * bus: passed to the spi_setup_slave()
 * cs: idem
 * max_hz: idem
 * spi_mode: idem
 * bus_width: idem
 *
 * Returns: return NULL is error , if right return a struct contaims some information
 */
struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, u32 mode, u32 fifo_width);


static /*inline*/ int spi_flash_read(struct spi_flash *flash, u32 offset,
		u32 len, void *buf, u32 mode)
{
	return flash->read(flash, offset, len, buf, mode);
}

static /*inline*/ int spi_flash_write(struct spi_flash *flash, u32 offset,
		u32 len, void *buf, u32 mode)
{
	return flash->write(flash, offset, len, buf, mode);
}

static /*inline*/ int spi_flash_erase(struct spi_flash *flash, u32 offset,
		u32 len, u32 mode)
{
	return flash->erase(flash, offset, len, mode);
}


#endif /* _SPI_FLASH_H_ */
