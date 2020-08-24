// See LICENSE for license details.
#include "encoding.h"
#include "platform.h"
#include "sys.h"

/* This is defined in sifive/platform.h, but that can't be included from
 * assembly. */
	.section .init
	.globl _start

_start:

	la t0, trap_entry
	csrw mtvec, t0
	csrwi mstatus, 0
	csrwi mie, 0

	// Allocate 4 KiB stack for each hart
	csrr t0, mhartid
	slli t0, t0, 12
	la sp, _sp
	sub sp, sp, t0

	li	 a1, NONSMP_HART;
	csrr a0, mhartid;
	bne  a0, a1, .LbootOtherHart //other hart

	// Load data section
	la t0, _data_lma
	la t1, _data
	beq t0, t1, 2f
	la t2, _edata
	bgeu t1, t2, 2f
1:
	ld t3, 0(t0)
	sd t3, 0(t1)
	addi t0, t0, 8
	addi t1, t1, 8
	bltu t1, t2, 1b
2:

	/* Clear bss section */
	la t1, _bss_start
	la t2, _bss_end
	bgeu t1, t2, 4f
3:
  sd   x0, 0(t1)
  addi t1, t1, 8
  blt  t1, t2, 3b
4:
	/*only hart 0*/
	call BootMain
	j .enter_uboot
	
.LbootOtherHart:
#if 0 //no need to clear
	li s1, CLINT_CTRL_ADDR
	csrr a0, mhartid
	slli s2, a0, 2
	add s2, s2, s1
	sw zero, 0(s2)
	fence
	csrw mip, 0
#endif

	# core 1 jumps to main_other_hart
	# Set MSIE bit to receive IPI
	li a2, MIP_MSIP
	csrw mie, a2
	
.LwaitOtherHart:
	# Wait for an IPI to signal that its safe to boot
//	call second_hart
	wfi 	
	# Only start if MIP_MSIP is set
	csrr a2, mip
	andi a2, a2, MIP_MSIP
	beqz a2, .LwaitOtherHart

	li s1, CLINT_CTRL_ADDR
	csrr a0, mhartid
	slli s2, a0, 2
	add s2, s2, s1
	sw zero, 0(s2)
	fence
	csrw mip, 0
	li a2, NUM_CORES  
	bltu a0, a2, .enter_uboot
	j .LwaitOtherHart

.enter_uboot:
	li t0, DEFAULT_UBOOT_ADDR
	csrr a0, mhartid
	la a1, 0
	jr t0

