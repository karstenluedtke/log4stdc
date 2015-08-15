
#if defined(__GNUC__)

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

static inline uint32_t
umul32_shr29(uint32_t a, uint32_t b)
{
	uint32_t result;

#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

	__asm__ __volatile__(
	"mul %2; shrd $29,%2,%0" : "=a"(result) : "a"(a), "d"(b) : "cc");

#elif defined(__ARMEL__) || defined(__ARMEB__) || defined(__ARM_EABI__)

	uint32_t hiword;

	__asm__ __volatile__(
	"umull %0,%1,%2,%3; lsr %3,%0,#29; orr %0,%3,%1,lsl #3"
		: "=&r"(result), "=&r"(hiword) : "r"(a), "r"(b));
		/* "=&r" means early clobbering, */
		/* these register must not be used as input registers */

#else
	result = (uint32_t) (((uint64_t) a * b) >> 29);
#endif
	return (result);
}

static inline uint32_t
umul32_shr26(uint32_t a, uint32_t b)
{
	uint32_t result;

#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

	__asm__ __volatile__(
	"mul %2; shrd $26,%2,%0" : "=a"(result) : "a"(a), "d"(b) : "cc");

#elif defined(__ARMEL__) || defined(__ARMEB__) || defined(__ARM_EABI__)

	uint32_t hiword;

	__asm__ __volatile__(
	"umull %0,%1,%2,%3; lsr %3,%0,#26; orr %0,%3,%1,lsl #6"
		: "=&r"(result), "=&r"(hiword) : "r"(a), "r"(b));
		/* "=&r" means early clobbering, */
		/* these register must not be used as input registers */

#else
	result = (uint32_t) (((uint64_t) a * b) >> 26);
#endif
	return (result);
}

static inline uint32_t
umul32_shr23(uint32_t a, uint32_t b)
{
	uint32_t result;

#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

	__asm__ __volatile__(
	"mul %2; shrd $23,%2,%0" : "=a"(result) : "a"(a), "d"(b) : "cc");

#elif defined(__ARMEL__) || defined(__ARMEB__) || defined(__ARM_EABI__)

	uint32_t hiword;

	__asm__ __volatile__(
	"umull %0,%1,%2,%3; lsr %3,%0,#23; orr %0,%3,%1,lsl #9"
		: "=&r"(result), "=&r"(hiword) : "r"(a), "r"(b));
		/* "=&r" means early clobbering, */
		/* these register must not be used as input registers */

#else
	result = (uint32_t) (((uint64_t) a * b) >> 23);
#endif
	return (result);
}

static inline uint32_t
umul32_shr19(uint32_t a, uint32_t b)
{
	uint32_t result;

#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

	__asm__ __volatile__(
	"mul %2; shrd $19,%2,%0" : "=a"(result) : "a"(a), "d"(b) : "cc");

#elif defined(__ARMEL__) || defined(__ARMEB__) || defined(__ARM_EABI__)

	uint32_t hiword;

	__asm__ __volatile__(
	"umull %0,%1,%2,%3; lsr %3,%0,#19; orr %0,%3,%1,lsl #13"
		: "=&r"(result), "=&r"(hiword) : "r"(a), "r"(b));
		/* "=&r" means early clobbering, */
		/* these register must not be used as input registers */

#else
	result = (uint32_t) (((uint64_t) a * b) >> 19);
#endif
	return (result);
}

#else
#define umul32_hiword(a,b)	(uint32_t)((((uint64_t)(a))*(b)) >> 32)
#define umul32_shr29(a,b)	(uint32_t)((((uint64_t)(a))*(b)) >> 29)
#define umul32_shr29(a,b)	(uint32_t)((((uint64_t)(a))*(b)) >> 26)
#define umul32_shr23(a,b)	(uint32_t)((((uint64_t)(a))*(b)) >> 23)
#define umul32_shr19(a,b)	(uint32_t)((((uint64_t)(a))*(b)) >> 19)
#endif
