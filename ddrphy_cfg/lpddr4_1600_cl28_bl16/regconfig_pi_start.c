#include <regconfig.h.sim_PI.h>
#include <regconfig.h.sim_PHY.h>
#include <sys.h>
#include <clkgen_ctrl_macro.h>
#include <comdef.h>
void regconfig_pi_start(uint32_t OMC_APB_BASE_ADDR, uint32_t OMC_SECURE_APB_BASE_ADDR, uint32_t PHY_APB_BASE_ADDR, u32 ddr_num)
{
    u32 tmp = 0;
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +89 <<2));//`DENALI_PHY_89_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +89 <<2), tmp&0xffffff00|0x00000051);

    //disable RDLVL VREF
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +78 <<2));//`DENALI_PHY_78_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +78 <<2), tmp&0xfffffcff);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +345 <<2));//`DENALI_PHY_345_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +345 <<2), tmp&0xffffff00|0x00000051);

    //disable RDLVL VREF
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +334 <<2));//`DENALI_PHY_334_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +334 <<2), tmp&0xfffffcff);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +601 <<2));//`DENALI_PHY_601_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +601 <<2), tmp&0xffffff00|0x00000051);

    //disable RDLVL VREF
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +590 <<2));//`DENALI_PHY_590_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +590 <<2), tmp&0xfffffcff);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +857 <<2));//`DENALI_PHY_857_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +857 <<2), tmp&0xffffff00|0x00000051);

    //disable RDLVL VREF
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +846 <<2));//`DENALI_PHY_846_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +846 <<2), tmp&0xfffffcff);

    //turn off multicast
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1793 <<2));//`DENALI_PHY_1793_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1793 <<2), tmp&0xfffffeff);

    //set to freq copy 0
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1793 <<2));//`DENALI_PHY_1793_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1793 <<2), tmp&0xfffcffff);

    //data slice registers
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +125 <<2));//`DENALI_PHY_125_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +125 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +102 <<2));//`DENALI_PHY_102_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +102 <<2), tmp&0xfffffffc|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +105 <<2));//`DENALI_PHY_105_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +105 <<2), tmp&0xffffffe0|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +92 <<2));//`DENALI_PHY_92_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +92 <<2), tmp&0xfffffffe|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +94 <<2));//`DENALI_PHY_94_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +94 <<2), tmp&0xffffe0ff|0x00000200);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +96 <<2));//`DENALI_PHY_96_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +96 <<2), tmp&0xfffff0ff|0x00000400);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +89 <<2));//`DENALI_PHY_89_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +89 <<2), tmp&0xffffff00|0x00000051);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +381 <<2));//`DENALI_PHY_381_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +381 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +358 <<2));//`DENALI_PHY_358_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +358 <<2), tmp&0xfffffffc|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +361 <<2));//`DENALI_PHY_361_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +361 <<2), tmp&0xffffffe0|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +348 <<2));//`DENALI_PHY_348_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +348 <<2), tmp&0xfffffffe|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +350 <<2));//`DENALI_PHY_350_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +350 <<2), tmp&0xffffe0ff|0x00000200);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +352 <<2));//`DENALI_PHY_352_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +352 <<2), tmp&0xfffff0ff|0x00000400);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +345 <<2));//`DENALI_PHY_345_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +345 <<2), tmp&0xffffff00|0x00000051);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +637 <<2));//`DENALI_PHY_637_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +637 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +614 <<2));//`DENALI_PHY_614_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +614 <<2), tmp&0xfffffffc|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +617 <<2));//`DENALI_PHY_617_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +617 <<2), tmp&0xffffffe0|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +604 <<2));//`DENALI_PHY_604_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +604 <<2), tmp&0xfffffffe|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +606 <<2));//`DENALI_PHY_606_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +606 <<2), tmp&0xffffe0ff|0x00000200);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +608 <<2));//`DENALI_PHY_608_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +608 <<2), tmp&0xfffff0ff|0x00000400);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +601 <<2));//`DENALI_PHY_601_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +601 <<2), tmp&0xffffff00|0x00000051);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +893 <<2));//`DENALI_PHY_893_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +893 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +870 <<2));//`DENALI_PHY_870_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +870 <<2), tmp&0xfffffffc|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +873 <<2));//`DENALI_PHY_873_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +873 <<2), tmp&0xffffffe0|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +860 <<2));//`DENALI_PHY_860_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +860 <<2), tmp&0xfffffffe|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +862 <<2));//`DENALI_PHY_862_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +862 <<2), tmp&0xffffe0ff|0x00000200);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +864 <<2));//`DENALI_PHY_864_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +864 <<2), tmp&0xfffff0ff|0x00000400);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +857 <<2));//`DENALI_PHY_857_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +857 <<2), tmp&0xffffff00|0x00000051);

    //phy level registers
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1895 <<2));//`DENALI_PHY_1895_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1895 <<2), tmp&0xffffe000|0x00001342);

    //apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd1895 <<2), tmp);//`DENALI_PHY_1895_DATA
    //apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd1895 <<2), tmp&32'hfffe_ffff|32'h0001_0000); //for memory clock<=400M

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1835 <<2));//`DENALI_PHY_1835_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1835 <<2), tmp&0xfffff0ff|0x00000200);

    //turn on multicast
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1793 <<2));//`DENALI_PHY_1793_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1793 <<2), tmp&0xfffffeff|0x00000100);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xfffffeff);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +66 <<2));//`DENALI_PI_66_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +66 <<2), tmp&0xfffffeff);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +166 <<2));//`DENALI_PI_166_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +166 <<2), tmp&0xffffff80|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xf0ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +166 <<2));//`DENALI_PI_166_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +166 <<2), tmp&0xffff80ff|0x00000100);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +179 <<2));//`DENALI_PI_179_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +179 <<2), tmp&0xff80ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp&0xffe0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp&0xe0ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +179 <<2));//`DENALI_PI_179_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +179 <<2), tmp&0x80ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +166 <<2));//`DENALI_PI_166_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +166 <<2), tmp&0xff80ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xf0ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +166 <<2));//`DENALI_PI_166_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +166 <<2), tmp&0x80ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +182 <<2));//`DENALI_PI_182_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +182 <<2), tmp&0xff80ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp&0xffe0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp&0xe0ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +182 <<2));//`DENALI_PI_182_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +182 <<2), tmp&0x80ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +167 <<2));//`DENALI_PI_167_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +167 <<2), tmp&0xffffff80|0x00000001);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xfff0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +62 <<2));//`DENALI_PI_62_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +62 <<2), tmp&0xf0ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +167 <<2));//`DENALI_PI_167_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +167 <<2), tmp&0xffff80ff|0x00000100);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +185 <<2));//`DENALI_PI_185_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +185 <<2), tmp&0xff80ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp&0xffe0ffff|0x00010000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +67 <<2));//`DENALI_PI_67_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +67 <<2), tmp&0xe0ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +185 <<2));//`DENALI_PI_185_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +185 <<2), tmp&0x80ffffff|0x01000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +10 <<2));//`DENALI_PI_10_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +10 <<2), tmp&0xffffffe0|0x00000002);

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +0 <<2));//`DENALI_PI_00_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +0 <<2), tmp&0xfffffffe|0x00000001);

    //Reduce time for IO pad calibration TODO
    //tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1860 <<2));//`DENALI_PHY_1860_DATA
    //apb_write(PHY_APB_BASE_ADDR + (4096 +1860 <<2), tmp&0x80ffffff|0x01000000);


    //PI_CS_MAP: 0xf->0x3
    //apb_read(PHY_APB_BASE_ADDR + (16'd2048 +16'd11 <<2), tmp);
    //if(tmp != 32'h0300080f) $stop; //`DENALI_PI_11_DATA
    //apb_write(PHY_APB_BASE_ADDR + (16'd2048 +16'd11 <<2), 32'h03000803);

    //set CS0 MR13.VRCG=1
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +247 <<2));//`DENALI_PI_247_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +247 <<2), tmp | 0x00000008);

    //set CS1 MR13.VRCG=1
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +249 <<2));//`DENALI_PI_249_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +249 <<2), tmp | 0x00000800);

    //set CS2 MR13.VRCG=1
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +252 <<2));//`DENALI_PI_252_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +252 <<2), tmp | 0x00000008);

    //set CS3 MR13.VRCG=1
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +254 <<2));//`DENALI_PI_254_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +254 <<2), tmp | 0x00000800);
/*
    //set PHY_LP4_WDQS_OE_EXTEND_x
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +65 <<2));//`DENALI_PHY_65_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +65 <<2), tmp | 0x00000100);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +321 <<2));//`DENALI_PHY_321_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +321 <<2), tmp | 0x00000100);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +577 <<2));//`DENALI_PHY_577_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +577 <<2), tmp | 0x00000100);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +833 <<2));//`DENALI_PHY_833_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +833 <<2), tmp | 0x00000100);
*/
    //PHY_RPTR_UPDATE_x: bit[11:8]+=3
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +96 <<2));//`DENALI_PHY_96_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +96 <<2), tmp+0x0300);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +352 <<2));//`DENALI_PHY_352_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +352 <<2), tmp+0x0300);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +608 <<2));//`DENALI_PHY_608_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +608 <<2), tmp+0x0300);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +864 <<2));//`DENALI_PHY_864_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +864 <<2), tmp+0x0300);

