/**
  ******************************************************************************
  * @file  sys.h
  * @author  StarFive Technology
  * @version  V1.0
  * @date  07/22/2020
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

#ifndef SYS_H_
#define SYS_H_

/*********************************** sys clock************************/

#define E32_ADDR_REMAP_OFFSET 0
#define U74_SYS_PORT_DDRC_BASE_ADDR 0x1000000000LL
#define OMC_CFG0_BASE_ADDR (E32_ADDR_REMAP_OFFSET+0x0011000000LL)
#define OMC_CFG1_BASE_ADDR (E32_ADDR_REMAP_OFFSET+0x0011010000LL)
#define DDRPHY0_CSR_BASE_ADDR (E32_ADDR_REMAP_OFFSET+0x0011820000LL)
#define DDRPHY1_CSR_BASE_ADDR (E32_ADDR_REMAP_OFFSET+0x0011830000LL)


#define QSPI_CLK            100000000
#define QSPI_REF_CLK        250000000
#define TIMER_CLK_HZ		6250000


#define DEFAULT_SD_BOOT_LOAD_BLOCK  1000

#define DEFAULT_SPI_BOOT_START_ADDR 1
#define DEFAULT_SPI_BOOT_SIZE_ADDR 0x0

#define DEFAULT_SPI_BOOT_LOAD_PAGES     512

// QSPI
#define QSPI_BASE_ADDR		0x11860000
//CADENCE QSPI AHB
#define QSPI_BASE_AHB_ADDR  0x20000000

// TIMER --WDT
#define TIMER_BASE_ADDR         0x12480000

// SDIO0
#define SDIO0_BASE_ADDR     0x10000000

// SDIO1
#define SDIO1_BASE_ADDR     0x10010000


// GPIO
#define GPIO_BASE_ADDR		0x11910000///0xA9060000
#define EZGPIO_FULLMUX_BASE_ADDR		0x11910000
#define RSTGEN_BASE_ADDR                0x11840000
#define CLKGEN_BASE_ADDR                0x11800000
// CLKGEN
#define VIC_CLKGEN_TOP_SV_BASE_ADDR 0x11800000///0xA9090000

// RSTGEN
#define VIC_RSTGEN_BASE_ADDR 0x11840000///0xA90A0000
#define SYSCON_IOPAD_CTRL_BASE_ADDR     0x11858000
// SYSCON
#define SYSCON_SYSMAIN_CTRL_BASE_ADDR	0x11850000

// HS-UART0
#define UART0_HS_BASE_ADDR                 0x11870000

// HS-UART1
#define UART1_HS_BASE_ADDR              0x11880000

// UART2
#define UART2_BASE_ADDR                 0x12430000

// UART3
#define UART3_BASE_ADDR                 0x12440000


/*Reserved information:
 *second boot:64K  0x0 - 0xffff 
 *ddr init:64K		0x10000 - 0x1ffff
 *uboot:1M     		0x20000 - 0x11ffff
 *kernel:25M		0x120000 - 0x1a1ffff
*/

/*Reserved information:
 *second boot:32K  0x0 - 0x7fff
 *ddr init:50K		0x8000 - 0x147ff
 *uboot:500K     	0x14800 - 0x917ff
 *kernel:25M		0x130000 - 0x1a3ffff
*/

#define UBOOT_EXEC_AT_NBDLA_2M  0
#define UBOOT_EXEC_AT_SYS_PORT  0

/*DDR bin file */
#define DEFAULT_DDR_ADDR		0x18080000
#define DEFAULT_DDR_OFFSET		0x100

/*uboot bin file*/
#if (UBOOT_EXEC_AT_NBDLA_2M == 1)
#define DEFAULT_UBOOT_ADDR		0x19c00000
#elif (UBOOT_EXEC_AT_SYS_PORT == 1)
#define DEFAULT_UBOOT_ADDR      0x1000000000
#else
#define DEFAULT_UBOOT_ADDR		0x80000000
#endif
#define DEFAULT_UBOOT_OFFSET	0x400

/*kernel image file*/
#define DEFAULT_KERNEL_ADDR		0xa0000000
#define DEFAULT_KERNEL_OFFSET	0x1200


#define FLASH_SECONDBOOT_START_ADDR     0
#define FLASH_SECONDBOOT_SIZE_LIMIT     (64*1024)
#define FLASH_DDRINIT_START_ADDR        0x10000
#define FLASH_DDRINIT_SIZE_LIMIT        (192*1024)
#define FLASH_UBOOT_START_ADDR          0x40000
#define FLASH_UBOOT_SIZE_LIMIT          (10*1024*1024)

#define DEFAULT_UBOOT_LOAD_ADDR  DEFAULT_UBOOT_ADDR
#define DEFAULT_SECONDBOOT_LOAD_ADDR	0x180a8000 /*32k*/
#define DEFAULT_DDRINIT_LOAD_ADDR	0x18000000	/*128K*/

#endif
