
static inline uint32_t
umul32_hiword(uint32_t a, uint32_t b)
{
	uint32_t hiword;

#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

	__asm__ __volatile__(
	"mul %2" : "=d"(hiword) : "a"(a), "d"(b) : "cc");

#elif defined(__ARMEL__) || defined(__ARMEB__) || defined(__ARM_EABI__)

	uint32_t loword;

	__asm__ __volatile__(
	"umull %0,%1,%2,%3" : "=&r"(loword), "=&r"(hiword) : "r"(a), "r"(b));
			   /* "=&r" means early clobbering, */
		/* these register must not be used as input registers */

#else
	hiword = (uint32_t) (((uint64_t) a * b) >> 32);
#endif
	return (hiword);
}