#ifdef G_SPEED_3200
    //PHY_WRLVL_DLY_STEP_X: 8'hC -> 8'h18
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +96 <<2));//`DENALI_PHY_96_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +96 <<2), tmp&0xff00ffff|0x00180000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +352 <<2));//`DENALI_PHY_352_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +352 <<2), tmp&0xff00ffff|0x00180000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +608 <<2));//`DENALI_PHY_608_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +608 <<2), tmp&0xff00ffff|0x00180000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +864 <<2));//`DENALI_PHY_864_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +864 <<2), tmp&0xff00ffff|0x00180000);
#elif defined G_SPEED_2666
    //PHY_WRLVL_DLY_STEP_X: 8'hC -> 8'h14
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +96 <<2));//`DENALI_PHY_96_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +96 <<2), tmp&0xff00ffff|0x00140000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +352 <<2));//`DENALI_PHY_352_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +352 <<2), tmp&0xff00ffff|0x00140000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +608 <<2));//`DENALI_PHY_608_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +608 <<2), tmp&0xff00ffff|0x00140000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +864 <<2));//`DENALI_PHY_864_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +864 <<2), tmp&0xff00ffff|0x00140000);
#elif defined G_SPEED_2133
    //PHY_WRLVL_DLY_STEP_X: 8'hC -> 8'h12
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +96 <<2));//`DENALI_PHY_96_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +96 <<2), tmp&0xff00ffff|0x00120000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +352 <<2));//`DENALI_PHY_352_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +352 <<2), tmp&0xff00ffff|0x00120000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +608 <<2));//`DENALI_PHY_608_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +608 <<2), tmp&0xff00ffff|0x00120000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +864 <<2));//`DENALI_PHY_864_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +864 <<2), tmp&0xff00ffff|0x00120000);
    //`elsif G_SPEED_1600
