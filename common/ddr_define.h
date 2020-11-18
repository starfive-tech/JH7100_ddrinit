//macro definition in this file is used for choose various ddr simulation
//this file(used for c language) originate from .../SFC_VIC/sim_model/analog/ddrphy/ddr_define.f(used for verilog)
#define G_DRAM_LPDDR4   1
#define G_SPEED_2133    1

#ifdef G_DRAM_DDR4

#define dram_ddr4
//#define speed_bin=3200aa
#ifdef G_16G_X8
#define dram_dq_x8
//#define dram_density=16
#define DDR4_16G_X8
#else
#define dram_dq_x16
//+define+dram_density=8
#define DDR4_8G_X16
#endif
//#define FIXED_3200
//#define ALLOW_JITTER

#elif G_DRAM_LPDDR4

#define dram_lpddr4
//#define speed_bin=3200
//#define dram_density=8
#define dram_dq_x16
//#define lpddr4_pwup_off
//#define lpddr4_proc

#elif G_DRAM_DDR3

#define dram_ddr3
//#define speed_bin=2133kk
//#define dram_density=8
#define dram_dq_x16
//#define sg093
//#define x16
//#define den8192Mb

#elif G_DRAM_LPDDR3

#define dram_lpddr3
//#define speed_bin=2133
#ifdef G_8G_X32
//#define #dram_density=8
#define dram_dq_x32
#else
//#define #dram_density=8
#define dram_dq_x16
#endif

//#define pwup_off
//#define proc

#endif
