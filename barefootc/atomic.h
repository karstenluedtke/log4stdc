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

#define bfc_decr_and_test_atomic_counter(ctr) \
					(InterlockedDecrement(&(ctr)) == 0)

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_ATOMICS__)

#include <stdatomic.h>

typedef atomic_int_least32_t bfc_atomic_counter_t;

/*
 * The atomic_fetch_xxx functions return the old value before modification
 */
#define bfc_init_atomic_counter(ctr,v)	atomic_init(&(ctr),(v))
#define bfc_incr_atomic_counter(ctr)	(atomic_fetch_add(&(ctr),1) + 1)
#define bfc_decr_atomic_counter(ctr)	(atomic_fetch_sub(&(ctr),1) - 1)
#define bfc_add_atomic_counter(ctr,v)	(atomic_fetch_add(&(ctr),(v)) + (v))

#define bfc_decr_and_test_atomic_counter(ctr) (atomic_fetch_sub(&(ctr),1) == 1)

#elif defined(__GNUC__) && ( __GNUC__ >= 4 )

typedef int bfc_atomic_counter_t;

#define bfc_init_atomic_counter(ctr,v)	ctr = (v)
#define bfc_incr_atomic_counter(ctr)	__sync_add_and_fetch(&(ctr),1)
#define bfc_decr_atomic_counter(ctr)	__sync_sub_and_fetch(&(ctr),1)
#define bfc_add_atomic_counter(ctr,v)	__sync_add_and_fetch(&(ctr),(v))

#define bfc_decr_and_test_atomic_counter(ctr) \
					(__sync_fetch_and_sub(&(ctr),1) == 1)

#elif defined(__i386) || defined(__i386__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64)

#include <inttypes.h>
typedef volatile int32_t bfc_atomic_counter_t;

#define bfc_init_atomic_counter(ctr,v)	ctr = (v)

#define bfc_incr_atomic_counter(ctr)	\
	__asm__ __volatile__("lock incl %0" :"=m"(ctr) :"m"(ctr));

#define bfc_decr_and_test_atomic_counter(ctr)	\
	bfc_decr_and_test_i386_atomic(&(ctr))

static inline int bfc_decr_and_test_i386_atomic(bfc_atomic_counter_t *cp)
{
	unsigned char c;

	__asm__ __volatile__("lock decl %0; sete %1"	:"=m"(*cp), "=qm"(c)
							:"m"(*cp) : "memory");
	return (c);
}

#else

#endif

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_ATOMIC_H_ */
