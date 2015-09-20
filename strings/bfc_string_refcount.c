
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/string.h"
#include "barefootc/atomic.h"
#include "barefootc/mempool.h"
#include "log4stdc.h"

int
bfc_string_init_refcount(bfc_objptr_t s, int n)
{
#ifdef HAVE_STRING_REFCOUNT
	bfc_init_atomic_counter(s->refc, n);
	return (n);
#else
	return (-ENOSYS);
#endif
}

int
bfc_string_incr_refcount(bfc_objptr_t s)
{
#ifdef HAVE_STRING_REFCOUNT
	int incremented_refcount = bfc_incr_atomic_counter(s->refc);
	return (incremented_refcount);
#else
	return (-ENOSYS);
#endif
}

int
bfc_string_decr_refcount(bfc_objptr_t s)
{
#ifdef HAVE_STRING_REFCOUNT
	int decremented_refcount = bfc_decr_atomic_counter(s->refc);
	if (decremented_refcount == 0) {
		bfc_mempool_t pool = s->parent_pool;
		bfc_destroy(s);
		if (pool) {
			s->pool = NULL;
			s->parent_pool = NULL;
			mempool_free(pool, s);
		}
	}
	return (decremented_refcount);
#else
	return (-ENOSYS);
#endif
}

