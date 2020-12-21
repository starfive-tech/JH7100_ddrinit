
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
    //apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000401);//ignore_phy_rresp=1(Ignore dfi_rddata_valid from PHY,Only used in booting period);dram type=LPDDR4
    apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001030);//enable address region 0; 2ranks; bit15?
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00000001);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);//region0_start //0x00800000
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x027fffff);//region0_end,SOC addr shift right 8bits //0x01ffffff
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf18, 0x00000001);//2-channel interleaving
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x0f000031);//chip0:rank_size=128MB*(f+1)(2GB?);bg=0;row_width=16;col_width=10
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf34, 0x0f000031);//chip1
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x110, 0xc0000001);//region0 attribute: allow secure write/read;enable security check for this region
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x114, 0xffffffff);//region0: [31:16]:nsaid_wr_en(allow non-secure write); [15:0]:nsaid_rd_en(allow non-secure read)
    apb_write(OMC_APB_BASE_ADDR + 0x10c, 0x00000505);//Temperature monitor reads to determine the maximum re-fresh interval
    apb_write(OMC_APB_BASE_ADDR + 0x11c, 0x00000000);//nop(byte lane no inversion,temperature monitor used)
    apb_write(OMC_APB_BASE_ADDR + 0x500, 0x00000201);//enable retimer for read data; freq_ratio
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000100);//phymstr_ack_disable=1
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);//Keep OMC idle for additional idle_num_upd cycles (ACLK) after dfi_phyupd_ack assertion
    apb_write(OMC_APB_BASE_ADDR + 0xea8, 0x00040000);//Keep OMC idle for additional idle_num_upd cycles (ACLK) after dfi_phyupd_ack assertion
    // Memory frequency should be changed below 50MHz somewhere before here
    apb_write(OMC_APB_BASE_ADDR + 0x504, 0x40000000);//assert dfi_init_start
    udelay(300);
    //cdns_dll_rst_deassert()

    tmp = apb_read(OMC_APB_BASE_ADDR + 0x504);//wait for dfi_init_complete
    while((tmp & 0x80000000) != 0x80000000) {
      udelay(1);
      tmp = apb_read(OMC_APB_BASE_ADDR + 0x504);
    }

    apb_write(OMC_APB_BASE_ADDR + 0x504, 0x00000000);//de-assert dfi_init_start; de-assert dfi_ctrlupd_req on exit from self-refresh mode
    // tINIT0 is controlled by System
    apb_write(OMC_APB_BASE_ADDR + 0x50c, 0x00000000);//Drive DRAM_RST_N low
    // Waits tINIT1 (200 us): Minimum RESET_n LOW time after completion of voltage ramp
    udelay(300);
    apb_write(OMC_APB_BASE_ADDR + 0x50c, 0x00000001);//Drive DRAM_RST_N high
    // Waits tINIT3 (2 ms): Minimum CKE low time after RESET_n high
    udelay(3000);
    // Drive CKE high
    apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000003c);//dram_command_cs: rank0/1; dram_command: power-down exit
    apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);//dram_command_start
    // Waits tINIT5 (2 us): Minimum idle time before first MRW/MRR command
    udelay(4);
    apb_write(OMC_APB_BASE_ADDR + 0x310, 0x00020000);//dfs_dfi_frequency=2
    apb_write(OMC_APB_BASE_ADDR + 0x310, 0x00020001);//dfs_dfi_init_start_toggle; assert dfi_init_start when dfi_frequency change
    // Write down RCLK-related CRs
    apb_write(OMC_APB_BASE_ADDR + 0x600, 0x002e0176);//Per-bank(31:16)/all-bank(15:0) refresh interval in OSC clock cycles for each of eight MR4 values.
    apb_write(OMC_APB_BASE_ADDR + 0x604, 0x002e0176);
    apb_write(OMC_APB_BASE_ADDR + 0x608, 0x001700bb);
    apb_write(OMC_APB_BASE_ADDR + 0x60c, 0x000b005d);
    apb_write(OMC_APB_BASE_ADDR + 0x610, 0x0005002e);
    apb_write(OMC_APB_BASE_ADDR + 0x614, 0x00020017);
    apb_write(OMC_APB_BASE_ADDR + 0x618, 0x00020017);
    apb_write(OMC_APB_BASE_ADDR + 0x61c, 0x00020017);//Per-bank(31:16)/all-bank(15:0) refresh interval in OSC clock cycles for each of eight MR4 values.
    apb_write(OMC_APB_BASE_ADDR + 0x678, 0x00000019);//t_zqcal(lpddr4) >= 1us(0x640) 
    apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000f8);//abr_en[7:0]
    apb_write(OMC_APB_BASE_ADDR + 0x620, 0x03030404);//error: tCKCKEH(tCKSRX)>=3,tCMDCKE>=3,tRPpb(tRP)>=29,tRPab(tRP)>=34
    apb_write(OMC_APB_BASE_ADDR + 0x624, 0x04030505);//error: tCKE>=12,tSR(tCKESR)>=24,tCKELCK>=8,tXP>=12
    apb_write(OMC_APB_BASE_ADDR + 0x628, 0x07030884);
    apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x13150401);
    apb_write(OMC_APB_BASE_ADDR + 0x630, 0x17150604);
    apb_write(OMC_APB_BASE_ADDR + 0x634, 0x00110000);
    apb_write(OMC_APB_BASE_ADDR + 0x638, 0x200a0a08);
    apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x1730f803);//t_add_wr_p
    apb_write(OMC_APB_BASE_ADDR + 0x640, 0x00080c00);
    apb_write(OMC_APB_BASE_ADDR + 0x644, 0xa0040007);
    apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x00081306);
    apb_write(OMC_APB_BASE_ADDR + 0x650, 0x04070304);
    apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00000404);
    apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
    apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00030008);
    apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
    apb_write(OMC_APB_BASE_ADDR + 0x680, 0x00000603);//DFI_timing
    apb_write(OMC_APB_BASE_ADDR + 0x684, 0x01000202);
    apb_write(OMC_APB_BASE_ADDR + 0x688, 0x0413040d);
    apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20002420);
    apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
    apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
    apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);//dfi_tlvl_max=0, dfi_tctrlupd_interval=0
    apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20240c00);
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30010006);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30020000);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30030031);
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x300b0066); //0x66->0x66
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30160016); //0x06->0x16
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
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);//phymstr_ack_disable=0

    tmp = apb_read(OMC_APB_BASE_ADDR + 0x518);//wait for dfi_phymstr_ack=1
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
            if(ddr_num == 0) //ddrc_clock=1600/8=200M
            {
                _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div8_;
                udelay(100);
            }
            else
            {
                _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div8_;
                udelay(100);
            }
            break;
          case 2:
            if(ddr_num == 0) //ddrc_clock=1600/2=800M
            {
                _SWITCH_CLOCK_clk_ddrc0_SOURCE_clk_ddrpll_div2_;
                udelay(100);
            }
            else
            {
                _SWITCH_CLOCK_clk_ddrc1_SOURCE_clk_ddrpll_div2_;
                udelay(100);
            }
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

    apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);//abr_en[7:5]
    apb_write(OMC_APB_BASE_ADDR + 0x620, 0x04041d22);//tCKCKEH(tCKSRX)[31:24]>=3,tCMDCKE[18:16]>=3,tRPpb(tRP)[13:8]>=0x1D,tRPab(tRP)>=0x22
    apb_write(OMC_APB_BASE_ADDR + 0x624, 0x0c18080c);//tCKE[28:24]>=0xc,tSR(tCKESR)[21:16]>=0x18,tCKELCK[12:8]>=8,tXP[4:0]>=0xc
    apb_write(OMC_APB_BASE_ADDR + 0x628, 0x60440c9d);//tRC[31:24]>=(0x60||0x63)/tCK,tRAS[22:16]>=0x44,tRTP[13:8]>=0xc,adv_al[7]=1,tRCD[5:0]>=0x1d
    apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x34271140);//t_wra[30:24]>=,t_wtra[21:16]>=,tRRD[12:8]>=0x10,tfaw[6:0]>=0x40
    apb_write(OMC_APB_BASE_ADDR + 0x630, 0x36231c0e);//t_rtrrd[30:24]>=,t_wrwtr[21:16]>=,RL[13:8]>=,WL[5:0]>=
    apb_write(OMC_APB_BASE_ADDR + 0x634, 0x00230000);//t_rtw[21:16]>=
    apb_write(OMC_APB_BASE_ADDR + 0x638, 0x20171708);//t_ccdmw[29:24]>=,t_mrw[21:16]>=,t_mrd[13:8]>=,t_mrr[3:0]>=8
    apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x36420a06);//t_rtrrd[30:24]>=,n_add_wr_p[23:20]>=,t_add_wr_p[19:16]>=,twrcsgap[11:8]>=a(2*5), trdcsgap[3:0]>=6(2*3)
    apb_write(OMC_APB_BASE_ADDR + 0x640, 0x01cf2cd6);//t_ccd_gap[30:28]>=?,t_xsr[25:16]>=0x1CC(287.5ns_8gibx16),t_xp_mrri[14:8]>=31, t_refdpr[7:0]>=?
    apb_write(OMC_APB_BASE_ADDR + 0x644, 0x01320262);//t_wtrcr[31:28]>=?, t_rfcpb[25:16]>=0xe0(0x130),t_rfcab[9:0]>=0x1c0(0x260)
    apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);//t_pbr2pbr[7:0]=0x00
    apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x000d351e);//n_rtp[21:16]>=0xC, n_wr_a[14:8]>=0x35, n_wr[5:0]>=0x1e
    apb_write(OMC_APB_BASE_ADDR + 0x650, 0x20664713);//t_rcd_derate[29:24]>=0x20,t_rc_derate[23:16]>=0x66(per-bank-precharge),t_ras_derate[14:8]>=0x47,t_rrd_derate[4:0]>=0x13
    apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00002520);//t_rpab_derate[13:8]>=0x,t_rppb_derate[5:0]>=0x
    apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);//delay_wrexit_cyc_th[11:0]=?
    apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00900030);//t_zqcs[23:16]=0(lpddr4), t_zqlatch[9:0]>=0x20
    apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);//pwdn_entry_th[31:16], sr_entry_th[15:0]
    apb_write(OMC_APB_BASE_ADDR + 0x680, 0x14000416);//trdcslat=0x16->0x14;trddata_en=0x16->0x16
    apb_write(OMC_APB_BASE_ADDR + 0x684, 0x07000a04);//twrcslat=0x8->0x7;twrlat=0xa;twrdata=0x4
    apb_write(OMC_APB_BASE_ADDR + 0x688, 0x0515040e);//dfi_tctrl_delay[31:24]=4+1||5+1;dfi_twrdata_delay[23:16]=0x5(phy_ctrl);dfi_tdram_clk_disable[15:8]=2+1||3+1(phy);dfi_tdram_clk_enable[7:0]=2+1||3+1;
    apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20003320);//dfi_trdlvl_en[31:24];dfi_trdlvl_rr[15:8];dfi_twrlvl_en[7:0]
    apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);//dfi_twrlvl_ww[23:16];
    apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);//dfi_tctrlupd_max[25:16]>=0x124??;dfi_tctrlupd_min[9:0]==0x74;
    apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);//dfi_tlvl_max[31:16]=0; dfi_tctrlupd_interval[9:0]=0???;
    apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20330c00);//dfi_twdqlvl_en[31:24];dfi_twdqlvl_rw[23:16];dfi_twdqlvl_ww[15:8];
    apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);//idle_num_upd[31:16];dfi_tphymstr_rfsh[13:8];dfi_twdqlvl_resp[7:0];
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30010056);//nWR=30,BL=on the fly
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x3002002d);//WL/RL for 1333M~1600M
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30030031);//PDDS=RZQ/6,PU-CAl=VDDQ/3(default)
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x300b0066); //0x06->0x66
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x30160016);//soc_ODT=RZQ/6, 0x06->0x1e->0x16
    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
