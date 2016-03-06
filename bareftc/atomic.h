/**
 * @file      atomic.h
 *
 * @brief     Simple synchronization within the barefootc library.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-09-13
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_ATOMIC_H_
#define _BFC_ATOMIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__APPLE__)
#include <inttypes.h>
#include <libkern/OSAtomic.h>

typedef volatile int32_t bfc_atomic_counter_t;

#define bfc_init_atomic_counter(ctr,v)	ctr = (v)
#define bfc_incr_atomic_counter(ctr)	OSAtomicIncrement32Barrier(&(ctr))
#define bfc_decr_atomic_counter(ctr)	OSAtomicDecrement32Barrier(&(ctr))
#define bfc_add_atomic_counter(ctr,v)	OSAtomicAdd32Barrier((v),&(ctr))

#define bfc_decr_and_test_atomic_counter(ctr) \
				(OSAtomicDecrement32Barrier(&(ctr)) == 0)

#elif defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>

typedef volatile LONG bfc_atomic_counter_t;

#define bfc_init_atomic_counter(ctr,v)	ctr = (v)
#define bfc_incr_atomic_counter(ctr)	InterlockedIncrement(&(ctr))
#define bfc_decr_atomic_counter(ctr)	InterlockedDecrement(&(ctr))

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_ATOMICS__)

#include <stdatomic.h>

typedef atomic_int bfc_atomic_counter_t;

/*
 * The atomic_fetch_xxx functions return the old value before modification
 */
#define bfc_init_atomic_counter(ctr,v) \
	atomic_init((bfc_atomic_counter_t*)&(ctr),(v))

#define bfc_incr_atomic_counter(ctr) \
	(atomic_fetch_add((bfc_atomic_counter_t*)&(ctr),1) + 1)

#define bfc_decr_atomic_counter(ctr) \
	(atomic_fetch_sub((bfc_atomic_counter_t*)&(ctr),1) - 1)

#define bfc_add_atomic_counter(ctr,v) \
	(atomic_fetch_add((bfc_atomic_counter_t*)&(ctr),(v)) + (v))

#elif defined(__GNUC__) && ( __GNUC__ >= 4 )

typedef volatile int bfc_atomic_counter_t;

#define bfc_init_atomic_counter(ctr,v)	ctr = (v)
#define bfc_incr_atomic_counter(ctr)	__sync_add_and_fetch(&(ctr),1)
#define bfc_decr_atomic_counter(ctr)	__sync_sub_and_fetch(&(ctr),1)
#define bfc_add_atomic_counter(ctr,v)	__sync_add_and_fetch(&(ctr),(v))

#elif defined(__i386) || defined(__i386__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

#include <inttypes.h>
typedef volatile int32_t bfc_atomic_counter_t;

#define bfc_init_atomic_counter(ctr,v)	ctr = (v)

#define bfc_incr_atomic_counter(ctr)	\
	bfc_incr_i386_atomic(&(ctr))

static inline int bfc_incr_i386_atomic(bfc_atomic_counter_t *cp)
{
	int32_t rc;

	__asm__ __volatile__("lock incl %0; movl %0, %1"
				:"=m"(*cp), "=r"(rc)
				:"m"(*cp));

	/* we return at least 1 */
	return ((rc > 0)? rc: 1);
}

#define bfc_decr_atomic_counter(ctr)	\
	bfc_decr_i386_atomic(&(ctr))

static inline int bfc_decr_i386_atomic(bfc_atomic_counter_t *cp)
{
	int32_t rc;
	unsigned char c; /* zero flag */

	__asm__ __volatile__("lock decl %0; sete %1; movl %0, %2"
				:"=m"(*cp), "=qm"(c), "=r"(rc)
				:"m"(*cp) : "memory");

	if (c)  /* if the zero flag is set */ {
		/* then this is the exact result of the atomic operation */
		return (0);
	}
	/* otherwise we return at least 1 */
	return ((rc > 0)? rc: 1);
}

#else
#define bfc_init_atomic_counter(ctr,v)	ctr = (v)
#define bfc_incr_atomic_counter(ctr)	++(ctr)
#define bfc_decr_atomic_counter(ctr)	--(ctr)
#define bfc_add_atomic_counter(ctr,v)	ctr += (v)

#endif

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_ATOMIC_H_ */
