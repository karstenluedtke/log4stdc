
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/string.h"
#include "barefootc/atomic.h"
#include "log4stdc.h"

void
bfc_string_init_refcount(bfc_strptr_t s, int n)
{
	bfc_init_atomic_counter(s->refc, n);
}

void
bfc_string_incr_refcount(bfc_strptr_t s)
{
	(void) bfc_incr_atomic_counter(s->refc);
}

int
bfc_string_decr_refcount(bfc_strptr_t s)
{
	int unrefd = bfc_decr_and_test_atomic_counter(s->refc);
	if (unrefd) {
		bfc_destroy(s);
	}
	return (!unrefd);
}