#endif

    //PHY_WDQLVL_CLK_JITTER_TOLERANCE_X: 8'h20 -> 8'h40 
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +33 <<2));//`DENALI_PHY_33_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +33 <<2), tmp&0xffffff00|0x0040);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +289 <<2));//`DENALI_PHY_289_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +289 <<2), tmp&0xffffff00|0x0040);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +545 <<2));//`DENALI_PHY_545_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +545 <<2), tmp&0xffffff00|0x0040);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +801 <<2));//`DENALI_PHY_801_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +801 <<2), tmp&0xffffff00|0x0040);

    //PHY_ADR_CALVL_NUM_PATTERNS_X: 2'h0 -> 2'h01 TODO
    //apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd1039 <<2), tmp);//`DENALI_PHY_1039_DATA
    //apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd1039 <<2), tmp|32'h01);
    //
    //apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd1295 <<2), tmp);//`DENALI_PHY_1295_DATA
    //apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd1295 <<2), tmp|32'h01);
    //
    //apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd1551 <<2), tmp);//`DENALI_PHY_1551_DATA
    //apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd1551 <<2), tmp|32'h01);


    //PHY_ADR_CALVL_RANK_CTRL_x: 2'b11 -> 2'b10
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1038 <<2));//`DENALI_PHY_1038_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1038 <<2), tmp&0xfcffffff|0x02000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1294 <<2));//`DENALI_PHY_1294_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1294 <<2), tmp&0xfcffffff|0x02000000);

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1550 <<2));//`DENALI_PHY_1550_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1550 <<2), tmp&0xfcffffff|0x02000000);

