// void inline orbit_writel(volatile void __iomem *addr, uint32_t data);
// void inline orbit_readl_poll(volatile void __iomem *addr, uint32_t expected, uint32_t strobe);
// void orbit_nsleep(int ns); // Sleep ns nano-seconds
// void orbit_usleep(int us); // Sleep us micro-seconds
//cdns_phy_initialize(1);
apb_write(OMC_APB_BASE_ADDR + 32'h0, 32'h00000401);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf00, 32'h40001030);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf04, 32'h00000001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf10, 32'h00800000);//region0_start //32'h00800000
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf14, 32'h027fffff);//region0_end,SOC addr shift right 8bits //0x01ffffff
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf18, 32'h00000001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf30, 32'h0f000031);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'hf34, 32'h0f000031);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'h110, 32'hc0000001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'h114, 32'hffffffff);
apb_write(OMC_APB_BASE_ADDR + 32'h10c, 32'h00000505);
apb_write(OMC_APB_BASE_ADDR + 32'h11c, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h500, 32'h00000201);
apb_write(OMC_APB_BASE_ADDR + 32'h514, 32'h00000100);
apb_write(OMC_APB_BASE_ADDR + 32'h6a8, 32'h00040000);
apb_write(OMC_APB_BASE_ADDR + 32'hea8, 32'h00040000);
// Memory frequency should be changed below 50MHz somewhere before here
apb_write(OMC_APB_BASE_ADDR + 32'h504, 32'h40000000);
//cdns_dll_rst_deassert()

apb_read(OMC_APB_BASE_ADDR + 32'h504, tmp);
while((tmp & 32'h80000000) != 32'h80000000) begin
  orbit_usleep(1);
  apb_read(OMC_APB_BASE_ADDR + 32'h504, tmp);
end

apb_write(OMC_APB_BASE_ADDR + 32'h504, 32'h00000000);
// tINIT0 is controlled by System
apb_write(OMC_APB_BASE_ADDR + 32'h50c, 32'h00000000);
// Waits tINIT1 (200 us): Minimum RESET_n LOW time after completion of voltage ramp
orbit_usleep(200);
apb_write(OMC_APB_BASE_ADDR + 32'h50c, 32'h00000001);
// Waits tINIT3 (2 ms): Minimum CKE low time after RESET_n high
orbit_usleep(2000);
// Drive CKE high
apb_write(OMC_APB_BASE_ADDR + 32'h10, 32'h0000003c);
apb_write(OMC_APB_BASE_ADDR + 32'h14, 32'h00000001);
// Waits tINIT5 (2 us): Minimum idle time before first MRW/MRR command
orbit_usleep(2);
apb_write(OMC_APB_BASE_ADDR + 32'h10, 32'h00000010);
apb_write(OMC_APB_BASE_ADDR + 32'h14, 32'h00000001);
// Waits tZQCAL (1 us)
orbit_usleep(1);
apb_write(OMC_APB_BASE_ADDR + 32'h10, 32'h00000011);
apb_write(OMC_APB_BASE_ADDR + 32'h14, 32'h00000001);
apb_write(OMC_APB_BASE_ADDR + 32'h10, 32'h00000020);
apb_write(OMC_APB_BASE_ADDR + 32'h14, 32'h00000001);
// Waits tZQCAL (1 us)
orbit_usleep(1);
apb_write(OMC_APB_BASE_ADDR + 32'h10, 32'h00000021);
apb_write(OMC_APB_BASE_ADDR + 32'h14, 32'h00000001);
apb_write(OMC_APB_BASE_ADDR + 32'h310, 32'h00020000);
apb_write(OMC_APB_BASE_ADDR + 32'h310, 32'h00020001);
// Write down RCLK-related CRs
apb_write(OMC_APB_BASE_ADDR + 32'h600, 32'h002e0176);
apb_write(OMC_APB_BASE_ADDR + 32'h604, 32'h002e0176);
apb_write(OMC_APB_BASE_ADDR + 32'h608, 32'h001700bb);
apb_write(OMC_APB_BASE_ADDR + 32'h60c, 32'h000b005d);
apb_write(OMC_APB_BASE_ADDR + 32'h610, 32'h0005002e);
apb_write(OMC_APB_BASE_ADDR + 32'h614, 32'h00020017);
apb_write(OMC_APB_BASE_ADDR + 32'h618, 32'h00020017);
apb_write(OMC_APB_BASE_ADDR + 32'h61c, 32'h00020017);
apb_write(OMC_APB_BASE_ADDR + 32'h678, 32'h00000019);
apb_write(OMC_APB_BASE_ADDR + 32'h100, 32'h000000fc);
apb_write(OMC_APB_BASE_ADDR + 32'h620, 32'h03030404);
apb_write(OMC_APB_BASE_ADDR + 32'h624, 32'h04030505);
apb_write(OMC_APB_BASE_ADDR + 32'h628, 32'h07030884);
apb_write(OMC_APB_BASE_ADDR + 32'h62c, 32'h13150401);
apb_write(OMC_APB_BASE_ADDR + 32'h630, 32'h17150604);
apb_write(OMC_APB_BASE_ADDR + 32'h634, 32'h000d0000);
apb_write(OMC_APB_BASE_ADDR + 32'h638, 32'h200a0a08);
apb_write(OMC_APB_BASE_ADDR + 32'h63c, 32'h17300803);
apb_write(OMC_APB_BASE_ADDR + 32'h640, 32'h00060c00);
apb_write(OMC_APB_BASE_ADDR + 32'h644, 32'ha0030006);
apb_write(OMC_APB_BASE_ADDR + 32'h648, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h64c, 32'h00081306);
apb_write(OMC_APB_BASE_ADDR + 32'h650, 32'h04070304);
apb_write(OMC_APB_BASE_ADDR + 32'h654, 32'h00000404);
apb_write(OMC_APB_BASE_ADDR + 32'h658, 32'h00000060);
apb_write(OMC_APB_BASE_ADDR + 32'h65c, 32'h00020008);
apb_write(OMC_APB_BASE_ADDR + 32'h660, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h680, 32'h00000400);
apb_write(OMC_APB_BASE_ADDR + 32'h684, 32'h02000202);
apb_write(OMC_APB_BASE_ADDR + 32'h688, 32'h04130409);
apb_write(OMC_APB_BASE_ADDR + 32'h68c, 32'h20002420);
apb_write(OMC_APB_BASE_ADDR + 32'h690, 32'h00140000);
apb_write(OMC_APB_BASE_ADDR + 32'h69c, 32'h01240074);
apb_write(OMC_APB_BASE_ADDR + 32'h6a0, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h6a4, 32'h20240c00);
apb_write(OMC_APB_BASE_ADDR + 32'h6a8, 32'h00040000);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30010006);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30020000);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30030031);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30160006);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h514, 32'h00000000);

