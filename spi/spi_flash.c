/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  spi_flash.c
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
#include "spi.h"
#include "spi_flash.h"
#include "spi_flash_internal.h"
#include <cadence_qspi.h>

static void spi_flash_addr(u32 addr, u8 *cmd)
{
	/* cmd[0] is actual command */
	cmd[1] = (addr & 0x00FF0000) >> 16;
	cmd[2] = (addr & 0x0000FF00) >> 8;
	cmd[3] = (addr & 0x000000FF) >> 0;
}

static int spi_flash_read_write(struct spi_slave *spi,
		u8 *cmd, u32 cmd_len,
		u8 *data_out, u8 *data_in,
		u32 data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags, SPI_DATAMODE_8);
	if (ret)
	{
		//uart_printf("SF: Failed to send command (%d bytes): %d\n",
				//cmd_len, ret);
	}
	else if (data_len != 0)
	{
		ret = spi_xfer(spi, data_len * 8, data_out, data_in, SPI_XFER_END, SPI_DATAMODE_8);
	}

	return ret;
}

int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, u32 len)
{
	return spi_flash_cmd_read(spi, &cmd, 1, response, len);
}

int spi_flash_cmd_read(struct spi_slave *spi, u8 *cmd,
		u32 cmd_len, void *data, u32 data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, NULL, data, data_len);
}

int spi_flash_cmd_write(struct spi_slave *spi, u8 *cmd, u32 cmd_len,
		void *data, u32 data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, data, NULL, data_len);
}

int spi_flash_cmd_write_enable(struct spi_flash *flash)
{
	return spi_flash_cmd(flash->spi, CMD_WRITE_ENABLE, (void*)NULL, 0);
}

int spi_flash_cmd_write_status_enable(struct spi_flash *flash)
{
	return spi_flash_cmd(flash->spi, CMD_STATUS_ENABLE, (void*)NULL, 0);
}

int spi_flash_cmd_write_disable(struct spi_slave *spi)
{
	return spi_flash_cmd(spi, CMD_WRITE_DISABLE, (void*)NULL, 0);
}
int spi_flash_cmd_read_status(struct spi_flash *flash, u8 *cmd, u32 cmd_len, u8 *status)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	ret = spi_xfer(spi, 8*cmd_len, cmd, NULL, SPI_XFER_BEGIN, SPI_DATAMODE_8);
	if (ret) {
		//uart_printf("SF: Failed to send command %x: %d\n", (unsigned int)cmd, ret);
		return ret;
	}

	ret = spi_xfer(spi, 8*1, NULL, status, SPI_XFER_END, SPI_DATAMODE_8);
	//uart_printf("status = 0x%x\r\n", status[0]);
	if (ret)
		return ret;

	return 0;
}

int spi_flash_cmd_poll_bit(struct spi_flash *flash, unsigned long timeout,
		u8 cmd, u8 poll_bit)
{
	int ret;
	u8 status;
    u32 status_tmp = 0;
	u32 timebase_1 = 0;
	do {
		ret = spi_flash_cmd_read_status(flash, &cmd, 1, &status);
        //uart_printf("cmd = 0x%x, status = 0x%x\r\n", cmd, status);
		if (ret)
			return ret;
		if ((status & poll_bit) == 0)
			break;
		timebase_1++;//libo
	} while (timebase_1 < timeout);

	if ((status & poll_bit) == 0)
		return 0;

	/* Timed out */
	//uart_printf("SF: time out!\r\n");
	return -1;
}

int spi_flash_cmd_wait_ready(struct spi_flash *flash, unsigned long timeout)
{
	return spi_flash_cmd_poll_bit(flash, timeout,
			CMD_READ_STATUS, STATUS_WIP);
}