#if 0
    //tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1918 <<2));//`DENALI_PHY_1918_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1918 <<2), 0x1c088);

    //PHY_PAD_VREF_CTRL_AC
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1896 <<2)); //DENALI_PI_1896_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +1896 <<2), tmp&0xfffffc00|0x03d5); //DENALI_PHY_1896_DATA

    //PHY_PAD_VREF_CTRL_DQ_x:10'h11f->10'h3d5
    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +91 <<2)); //DENALI_PI_91_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +91 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_91_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +347 <<2)); //DENALI_PI_347_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +347 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_347_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +603 <<2)); //DENALI_PI_603_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +603 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_603_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +859 <<2)); //DENALI_PI_859_DATA
    apb_write(PHY_APB_BASE_ADDR + (4096 +859 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_859_DATA

    //PHY_PAD_FDBK_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1912 <<2), 0x1c088); //DENALI_PHY_1912_DATA
    //PHY_PAD_FDBK_DRIVE2
    apb_write(PHY_APB_BASE_ADDR + (2048 +1913 <<2), 0x88); //DENALI_PHY_1913_DATA
    //PHY_PAD_DATA_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1914 <<2), 0x1c0); //DENALI_PHY_1914_DATA
    //PHY_PAD_DQS_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1915 <<2), 0x1c0); //DENALI_PHY_1915_DATA
    //PHY_PAD_ADDR_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1916 <<2), 0x1c000); //DENALI_PHY_1916_DATA
    //PHY_PAD_CLK_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1918 <<2), 0x1c000); //DENALI_PHY_1918_DATA
    //PHY_PAD_CLK_DRIVE2
    apb_write(PHY_APB_BASE_ADDR + (2048 +1919 <<2), 0x0); //DENALI_PHY_1919_DATA
    //PHY_PAD_CKE_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1922 <<2), 0x1c000); //DENALI_PHY_1922_DATA
    //PHY_PAD_CKE_DRIVE2
    apb_write(PHY_APB_BASE_ADDR + (2048 +1923 <<2), 0x0); //DENALI_PHY_1923_DATA
    //PHY_PAD_RST_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1924 <<2), 0x0); //DENALI_PHY_1924_DATA
    //PHY_PAD_RST_DRIVE2
    apb_write(PHY_APB_BASE_ADDR + (2048 +1925 <<2), 0x0); //DENALI_PHY_1925_DATA
    //PHY_PAD_CS_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1926 <<2), 0x0); //DENALI_PHY_1926_DATA
    //PHY_PAD_CS_DRIVE2
    apb_write(PHY_APB_BASE_ADDR + (2048 +1927 <<2), 0x0); //DENALI_PHY_1927_DATA
    //PHY_PAD_ODT_DRIVE
    apb_write(PHY_APB_BASE_ADDR + (2048 +1928 <<2), 0x0); //DENALI_PHY_1928_DATA

    //PHY_DQ_TSEL_SELECT_X
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +76 <<2)); //DENALI_PI_76_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +76 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_76_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +332 <<2)); //DENALI_PI_332_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +332 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_332_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +588 <<2)); //DENALI_PHY_588_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +588 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_588_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +844 <<2)); //DENALI_PHY_844_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +844 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_844_DATA

    //PHY_DQS_TSEL_SELECT_X
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +77 <<2)); //DENALI_PI_77_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +77 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_77_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +333 <<2)); //DENALI_PI_333_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +333 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_333_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +589 <<2)); //DENALI_PHY_589_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +589 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_589_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +845 <<2)); //DENALI_PHY_845_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +845 <<2), tmp&0xff0000ff|0x00888800); //DENALI_PHY_845_DATA

    //PHY_ADR_TSEL_SELECT_X
    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +1062 <<2)); //DENALI_PI_1062_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +1062 <<2), tmp&0xffffff00|0x88); //DENALI_PHY_1062_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +1318 <<2)); //DENALI_PI_1318_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +1318 <<2), tmp&0xffffff00|0x88); //DENALI_PHY_1318_DATA

    tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +1574 <<2)); //DENALI_PI_1574_DATA
    apb_write(PHY_APB_BASE_ADDR + (2048 +1574 <<2), tmp&0xffffff00|0x88); //DENALI_PHY_1574_DATA
