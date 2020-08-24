//
// gpio.h
//
// GPIO
//
// Description:
//
// Revision History:
//        DATE        VERSION  AUTHOR        NOTE
//        ----------  -------  -----------   ------------------------------
//        2019/12/27  0.1      Zavier           Initial
//


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
