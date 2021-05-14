/**
  ******************************************************************************
  * @file  bootmain.c
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
  *  COPYRIGHT 20120 Shanghai StarFive Technology Co., Ltd.
  */

#include "sys.h"
#include "spi_flash.h"
#include "spi.h"
#include "timer.h"
#include "encoding.h"
#include "gpt.h"
#include "clkgen_ctrl_macro.h"
#include "syscon_sysmain_ctrl_macro.h"
#include "rstgen_ctrl_macro.h"

#include "crc32.h"

extern void boot_sdio_init(void);
extern int boot_load_gpt_partition(void* dst, const gpt_guid* partition_type_guid);
extern const gpt_guid gpt_guid_sifive_uboot;
extern const gpt_guid gpt_guid_sifive_kernel;
extern unsigned int receive_count;


typedef void ( *STARTRUNNING )( unsigned int par1 );

/*
To run a procedure from the address:start
	start:	start address of runing in armmode, not do address align checking
*/
void start2run32(unsigned int start)
{
	(( STARTRUNNING )(start))(0);		
}

#define     SPIBOOT_LOAD_ADDR_OFFSET    252


/*read data from flash to the destination address
 *
 *spi_flash: flash device informations
 *des_addr: store the data read from flash
 *page_offset:Offset of data stored in flash 
 *mode:flash work mode
*/

static int load_data(struct spi_flash* spi_flash,void *des_addr,unsigned int page_offset,int mode)
{
	u8 dataBuf[260];
	u32 startPage,endPage;
	u32 pageSize;
	u32 fileSize;
	u8 *addr;
	int ret;
	int i;
	u32 offset;

	pageSize = spi_flash->page_size;
	addr = (u8 *)des_addr;
	offset = page_offset*pageSize;
	
	/*read first page,get the file size*/
	ret = spi_flash->read(spi_flash,offset,pageSize,dataBuf,mode);
	if(ret != 0)
    {
        printk("read fail#\r\n");
		return -1;
    }
	
	/*calculate file size*/
	fileSize = (dataBuf[3] << 24) | (dataBuf[2] << 16) | (dataBuf[1] << 8) | (dataBuf[0]) ;
	if(fileSize == 0)
		return -1;

	endPage = ((fileSize + 255) >> 8);//page align
	/*copy the first page data*/
	sys_memcpy(addr, &dataBuf[4], SPIBOOT_LOAD_ADDR_OFFSET);

	//copy data from flash to des_addr, with crc check
#if 1
    uint32_t crc_org = 0, crc_ddr = 0;
    uint32_t n_bytes_left = fileSize - SPIBOOT_LOAD_ADDR_OFFSET;
    uint32_t once = 0;

    crc32(dataBuf+4, SPIBOOT_LOAD_ADDR_OFFSET, &crc_org);
    crc32(addr, SPIBOOT_LOAD_ADDR_OFFSET, &crc_ddr);

    offset += pageSize;
    addr += SPIBOOT_LOAD_ADDR_OFFSET;

    /*read Remaining pages data*/
    while (n_bytes_left) {
        once = n_bytes_left > pageSize ? pageSize : n_bytes_left;
        ret = spi_flash->read(spi_flash, offset, once, dataBuf, mode);
        if(ret != 0)
        {
            printk("read 0x%x fail##\r\n", offset);
            return -1;
        }
        sys_memcpy(addr, dataBuf, once);
        crc32(dataBuf, once, &crc_org);
        crc32(addr, once, &crc_ddr);

        offset += once;
        addr += once;
        n_bytes_left -= once;
    }

    printk("crc flash: %08x, crc ddr: %08x\r\n", crc_org, crc_ddr);
    if (crc_org != crc_ddr) {
        printk("ERROR: crc check FAILED\r\n");
        while (1) {
            __asm__ ("wfi");
        }
    } else {
        printk("crc check PASSED\r\n");
    }
#else
	offset += pageSize;
	addr += SPIBOOT_LOAD_ADDR_OFFSET;

	for(i=1; i<=endPage; i++)
	{
		ret = spi_flash->read(spi_flash,offset,pageSize, addr, mode);
		if(ret != 0)
        {
            printk("read fail##\r\n");
			return -1;
        }
	
		offset += pageSize;
		addr +=pageSize;
	}
#endif

	return 0;
}

