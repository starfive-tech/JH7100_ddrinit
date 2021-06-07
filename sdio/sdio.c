/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  sdio.c
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

#include "comdef.h"
#include "sys.h"
#include "ezGPIO_fullMux_ctrl_macro.h"
#include "dwmmc.h"
#include "gpt.h"

struct dwmci_host local_host0;
struct mmc local_mmc0;

#define DWMMC_BUS_FREQ		100000000
#define DWMMC_MAX_FREQ		10000000
#define DWMMC_MIN_FREQ		400000

#define GPT_BLOCK_SIZE 512


static void sdio0_gpio_mux_select()
{
#if 0//for fpga 
    SET_GPIO_18_dout_sdio0_pad_cclk_out;
    //SET_GPIO_18_doen_sdio0_pad_cclk_out;
    SET_GPIO_18_doen_LOW;
    //_SET_SYSCON_REG_register9_SCFG_gpio_pad_ctrl_9(5);

    SET_GPIO_17_doen_reverse_(1);
    SET_GPIO_17_doen_sdio0_pad_ccmd_oe;
    SET_GPIO_17_dout_sdio0_pad_ccmd_out;
    SET_GPIO_sdio0_pad_ccmd_in(17);

    SET_GPIO_15_doen_reverse_(1);
    SET_GPIO_16_doen_reverse_(1);
    SET_GPIO_19_doen_reverse_(1);
    SET_GPIO_20_doen_reverse_(1);

    SET_GPIO_19_doen_sdio0_pad_cdata_oe_bit0;
    SET_GPIO_19_dout_sdio0_pad_cdata_out_bit0;
    SET_GPIO_sdio0_pad_cdata_in_bit0(19);
   // _SET_SYSCON_REG_register9_SCFG_gpio_pad_ctrl_9(5);

    SET_GPIO_20_doen_sdio0_pad_cdata_oe_bit1;
    SET_GPIO_20_dout_sdio0_pad_cdata_out_bit1;
    SET_GPIO_sdio0_pad_cdata_in_bit1(20);
    //_SET_SYSCON_REG_register10_SCFG_gpio_pad_ctrl_10(5);

    SET_GPIO_15_doen_sdio0_pad_cdata_oe_bit2;
    SET_GPIO_15_dout_sdio0_pad_cdata_out_bit2;
    SET_GPIO_sdio0_pad_cdata_in_bit2(15);

    SET_GPIO_16_doen_sdio0_pad_cdata_oe_bit3;
    SET_GPIO_16_dout_sdio0_pad_cdata_out_bit3;
    SET_GPIO_sdio0_pad_cdata_in_bit3(16);
#endif
	SET_GPIO_sdio0_pad_card_detect_n(26);
	SET_GPIO_26_doen_HIGH;

	SET_GPIO_33_dout_sdio0_pad_cclk_out;
	SET_GPIO_33_doen_LOW;

	SET_GPIO_34_doen_reverse_(1);
	SET_GPIO_34_doen_sdio0_pad_ccmd_oe;
	SET_GPIO_34_dout_sdio0_pad_ccmd_out;
	SET_GPIO_sdio0_pad_ccmd_in(34);	

	SET_GPIO_32_doen_reverse_(1);
	SET_GPIO_31_doen_reverse_(1);
	SET_GPIO_30_doen_reverse_(1);
	SET_GPIO_36_doen_reverse_(1);

	SET_GPIO_32_doen_sdio0_pad_cdata_oe_bit0;
	SET_GPIO_32_dout_sdio0_pad_cdata_out_bit0;
	SET_GPIO_sdio0_pad_cdata_in_bit0(32);

	SET_GPIO_31_doen_sdio0_pad_cdata_oe_bit1;
	SET_GPIO_31_dout_sdio0_pad_cdata_out_bit1;
	SET_GPIO_sdio0_pad_cdata_in_bit1(31);

	SET_GPIO_30_doen_sdio0_pad_cdata_oe_bit2;
	SET_GPIO_30_dout_sdio0_pad_cdata_out_bit2;
	SET_GPIO_sdio0_pad_cdata_in_bit2(30);

	SET_GPIO_36_doen_sdio0_pad_cdata_oe_bit3;
	SET_GPIO_36_dout_sdio0_pad_cdata_out_bit3;
	SET_GPIO_sdio0_pad_cdata_in_bit3(36);

}

