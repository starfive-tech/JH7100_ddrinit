/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  ctype.c
  * @author  StarFive Technology
  * @version  V1.0
  * @date  07/29/2020
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
  * COPYRIGHT 2020 Shanghai StarFive Technology Co., Ltd.
  */


#include "ctype.h"

const unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,				/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,			/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,				/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,				/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,				/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,				/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,				/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,				/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,		/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,				/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,				/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,				/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,		/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,				/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,				/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,				/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,	/* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,	/* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,	/* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,	/* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,	/* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};	/* 240-255 */

