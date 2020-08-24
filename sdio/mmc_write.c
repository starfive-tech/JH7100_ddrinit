/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <comdef.h>
#include "mmc_private.h"
#include <mmc.h>
#include <uart.h>
#include <div64.h>

extern struct mmc local_mmc0;
static u32 mmc_erase_t(struct mmc *mmc, u32 start, unsigned int blkcnt)
{
	struct mmc_cmd cmd;
	u32 end;
	int err, start_cmd, end_cmd;

	if (mmc->high_capacity) {
		end = start + blkcnt - 1;
	} else {
		end = (start + blkcnt - 1) * mmc->write_bl_len;
		start *= mmc->write_bl_len;
	}

	if (IS_SD(mmc)) {
		start_cmd = SD_CMD_ERASE_WR_BLK_START;
		end_cmd = SD_CMD_ERASE_WR_BLK_END;
	} else {
		start_cmd = MMC_CMD_ERASE_GROUP_START;
		end_cmd = MMC_CMD_ERASE_GROUP_END;
	}

	//printf("start_cmd = %d, end_cmd = %d\n", start_cmd,end_cmd);

	cmd.cmdidx = start_cmd;
	cmd.cmdarg = start;
	cmd.resp_type = MMC_RSP_R1;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = end_cmd;
	cmd.cmdarg = end;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = MMC_ERASE_ARG;
	cmd.resp_type = MMC_RSP_R1b;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	return 0;

err_out:
	rlSendString("mmc erase failed\n");
	return err;
}

unsigned long mmc_berase(int dev_num, unsigned int start, unsigned int blkcnt)
{
	int err = 0;
	u32 start_rem, blkcnt_rem;
    struct mmc *mmc; 
	if(dev_num == 0)
        mmc = &local_mmc0;
	unsigned int blk = 0, blk_r = 0;
	int timeout = 1000;

	if (!mmc)
		return -1;

	/*
	 * We want to see if the requested start or total block count are
	 * unaligned.  We discard the whole numbers and only care about the
	 * remainder.
	 */
	err = div_u64_rem(start, mmc->erase_grp_size, &start_rem);
	err = div_u64_rem(blkcnt, mmc->erase_grp_size, &blkcnt_rem);
	if (start_rem || blkcnt_rem)
		rlSendString("\n\nCaution! Your devices Erase group  ,\n\n");

	while (blk < blkcnt) {
		blk_r = ((blkcnt - blk) > mmc->erase_grp_size) ?
			mmc->erase_grp_size : (blkcnt - blk);
		err = mmc_erase_t(mmc, start + blk, blk_r);
		if (err)
			break;

		//printf("erase blk = %d\n", blk);

		blk += blk_r;

		/* Waiting for the ready status */
		if (mmc_send_status(mmc, timeout))
			return 0;
	}

	return blk;
}

static u32 mmc_write_blocks(struct mmc *mmc, unsigned int start,
		unsigned int blkcnt, const void *src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		//printf("MMC: block number 0x%d,exceeds max(0x%d)\n",
		       //start + blkcnt, mmc->block_dev.lba);
		return 0;
	}

	//printf("write blkcnt = %d\n", blkcnt);

	if (blkcnt == 0)
		return 0;
	else if (blkcnt == 1)
		cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	cmd.resp_type = MMC_RSP_R1;

	data.src = src;
	data.blocks = blkcnt;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;
	//printf("data.blocksize = %d\n", data.blocksize);

	if (mmc_send_cmd(mmc, &cmd, &data)) {
		rlSendString("mmc write failed\n");
		return 0;
	}

	/* SPI multiblock writes terminate using a special
	 * token, not a STOP_TRANSMISSION request.
	 */
	if (!mmc_host_is_spi(mmc) && blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			rlSendString("mmc fail to send stop cmd\n");
			return 0;
		}
	}

	/* Waiting for the ready status */
	if (mmc_send_status(mmc, timeout))
		return 0;

	return blkcnt;
}

unsigned long mmc_bwrite(int dev_num, unsigned int start, unsigned int blkcnt, const void *src)
{
	unsigned int cur, blocks_todo = blkcnt;
    struct mmc *mmc; 

	if(dev_num == 0)
        mmc = &local_mmc0;

	if (!mmc)
		return 0;

	if (mmc_set_blocklen(mmc, mmc->write_bl_len))
		return 0;

	do {
		cur = (blocks_todo > mmc->cfg->b_max) ?
			mmc->cfg->b_max : blocks_todo;
		if (mmc_write_blocks(mmc, start, cur, src) != cur)
			return 0;
		blocks_todo -= cur;
		start += cur;
		src += cur * mmc->write_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}