int spi_flash_cmd_poll_enable(struct spi_flash *flash, unsigned long timeout,
		u8 cmd, u32 poll_bit)
{
	int ret;
	u8 status;
    u32 status_tmp = 0;

	u32 timebase_1 = 0;
	do {

		ret = spi_flash_cmd_read_status(flash, &cmd, 1, &status);
		if (ret)
			return ret;
        //uart_printf("read status = 0x%x\r\n", status);
		if ((status & poll_bit) == 1)
			break;
		timebase_1++;
	} while (timebase_1 < timeout);

	/* Timed out */
	//uart_printf("SF: time out!\r\n");
	return 0;
} 

int spi_flash_cmd_status_poll_enable(struct spi_flash *flash, unsigned long timeout,
		u8 cmd, u32 poll_bit)
{
	int ret;
	u8 status;
    u32 status_tmp = 0;

	u32 timebase_1 = 0;
	do {

		ret = spi_flash_cmd_read_status(flash, &cmd, 1, &status);
		if (ret)
			return ret;
        //uart_printf("read status = 0x%x\r\n", status);
		if ((status & poll_bit) == 0x2)
			break;
		timebase_1++;
	} while (timebase_1 < timeout);

	/* Timed out */
	//uart_printf("SF: time out!\r\n");
	return 0;
}

int spi_flash_cmd_wait_enable(struct spi_flash *flash, unsigned long timeout)
{
	return spi_flash_cmd_status_poll_enable(flash, timeout,
			CMD_READ_STATUS, FLASH_ENABLE);
}
int spi_flash_write_status(struct spi_flash *flash,  u8 *cmd, unsigned int cmd_len,void *data, unsigned int data_len)
{
	int ret;

	ret = spi_flash_cmd_write_enable(flash);
	if (ret) {
	//	uart_printf("SF: Unable to claim SPI bus\n");
		return ret;
	}

	ret = spi_flash_cmd_write(flash->spi, cmd, cmd_len, data, data_len);
	if (ret < 0) {
		//uart_printf("SF: write failed\n");
		return ret;
	}
	ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT);
	if (ret < 0) {
		//uart_printf("SF: wait ready failed\n");
		return ret;
	}
	ret = spi_flash_cmd_write_disable(flash->spi);
	if (ret < 0) {
		//uart_printf("SF: disable write failed\n");
		return ret;
	}
	return 0;
}
#ifdef ISP_BOARD
int spi_flash_write_status_bit(struct spi_flash *flash, u8 status0, u8 bit0)
{
	u8 status[2];
	int ret = 0;

	status[0] = CMD_WRITE_STATUS;
	status[1] = status0|bit0;
	spi_flash_write_status(flash, &status[0], 1, &status[1], 1);

	if (bit0)
	{
		ret &= spi_flash_cmd_poll_bit(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT, CMD_READ_STATUS, ~bit0);
	}

	return ret;
   delay(1000);
    

	return ret;
}
int spi_flash_protect(struct spi_flash *flash)
{
	/* set PB=0 all can write */
	return spi_flash_write_status_bit(flash, 0x00, 0);
}
#else
int spi_flash_write_status_bit(struct spi_flash *flash, u8 status1, u8 status2,  u8 bit1,  u8 bit2)
{
	u8 status[3];
	int ret = 0;

	status[0] = CMD_WRITE_STATUS;
	status[1] = status1|bit1;
	status[2] = status2|bit2;
	spi_flash_write_status(flash, &status[0], 1, &status[1], 2);

	if (bit1)
	{
		ret &= spi_flash_cmd_poll_bit(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT, CMD_READ_STATUS, ~bit1);
	}
	if (bit2)
	{
		ret &= spi_flash_cmd_poll_bit(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT, CMD_READ_STATUS1, ~bit2);
	}

	return ret;
   delay(1000);
    

	return ret;
}

