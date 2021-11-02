# Cross Compile
Download compiler according to your enviroment as the below link:

https://github.com/sifive/freedom-tools/releases/tag/v2020.12.0

Add the compiler to your PATH:

export PATH=/home/user/compiler/bin:$PATH

# Build
Enter the build directory, generate the ddrinit-2133-xxxxxx.bin.out file after make

# Upgrade
Follow the instruction as the below steps to update the ddrinit
	
Step 1:	After the hardware connections(power and serial port), power on the StarLight / VisionFive and you will see the startup information as follows. 
	
	bootloader version:211102-0b86f96
	ddr 0x00000000, 1M test
	ddr 0x00100000, 2M test
	DDR clk 2133M,Version: 211102-d086aee
	2

Step 2:	Press any key as soon as it starts up to enter the upgrade menu. In this menu, you can only update u-boot.


	bootloader version:211102-0b86f96
	ddr 0x00000000, 1M test
	ddr 0x00100000, 2M test
	DDR clk 2133M,Version: 211102-d086aee
	0
	***************************************************
	*************** FLASH PROGRAMMING *****************
	***************************************************
	0:update uboot
	1:quit
	select the function:

Step 3: Type *root@s5t* and press *Enter* to enter the extended version of the upgrade menu. In this menu, you can update u-boot, secondboot and ddrinit.

	***************************************************
	*************** FLASH PROGRAMMING *****************
	***************************************************
	0:update uboot
	1:quit
	select the function: root@s5t
	0:update second boot
	1:update ddr init boot
	2:update uboot
	3:quit
	select the function:

Step 4: Type *1* and press *Enter* to update the ddrinit.
Step 5: Navigate to File > Transfer > XMODEM > Send... and choose the ddrinit-2133-xxxxxx.bin.out.
