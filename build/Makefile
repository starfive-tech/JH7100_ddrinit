# Copyright (c) 2020 StarFiveTech, Inc

# compiler tool chain
CROSSCOMPILE?=riscv64-unknown-elf-
CC=${CROSSCOMPILE}gcc
LD=${CROSSCOMPILE}ld
OBJCOPY=${CROSSCOMPILE}objcopy
OBJDUMP=${CROSSCOMPILE}objdump


TARGET 	= ddrinit
LINKER_SCRIPT := ddrinit.lds
MAP_FILE := ddrinit.map

#DDR_SPEED 1:1600. 2:2133 3:2666 4 :3200
DDR_SPEED = 25   


INCLUDE_DIR = -I. -I../boot\
			-I../common\
			-I../gpio\
			-I../spi\
			-I../system\
			-I../uart \
			-I../timer \
			-I../sdio\
			-I../gpt\
			-I../ddrphy_cfg/memory/jedec_lpddr4_16gb_3200/regconfig_sim_1066_cl20_bl16


# compiler options
RISCV_ARCH := rv64imafdc
RISCV_ABI  := lp64d 

CFLAGS += -O2 -g -c -Wall
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
		../ddrphy_cfg/lpddr4_1600_cl28_bl16/regconfig_pi_start.o \
		../ddrc_cfg/lpddr4_1066_cl20_bl16/orbit_boot_1066.o 
		
ifeq ($(DDR_SPEED), 1)
OBJECTLIST +=../ddrc_cfg/orbit_boot_800.o
else ifeq ($(DDR_SPEED), 2)
OBJECTLIST +=../ddrc_cfg/orbit_boot_1066.o
else ifeq ($(DDR_SPEED), 3)
OBJECTLIST +=../ddrc_cfg/orbit_boot_1333.o
else ifeq ($(DDR_SPEED), 4)
OBJECTLIST +=../ddrc_cfg/orbit_boot_1600.o
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
ifeq ($(DDR_SPEED), 1)
	echo #
else ifeq ($(DDR_SPEED), 2)
	echo ##
else ifeq ($(DDR_SPEED), 3)
	echo ###
else ifeq ($(DDR_SPEED), 4)
	echo ####
endif
	echo ddr_speed $(DDR_SPEED)
	$(CC) $(CFLAGS) -o $@ -c $<
	
.PHONY: clean
clean:
	rm -f $(OBJECTLIST) $(TARGET).bin *.asm
