# Copyright (c) 2020 StarFiveTech, Inc

# compiler tool chain
CROSSCOMPILE?=riscv64-unknown-elf-
CC=${CROSSCOMPILE}gcc
LD=${CROSSCOMPILE}ld
OBJCOPY=${CROSSCOMPILE}objcopy
OBJDUMP=${CROSSCOMPILE}objdump

SUFFIX=$(shell date +%y%m%d)
GIT_VERSION=$(shell git show -s --pretty=format:%h)
VERSION=$(SUFFIX)-$(GIT_VERSION)

TARGET 	= ddrinit-$(SUFFIX)

LINKER_SCRIPT := ddrinit.lds
MAP_FILE := ddrinit.map

#DDR_SPEED 1:1600. 2:2133 3:2666 4 :3200
DDR_SPEED = DDR2133


INCLUDE_DIR = -I. -I../boot\
			-I../common\
			-I../gpio\
			-I../spi\
			-I../system\
			-I../uart \
			-I../timer \
			-I../sdio\
			-I../gpt

ifeq ($(strip $(DDR_SPEED)),DDR1600)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_800_cl14_bl16
else ifeq ($(strip $(DDR_SPEED)),DDR2133)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_1066_cl20_bl16
else ifeq ($(strip $(DDR_SPEED)),DDR2666)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_1333_cl24_bl16
else ifeq ($(strip $(DDR_SPEED)),DDR2666)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_1600_cl28_bl16
else
$(error DDR_SPEED $(DDR_SPEED) illegal!)
endif

# compiler options
RISCV_ARCH := rv64imafdc
RISCV_ABI  := lp64d 

CFLAGS += -O2 -g -c -Wall -DVERSION=\"$(VERSION)\"
CFLAGS += -march=$(RISCV_ARCH)
CFLAGS += -mabi=$(RISCV_ABI)
CFLAGS += -mcmodel=medany
CFLAGS += $(INCLUDE_DIR)
CCASFLAGS= -mcmodel=medany -mexplicit-relocs	
					
LDFLAGS = -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI)-T $(LINKER_SCRIPT) -nostartfiles --specs=nano.specs -Wl,-Map,$(MAP_FILE)

# object list
OBJECTLIST=../boot/start.o \
		../boot/bootmain.o \
		../boot/trap.o \
		../uart/uart.o \
		../uart/cmd.o \
		../uart/xmodem.o \
		../uart/crc16.o \
		../common/util.o \
		../common/ctype.o \
		../gpio/gpio.o \
		../spi/spi.o \
		../spi/spi_probe.o \
		../spi/cadence_qspi.o \
		../spi/spi_flash.o \
		../spi/cadence_qspi_apb.o \
		../timer/timer.o \
		../sdio/bouncebuf.o \
		../sdio/dw_mmc.o \
		../sdio/mmc.o \
		../sdio/mmc_write.o \
		../sdio/sdio.o \
		../gpt/gpt.o \
		../ddrphy_cfg/regconfig.h.sim_PHY.o \
		../ddrphy_cfg/regconfig.h.sim_PI.o \
		../ddrphy_cfg/lpddr4_1600_cl28_bl16/regconfig_pi_start.o 

ifeq ($(strip $(DDR_SPEED)),DDR1600)
OBJECTLIST +=../ddrc_cfg/lpddr4_800_cl14_bl16/orbit_boot_800.o
else ifeq ($(strip $(DDR_SPEED)),DDR2133)
OBJECTLIST +=../ddrc_cfg/lpddr4_1066_cl20_bl16/orbit_boot_1066.o
else ifeq ($(strip $(DDR_SPEED)),DDR2666)
OBJECTLIST +=../ddrc_cfg/lpddr4_1333_cl24_bl16/orbit_boot_1333.o
else ifeq ($(strip $(DDR_SPEED)),DDR2666)
OBJECTLIST +=../ddrc_cfg/lpddr4_1600_cl28_bl16/orbit_boot_1600.o
else
$(error DDR_SPEED $(DDR_SPEED) illegal!)
endif

all:$(TARGET).elf $(TARGET).bin $(TARGET).asm

$(TARGET).elf:$(OBJECTLIST)
	@echo ================================================================================
	$(CC) -o $(TARGET).elf  $(LDFLAGS) $(OBJECTLIST)
	@echo $(TARGET).elf LINK SUCCEED!	

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@
	@if [ -f fsz.sh ]; then ./fsz.sh $(TARGET).bin; fi

%.asm: %.elf
	$(OBJDUMP) -S $^ > $@

%.dtb: %.dts
	dtc $^ -o $@ -O dtb

%.o: %.S 
	$(CC) $(CFLAGS) $(CCASFLAGS) -c $< -o $@

%.o: %.c 
	$(CC) $(CFLAGS) -o $@ -c $<
	
.PHONY: clean
clean:
	rm -f $(OBJECTLIST) $(TARGET).bin *.asm
