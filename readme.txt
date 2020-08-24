编译环境设置
	在PATH变量中增加riscv工具链所在路径[工具链要使用8.3]，进入build目录，执行make/make clean即可，最终生成的二进制文件位于build目录下

使用方式：
1.正常编译后，在build目录下会生成ddrinit.bin
2.使用build目录下的convert程序转换ddrinit.bin文件，目的是在ddrinit.bin文件的开始处增加4个字节，保存ddrinit.bin文件大小信息【命令：./convert  ddrinit.bin ddrinit.bin.out】
3.使用freedomstudio下载build目录下的fw_vic_1800.elf文件，暂停在start处
4.使用freedomstudio的import功能，将bootloader.bin.out上传至0x18080000地址，ddrinit.bin.out上传至0x18090000地址
5.继续运行fw_vic.elf程序，选择相应项，将bootloader.bin.out、ddrinit.bin.out的数据写入flash
6.下电，插入跳线，将boot_mode选择到从spi nor flash启动即可


备注：
	fw_vic_1800.elf文件是加载到0x18000000地址运行
	fw_vic_8000.elf文件是加载到0x80000000地址运行
	