//    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x300c0015); //vref(ca)
//    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
//    apb_write(OMC_APB_BASE_ADDR + 0x4, 0x300e0018); //vref(dq)
//    apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
    apb_write(OMC_APB_BASE_ADDR + 0x410, 0x00101010);//generation/urgent<->oldest request
    apb_write(OMC_APB_BASE_ADDR + 0x420, 0x0c181006);
    apb_write(OMC_APB_BASE_ADDR + 0x424, 0x20200820);
    apb_write(OMC_APB_BASE_ADDR + 0x428, 0x80000020);
    apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000001);//ignore_phy_rresp=0(Ignore dfi_rddata_valid from PHY,Only used in booting period);dram type=LPDDR4
    apb_write(OMC_APB_BASE_ADDR + 0x108, 0x00003000);
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x704, 0x00000007);//enable scrambler/axi/apb logic clock gating
    apb_write(OMC_APB_BASE_ADDR + 0x330, 0x09313fff);//enable various clock gating
    apb_write(OMC_APB_BASE_ADDR + 0x508, 0x00000013);//drive ODT pin for rank0/rank1; ODT pins is always driven by fixed_odt_cs
    apb_write(OMC_APB_BASE_ADDR + 0x324, 0x00002000);//disable dynamic DRAM power down/self-refresh;wait until enough auto-refresh is issued.
    apb_write(OMC_APB_BASE_ADDR + 0x104, 0x90000000);//Enable auto-refresh;Opportunistic selection of target bank refresh to reduce bank confliction
    apb_write(OMC_APB_BASE_ADDR + 0x510, 0x00000100);//Issue {num_phyupd_abr} number of all-bank-refresh commands before ac-knowledging phyupd_req.
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);//phymstr_ack_disable=0
    apb_write(OMC_SECURE_APB_BASE_ADDR + 0x700, 0x00000003);//CR_UPDATE_CTRL
    apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000600);//Wait dfi_phymstr_* handshake after (POWER Gating mode/DFS completed) exited
    apb_write(OMC_APB_BASE_ADDR + 0x20, 0x00000001);//DRAM_INIT_DONE
}
//cdns_per_tr_longcnt_set( 16);
// Initialization done
