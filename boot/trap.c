#include "encoding.h"
#include "uart.h"
#include "comdef.h"

#if __riscv_xlen == 32
  #define MCAUSE_INT         0x80000000UL
  #define MCAUSE_CAUSE       0x000003FFUL
#else
   #define MCAUSE_INT         0x8000000000000000UL
   #define MCAUSE_CAUSE       0x00000000000003FFUL
#endif


uintptr_t handle_trap(uintptr_t mcause, uintptr_t epc)
{
#if 0
	if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_EXT)) {

		rlSendString("extern interrupt.\n");

	} else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_TIMER)){
		rlSendString("timer interrupt.\n");

	} else if (mcause & MCAUSE_INT) {
		rlSendString("local interrupt.\n");
	}
	else {
		rlSendString("unhandle trap.\n");
	}
#endif
	printk("trap mcause:0x%x epc:0x%x\n",mcause,epc);
	return epc;
}

void trap_entry(void)
{
  unsigned long mcause = read_csr(mcause);
  unsigned long mepc = read_csr(mepc);
  handle_trap(mcause, mepc);
}