apb_read(OMC_APB_BASE_ADDR + 32'h518, tmp);
while((tmp & 32'h00000002) != 32'h00000002) begin
  orbit_usleep(1);
  apb_read(OMC_APB_BASE_ADDR + 32'h518, tmp);
end


apb_read(OMC_APB_BASE_ADDR + 32'h518, tmp);
while((tmp & 32'h00000002) != 32'h00000000) begin
  orbit_usleep(1);
  apb_read(OMC_APB_BASE_ADDR + 32'h518, tmp);
end

apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd11 <<2), tmp);//`DENALI_PI_11_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd11 <<2), 32'h03000803);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd83 <<2), tmp);//`DENALI_PI_83_DATA
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd84 <<2), tmp);//`DENALI_PI_84_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd84 <<2), 32'h00000000);
//cdns_pi_end( 3);

apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd46 <<2), tmp);//`DENALI_PI_46_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd46 <<2), 32'h030f0000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd33 <<2), tmp);//`DENALI_PI_33_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd33 <<2), 32'h00000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), tmp);//`DENALI_PI_44_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), 32'h01000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), tmp);//`DENALI_PI_44_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), 32'h00000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd45 <<2), tmp);//`DENALI_PI_45_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd45 <<2), 32'h00000001);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd146 <<2), tmp);//`DENALI_PI_146_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd146 <<2), 32'h03011808);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), tmp);//`DENALI_PI_147_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), 32'h01010301);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), tmp);//`DENALI_PI_147_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), 32'h03010301);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), tmp);//`DENALI_PI_44_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), 32'h00000100);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd46 <<2), tmp);//`DENALI_PI_46_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd46 <<2), 32'h03030000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd33 <<2), tmp);//`DENALI_PI_33_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd33 <<2), 32'h00000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd43 <<2), tmp);//`DENALI_PI_43_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd43 <<2), 32'h01000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd43 <<2), tmp);//`DENALI_PI_43_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd43 <<2), 32'h00000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), tmp);//`DENALI_PI_44_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd44 <<2), 32'h00000101);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd146 <<2), tmp);//`DENALI_PI_146_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd146 <<2), 32'h03031808);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), tmp);//`DENALI_PI_147_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), 32'h03010303);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), tmp);//`DENALI_PI_147_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd147 <<2), 32'h03030303);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd148 <<2), tmp);//`DENALI_PI_148_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd148 <<2), 32'h03170a0a);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd149 <<2), tmp);//`DENALI_PI_149_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd149 <<2), 32'h03000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd150 <<2), tmp);//`DENALI_PI_150_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd150 <<2), 32'h03000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd43 <<2), tmp);//`DENALI_PI_43_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd43 <<2), 32'h00000100);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd67 <<2), tmp);//`DENALI_PI_67_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd67 <<2), 32'h01010300);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd68 <<2), tmp);//`DENALI_PI_68_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd68 <<2), 32'h34000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd71 <<2), tmp);//`DENALI_PI_71_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd71 <<2), 32'h01000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd71 <<2), tmp);//`DENALI_PI_71_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd71 <<2), 32'h00000000);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd72 <<2), tmp);//`DENALI_PI_72_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd72 <<2), 32'h00000100);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd180 <<2), tmp);//`DENALI_PI_180_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd180 <<2), 32'h06000301);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd183 <<2), tmp);//`DENALI_PI_183_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd183 <<2), 32'h01000301);
apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd186 <<2), tmp);//`DENALI_PI_186_DATA
apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd186 <<2), 32'h13000301);
//cdns_rdlvl_gate_tr_init( 3,0,0,0,0);

