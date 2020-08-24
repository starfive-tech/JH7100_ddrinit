#ifndef _ASM_GENERIC_DIV64_H
#define _ASM_GENERIC_DIV64_H
/*
 * Copyright (C) 2003 Bernardo Innocenti <bernie@develer.com>
 * Based on former asm-ppc/div64.h and asm-m68knommu/div64.h
 *
 * The semantics of do_div() are:
 *
 * u32 do_div(UINT64 *n, u32 base)
 * {
 *	u32 remainder = *n % base;
 *	*n = *n / base;
 *	return remainder;
 * }
 *
 * NOTE: macro parameter n is evaluated multiple times,
 *       beware of side effects!
 */

//#include <linux/types.h>
#include <comdef.h>

static unsigned int __div64_32(UINT64 *n, unsigned int base)
{
	UINT64 rem = *n;
	UINT64 b = base;
	UINT64 res, d = 1;
	unsigned int high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (UINT64) high << 32;
		rem -= (UINT64) (high*base) << 32;
	}

	while ((s64)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

/* The unnecessary pointer compare is there
 * to check for type safety (n must be 64bit)
 */
# define do_div(n,base) ({				\
	u32 __base = (base);			\
	u32 __rem;					\
	(void)(((typeof((n)) *)0) == ((UINT64 *)0));	\
	if (((n) >> 32) == 0) {			\
		__rem = (u32)(n) % __base;		\
		(n) = (u32)(n) / __base;		\
	} else						\
		__rem = __div64_32(&(n), __base);	\
	__rem;						\
 })

/* Wrapper for do_div(). Doesn't modify dividend and returns
 * the result, not reminder.
 */
static inline UINT64 lldiv(UINT64 dividend, u32 divisor)
{
	UINT64 __res = dividend;
	do_div(__res, divisor);
	return(__res);
}

static inline UINT64 div_u64_rem(UINT64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

#endif /* _ASM_GENERIC_DIV64_H */