int updata_flash(struct spi_flash* spi_flash,u32 flash_addr,u32 flash_size_limit, u32 load_addr,unsigned char mode)
{
    int ret = 0;
    u32 offset = 0;
    int erase_block = 0;
    unsigned int page_count = 0;
    unsigned int index = 0;
	unsigned int blockSize,pageSize;
	u8 *data;

    printk("send file by xmodem\r\n");
	
	blockSize = spi_flash->block_size;
	pageSize = spi_flash->page_size;
	
	ret = xmodemReceive((unsigned char *)load_addr,0);
	if(ret <= 0)
		return -1;

	if ((u32)ret > flash_size_limit) {
		printk("error: size %d exceeds the limit %d\r\n", ret, flash_size_limit);
		return -1;
	}
	erase_block = (ret + blockSize - 1) / blockSize;
	page_count = (ret + pageSize - 1) / pageSize;

	/*erase flash*/
	offset = flash_addr;
	for(index=0; index<erase_block; index++)
	{
		ret = spi_flash->erase(spi_flash, offset, blockSize, 64);
		if(ret < 0)
		{
			printk("erases block %d fail\r\n",offset);
	        return -1;
		}
		offset +=blockSize;
	}

	/*write data*/
	offset = flash_addr;
	data = (u8 *)load_addr;
	for(index=0; index<page_count; index++)
	{
		ret = spi_flash->write(spi_flash, offset,pageSize, data, mode);
		
		printk(".");
		if(index%64 == 0)
			printk("\r\n");
		
		if(ret < 0)
		{
			printk("write page %d fail\r\n",offset);
	        return -1;
		}
		offset +=pageSize;
		data += pageSize;
	}

	return 0;
}

static int updata_flash_code(struct spi_flash* spi_flash,unsigned int updata_num,unsigned char mode)
{
    int ret = 0;
    switch (updata_num){
        case 0:
            ret = updata_flash(spi_flash,FLASH_SECONDBOOT_START_ADDR,FLASH_SECONDBOOT_SIZE_LIMIT,DEFAULT_SECONDBOOT_LOAD_ADDR,mode);
            break;
        case 1:
            ret = updata_flash(spi_flash,FLASH_DDRINIT_START_ADDR,FLASH_DDRINIT_SIZE_LIMIT,DEFAULT_DDRINIT_LOAD_ADDR,mode);
            break;
        case 2:
            ret = updata_flash(spi_flash,FLASH_UBOOT_START_ADDR,FLASH_UBOOT_SIZE_LIMIT,DEFAULT_UBOOT_LOAD_ADDR,mode);
            break;
        default:
            break;
    }
    return ret;
}

static int prog_2ndboot(struct spi_flash *flash, int mode)
{
    updata_flash_code(flash, 0, mode);
    return 0;
}

static int prog_ddrinit(struct spi_flash *flash, int mode)
{
    updata_flash_code(flash, 1, mode);
    return 0;
}

static int prog_uboot(struct spi_flash *flash, int mode)
{
    updata_flash_code(flash, 2, mode);
    return 0;
}

typedef struct
{
    const char *text;
    int (*prog_op)(struct spi_flash *, int);
} prog_menu_t;

static int prog_menu(struct spi_flash *flash, int mode, const prog_menu_t *menu, int count)
{
    int i, sel;
    char buff[32]={0};
    char *str = NULL, *str_end = NULL;
    const char *ROOT_MENU_KEY = "root@s5t";
    
    while (1) {
        printk("\r\n");
        for (i = 0; i < count; i++) {
            printk("%d:%s\r\n", i, menu[i].text);
        }
        printk("%d:quit\r\n", count);
        printk("select the function: ");
        serial_gets(buff);
        str = buff;
        while (*str == '\n' || *str == ' ') {
            //skip white
            str++;
        }

        if (strcmp(str, ROOT_MENU_KEY) == 0) {
            //request root menu
            return 1;
        }
        sel = (int)strtol(str, &str_end, 10);
        if (str == str_end || sel < 0 || sel > count) {
            //invalid sel
            continue;
        }
        if (sel == count) {
            //quit
            printk("\r\nquit\r\n");
            return 0;
        }
        if (menu[sel].prog_op) {
            menu[sel].prog_op(flash, mode);
        }
    }
    return 0;
}