//cdns_rdlvl_tr_init( 3,0,0,0,0);

//cdns_wdqlvl_tr_init( 3,0,0,0,0);

apb_write(OMC_APB_BASE_ADDR + 32'h100, 32'h000000e0);
apb_write(OMC_APB_BASE_ADDR + 32'h620, 32'h04031d22);
apb_write(OMC_APB_BASE_ADDR + 32'h624, 32'h0c18080c);
apb_write(OMC_APB_BASE_ADDR + 32'h628, 32'h60440c9d);
apb_write(OMC_APB_BASE_ADDR + 32'h62c, 32'h34271140);//tRRD[12:8]>=0x10
apb_write(OMC_APB_BASE_ADDR + 32'h630, 32'h36231c0e);
apb_write(OMC_APB_BASE_ADDR + 32'h634, 32'h001e0000);
apb_write(OMC_APB_BASE_ADDR + 32'h638, 32'h20171708);
apb_write(OMC_APB_BASE_ADDR + 32'h63c, 32'h36420806);
apb_write(OMC_APB_BASE_ADDR + 32'h640, 32'h01cf2cd6);//tXSR>=0x1CC(287.5ns_8gibx16);tXP+tmrri?
apb_write(OMC_APB_BASE_ADDR + 32'h644, 32'h00e101c1);//lpddr4:tRFCpb>=0xe0,tRFCab>=0x1c0
apb_write(OMC_APB_BASE_ADDR + 32'h648, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h64c, 32'h000c351e);
apb_write(OMC_APB_BASE_ADDR + 32'h650, 32'h20664713);
apb_write(OMC_APB_BASE_ADDR + 32'h654, 32'h00002520);
apb_write(OMC_APB_BASE_ADDR + 32'h658, 32'h00000060);
apb_write(OMC_APB_BASE_ADDR + 32'h65c, 32'h00900030);
apb_write(OMC_APB_BASE_ADDR + 32'h660, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h680, 32'h16000416);
apb_write(OMC_APB_BASE_ADDR + 32'h684, 32'h08000a04);
apb_write(OMC_APB_BASE_ADDR + 32'h688, 32'h0415040a);
apb_write(OMC_APB_BASE_ADDR + 32'h68c, 32'h20003320);
apb_write(OMC_APB_BASE_ADDR + 32'h690, 32'h00140000);
apb_write(OMC_APB_BASE_ADDR + 32'h69c, 32'h01240074);
apb_write(OMC_APB_BASE_ADDR + 32'h6a0, 32'h00000000);
apb_write(OMC_APB_BASE_ADDR + 32'h6a4, 32'h20330c00);
apb_write(OMC_APB_BASE_ADDR + 32'h6a8, 32'h00040000);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30010056);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h3002002d);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30030031);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h4, 32'h30160006);
apb_write(OMC_APB_BASE_ADDR + 32'hc, 32'h00000002);
apb_write(OMC_APB_BASE_ADDR + 32'h410, 32'h00101010);
apb_write(OMC_APB_BASE_ADDR + 32'h420, 32'h0c181006);
apb_write(OMC_APB_BASE_ADDR + 32'h424, 32'h20200820);
apb_write(OMC_APB_BASE_ADDR + 32'h428, 32'h80000020);
apb_write(OMC_APB_BASE_ADDR + 32'h0, 32'h00000001);
apb_write(OMC_APB_BASE_ADDR + 32'h108, 32'h00003000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'h704, 32'h00000007);
apb_write(OMC_APB_BASE_ADDR + 32'h330, 32'h09313fff);
apb_write(OMC_APB_BASE_ADDR + 32'h508, 32'h00000033);
apb_write(OMC_APB_BASE_ADDR + 32'h324, 32'h00002000);
apb_write(OMC_APB_BASE_ADDR + 32'h104, 32'h90000000);
apb_write(OMC_APB_BASE_ADDR + 32'h510, 32'h00000100);
apb_write(OMC_APB_BASE_ADDR + 32'h514, 32'h00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 32'h700, 32'h00000003);
apb_write(OMC_APB_BASE_ADDR + 32'h514, 32'h00000600);
apb_write(OMC_APB_BASE_ADDR + 32'h20, 32'h00000001);
//cdns_per_tr_longcnt_set( 16);
// Initialization done
