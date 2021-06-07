/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  gpio.c
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


#include "gpio.h"
#include <sys.h>
//#include <ezGPIO_fullMux_ctrl_macro.h>
#include <clkgen_ctrl_macro.h>
#include <rstgen_ctrl_macro.h>

#define GPIO_EN         (0x0)
#define GPIO_IS_LOW     (0x10)  //select level or edge trigge
#define GPIO_IS_HIGH    (0x14)
#define GPIO_IBE_LOW    (0x18)  //interrupt both edge trigger 1:both, 0:disable both 
#define GPIO_IBE_HIGH   (0x1c)
#define GPIO_IEV_LOW    (0x20)  //trigger condition  1:rising, 0 falling 
#define GPIO_IEV_HIGH   (0x24)
#define GPIO_IE_LOW     (0x28)  //interrupt enable
#define GPIO_IE_HIGH    (0x2c)
#define GPIO_IC_LOW     (0x30)  //interrupt clear
#define GPIO_IC_HIGH    (0x34)
//read only
#define GPIO_RIS_LOW    (0x38)  //raw interrupt
#define GPIO_RIS_HIGH   (0x3c)
#define GPIO_MIS_LOW    (0x40)  //masked interrupt
#define GPIO_MIS_HIGH   (0x44)
#define GPIO_DIN_LOW    (0x48)  //data in
#define GPIO_DIN_HIGH   (0x4c)


static inline void gpio_write_reg(unsigned long base, unsigned long addr, unsigned int val)
{
	writel(val, (volatile void *)(base + addr));
}

static inline unsigned int gpio_read_reg(unsigned long base, unsigned long addr)
{
	return readl((volatile void *)(base + addr));
}
void gpio_enable(int enable)
{
    if(enable == 0)
        gpio_write_reg(EZGPIO_FULLMUX_BASE_ADDR, GPIO_EN, 0);
    else 
        gpio_write_reg(EZGPIO_FULLMUX_BASE_ADDR, GPIO_EN, 1);
}
int gpio_get_val(int gpio_num)
{
    int value = 0;
    if(gpio_num < 32)
    {
        value = gpio_read_reg(EZGPIO_FULLMUX_BASE_ADDR, GPIO_DIN_LOW);
        return (value >> gpio_num) & 0x1;
    }
    else
    {
        value = gpio_read_reg(EZGPIO_FULLMUX_BASE_ADDR, GPIO_DIN_HIGH);
        return (value >> (gpio_num - 32)) & 0x1;
    }
}

int get_boot_mode()
{
    int boot_mode = 0;
    boot_mode = gpio_get_val(BOOT_GPIO_60);
    boot_mode |= (gpio_get_val(BOOT_GPIO_61) << 1);
    boot_mode |= (gpio_get_val(BOOT_GPIO_62) << 2);

    return boot_mode;
    
}
void gpio_init(void)
{
    _ENABLE_CLOCK_clk_gpio_apb_;
    _ASSERT_RESET_rstgen_rstn_gpio_apb_;
    _CLEAR_RESET_rstgen_rstn_gpio_apb_;
    gpio_enable(1);
}

