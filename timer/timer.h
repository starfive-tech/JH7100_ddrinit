/* SPDX-License-Identifier: GPL-2.0-or-later */
/**
  ******************************************************************************
  * @file  timer.h
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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <comdef.h>

#if 0
#define RUN_MODE_CONTINOUS     0
#define RUN_MODE_SINGLE		1

#define SOC_SYS_TIMER			0 /* system timer0 */

#define CONFIG_SYS_HZ_CLOCK         TIMER_CLK_HZ

struct timer_driver {
	void 	*base;
	unsigned long	freq;
	int		irq;
};

struct timer_init_s{
	u32 int_en;
	u32 run_mode;
	u32 size;
	u32 one_shot;
	u32 count; /* TODO: time value */
	void (*callback)(void*);
	void *arg;
    u32 wdog_reset_en;
};

/*
* public function definition
*/
/*---------------------------------------------------
 *
 * timer_clr_int_status:
 * This function clear int_status register.
 *
 * timer_clr_int_status() interface:
 * id:0/1/2 indicate clear timer0/timer1/timer2
      int_status register.
 *
 * Returns: 0 on success, not 0 on failure
 */
int timer_clr_int_status(u32 id);

/*---------------------------------------------------
 *
 * timer_set:
 * This function set registers defined by timer_init_s
   struct.
 *
 * timer_set() interface:
 * id:0/1/2 indicate clear timer0/timer1/timer2
      int_status register.
   init:timer_init_s struct ptr.
 *
 * Returns: 0 on success, not 0 on failure
 */
int timer_set(u32 id,struct timer_init_s *init);



/*---------------------------------------------------
 *
 */
void timer_reload(u32 id);

/*---------------------------------------------------
 *
 * timer_stop:
 * This function disable and clear interrupt regs of the timer.
 *
 * timer_stop() interface:
 * id:0/1/2 indicate clear timer0/timer1/timer2
      int_status register.
 *
 * Returns: 0 on success, not 0 on failure
 */
int timer_stop(u32 id);
/*---------------------------------------------------
 *
 * timer_start:
 * This function enable the timer.
 *
 * timer_start() interface:
 * id:0/1/2 indicate clear timer0/timer1/timer2
      int_status register.
 *
 * Returns: 0 on success, not 0 on failure
 */
int timer_start(u32 id);
/*---------------------------------------------------
 * timer_init:
 * This function initialize module hardware and some software structures.
 * You must call this function before other operation
 *
 * Returns: 0 on success, not 0 on failure
 */
int timer_init(int id);

/*---------------------------------------------------
 * module_exit:
 * This function free memory.
 *
 * Returns: 0 on success, not 0 on failure
 */
int timer_exit(void);
#endif

/*---------------------------------------------------
 *
 * udelay:
 * This function used for delay usec microsecond.
 *
 * udelay() interface:
 * usec:0~357913941(MAX) int number.
 *
 * Returns: 0 on success, not 0 on failure
 */
int udelay(u32 usec);

void mdelay(unsigned int ms);

void sdelay(unsigned int s);

u32 get_timer(unsigned int base);

/*---------------------------------------------------
 *
 * get_ticks:
 * This function get system ticks number.
 *
 * get_ticks() interface:
 * tick_base:base number of system ticks.
 *
 * Returns: a long long int number represent how many
            ticks from system start based on tick_base.
 */

unsigned long long get_ticks(void);


/*---------------------------------------------------
 *
 * usec_to_tick:
 * This function convert microsecond to system ticks.
 *
 * usec_to_tick() interface:
 * usec:0~357913941(MAX) int number.
 *
 * Returns: converted system ticks.
 */
u64 usec_to_tick(u64 usec);

#define delay	udelay

#endif /* __TIMER_H__ */
