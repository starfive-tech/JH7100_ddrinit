// void inline orbit_writel(volatile void __iomem *addr, uint32_t data);
// void inline orbit_readl_poll(volatile void __iomem *addr, uint32_t expected, uint32_t strobe);
// void orbit_nsleep(int ns); // Sleep ns nano-seconds
// void udelay(int us); // Sleep us micro-seconds
//cdns_phy_initialize(1);
#include <regconfig.h.sim_PI.h>
#include <regconfig.h.sim_PHY.h>
#include <sys.h>
#include <clkgen_ctrl_macro.h>
#include <comdef.h>
#include <timer.h>
void orbit_boot(uint32_t OMC_APB_BASE_ADDR, uint32_t OMC_SECURE_APB_BASE_ADDR, uint32_t PHY_APB_BASE_ADDR, u32 ddr_num)
{
    u32 tmp = 0;
    int count = 0;
    int i = 0;
    //apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000401);
    apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001030);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x027fffff);
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
    udelay(300);
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
    udelay(300);
    apb_write(OMC_APB_BASE_ADDR + 0x50c, 0x00000001);
    // Waits tINIT3 (2 ms): Minimum CKE low time after RESET_n high
    udelay(3000);
    // Drive CKE high
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000003c);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    // Waits tINIT5 (2 us): Minimum idle time before first MRW/MRR command
    udelay(4);
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
    apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000f8);
    apb_write(OMC_APB_BASE_ADDR + 0x620, 0x03030404);
    apb_write(OMC_APB_BASE_ADDR + 0x624, 0x04030505);
    apb_write(OMC_APB_BASE_ADDR + 0x628, 0x07030884);
    apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x13150401);
    apb_write(OMC_APB_BASE_ADDR + 0x630, 0x17150604);
    apb_write(OMC_APB_BASE_ADDR + 0x634, 0x00110000);
    apb_write(OMC_APB_BASE_ADDR + 0x638, 0x200a0a08);
    apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x1730f803);
    apb_write(OMC_APB_BASE_ADDR + 0x640, 0x00080c00);
    apb_write(OMC_APB_BASE_ADDR + 0x644, 0xa0040007);
    apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x00081306);
    apb_write(OMC_APB_BASE_ADDR + 0x650, 0x04070304);
    apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00000404);
    apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
    apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00030008);
    apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x680, 0x00000603);
    apb_write(OMC_APB_BASE_ADDR + 0x684, 0x01000202);
    apb_write(OMC_APB_BASE_ADDR + 0x688, 0x0413040d);
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
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x300b0000);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30160000);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000010);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    // Waits tZQCAL (1 us)
    udelay(4);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000011);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000020);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    // Waits tZQCAL (1 us)
    udelay(4);
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000021);
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);

    tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    while((tmp & 0x00000002) != 0x00000002) {
      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    }


    //------ training start ------
    tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    while((tmp & 0x00000002) != 0x00000000) {
      tmp = apb_read(PHY_APB_BASE_ADDR + (0 +1 << 2)); //read freq_change_req_type
      if((tmp & 0x00000020) == 0x00000020) { //judge freq_change_req
        switch(tmp & 0x0000001f) { //judge freq_change_req_type
          case 0: 
            if(ddr_num == 0) //ddrc_clock=12.5M
            {
                _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrosc_div2_;
                udelay(100);
            }
            else
            {
                _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrosc_div2_;
                udelay(100);
            }
            break;
          case 1:
            if(ddr_num == 0) //ddrc_clock=200M
            {
                _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div4_;
                //_SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrosc_div2_;
                udelay(100);
            }
            else
            {
                _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div4_;
                //_SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrosc_div2_;
                udelay(100);
            }
            break;
          case 2:
            if(ddr_num == 0) //ddrc_clock=400M
            {
                _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div2_;
                //_SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrosc_div2_;
                udelay(100);
            }
            else
            {
                _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div2_;
                //_SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrosc_div2_;
                udelay(100);
            }
            break;
          default:
            break;
        }

        apb_write(PHY_APB_BASE_ADDR + (0 +2 << 2), 0x01); //set freq_change_ack
        tmp = apb_read(PHY_APB_BASE_ADDR + (0 +2 << 2)); //read freq_change_ack
#if 0
        if(count == 4)
        {
            tmp = apb_read(PHY_APB_BASE_ADDR + ((4096 + 1826) << 2));
            printf("start phy %d = 0x%x\r\n", i,(u32)tmp);
            udelay(100);
            tmp = apb_read(PHY_APB_BASE_ADDR + ((4096 + 1826) << 2));
            printf("post phy %d = 0x%x\r\n", i,(u32)tmp);
        }
#endif
        count++;
        //printf("count = 0x%x\r\n", count);
        while((tmp & 0x00000001) != 0x00000000) {
          tmp = apb_read(PHY_APB_BASE_ADDR + (0 +2 << 2)); //read freq_change_ack
        }
      }

      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);
    }
    //------ training end --------

    //cdns_pi_end( 3);
    //apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd11 <<2), tmp);//`DENALI_PI_11_DATA
    //apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd11 <<2), tmp & ~4'hF | 4'h3);//set PI_CS_MAP=3
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +83 <<2));//`DENALI_PI_83_DATA //read PI_INT_STATUS
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +84 <<2));//`DENALI_PI_84_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +84 <<2), tmp & 0xF8000000);

    //cdns_rdlvl_gate_tr_init( 3,0,0,0,0);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +46 <<2));//`DENALI_PI_46_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +46 <<2), tmp & ~(0xF <<24) | (0x3 <<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +33 <<2));//`DENALI_PI_33_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +33 <<2), tmp & ~(0x1<<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), tmp & ~(0x1<<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), tmp & ~(0x1<<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +45 <<2));//`DENALI_PI_45_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +45 <<2), tmp | 0x1);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +146 <<2));//`DENALI_PI_146_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +146 <<2), tmp | (0x3 <<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), tmp | (0x3 <<8));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), tmp | (0x3 <<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), tmp | (0x1 <<8));

    //cdns_rdlvl_tr_init( 3,0,0,0,0);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +46 <<2));//`DENALI_PI_46_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +46 <<2), tmp & ~(0xF <<16) | (0x3 <<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +33 <<2));//`DENALI_PI_33_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +33 <<2), tmp & ~(0x1<<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +43 <<2));//`DENALI_PI_43_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +43 <<2), tmp & ~(0x1<<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +43 <<2));//`DENALI_PI_43_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +43 <<2), tmp & ~(0x1<<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +44 <<2));//`DENALI_PI_44_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +44 <<2), tmp | 0x1);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +146 <<2));//`DENALI_PI_146_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +146 <<2), tmp | (0x3 <<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), tmp | 0x3);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +147 <<2));//`DENALI_PI_147_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +147 <<2), tmp | (0x3 <<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +148 <<2));//`DENALI_PI_148_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +148 <<2), tmp | (0x3 <<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +149 <<2));//`DENALI_PI_149_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +149 <<2), tmp | (0x3 <<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +150 <<2));//`DENALI_PI_150_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +150 <<2), tmp | (0x3 <<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +43 <<2));//`DENALI_PI_43_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +43 <<2), tmp | (0x1 <<8));

    //cdns_wdqlvl_tr_init( 3,0,0,0,0);
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp & ~(0xF <<8) | (0x3 <<8));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +68 <<2));//`DENALI_PI_68_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +68 <<2), tmp & ~(0x1<<8));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +71 <<2));//`DENALI_PI_71_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +71 <<2), tmp & ~(0x1<<16));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +71 <<2));//`DENALI_PI_71_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +71 <<2), tmp & ~(0x1<<24));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +72 <<2));//`DENALI_PI_72_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +72 <<2), tmp | (0x1 <<8));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +180 <<2));//`DENALI_PI_180_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +180 <<2), tmp | (0x3 <<8));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +183 <<2));//`DENALI_PI_183_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +183 <<2), tmp | (0x3 <<8));
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +186 <<2));//`DENALI_PI_186_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +186 <<2), tmp | (0x3 <<8));

    apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);
    apb_write(OMC_APB_BASE_ADDR + 0x620, 0x03031417);
    apb_write(OMC_APB_BASE_ADDR + 0x624, 0x08100608);
    apb_write(OMC_APB_BASE_ADDR + 0x628, 0x402d0894);
    apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x271e0b2b);
    apb_write(OMC_APB_BASE_ADDR + 0x630, 0x281b140a);
    apb_write(OMC_APB_BASE_ADDR + 0x634, 0x001c0000);
    apb_write(OMC_APB_BASE_ADDR + 0x638, 0x200f0f08);
    apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x28410a06);//twrcsgap[11:8]>=a(2*5), trdcsgap[3:0]>=6(2*3)
    apb_write(OMC_APB_BASE_ADDR + 0x640, 0x01331f8b);
    apb_write(OMC_APB_BASE_ADDR + 0x644, 0x1096012b);
    apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x00082714);
    apb_write(OMC_APB_BASE_ADDR + 0x650, 0x16442f0d);
    apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00001916);
    apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
    apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00600020);
    apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x680, 0x0c00040f);//trdcslat=0xe->0xc;trddata_en=0xe->0xf
    apb_write(OMC_APB_BASE_ADDR + 0x684, 0x03000604);//twrcslat=0x4->0x3;twrlat=0x6;twrdata=0x4
    apb_write(OMC_APB_BASE_ADDR + 0x688, 0x0415040d);
    apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20002c20);
    apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
    apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
    apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x202c0c00);
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30010036);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x3002001b);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30030031);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x300b0066);
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