static struct mmc *init_mmc_device(int dev, unsigned int  force_init)
{
    struct mmc *mmc; 

	if(dev != 0)
		return NULL;
	
    mmc = &local_mmc0;

	if (force_init)
		mmc->has_init = 0;
	if (mmc_init(mmc))
		return NULL;
	return mmc;
}


int boot_sdio_init(void)
{
	struct dwmci_host *host;
    u32 fifo_depth = 32;

	/*HOST 0*/
	host = &local_host0;
	host->ioaddr = (void *)SDIO0_BASE_ADDR;
	host->name = "VIC DWMMC0";
	host->dev_index = 0;
    sdio0_gpio_mux_select();

	host->fifoth_val = MSIZE(0x2) |
	RX_WMARK(fifo_depth / 2 - 1) | TX_WMARK(fifo_depth / 2);

	host->buswidth = 4;
	host->bus_hz = DWMMC_BUS_FREQ;
	
	/* Add the mmc channel to be registered with mmc core */
	if (add_dwmci(host, DWMMC_MAX_FREQ, DWMMC_MIN_FREQ, 0)) {
		printk("DWMMC0 registration failed\n");
		return -1;
	}

	if(init_mmc_device(0, 0) == NULL)
	{
		printk("init dev fail.");
		return -1;
	}
	return 0;
}

static int sd_read_data(int dev_num,uint32_t start_lba, size_t blkcnt,void* dst)
{
	struct mmc *mmc=&local_mmc0;
	int size;
	
	size = mmc->block_dev.block_read(dev_num,start_lba,blkcnt,dst);
	if(size != blkcnt)
	{
		printk("sd_read_data size:%d blkcnt:%d\n",size,blkcnt);
		return -1;
	}
	return 0;
}

static gpt_partition_range find_sd_gpt_partition(
  uint64_t partition_entries_lba,
  uint32_t num_partition_entries,
  uint32_t partition_entry_size,
  const gpt_guid* partition_type_guid,
  void* block_buf  // Used to temporarily load blocks of SD card
)
{
	// Exclusive end
	uint16_t partition_entries_lba_end = (
		partition_entries_lba +
		(num_partition_entries * partition_entry_size + GPT_BLOCK_SIZE - 1) / GPT_BLOCK_SIZE
		);

	for (uint16_t i = partition_entries_lba; i < partition_entries_lba_end; i++) 
	{
		sd_read_data(0, i, 1,block_buf);

		gpt_partition_range range = gpt_find_partition_by_guid(block_buf, partition_type_guid, GPT_BLOCK_SIZE / partition_entry_size);

		if (gpt_is_valid_partition_range(range)) 
			return range;
	}
	return gpt_invalid_partition_range();
}

/*read data from sd to dst*/
int boot_load_gpt_partition(void* dst, const gpt_guid* partition_type_guid)
{
	uint8_t gpt_buf[GPT_BLOCK_SIZE];
	gpt_partition_range part_range;

	/*READ LBA1*/
	sd_read_data(0,1,1,gpt_buf);

	{
		// header will be overwritten by find_sd_gpt_partition(), so locally
		// scope it.
		gpt_header* header = (gpt_header*) gpt_buf;

		part_range = find_sd_gpt_partition(
			header->partition_entries_lba,
			header->num_partition_entries,
			header->partition_entry_size,
			partition_type_guid,
			gpt_buf
			);
	}
	printk("gpt_partition %d:%d\n",part_range.first_lba,part_range.last_lba);
	sd_read_data(0,part_range.first_lba,part_range.last_lba + 1 - part_range.first_lba,dst);
}

