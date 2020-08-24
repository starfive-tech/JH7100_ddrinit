/*
 * Copyright 2008,2010 Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based (loosely) on the Linux code
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _MMC_PRIVATE_H_
#define _MMC_PRIVATE_H_

#include <mmc.h>

extern int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
			struct mmc_data *data);
extern int mmc_send_status(struct mmc *mmc, int timeout);
extern int mmc_set_blocklen(struct mmc *mmc, int len);
#ifdef CONFIG_FSL_ESDHC_ADAPTER_IDENT
void mmc_adapter_card_type_ident(void);
#endif

extern unsigned long mmc_berase(int dev_num, unsigned int start, unsigned int blkcnt);


extern unsigned long mmc_bwrite(int dev_num, unsigned int start, unsigned int blkcnt, const void *src);

#endif /* _MMC_PRIVATE_H_ */
