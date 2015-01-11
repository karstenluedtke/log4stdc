
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/string.h"
#include "barefootc/atomic.h"
#include "barefootc/mempool.h"
#include "log4stdc.h"

void
bfc_string_init_refcount(bfc_strptr_t s, int n)
{
#ifdef HAVE_STRING_REFCOUNT
	 bfc_init_atomic_counter(s->refc, n);
#endif
}

void
bfc_string_incr_refcount(bfc_strptr_t s)
{
#ifdef HAVE_STRING_REFCOUNT
	(void) bfc_incr_atomic_counter(s->refc);
#endif
}

int
bfc_string_decr_refcount(bfc_strptr_t s)
{
#ifdef HAVE_STRING_REFCOUNT
	int unrefd = bfc_decr_and_test_atomic_counter(s->refc);
	if (unrefd) {
		struct mempool *pool = s->parent_pool;
		bfc_destroy(s);
		if (pool) {
			s->pool = NULL;
			s->parent_pool = NULL;
			mempool_free(pool, s);
		}
	}
	return (!unrefd);
#else
	return (-ENOSYS);
#endif
}

