


#ifndef _COMDEF_H_
#define _COMDEF_H_

typedef unsigned char 		UINT8;
typedef unsigned char 		UCHAR;
typedef unsigned short  	UINT16;
typedef unsigned  int	UINT32;
typedef unsigned short int	WCHAR;
typedef unsigned short int	UNICODE;
typedef unsigned long 	UINT64;
typedef  int		INT64;
typedef  int			INT32;
typedef short int			INT16;
typedef char				INT8;
typedef char				CHAR;
typedef unsigned short int	bool;
typedef unsigned short int	STATUS_T;
typedef signed char 		s8;
typedef unsigned char 		u8;
typedef signed short 		s16;
typedef unsigned short 	u16;
typedef signed int 		s32;
typedef unsigned int 		u32;
typedef unsigned long 		u64;
//typedef int 				ssize_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long  uintptr_t;
typedef unsigned long  uint64_t;
typedef signed  long s64;
typedef unsigned long size_t;


#define NULL				0

#define FALSE           0
#define TRUE            1
#define typeof __typeof__

#define ARCH_DMA_MINALIGN       32


#define BIT_0				0x00000001
#define BIT_1				0x00000002
#define BIT_2				0x00000004
#define BIT_3				0x00000008
#define BIT_4				0x00000010
#define BIT_5				0x00000020
#define BIT_6				0x00000040
#define BIT_7				0x00000080
#define BIT_8				0x00000100
#define BIT_9				0x00000200
#define BIT_10				0x00000400
#define BIT_11				0x00000800
#define BIT_12				0x00001000
#define BIT_13				0x00002000
#define BIT_14				0x00004000
#define BIT_15				0x00008000
#define BIT_16				0x00010000
#define BIT_17				0x00020000
#define BIT_18				0x00040000
#define BIT_19				0x00080000
#define BIT_20				0x00100000
#define BIT_21				0x00200000
#define BIT_22				0x00400000
#define BIT_23				0x00800000
#define BIT_24				0x01000000
#define BIT_25				0x02000000
#define BIT_26				0x04000000
#define BIT_27				0x08000000
#define BIT_28				0x10000000
#define BIT_29				0x20000000
#define BIT_30				0x40000000
#define BIT_31				0x80000000

#define RSP_SUCCESS 			0
#define RSP_GENERAL_FAIL		1

#ifndef BIT
#define BIT(x)  (1UL << (x))
#endif

#ifndef writeq
static inline void writeq(u64 val, volatile void *addr)
{
	asm volatile("sd %0, 0(%1)" : : "r" (val), "r" (addr));
}
#endif

#ifndef readq
static inline u64 readq(volatile void *addr)
{
	u64 val;

	asm volatile("ld %0, 0(%1)" : "=r" (val) : "r" (addr));
	return val;
}
#endif

#ifndef writel
static inline void writel(u32 val, volatile void *addr)
{
	asm volatile("sw %0, 0(%1)" : : "r" (val), "r" (addr));
}
#endif

#ifndef readl
static inline u32 readl(volatile void *addr)
{
	u32 val;

	asm volatile("lw %0, 0(%1)" : "=r" (val) : "r" (addr));
	return val;
}
#endif

#ifndef writew

static inline void writew(u16 val, volatile void *addr)
{
	asm volatile("sh %0, 0(%1)" : : "r" (val), "r" (addr));
}
#endif

#ifndef readw
static inline u16 readw(const volatile void *addr)
{
	u16 val;

	asm volatile("lh %0, 0(%1)" : "=r" (val) : "r" (addr));
	return val;
}
#endif


#define inpb(port)			(*((volatile UINT8 *) (port)))
#define inph(port)			(*((volatile UINT16*) (port)))
#define inpw(port)			(*((volatile UINT32*) (port)))

#define outpb(port, val) 	(*((volatile UINT8 *) (port)) = ((UINT8 ) (val)))
#define outph(port, val) 	(*((volatile UINT16*) (port)) = ((UINT16) (val)))
#define outpw(port, val)	(*((volatile UINT32*) (port)) = ((UINT32) (val)))

