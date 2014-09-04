/**
 * @file bfc_datetime_frac_from_decimal.c
 */

#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/datetime.h"
#include "umul32_hiword.h"


uint32_t
bfc_datetime_frac_from_decimal(unsigned long sub, int sublen)
{
	if (sublen < 1) {
		return (0);
	}
	switch (sublen) {
	case 0: return (0);
	case 1: return ((uint32_t) 429496729uL * (uint32_t) sub);
	case 2: return ((uint32_t)  42949673uL * (uint32_t) sub);
	case 3: return ((uint32_t)   4294967uL * (uint32_t) sub);
	case 4: return ((uint32_t)    429497uL * (uint32_t) sub);
	case 5: return ((uint32_t)     42950uL * (uint32_t) sub);

	case 6: /* return (  2**32 * usecs / 10**6
		 *      =   usecs * (2**32 / 10**6) 
		 *      =  (usecs * (2**51 / 10**6)) >> 19
		 *     ~= ((usecs * 2251799814) >> 32) << 13
		 */
		return (umul32_hiword((uint32_t) sub, 2251799814uL) << 13);

	case 7: /* return (  2**32 * sub / 10**7
		 *      =   sub * (2**32 / 10**7) 
		 *      =  (sub * (2**55 / 10**7)) >> 23
		 *     ~= ((sub * 3602879702) >> 32) << 9
		 */
		return (umul32_hiword((uint32_t) sub, 3602879702uL) << 9);

	case 8: /* return (  2**32 * sub / 10**8
		 *      =   sub * (2**32 / 10**8) 
		 *      =  (sub * (2**58 / 10**8)) >> 26
		 *     ~= ((sub * 2882303762) >> 32) << 6
		 */
		return (umul32_hiword((uint32_t) sub, 2882303762uL) << 6);

	case 9: /* return (  2**32 * nsecs / 10**9
		 *      =   nsecs * (2**32 / 10**9) 
		 *      =  (nsecs * (2**61 / 10**9)) >> 29
		 *     ~= ((nsecs * 2305843010) >> 32) << 3
		 */
		return (umul32_hiword((uint32_t) sub, 2305843010uL) << 3);

	default: ;
		while (sublen > 9) {
			sub /= 10;
			sublen -= 1;
		}
	}
	return (umul32_hiword((uint32_t) sub, 2305843010uL) << 3);
}

