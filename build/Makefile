# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2020 StarFiveTech, Inc

# compiler tool chain
CROSS_COMPILE?=riscv64-unknown-elf-
CC=${CROSS_COMPILE}gcc
LD=${CROSS_COMPILE}ld
OBJCOPY=${CROSS_COMPILE}objcopy
OBJDUMP=${CROSS_COMPILE}objdump

#DDR_SPEED 1:1600. 2:2133 3:2800 4 :3200
DDR_SPEED = 2133

SUFFIX=$(shell date +%y%m%d)
GIT_VERSION=$(shell git show -s --pretty=format:%h)
VERSION=$(SUFFIX)-$(GIT_VERSION)

TARGET 	= ddrinit-$(DDR_SPEED)-$(SUFFIX)

LINKER_SCRIPT := ddrinit.lds
MAP_FILE := ddrinit.map

INCLUDE_DIR = -I. -I../boot\
			-I../common\
			-I../gpio\
			-I../spi\
			-I../system\
			-I../uart \
			-I../timer \
			-I../sdio\
			-I../gpt \
			-I../crc32 \

ifeq ($(strip $(DDR_SPEED)),1600)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_800_cl14_bl16
else ifeq ($(strip $(DDR_SPEED)),2133)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_1066_cl20_bl16
else ifeq ($(strip $(DDR_SPEED)),2800)
INCLUDE_DIR +=-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_1600_cl28_bl16
else ifeq ($(strip $(DDR_SPEED)),3200)
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
CFLAGS += -specs=nano.specs
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
		../crc32/crc32.o \

ifeq ($(strip $(DDR_SPEED)),1600)
OBJECTLIST +=../ddrc_cfg/lpddr4_800_cl14_bl16/orbit_boot_800.o
OBJECTLIST +=../ddrphy_cfg/lpddr4_1600_cl28_bl16/regconfig_pi_start.o
CFLAGS +=-DDDR_1600
else ifeq ($(strip $(DDR_SPEED)),2133)
OBJECTLIST +=../ddrc_cfg/lpddr4_1066_cl20_bl16/orbit_boot_1066.o
OBJECTLIST +=../ddrphy_cfg/lpddr4_1600_cl28_bl16/regconfig_pi_start.o
CFLAGS +=-DDDR_2133
else ifeq ($(strip $(DDR_SPEED)),2800)
OBJECTLIST +=../ddrc_cfg/lpddr4_1333_cl24_bl16/orbit_boot_1333.o
OBJECTLIST +=../ddrphy_cfg/lpddr4_1600_cl28_bl16/regconfig_pi_start_2800.o
CFLAGS +=-DDDR_2800
else ifeq ($(strip $(DDR_SPEED)),3200)
OBJECTLIST +=../ddrc_cfg/lpddr4_1600_cl28_bl16/orbit_boot_1600.o
OBJECTLIST +=../ddrphy_cfg/lpddr4_1600_cl28_bl16/regconfig_pi_start_3200.o
CFLAGS +=-DDDR_3200
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
	rm -f $(OBJECTLIST) $(TARGET).bin *.asm *.elf *.out $(MAP_FILE)
