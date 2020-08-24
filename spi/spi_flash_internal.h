/*
 * SPI flash internal definitions
 *
 * Copyright (C) 2008 Atmel Corporation
 */

/* Common parameters -- kind of high, but they should only occur when there
 * is a problem (and well your system already is broken), so err on the side
 * of caution in case we're dealing with slower SPI buses and/or processors.
 */
#ifndef __SPI_FLASH_INTERNAL_H_
#define __SPI_FLASH_INTERNAL_H_

#include <comdef.h>

#define CONFIG_SYS_HZ            				12000000
#define SPI_FLASH_PROG_TIMEOUT					(2 * CONFIG_SYS_HZ)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT			(5 * CONFIG_SYS_HZ)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT			(10 * CONFIG_SYS_HZ)

/* Common commands */
#define CMD_READ_ARRAY_SLOW		0x03	/* Read Data Bytes */
#define CMD_READ_ARRAY_FAST		0x0B
#define CMD_READ_ARRAY_DUAL		0x3B
#define CMD_READ_ARRAY_QUAD		0X6B
#define CMD_READ_ARRAY_LEGACY	0xEB

#define CMD_WRITE_STATUS		0x01	/* Write Status Register */

#define CMD_WRITE_STATUS_1		0x01	/* Write Status Register */
#define CMD_WRITE_STATUS_2		0x31	/* Write Status Register */
#define CMD_WRITE_STATUS_3		0x11	/* Write Status Register */
#define CMD_READ_STATUS_1       0x05	/* Read Status Register */
#define CMD_READ_STATUS_2       0x35	/* Read Status Register */
#define CMD_READ_STATUS_3       0x15	/* Read Status Register */

#define CMD_PAGE_PROGRAM		0x02	/* Page Program */
#define CMD_PAGE_PROGRAM_QUAD	0x32
#define CMD_WRITE_DISABLE		0x04	/* Write Disable */
#define CMD_READ_STATUS			0x05	/* Read Status Register */
#define CMD_READ_STATUS1		0x35
#define CMD_WRITE_ENABLE		0x06	/* Write Enable */
#define CMD_W25_FAST_READ		0x0b	/* Read Data Bytes at Higher Speed */
#define CMD_W25_SE				0x20	/* Sector (4K) Erase */
#define CMD_W25_BE				0xd8	/* Block (64K) Erase */
#define CMD_W25_CE				0xc7	/* Chip Erase */
#define CMD_W25_DP				0xb9	/* Deep Power-down */
#define CMD_W25_RES 			0xab	/* Release from DP, and Read Signature */
#define CMD_W25_BE_32			0x52	/* Sector (32K) Erase */
#define CMD_STATUS_ENABLE       0x50

/* Common status */
#define STATUS_WIP				0x01
#define FLASH_ENABLE			0x02
#define STATUS_QE				(0x01 << 1)

/* Send a single-byte command to the device and read the response */
int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, u32 len);

/*
 * Send a multi-byte command to the device and read the response. Used
 * for flash array reads, etc.
 */
int spi_flash_cmd_read(struct spi_slave *spi, u8 *cmd,u32 cmd_len,
		void *data, u32 data_len);

int spi_flash_cmd_read_fast(struct spi_flash *flash, u32 offset,
		u32 len, void *data, u32 mode);

int spi_flash_read_mode(struct spi_flash *flash, u32 offset,
		u32 len, void *data, u32 mode);

/*
 * Send a multi-byte command to the device followed by (optional)
 * data. Used for programming the flash array, etc.
 */
int spi_flash_cmd_write(struct spi_slave *spi, u8 *cmd, u32 cmd_len,
		void *data, u32 data_len);

/*
 * Write the requested data out breaking it up into multiple write
 * commands as needed per the write size.
 */
int spi_flash_cmd_write_mode(struct spi_flash *flash, u32 offset,
		u32 len, void *buf, u32 mode);

/*
 * Enable writing on the SPI flash.
 */
int spi_flash_cmd_write_enable(struct spi_flash *flash);

/*
 * Disable writing on the SPI flash.
 */
int spi_flash_cmd_write_disable(struct spi_slave *spi);

/*
 * Same as spi_flash_cmd_read() except it also claims/releases the SPI
 * bus. Used as common part of the ->read() operation.
 */
int spi_flash_read_common(struct spi_flash *flash, u8 *cmd,
		u32 cmd_len, void *data, u32 data_len);

/* Send a command to the device and wait for some bit to clear itself. */
int spi_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
			   u8 cmd, u8 poll_bit);

/*
 * Send the read status command to the device and wait for the wip
 * (write-in-progress) bit to clear itself.
 */
int spi_flash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout);

/* Erase sectors. */
int spi_flash_cmd_erase(struct spi_flash *flash, u8 erase_cmd, u32 offset, u32 len);
int spi_flash_erase_mode(struct spi_flash *flash, u32 offset, u32 len, u32 mode);

/* Manufacturer-specific probe functions */
struct spi_flash *spi_flash_probe_spansion(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_atmel(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_eon(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_macronix(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_sst(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_stmicro(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_winbond(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_fram_probe_ramtron(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_gigadevice(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, u32 mode, u32 fifo_width);


#endif