#define MA_INB( io )		(UINT8 ) inpb( io )
#define MA_INH( io )		(UINT16) inph( io )
#define MA_INW( io )		(UINT32) inpw( io )

#define MA_INBM( io, mask ) ( inpb( io ) & (mask) )
#define MA_INHM( io, mask ) ( inph( io ) & (mask) )
#define MA_INWM( io, mask ) ( inpw( io ) & (mask) )
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
/* The `const' in roundup() prevents gcc-3.3 from calling __divdi3 */
#define roundup(x, y) (					\
{							\
	const typeof(y) __y = y;			\
	(((x) + (__y - 1)) / __y) * __y;		\
}							\
)
#define LOG2(x) (((x & 0xaaaaaaaa) ? 1 : 0) + ((x & 0xcccccccc) ? 2 : 0) + \
		 ((x & 0xf0f0f0f0) ? 4 : 0) + ((x & 0xff00ff00) ? 8 : 0) + \
		 ((x & 0xffff0000) ? 16 : 0))

#define ___swab32(x) \
	((u32)( \
		(((u32)(x) & (u32)0x000000ffUL) << 24) | \
		(((u32)(x) & (u32)0x0000ff00UL) <<  8) | \
		(((u32)(x) & (u32)0x00ff0000UL) >>  8) | \
		(((u32)(x) & (u32)0xff000000UL) >> 24) ))
#define __be32_to_cpu(x) ___swab32((u32)(x))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define MA_OUTB( io, val )\
{\
	outpb( io, val );\
}

#define MA_OUTH( io, val )\
{\
	outph( io, val );\
}

#define MA_OUTW( io, val )\
{\
	outpw( io, val );\
}

#define MA_OUTBM( io, mask, val ) \
{\
	UINT8 temp;\
	(temp) =(((MA_INW(io) & (UINT8)(~(mask))) | ((UINT8)((val) & (mask)))));\
	outpb( io, temp );\
}

#define MA_OUTHM( io, mask, val ) \
{\
	UINT16 temp;\
	(temp) =(((MA_INH(io) & (UINT16)(~(mask))) |((UINT16)((val) & (mask)))));\
	outph( io, temp );\
}

#define MA_OUTWM( io, mask, val ) \
{\
	UINT32 temp;\
	(temp) =(((MA_INW(io) & (UINT32)(~(mask))) |((UINT32)((val) & (mask)))));\
	outpw( io, temp );\
}
#define min(x,y) 			((x)<(y)?(x):(y))
#define min3(x, y, z) min(min(x, y), z)

#define max(x, y) ((x)>(y)?(x):(y))

#define ROUND(a,b)		(((a) + (b) - 1) & ~((b) - 1))

#define apb_write(addr, data) 	((*(volatile unsigned int *) (addr)) = ((u32)data))
#define apb_read(addr)          (*((volatile unsigned int*) (addr)))

#define ALIGN(x,a)		__ALIGN_MASK((x),(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)	(((x)+(mask))&~(mask))

#define PAD_COUNT(s, pad) (((s) - 1) / (pad) + 1)
#define PAD_SIZE(s, pad) (PAD_COUNT(s, pad) * pad)
#define ALLOC_ALIGN_BUFFER_PAD(type, name, size, align, pad)		\
	char __##name[ROUND(PAD_SIZE((size) * sizeof(type), pad), align)  \
		      + (align - 1)];					\
									\
	type *name = (type *)ALIGN((uintptr_t)__##name, align)
#define ALLOC_ALIGN_BUFFER(type, name, size, align)		\
	ALLOC_ALIGN_BUFFER_PAD(type, name, size, align, 1)
#define ALLOC_CACHE_ALIGN_BUFFER_PAD(type, name, size, pad)		\
	ALLOC_ALIGN_BUFFER_PAD(type, name, size, ARCH_DMA_MINALIGN, pad)
#define ALLOC_CACHE_ALIGN_BUFFER(type, name, size)			\
	ALLOC_ALIGN_BUFFER(type, name, size, ARCH_DMA_MINALIGN)

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)



#ifndef false
#define false				(0)
#endif

#ifndef true
#define true				(1)
#endif

#endif