#endif
#if 1//0807
//PHY_PAD_DSLICE_IO_CFG_x:0->7
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +83 <<2)); //DENALI_PHY_83_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +83 <<2), tmp&0xffc0ffff|0x70000); //DENALI_PHY_83_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +339 <<2)); //DENALI_PHY_339_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +339 <<2), tmp&0xffc0ffff|0x70000); //DENALI_PHY_339_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +595 <<2)); //DENALI_PHY_595_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +595 <<2), tmp&0xffc0ffff|0x70000); //DENALI_PHY_595_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +851 <<2)); //DENALI_PHY_851_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +851 <<2), tmp&0xffc0ffff|0x70000); //DENALI_PHY_851_DATA

//PHY_PAD_ADR_IO_CFG_x:0->7
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1062 <<2)); //DENALI_PHY_1062_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1062 <<2), tmp&0xf800ffff|0x70000); //DENALI_PHY_1062_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1318 <<2)); //DENALI_PHY_1318_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1318 <<2), tmp&0xf800ffff|0x70000); //DENALI_PHY_1318_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1574 <<2)); //DENALI_PHY_1574_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1574 <<2), tmp&0xf800ffff|0x70000); //DENALI_PHY_1574_DATA

//PHY_PAD_CAL_IO_CFG_0:0->7
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1892 <<2)); //DENALI_PHY_1892_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1892 <<2), tmp&0xfffc0000|0x7); //DENALI_PHY_1892_DATA

//PHY_PAD_ACS_IO_CFG:0->7
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1893 <<2)); //DENALI_PHY_1893_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1893 <<2), tmp&0xfffc0000|0x7); //DENALI_PHY_1893_DATA

//PHY_CAL_MODE_0 TODO
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1852 <<2)); //DENALI_PHY_1852_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1852 <<2), tmp&0xffffe000|0x078); //DENALI_PHY_1852_DATA

//PHY_PLL_WAIT
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1822 <<2)); //DENALI_PHY_1822_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1822 <<2), tmp|0xFF); //DENALI_PHY_1822_DATA

//PHY_PAD_VREF_CTRL_AC:10'h0100->10'h3d5
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1896 <<2)); //DENALI_PHY_1896_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1896 <<2), tmp&0xfffffc00|0x03d5); //DENALI_PHY_1896_DATA

//PHY_PAD_VREF_CTRL_DQ_x:10'h11f->10'h3d5
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +91 <<2)); //DENALI_PHY_91_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +91 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_91_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +347 <<2)); //DENALI_PHY_347_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +347 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_347_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +603 <<2)); //DENALI_PHY_603_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +603 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_603_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +859 <<2)); //DENALI_PHY_859_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +859 <<2), tmp&0xfc00ffff|0x03d50000); //DENALI_PHY_859_DATA

