/**
  ******************************************************************************
  * @file  orbit_boot_training.c
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

#include <regconfig.h.sim_PI.h>
#include <regconfig.h.sim_PHY.h>
#include <sys.h>
#include <clkgen_ctrl_macro.h>
#include <comdef.h>
#include <timer.h>
void lpddr4_1600_orbit_boot_training(uint32_t OMC_APB_BASE_ADDR, uint32_t OMC_SECURE_APB_BASE_ADDR, uint32_t PHY_APB_BASE_ADDR, u32 ddr_num)
{
    u32 tmp = 0;
    apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000401);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001030);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);//region0_start //0x00800000
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x027fffff);//region0_end,SOC addr shift right 8bits //0x01ffffff
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf18, 0x00000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x0f000031);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf34, 0x0f000031);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x110, 0xc0000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x114, 0xffffffff);
    apb_write(OMC_APB_BASE_ADDR + 0x10c, 0x00000505);
    apb_write(OMC_APB_BASE_ADDR + 0x11c, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x500, 0x00000201);
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000100);
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
    apb_write(OMC_APB_BASE_ADDR + 0xea8, 0x00040000);
    // Memory frequency should be changed below 50MHz somewhere before here
    apb_write(OMC_APB_BASE_ADDR + 0x504, 0x40000000);
    //cdns_dll_rst_deassert()

    tmp = apb_read(OMC_APB_BASE_ADDR + 0x504);
    while((tmp & 0x80000000) != 0x80000000) {
      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x504);
    }

    apb_write(OMC_APB_BASE_ADDR + 0x504, 0x00000000);
    // tINIT0 is controlled by System
    apb_write(OMC_APB_BASE_ADDR + 0x50c, 0x00000000);
    // Waits tINIT1 (200 us): Minimum RESET_n LOW time after completion of voltage ramp
    udelay(200);
    apb_write(OMC_APB_BASE_ADDR + 0x50c, 0x00000001);
    // Waits tINIT3 (2 ms): Minimum CKE low time after RESET_n high
    udelay(2000);
    // Drive CKE high
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000003c);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    // Waits tINIT5 (2 us): Minimum idle time before first MRW/MRR command
    udelay(2);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000010);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    // Waits tZQCAL (1 us)
    udelay(1);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000011);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000020);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    // Waits tZQCAL (1 us)
    udelay(1);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000021);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    apb_write(OMC_APB_BASE_ADDR + 0x310, 0x00020000);
    apb_write(OMC_APB_BASE_ADDR + 0x310, 0x00020001);
    // Write down RCLK-related CRs
    apb_write(OMC_APB_BASE_ADDR + 0x600, 0x002e0176);
    apb_write(OMC_APB_BASE_ADDR + 0x604, 0x002e0176);
    apb_write(OMC_APB_BASE_ADDR + 0x608, 0x001700bb);
    apb_write(OMC_APB_BASE_ADDR + 0x60c, 0x000b005d);
    apb_write(OMC_APB_BASE_ADDR + 0x610, 0x0005002e);
    apb_write(OMC_APB_BASE_ADDR + 0x614, 0x00020017);
    apb_write(OMC_APB_BASE_ADDR + 0x618, 0x00020017);
    apb_write(OMC_APB_BASE_ADDR + 0x61c, 0x00020017);
    apb_write(OMC_APB_BASE_ADDR + 0x678, 0x00000019);
    apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000fc);
    apb_write(OMC_APB_BASE_ADDR + 0x620, 0x03030404);
    apb_write(OMC_APB_BASE_ADDR + 0x624, 0x04030505);
    apb_write(OMC_APB_BASE_ADDR + 0x628, 0x07030884);//
    apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x13150401);
    apb_write(OMC_APB_BASE_ADDR + 0x630, 0x17150604);
    apb_write(OMC_APB_BASE_ADDR + 0x634, 0x000d0000);
    apb_write(OMC_APB_BASE_ADDR + 0x638, 0x200a0a08);
    apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x17300803);
    apb_write(OMC_APB_BASE_ADDR + 0x640, 0x00060c00);
    apb_write(OMC_APB_BASE_ADDR + 0x644, 0xa0030006);
    apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x00081306);//
    apb_write(OMC_APB_BASE_ADDR + 0x650, 0x04070304);
    apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00000404);
    apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
    apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00020008);
    apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x680, 0x00000400);
    apb_write(OMC_APB_BASE_ADDR + 0x684, 0x02000202);
    apb_write(OMC_APB_BASE_ADDR + 0x688, 0x04130409);
    apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20002420);
    apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
    apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
    apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20240c00);
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30010006);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30020000);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30030031);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30160006);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);

    tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    while((tmp & 0x00000002) != 0x00000002) {
      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    }


    //------ training start ------
    tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);//wait for dfi_phymstr_ack=0
    while((tmp & 0x00000002) != 0x00000000) {
      tmp = apb_read(PHY_APB_BASE_ADDR + (0 +1 << 2)); //read freq_change_req_type
      if((tmp & 0x00000020) == 0x00000020) { //judge freq_change_req
        switch(tmp & 0x0000001f) { //judge freq_change_req_type
          case 0: 
            if(ddr_num == 1) //ddrc_clock=12.5M
              _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrosc_div2_
            else
              _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrosc_div2_
            break;
          case 1:
            if(ddr_num == 1) //ddrc_clock=200M
              _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div8_
            else
              _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div8_
            break;
          case 2:
            if(ddr_num == 1) //ddrc_clock=800M
              _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div2_
            else
              _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div2_
            break;
          default:
            break;
        }

        apb_write(PHY_APB_BASE_ADDR + (0 +2 << 2), 0x01); //set freq_change_ack
        tmp = apb_read(PHY_APB_BASE_ADDR + (0 +2 << 2)); //read freq_change_ack
        while((tmp & 0x00000001) != 0x00000000) {
          tmp = apb_read(PHY_APB_BASE_ADDR + (0 +2 << 2)); //read freq_change_ack
        }
      }

      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x518); //read dfi_phymstr_ack
    }
    //------ training end --------

    tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    while((tmp & 0x00000002) != 0x00000000) {
      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    }

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +11 <<2));//`DENALI_PI_11_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +11 <<2), 0x03000803);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +83 <<2));//`DENALI_PI_83_DATA
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +84 <<2));//`DENALI_PI_84_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +84 <<2), 0x00000000);
    //cdns_pi_end( 3);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +46 <<2));//`DENALI_PI_46_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +46 <<2), 0x030f0000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +33 <<2));//`DENALI_PI_33_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +33 <<2), 0x00000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), 0x01000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), 0x00000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +45 <<2));//`DENALI_PI_45_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +45 <<2), 0x00000001);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +146 <<2));//`DENALI_PI_146_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +146 <<2), 0x03011808);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), 0x01010301);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), 0x03010301);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), 0x00000100);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +46 <<2));//`DENALI_PI_46_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +46 <<2), 0x03030000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +33 <<2));//`DENALI_PI_33_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +33 <<2), 0x00000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +43 <<2));//`DENALI_PI_43_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +43 <<2), 0x01000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +43 <<2));//`DENALI_PI_43_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +43 <<2), 0x00000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), 0x00000101);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +146 <<2));//`DENALI_PI_146_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +146 <<2), 0x03031808);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), 0x03010303);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), 0x03030303);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +148 <<2));//`DENALI_PI_148_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +148 <<2), 0x03170a0a);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +149 <<2));//`DENALI_PI_149_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +149 <<2), 0x03000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +150 <<2));//`DENALI_PI_150_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +150 <<2), 0x03000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +43 <<2));//`DENALI_PI_43_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +43 <<2), 0x00000100);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), 0x01010300);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +68 <<2));//`DENALI_PI_68_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +68 <<2), 0x34000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +71 <<2));//`DENALI_PI_71_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +71 <<2), 0x01000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +71 <<2));//`DENALI_PI_71_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +71 <<2), 0x00000000);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +72 <<2));//`DENALI_PI_72_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +72 <<2), 0x00000100);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +180 <<2));//`DENALI_PI_180_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +180 <<2), 0x06000301);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +183 <<2));//`DENALI_PI_183_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +183 <<2), 0x01000301);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +186 <<2));//`DENALI_PI_186_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +186 <<2), 0x13000301);
    //cdns_rdlvl_gate_tr_init( 3,0,0,0,0);

    //cdns_rdlvl_tr_init( 3,0,0,0,0);

    //cdns_wdqlvl_tr_init( 3,0,0,0,0);

    apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);
    apb_write(OMC_APB_BASE_ADDR + 0x620, 0x04031d22);
    apb_write(OMC_APB_BASE_ADDR + 0x624, 0x0c18080c);
    apb_write(OMC_APB_BASE_ADDR + 0x628, 0x60440d9d);//bit[13:8]:tRTP>=0xC
    apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x34271140);//tRRD[12:8]>=0x10
    apb_write(OMC_APB_BASE_ADDR + 0x630, 0x36231c0e);
    apb_write(OMC_APB_BASE_ADDR + 0x634, 0x001e0000);
    apb_write(OMC_APB_BASE_ADDR + 0x638, 0x20171708);
    apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x36420806);
    apb_write(OMC_APB_BASE_ADDR + 0x640, 0x01cf2cd6);//tXSR>=0x1CC(287.5ns_8gibx16);tXP+tmrri?
    apb_write(OMC_APB_BASE_ADDR + 0x644, 0x00e101c1);//lpddr4:tRFCpb>=0xe0,tRFCab>=0x1c0
    apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x000d351e);//bit[21:16]:tRTP>=0xC
    apb_write(OMC_APB_BASE_ADDR + 0x650, 0x20664713);
    apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00002520);
    apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
    apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00900030);
    apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x680, 0x14000416);//trdcslat=0x16->0x14;trddata_en=0x16->0x16
    apb_write(OMC_APB_BASE_ADDR + 0x684, 0x08000a04);//twrcslat=0x8(-1);twrlat=0xa(-1);twrdata=0x4
    apb_write(OMC_APB_BASE_ADDR + 0x688, 0x0415040a);
    apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20003320);
    apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
    apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
    apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20330c00);
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30010056);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x3002002d);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30030031);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30160006);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x410, 0x00101010);
    apb_write(OMC_APB_BASE_ADDR + 0x420, 0x0c181006);
    apb_write(OMC_APB_BASE_ADDR + 0x424, 0x20200820);
    apb_write(OMC_APB_BASE_ADDR + 0x428, 0x80000020);
    apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000001);
    apb_write(OMC_APB_BASE_ADDR + 0x108, 0x00003000);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x704, 0x00000007);
    apb_write(OMC_APB_BASE_ADDR + 0x330, 0x09313fff);
    apb_write(OMC_APB_BASE_ADDR + 0x508, 0x00000033);
    apb_write(OMC_APB_BASE_ADDR + 0x324, 0x00002000);
    apb_write(OMC_APB_BASE_ADDR + 0x104, 0x90000000);
    apb_write(OMC_APB_BASE_ADDR + 0x510, 0x00000100);
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x700, 0x00000003);
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000600);
    apb_write(OMC_APB_BASE_ADDR + 0x20, 0x00000001);
}
//cdns_per_tr_longcnt_set( 16);

// Initialization done