static void prog_flash(void)
{
    prog_menu_t user_menu[] = {
        { "update uboot",           prog_uboot   },
    };
    prog_menu_t root_menu[] = {
        { "update second boot",     prog_2ndboot },
        { "update ddr init boot",   prog_ddrinit },
        { "update uboot",           prog_uboot   },
    };
    struct spi_flash *flash = NULL;
    unsigned char mode = 1;// or 4

    cadence_qspi_init(0, mode);
    flash = spi_flash_probe(0, 0, 31250000, 0, (u32)SPI_DATAMODE_8);

    printk("\r\n***************************************************\r\n");
    printk("*************** FLASH PROGRAMMING *****************\r\n");
    printk("***************************************************\r\n");
    if (prog_menu(flash, mode, &user_menu, sizeof(user_menu)/sizeof(user_menu[0]))) {
        prog_menu(flash, mode, &root_menu, ARRAY_SIZE(root_menu));
    }
}

void boot_from_chiplink(void)
{
	int bootdelay = 3;
	int abort = 0;
	char str[6];
	char *tmp;
	int ret=0;
	s32 usel;
	unsigned long ts;
	struct spi_flash* spi_flash;
	unsigned char mode = 1;// or 4

	while ((bootdelay > 0) && (!abort)) {
		--bootdelay;
		/* delay 1000 ms */
		ts = get_timer(0);
		do {
			if (serial_tstc()) {	/* we got a key press	*/
				abort  = 1; /* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/

				serial_getc();	/* consume input	*/
				break;
			}
			mdelay(100);
		} while (!abort && get_timer(ts) < 1000);

		printk("\b\b\b%d ", bootdelay);
	}
	
	if(1 == abort)
	{
		prog_flash();
#if 0
		cadence_qspi_init(0, mode);
		spi_flash = spi_flash_probe(0, 0, 31250000, 0, (u32)SPI_DATAMODE_8);
		
		printk("***************************************************\r\n");
		printk("***************VIC	DDR INIT BOOT ********************\r\n");
		printk("***************************************************\r\n");	
again:
		tmp = str;
		printk("0:updata second boot\r\n");  
		printk("1:updata ddr init boot\r\n"); 
		printk("2:updata uboot\r\n"); 
		printk("3:quit\r\n");
		printk("Select the function to test : ");
		serial_gets(str);
		
		if(str[0] == 0)
			goto again;

		while(*tmp == 4){tmp++;} /*skip EOT*/
		
		usel = atoi(tmp);
		if(usel > 3)
		{
			printk("error select,try again\r\n");
			goto again;
		}

		/*quit*/
		if(usel == 3)
			return;

		printk("usel:%d\n",usel);
		ret = updata_flash_code(spi_flash,usel,mode);
		if(ret < 0)
			printk("updata fail\r\n");
		else
			printk("updata success\r\n");

		goto again;
#endif
	}
}

void boot_from_uart(void)
{
	sys_cmd_proc();
}

void boot_from_sdio(void)
{
	boot_sdio_init();

	/*load uboot bin file from sd card*/
	boot_load_gpt_partition((void *)DEFAULT_UBOOT_ADDR,&gpt_guid_sifive_uboot);

	/*load kernel bin file from sd card*/
//	boot_load_gpt_partition((void *)DEFAULT_KERNEL_ADDR,&gpt_guid_sifive_kernel);
}


void boot_from_spi(int mode)
{
	struct spi_flash* spi_flash;
	int ret;
	u32	*addr;
	u32 val;

    cadence_qspi_init(0, mode);
	spi_flash = spi_flash_probe(0, 0, 31250000, 0, (u32)SPI_DATAMODE_8);
	/*load uboot*/
	load_data(spi_flash,DEFAULT_UBOOT_ADDR,DEFAULT_UBOOT_OFFSET,mode);
}

