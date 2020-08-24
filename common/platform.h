
#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef __ASSEMBLER__
#define _AC(X,Y)        X
#define _AT(T,X)        X
#else
#define _AC(X,Y)        (X##Y)
#define _AT(T,X)        ((T)(X))
#endif /* !__ASSEMBLER__*/


/****************************************************************************
 * Platform definitions
 *****************************************************************************/
 
 // CPU info
#define NUM_CORES 2
#define MAX_HART_ID 1

// The hart that non-SMP tests should run on
#ifndef NONSMP_HART
#define NONSMP_HART 0
#endif


// Memory map
#define CLINT_CTRL_ADDR _AC(0x2000000,UL)
#define CLINT_CTRL_SIZE _AC(0x10000,UL)
#define CLINT_CTRL_MTIME _AC(0x200BFF8,UL)
#define CLINT_CTRL_HART0_MTIMECMP _AC(0x2004000,UL)
#define CLINT_CTRL_HART1_MTIMECMP _AC(0x2004008,UL)

#endif
