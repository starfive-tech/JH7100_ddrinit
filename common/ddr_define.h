/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  ddr_define.h
  * @author  StarFive Technology
  * @version  V1.0
  * @date  07/20/2020
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

#define G_DRAM_LPDDR4   1
#define G_SPEED_2133    1

#ifdef G_DRAM_DDR4

#define dram_ddr4
//#define speed_bin=3200aa
#ifdef G_16G_X8
#define dram_dq_x8
//#define dram_density=16
#define DDR4_16G_X8
#else
#define dram_dq_x16
//+define+dram_density=8
#define DDR4_8G_X16
#endif
//#define FIXED_3200
//#define ALLOW_JITTER

#elif G_DRAM_LPDDR4

#define dram_lpddr4
//#define speed_bin=3200
//#define dram_density=8
#define dram_dq_x16
//#define lpddr4_pwup_off
//#define lpddr4_proc

#elif G_DRAM_DDR3

#define dram_ddr3
//#define speed_bin=2133kk
//#define dram_density=8
#define dram_dq_x16
//#define sg093
//#define x16
//#define den8192Mb

#elif G_DRAM_LPDDR3

#define dram_lpddr3
//#define speed_bin=2133
#ifdef G_8G_X32
//#define #dram_density=8
#define dram_dq_x32
#else
//#define #dram_density=8
#define dram_dq_x16
#endif

//#define pwup_off
//#define proc

#endif