static int init_ddr(void)
{
  
  int fail_flag = 0;
  uint32_t tmp;
  uint32_t OMC_APB_BASE_ADDR, OMC_SECURE_APB_BASE_ADDR, PHY_APB_BASE_ADDR;
  uint64_t mem_addr;
  uint32_t wdata;
  int i = 0;
  u32 count = 0;
  
  uint64_t base_addr = U74_SYS_PORT_DDRC_BASE_ADDR;

  ///printf_led("Main start");

  //Set PLL to 15750M
  //_ASSERT_RESET_rstgen_rstn_ddrphy_apb_ //reset ddrphy,unvalid
  _SWITCH_CLOCK_clk_dla_root_SOURCE_clk_osc_sys_;
  //SCFG_PLL [31:24] [23:16] [15:8]   [7:4] [3]    [2]   [1] [0]
  //          OD      BWADJ  CLKFDIV  CLKR  bypass infb  pd  rst
#if defined(DDR_2133)
  MA_OUTW(syscon_sysmain_ctrl_SCFG_pll1_REG_ADDR,0x292905);//set reset
  udelay(10); //wait(500*(1/25M))
  MA_OUTW(syscon_sysmain_ctrl_SCFG_pll1_REG_ADDR,0x0292904);//clear reset
  udelay(10); //wait(500*(1/25M))
#elif defined(DDR_2800)
  MA_OUTW(syscon_sysmain_ctrl_SCFG_pll1_REG_ADDR,0x373705);//set reset
  udelay(10); //wait(500*(1/25M))
  MA_OUTW(syscon_sysmain_ctrl_SCFG_pll1_REG_ADDR,0x0373704);//clear reset
  udelay(10); //wait(500*(1/25M))
#elif defined(DDR_3200)
  MA_OUTW(syscon_sysmain_ctrl_SCFG_pll1_REG_ADDR,0x3f3f05);//set reset
  udelay(10); //wait(500*(1/25M))
  MA_OUTW(syscon_sysmain_ctrl_SCFG_pll1_REG_ADDR,0x03f3f04);//clear reset
  udelay(10); //wait(500*(1/25M))
#endif

  _SWITCH_CLOCK_clk_dla_root_SOURCE_clk_pll1_out_;
  //_CLEAR_RESET_rstgen_rstn_ddrphy_apb_ //clear reset of ddrphy,unvalid

  //ddrc_clock=400M test
  //_SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div4_;
  //_SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div4_;
  //ddrc_clock=800M
  //_SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div2_;
  //_SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div2_;
  //12.5M
  _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrosc_div2_;
  _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrosc_div2_; 
  
  _ENABLE_CLOCK_clk_ddrc0_;
  _ENABLE_CLOCK_clk_ddrc1_;

#if 0
  _ENABLE_CLOCK_clk_ddrphy_apb_ ;
  _CLEAR_RESET_rstgen_rstn_ddrphy_apb_ ;
    _ENABLE_CLOCK_clk_dla_bus_ ;
    _ENABLE_CLOCK_clk_dla_axi_ ;
    _ENABLE_CLOCK_clk_dlanoc_axi_ ;
    _ENABLE_CLOCK_clk_dla_apb_ ;
    _ENABLE_CLOCK_clk_dlaslv_axi_ ;


    _CLEAR_RESET_rstgen_rstn_dla_axi_ ;
    _CLEAR_RESET_rstgen_rstn_dlanoc_axi_ ;
    _CLEAR_RESET_rstgen_rstn_dla_apb_ ;
    _CLEAR_RESET_rstgen_rstn_dlaslv_axi_ ;
#endif

  //---- config ddrphy0/omc0 ----
  for(int ddr_num=0; ddr_num<2; ddr_num++) {
    if(ddr_num == 0) {
      OMC_APB_BASE_ADDR        = OMC_CFG0_BASE_ADDR;
      OMC_SECURE_APB_BASE_ADDR = OMC_CFG0_BASE_ADDR+0x1000;
      PHY_APB_BASE_ADDR        = DDRPHY0_CSR_BASE_ADDR;
    } else {
      OMC_APB_BASE_ADDR        = OMC_CFG1_BASE_ADDR;
      OMC_SECURE_APB_BASE_ADDR = OMC_CFG1_BASE_ADDR+0x1000;
      PHY_APB_BASE_ADDR        = DDRPHY1_CSR_BASE_ADDR;
    }
    //reg_wr_test;

    //`ifdef G_OPENEDGE_DDRPHY
    //  `include "noc/continue_wr/orbit_cfg/orbit_boot.c"
    //`else
#if 0
      #include "./ddrphy_cfg/regconfig.h.sim_PI.C"
      #include "./ddrphy_cfg/regconfig.h.sim_PHY.C"
#else
      regconfig_h_sim_pi(OMC_APB_BASE_ADDR,  OMC_SECURE_APB_BASE_ADDR,  PHY_APB_BASE_ADDR);
      regconfig_h_sim_phy(OMC_APB_BASE_ADDR,  OMC_SECURE_APB_BASE_ADDR,  PHY_APB_BASE_ADDR);
#endif
    
    //#include "./ddrc_cfg/lpddr4_1600_cl28_bl16/orbit_boot_8gx16.v"
    regconfig_pi_start(OMC_APB_BASE_ADDR,  OMC_SECURE_APB_BASE_ADDR,  PHY_APB_BASE_ADDR, ddr_num);
           
    if(ddr_num == 0) //ddrc_clock=12.5M
    {
      _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrosc_div2_;
      //_SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div4_;
    }
    else
    {
      _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrosc_div2_;
    }
    udelay(300);
    apb_write(PHY_APB_BASE_ADDR + (0 +0 << 2), 0x01);//release dll_rst_n
    udelay(300);
    
    orbit_boot(OMC_APB_BASE_ADDR,  OMC_SECURE_APB_BASE_ADDR,  PHY_APB_BASE_ADDR, ddr_num);

    //`endif
  }

 #if 1
    //while(1)
    {

        for(i = 0; i < 0x80000; i++)
        {
            writel(0xa5a5a5a5, 0x1000000000 + i *4);
            tmp = readl(0x1000000000 + i *4);
            if(tmp != 0xa5a5a5a5)
            {
                printk("error addr %d = 0x%x\r\n", i *4, tmp);
				fail_flag = -1;
            }

            writel(0x5a5a5a5a, 0x1000000000 + i *4);
            tmp = readl(0x1000000000 + i *4);
            if(tmp != 0x5a5a5a5a)
            {
                printk("error addr %d = 0x%x\r\n", i *4, tmp);
				fail_flag = -1;
            }


            writel(0x00000000, 0x1000000000 + i *4);
            tmp = readl(0x1000000000 + i *4);
            if(tmp != 0x00000000)
            {
                printk("error addr %d = 0x%x\r\n", i *4, tmp);
				fail_flag = -1;
            }



            writel(0xffffffff, 0x1000000000 + i *4);
            tmp = readl(0x1000000000 + i *4);
            if(tmp != 0xffffffff)
            {
                printk("error addr %d = 0x%x\r\n", i *4, tmp);
				fail_flag = -1;
            }

			if((i% 262144) == 0)						
			{								
				count++;								
				printk("ddr 0x%x, %dM test\r\n",(i * 4), count);						
			}
        }
    }
 #endif
}