int spi_flash_protect(struct spi_flash *flash)
{
	/* set PB=0 all can write */
	return spi_flash_write_status_bit(flash, 0x00, 0x00, 0, 0);
}
#endif
int spi_flash_cmd_erase(struct spi_flash *flash, u8 erase_cmd,
		u32 offset, u32 len)
{
	u32 start, end, erase_size;
	int ret;
	u8 cmd[4];
 
    //uart_printf("spi_flash_cmd_erase \r\n");

	switch(erase_cmd){
		case CMD_W25_SE:
			erase_size = flash->sector_size;
			break;
		case CMD_W25_BE_32:
			erase_size = flash->sector_size * 8;
			break;
		case CMD_W25_BE:
			erase_size = flash->block_size;
			break;
		default:
			erase_size = flash->sector_size;
			break;
	}

	if (offset % erase_size || len % erase_size) {
		//uart_printf("SF: Erase offset/length not multiple of erase size\n");
		return -1;
	}

   // spi_flash_cmd_write_status_enable(flash);
	spi_flash_protect(flash);

	cmd[0] = erase_cmd;
	start = offset;
	end = start + len;
	while (offset < end)
	{
		spi_flash_addr(offset, cmd);
		offset += erase_size;

		//uart_printf("SF: erase %x %x %x %x (%x)\n", cmd[0], cmd[1],
		//		cmd[2], cmd[3], offset);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret)
			goto out;

		//spi_flash_cmd_wait_enable(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT);

		ret = spi_flash_cmd_write(flash->spi, cmd, sizeof(cmd), NULL, 0);
		if (ret)
			goto out;

		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT);
		if (ret)
			goto out;

		ret = spi_flash_cmd_write_disable(flash->spi);
		if (ret)
			goto out;

	}

	//uart_printf("SF: Successfully erased %d bytes @ %x\n", len , start);

out:
	return ret;
}

/* mode is 4, 32, 64*/
int spi_flash_erase_mode(struct spi_flash *flash, u32 offset, u32 len, u32 mode)
{
	int ret = 0;
	switch (mode)
	{
		case 4:
			ret = spi_flash_cmd_erase(flash, CMD_W25_SE, offset, len);
			break;
		case 32:
			ret = spi_flash_cmd_erase(flash, CMD_W25_BE_32, offset, len);
			break;
		case 64:
			ret = spi_flash_cmd_erase(flash, CMD_W25_BE, offset, len);
			break;
		default:
			ret = spi_flash_cmd_erase(flash, CMD_W25_BE, offset, len);
			break;
	}
	return ret;
}

int spi_flash_cmd_write_mode(struct spi_flash *flash, u32 offset,u32 len, void *buf, u32 mode)
{
	struct spi_slave *spi = flash->spi;
	unsigned long byte_addr, page_size;
	u32 write_addr;
	u32 chunk_len, actual;
	int ret;
	u8 cmd[4];
    int write_data = 1;
	unsigned long flags = SPI_XFER_BEGIN;

	page_size = flash->page_size;

	switch (mode){
		case 1:
			cmd[0] = CMD_PAGE_PROGRAM;
			break;
		case 4:
			cmd[0] = CMD_PAGE_PROGRAM_QUAD;
#ifdef ISP_BOARD
			spi_flash_write_status_bit(flash, 0x00, STATUS_QE);
#else
			spi_flash_write_status_bit(flash, 0x00, 0x00, 0, STATUS_QE);
#endif
			break;
		default:
			cmd[0] = CMD_PAGE_PROGRAM;
			break;
	}


	for (actual = 0; actual < len; actual += chunk_len)
	{
		write_addr = offset;
		byte_addr = offset % page_size;
		chunk_len = min(len - actual, page_size - byte_addr);

		spi_flash_addr(write_addr, cmd);

		ret = spi_flash_cmd_write_enable(flash);
		if (ret < 0) {
			//uart_printf("SF: enabling write failed\n");
			break;
		}
		spi_flash_cmd_wait_enable(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT);
#if 1
		if (mode == 1)
		{

            
            ret = spi_flash_cmd_write(flash->spi, cmd, 4,
					(unsigned char*)buf + actual, chunk_len);
			if (ret < 0) {
				//uart_printf("SF: write failed\n");
				break;
			}
            
		}
#endif
		if (mode == 4)
		{
			flags = SPI_XFER_BEGIN;
			if (chunk_len == 0)
				flags |= SPI_XFER_END;

			ret = spi_xfer(spi, 4 * 8, cmd, NULL, flags, SPI_DATAMODE_8);
			if (ret < 0)
			{
				//uart_printf("xfer failed\n");
				return ret;
			}
			else if (chunk_len != 0)
			{
				//qspi_mode_ctl(SPI4_DATEMODE_4);
				ret = spi_xfer(spi, chunk_len * 8, (unsigned char*)buf + actual, NULL, SPI_XFER_END, SPI_DATAMODE_8);
			
		    }
		}
        //qspi_mode_ctl(SPI4_DATEMODE_0);
		ret = spi_flash_cmd_wait_ready(flash, SPI_FLASH_PROG_TIMEOUT);
		if (ret < 0)
		{
			//uart_printf("SF: spi_flash_cmd_wait_ready failed\n");
			break;
		}
		ret = spi_flash_cmd_write_disable(flash->spi);
		if (ret < 0)
		{
			//uart_printf("SF: disable write failed\n");
			break;
		}
         
    	offset += chunk_len;
	//uart_printf("SF: program %s %d bytes @ %d\n", ret ? "failure" : "success", len, offset);
    }
	return ret;
}