//PHY_PAD_FDBK_DRIVE:bit[7:0]:{ENSLICEP_DRV,ENSLICEN_DRV}
apb_write(PHY_APB_BASE_ADDR + (4096 +1912 <<2), 0xcf3bf8c); //DENALI_PHY_1912_DATA //0x1c088
//PHY_PAD_FDBK_DRIVE2:bit[7:0]:{ENSLICEP_ODT,ENSLICEN_ODT}
apb_write(PHY_APB_BASE_ADDR + (4096 +1913 <<2), 0xf00c); //DENALI_PHY_1913_DATA //0x88
//PHY_PAD_DATA_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1914 <<2), 0x33f07ff); //DENALI_PHY_1914_DATA //0x1c0
//PHY_PAD_DQS_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1915 <<2), 0xc3c37ff); //DENALI_PHY_1915_DATA //0x1c0
//PHY_PAD_ADDR_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1916 <<2), 0x1f87ff70); //DENALI_PHY_1916_DATA //0x1c000
//PHY_PAD_ADDR_DRIVE2
apb_write(PHY_APB_BASE_ADDR + (4096 +1917 <<2), 0x230010); //DENALI_PHY_1917_DATA //0x220000
//PHY_PAD_CLK_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1918 <<2), 0x3ff7ff8c); //DENALI_PHY_1918_DATA //0x1c088 // 0xbc wave good
//PHY_PAD_CLK_DRIVE2
apb_write(PHY_APB_BASE_ADDR + (4096 +1919 <<2), 0xe10); //DENALI_PHY_1919_DATA
//PHY_PAD_ERR_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1920 <<2), 0x1f87ff8c); //DENALI_PHY_1920_DATA
//PHY_PAD_ERR_DRIVE2
apb_write(PHY_APB_BASE_ADDR + (4096 +1921 <<2), 0x188411); //DENALI_PHY_1921_DATA
//PHY_PAD_CKE_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1922 <<2), 0x6bfff); //DENALI_PHY_1922_DATA //0x1c088
//PHY_PAD_CKE_DRIVE2
apb_write(PHY_APB_BASE_ADDR + (4096 +1923 <<2), 0x180400); //DENALI_PHY_1923_DATA
//PHY_PAD_RST_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1924 <<2), 0x6bfff); //DENALI_PHY_1924_DATA //0x1c088
//PHY_PAD_RST_DRIVE2
apb_write(PHY_APB_BASE_ADDR + (4096 +1925 <<2), 0x180400); //DENALI_PHY_1925_DATA
//PHY_PAD_CS_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1926 <<2), 0x1f87ff8c); //DENALI_PHY_1926_DATA //0x1c088
//PHY_PAD_CS_DRIVE2
apb_write(PHY_APB_BASE_ADDR + (4096 +1927 <<2), 0x188400); //DENALI_PHY_1927_DATA
//PHY_PAD_ODT_DRIVE
apb_write(PHY_APB_BASE_ADDR + (4096 +1928 <<2), 0x1f87df8c); //DENALI_PHY_1928_DATA //0x1c088
//PHY_CAL_CLK_SELECT_0,PHY_PAD_ODT_DRIVE2:1->4
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1929 <<2)); //DENALI_PHY_1929_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1929 <<2), 0x4188411); //DENALI_PHY_1929_DATA

//PHY_PAD_FDBK_TERM
apb_write(PHY_APB_BASE_ADDR + (4096 +1837 <<2), 0x24410); //DENALI_PHY_1837_DATA
//PHY_PAD_ADDR_TERM
apb_write(PHY_APB_BASE_ADDR + (4096 +1840 <<2), 0x24410); //DENALI_PHY_1840_DATA
//PHY_PAD_ERR_TERM
apb_write(PHY_APB_BASE_ADDR + (4096 +1842 <<2), 0x2ffff); //DENALI_PHY_1842_DATA

//PHY_DQ_TSEL_SELECT_X bit[15:8]={ENSLICEP_DRV,ENSLICEN_DRV}:tsel_wr_select  bit[7:0]={ENSLICEP_ODT,ENSLICEN_ODT}:tsel_rd_select
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +76 <<2)); //DENALI_PHY_76_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +76 <<2), tmp&0xff0000ff|0x00c47c00); //DENALI_PHY_76_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +332 <<2)); //DENALI_PHY_332_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +332 <<2), tmp&0xff0000ff|0x00c47c00); //DENALI_PHY_332_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +588 <<2)); //DENALI_PHY_588_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +588 <<2), tmp&0xff0000ff|0x00c47c00); //DENALI_PHY_588_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +844 <<2)); //DENALI_PHY_844_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +844 <<2), tmp&0xff0000ff|0x00c47c00); //DENALI_PHY_844_DATA

//PHY_DQS_TSEL_SELECT_X  bit[15:8]={ENSLICEP_DRV,ENSLICEN_DRV}:tsel_wr_select  bit[7:0]={ENSLICEP_ODT,ENSLICEN_ODT}:tsel_rd_select
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +77 <<2)); //DENALI_PHY_77_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +77 <<2), tmp&0xffff0000|0xb77c); //DENALI_PHY_77_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +333 <<2)); //DENALI_PHY_333_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +333 <<2), tmp&0xffff0000|0xb77c); //DENALI_PHY_333_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +589 <<2)); //DENALI_PHY_589_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +589 <<2), tmp&0xffff0000|0xb77c); //DENALI_PHY_589_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +845 <<2)); //DENALI_PHY_845_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +845 <<2), tmp&0xffff0000|0xb77c); //DENALI_PHY_845_DATA

//PHY_ADR_TSEL_SELECT_X:bit[7:0]:{ENSLICEP_ODT/DRV,PENSLICEN_ODT/DRV}
tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1062 <<2)); //DENALI_PHY_1062_DATA for addr5-0
apb_write(PHY_APB_BASE_ADDR + (4096 +1062 <<2), tmp&0xffffff00|0xff); //DENALI_PHY_1062_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1318 <<2)); //DENALI_PHY_1318_DATA for addr11-6
apb_write(PHY_APB_BASE_ADDR + (4096 +1318 <<2), tmp&0xffffff00|0xff); //DENALI_PHY_1318_DATA