/*only hartid 0 call this function*/
void BootMain(void)
{	
	int boot_mode = 0;
	int ret=0;

//	gpio_init();
	uart_init(3);
	printk("\r\n");

	ret = init_ddr();
	if(ret == 0)
	{
		_SET_SYSCON_REG_register68_SCFG_disable_u74_memaxi_remap(1);
#if defined(DDR_2133)
		  printk("DDR clk 2133M,Version: %s\r\n",VERSION);
#elif defined(DDR_2800)
		  printk("DDR clk 2800M,Version: %s\r\n",VERSION);
#elif defined(DDR_3200)
		  printk("DDR clk 3200M,Version: %s\r\n",VERSION);
#endif
	}
	else
		printk("End init lpddr4, test ddr fail\r\n");

#if (UBOOT_EXEC_AT_NBDLA_2M == 1)
	printk("init nbdla 2M ram\r\n");
	_SET_SYSCON_REG_register16_SCFG_nbdla_clkgating_en(1);
	_ENABLE_CLOCK_clk_dla_axi_;
	_ENABLE_CLOCK_clk_dlanoc_axi_;
	_ENABLE_CLOCK_clk_dla_apb_;
	_ENABLE_CLOCK_clk_dlaslv_axi_;
	_CLEAR_RESET_rstgen_rstn_dla_axi_;
	_CLEAR_RESET_rstgen_rstn_dlanoc_axi_;
	_CLEAR_RESET_rstgen_rstn_dla_apb_;
	_CLEAR_RESET_rstgen_rstn_dlaslv_axi_;
#endif  /* UBOOT_EXEC_AT_NBDLA_2M */

	boot_from_chiplink();
	
	boot_mode = get_boot_mode();
	switch(boot_mode){
	 	case 0:
		 	boot_from_spi(1);
			break;
	 	case 1:
		 	boot_from_spi(4);
		 	break;
		case 2:
		 	boot_from_sdio();
		 	break;
	 	case 4:
		 	boot_from_uart();
		 	break;
	 	case 6:
		 	boot_from_chiplink();
		 	break;

	 	default:
	 		break;		 
	}
	printk("\r\nbootloader.\r\n");
	writel(0x1, 0x2000004); 
}