int spi_flash_read_common(struct spi_flash *flash, u8 *cmd,
		u32 cmd_len, void *data, u32 data_len)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	ret = spi_flash_cmd_read(spi, cmd, cmd_len, data, data_len);

	return ret;
}

int spi_flash_cmd_read_fast(struct spi_flash *flash, u32 offset,
		u32 len, void *data, u32 mode)
{
	u8 cmd[5];

	cmd[0] = CMD_READ_ARRAY_FAST;
	spi_flash_addr(offset, cmd);
	cmd[4] = 0x00;

	return spi_flash_read_common(flash, cmd, sizeof(cmd), data, len);
}

int spi_flash_read_mode(struct spi_flash *flash, u32 offset,
		u32 len, void *data, u32 mode)
{
	struct spi_slave *spi = flash->spi;
	u8 cmd[5];
	int ret;
    int write_data = 0;
    u8 status[2] = {2};
    int i = 0;

	switch (mode)
	{
		case 1:
			cmd[0] = CMD_READ_ARRAY_FAST;
			break;
		case 2:
			cmd[0] = CMD_READ_ARRAY_DUAL;
			break;
		case 4:
#ifdef ISP_BOARD
			cmd[0] = CMD_READ_ARRAY_LEGACY;
            //spi_flash_cmd_write_reg_mode(flash, flash_reg_offset, 1, flash_reg);
            //flash_reg[0] = 0;
			//spi_flash_cmd_write_reg_mode(flash, flash_reg_offset, 1, flash_reg);
            //spi_flash_read_reg_mode(flash, flash_reg_offset, 1, flash_reg);
#else
			cmd[0] = CMD_READ_ARRAY_QUAD;
			spi_flash_write_status_bit(flash, 0x00, 0x00, 0, STATUS_QE);
#endif
			break;
		default:
			cmd[0] = CMD_READ_ARRAY_FAST;
			break;
	}

    //spi_flash_cmd_poll_bit(flash, SPI_FLASH_PAGE_ERASE_TIMEOUT, CMD_READ_STATUS1, ~STATUS_QE);

    
	spi_flash_addr(offset, cmd);
	cmd[4] = 0x00;


    ret = spi_xfer(spi, 5*8, cmd, NULL, SPI_XFER_BEGIN, SPI_DATAMODE_8);
    if (ret < 0)
	{
		//uart_printf("xfer failed\n");
		return ret;
	}
	ret = spi_xfer(spi,  len*8,  NULL, data, SPI_XFER_END, SPI_DATAMODE_8);
	if (ret < 0)
	{
		//uart_printf("xfer failed\n");
		return ret;
	}

	return 0;
}
