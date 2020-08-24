/* Copyright (c) 2020 StarFiveTech, Inc */

OUTPUT_ARCH( "riscv" )

MEMORY
{
  ram (rwx) : ORIGIN = 0x18080000, LENGTH = 192K
}

ENTRY( _start )

PHDRS
{
  text PT_LOAD;
  rodata PT_LOAD;
  data PT_LOAD;
  bss PT_LOAD;
}

SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;
  
  .init : 
  {
    KEEP (*(SORT_NONE(.init)))
  } >ram AT>ram :text
  
  .text :
  {
    PROVIDE(_ftext = .);
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
	PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);
  }>ram AT>ram :text
  
  .rodata :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
  }>ram AT>ram :rodata
  
  .eh_frame :
  {
    *(.eh_frame)
  } >ram AT>ram  :text
  

  
  .srodata :
  {
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata.*)
  } >ram AT>ram  :rodata
  
  .data :
  {
    *(.data .data.* .gnu.linkonce.d.*)
	. = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.tohost) /* TODO: Support sections that aren't explicitly listed in this linker script */
  } >ram :data

  .sdata :
  {
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } >ram AT>ram :data

  PROVIDE(_data = ADDR(.data));
  PROVIDE(_data_lma = LOADADDR(.data));
  
  PROVIDE( _edata = . );

  . = ALIGN(8);
  PROVIDE( _bss_start = . );
  
  .bss :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
	. = ALIGN(8);
    PROVIDE(_ebss = .);
  } >ram AT>ram :bss

  PROVIDE( _end = . );
  PROVIDE( _bss_end = . );
  
  /*
   * heap_stack_region_usable_end: (ORIGIN(ram) + LENGTH(ram))
   * heap_stack_min_size: 4096
   * heap_stack_max_size: 8292
   */
  PROVIDE(_sp = ALIGN(MIN((ORIGIN(ram) + LENGTH(ram)), _bss_end + 8292) - 7, 8));
  
  /*
	* Protect top of stack from heap, but this will not protect the heap from
	* stack overruns.
  */
  PROVIDE(_heap_end = _sp - 0x100);
  
  .stack :
  {
     ASSERT(_sp >= (_bss_end + 4096), "Error: No room left for the heap and stack");
  }
}