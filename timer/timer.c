/*************************************************************************
* All rights reserved.
*
* Filename     : timer.c
* Version       : version 001
* Author        : 
* Created on  : 2018.07.14
*************************************************************************/
#include <comdef.h>
#include <sys.h>
#include <timer.h>
#include <clkgen_ctrl_macro.h>
#include <uart.h>
#include <div64.h>

#define VIC_TIMER_NUM	        7
#define VIC_TIMER_NUM_MAX        7
#define VIC_SYS_TIMER          0

#define CONFIG_SYS_HZ               1000

#define TIMER_RATE_HZ				(CONFIG_SYS_HZ_CLOCK /CONFIG_SYS_HZ)  
#define WDOG_UNKOCK                 0x378f0765

u32 interrupt_count = 0;

/*
* variables definition for local use
*/

static const u32 timer_clk = CONFIG_SYS_HZ_CLOCK;
static u64 lastdec;//Last decremneter snapshot
static u64 timestamp;//Monotonic incrementing timer
static u64 timer_load_val;

struct timer_regs{
	u32 interrupt_status;		//0x00
	struct time_reg{
        u32 control;	//0x04+i*0x10
    	u32 load_reg;		//0x08+i*0x10
    	u32 res0;			//0x0c+i*0x10
    	u32 enable_reg;		//0x10+i*0x10
    	u32 reload_reg;		//0x14 + i * 0x10
    	u32 timer_value;	//0x18 + i * 0x10
    	u32 res1;				//0x1c
    	u32 interrupt_clr;  //0x20
    	u32 interrupt_mask; //0x24 
    	u32 res3[5];
		u32 watchdog_lock;  //0x3c+i*0x40 watchdog use ONLY
		u32 res4;			//0x40+i*0x40
    }timer[VIC_TIMER_NUM_MAX];
};


struct timer_int_arg{
	u32 id;
	void (*callback)(void *);
	void *arg;
    u32 wdog_reset;
};

/*
* local functions
*/
struct timer_regs *timer_driver_to_base(struct timer_driver *driver)
{
	return (struct timer_regs *)driver->base;
}

static struct timer_driver g_timer_driver;

void timer_int_enable(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].interrupt_mask = 0;
}

void timer_int_disable(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].interrupt_mask = 1;
}


void timer_disable(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].enable_reg = 0;

}

int timer_is_timeout(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

    //printf("addr = 0x%x,base[id].raw_int_status = 0x%x\r\n", &(base[id].raw_int_status), base[id].raw_int_status);
	
	return (base->interrupt_status == 1) ? true : false;
}

void timer_set_mode(u32 id,u32 run_mode)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].control = run_mode;
}

void timer_set_val(u32 id,u32 val)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].load_reg = val;
}

void timer_set_reload_val(u32 id,u32 val)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].reload_reg = val;
}

static struct timer_int_arg int_arg[VIC_TIMER_NUM_MAX];

int timer_set(u32 id,struct timer_init_s *init)
{
	u32 irq = 0;
	timer_disable(id);
	timer_set_mode(id,init->run_mode);
	timer_set_val(id,init->count);
	timer_int_disable(id);
	timer_set_reload_val(id, init->count);

	timestamp=0;
	timer_load_val=init->count;
	lastdec=timer_load_val;
    //_ENABLE_CLOCK_clk_wdtimer_apb_;

	switch (id){
	case 0:
        _ENABLE_CLOCK_clk_timer0_coreclk_;
		break;
	case 1:
        _ENABLE_CLOCK_clk_timer1_coreclk_;
		break;
	case 2:
        _ENABLE_CLOCK_clk_timer2_coreclk_;
		break;
	case 3:
        _ENABLE_CLOCK_clk_timer3_coreclk_;
        break;
    case 4:
        _ENABLE_CLOCK_clk_timer4_coreclk_;
        break;
    case 5:
        _ENABLE_CLOCK_clk_timer5_coreclk_;
        break;
    case 6:
        _ENABLE_CLOCK_clk_timer6_coreclk_;
		break;
	}
  
	timer_int_disable(id);


	//timer_int_disable(id);
	
	return 0;
}