tmp = apb_read(PHY_APB_BASE_ADDR + (4096 +1574 <<2)); //DENALI_PHY_1574_DATA for addr15-12
apb_write(PHY_APB_BASE_ADDR + (4096 +1574 <<2), tmp&0xffffff00|0xff); //DENALI_PHY_1574_DATA

//PHY_TST_CLK_PAD_CTRL_x
apb_write(PHY_APB_BASE_ADDR + (4096 +1848 <<2), 0x3cf07f9); //DENALI_PHY_1848_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1849 <<2), 0x3f); //DENALI_PHY_1849_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1850 <<2), 0x70000); //DENALI_PHY_1850_DATA
apb_write(PHY_APB_BASE_ADDR + (4096 +1851 <<2), 0x190000); //DENALI_PHY_1851_DATA

//PHY_DSLICE_PAD_BOOSTPN_SETTING_x
tmp = apb_read(PHY_APB_BASE_ADDR + ((4096 + 130) <<2)); //DENALI_PI_130_DATA
apb_write(PHY_APB_BASE_ADDR + ((4096 + 130) <<2), tmp|0xff0000); //DENALI_PI_130_DATA
tmp = apb_read(PHY_APB_BASE_ADDR + ((4096 + 386) <<2)); //DENALI_PI_386_DATA
apb_write(PHY_APB_BASE_ADDR + ((4096 + 386) <<2), tmp|0xff0000); //DENALI_PI_386_DATA
tmp = apb_read(PHY_APB_BASE_ADDR + ((4096 + 642) <<2)); //DENALI_PI_642_DATA
apb_write(PHY_APB_BASE_ADDR + (2048 +642 <<2), tmp|0x66000000); //DENALI_PI_642_DATA
tmp = apb_read(PHY_APB_BASE_ADDR + ((4096 + 898) <<2)); //DENALI_PI_898_DATA
apb_write(PHY_APB_BASE_ADDR + ((4096 + 898) <<2), tmp|0x66000000); //DENALI_PI_898_DATA

//PI_MR11_DATA_F2_X
tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +289 <<2)); //DENALI_PI_289_DATA
apb_write(PHY_APB_BASE_ADDR + (2048 +289 <<2), tmp|0x66000000); //DENALI_PI_289_DATA
tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +313 <<2)); //DENALI_PI_313_DATA
apb_write(PHY_APB_BASE_ADDR + (2048 +313 <<2), tmp|0x66000000); //DENALI_PI_313_DATA
tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +337 <<2)); //DENALI_PI_337_DATA
apb_write(PHY_APB_BASE_ADDR + (2048 +337 <<2), tmp|0x66000000); //DENALI_PI_337_DATA
tmp = apb_read(PHY_APB_BASE_ADDR + (2048 +361 <<2)); //DENALI_PI_361_DATA
apb_write(PHY_APB_BASE_ADDR + (2048 +361 <<2), tmp|0x66000000); //DENALI_PI_361_DATA

#endif

}

//PHY_WRLVL_DELAY_EARLY_THRESHOLD_X: 10'h1A0 -> 10'h00 TODO:need deleted
//apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd123 <<2), tmp);//`DENALI_PHY_123_DATA
//apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd123 <<2), tmp&32'hfc00_ffff);
//
//apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd379 <<2), tmp);//`DENALI_PHY_379_DATA
//apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd379 <<2), tmp&32'hfc00_ffff);
//
//apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd635 <<2), tmp);//`DENALI_PHY_635_DATA
//apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd635 <<2), tmp&32'hfc00_ffff);
//
//apb_read(PHY_APB_BASE_ADDR + (16'd4096 +16'd891 <<2), tmp);//`DENALI_PHY_891_DATA
//apb_write(PHY_APB_BASE_ADDR + (16'd4096 +16'd891 <<2), tmp&32'hfc00_ffff);

//release dll_rst_n
//apb_write(PHY_APB_BASE_ADDR + (16'd0000 +16'd00 << 2), 32'h01);
