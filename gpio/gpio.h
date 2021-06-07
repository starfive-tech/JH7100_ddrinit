/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  gpio.h
  * @author  StarFive Technology
  * @version  V1.0
  * @date  08/13/2020
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


#ifndef _SFC_GPIO_H
#define _SFC_GPIO_H

#include <comdef.h>

int gpio_get_val(int gpio_num);
extern void gpio_init(void);
extern int get_boot_mode();

#define BOOT_GPIO_60    60
#define BOOT_GPIO_61    61
#define BOOT_GPIO_62    62

#endif /* _SFC_GPIO_H */