void timer_enable(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	base->timer[id].enable_reg = 1;
}

/*
* public functions
*/
int timer_clr_int_status(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;
	int clr;
    //printf("clr timer id = %d\r\n", id);
	base = timer_driver_to_base(driver);	
    base->timer[id].interrupt_clr = 1;
	return clr;
}

u32 timer_get_val(u32 id)
{
	struct timer_driver *driver = &g_timer_driver;
	volatile struct timer_regs *base;

	base = timer_driver_to_base(driver);

	return base->timer[id].timer_value;
}

int timer_stop(u32 id)
{
	int irq;
	
	timer_int_disable(id);
	timer_disable(id);
	timer_clr_int_status(id);
	return 0;
}

int timer_start(u32 id)
{
	timer_enable(id);

	return 0;
}

int timer_exit(void)
{
	return 0;
}

int timer_init(int id)
{
	struct timer_driver *driver = &g_timer_driver;
	struct timer_init_s timer_init_param;
	int reg_value;
	/* timer clk use apb clk */
	/* reset */
	driver->base = (void *)TIMER_BASE_ADDR;
	driver->freq = CONFIG_SYS_HZ_CLOCK;

  // _ASSERT_RESET_rstn_apb_timer_;
  //  _CLEAR_RESET_rstn_apb_timer_;
    
	
	if(id == SOC_SYS_TIMER)/* start timer0 as system delay timer */
	{
		timer_init_param.int_en = 0;//0:interrupt disable 1:interrupt enable

		timer_init_param.run_mode = 0;//0:continuous 1:single
		timer_init_param.count = 0xffffffff;
		timer_init_param.one_shot = 0;
		timer_init_param.callback = NULL;
		timer_init_param.arg = NULL;
        //_SET_SYSCON_REG_SCFG_misc_ctrl1_timclken1(1);
		timer_set(id, &timer_init_param);
		timer_start(id);
	}
	else
	{
		timer_set(id, &timer_init_param);
		timer_start(id);
	}

	return 0;
}

u32 get_ticks(u32 tick_base)
{
	u32 id = SOC_SYS_TIMER;
	u32 now = timer_get_val(id);
    //printf("time value = 0x%x\r\n",now);
    

	if (lastdec >= now) {
	/* normal mode */
		timestamp += lastdec - now;
	} else {
	/* we have an overflow ... */
		timestamp += lastdec + timer_load_val - now;
	}
	lastdec = now;
	return (u32)(timestamp-tick_base);
}

u32 get_timer(unsigned int base)
{
	return lldiv(get_ticks(0), TIMER_RATE_HZ) - base;
}
#if 0
u32 usec_to_tick(u32 usec)
{
    u32 value = usec*(timer_clk/1000000);
    //printf("value = 0x%x\r\n", value);
    
    return usec*(timer_clk/1000000);
}

int udelay(u32 usec)
{
    UINT64 tmp;
    UINT64 tmo;

    tmo = usec_to_tick(usec);
    tmp = get_ticks(0) + tmo;	/* get current timestamp */

    while (get_ticks(0) < tmp)/* loop till event */
    	 /*NOP*/
    {
    }   

    return 0;
}

#endif
#include "platform.h"

u64 usec_to_tick(u64 usec)
{
    u64 value;
    value = usec*(TIMER_CLK_HZ/1000000);
    return value;
}

/* delay x useconds */
int udelay(unsigned int usec)
{
	unsigned long  tmp;

	tmp = readq((volatile void *)CLINT_CTRL_MTIME) + usec_to_tick(usec);	/* get current timestamp */
    
	while (readq((volatile void *)CLINT_CTRL_MTIME) < tmp);
}

void mdelay(unsigned int ms)
{
	udelay(1000*ms);
}

void sdelay(unsigned int s)
{
	mdelay(1000*s);
}


