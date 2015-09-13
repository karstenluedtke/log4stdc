
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/pair.h"
#include "barefootc/container.h"
#include "barefootc/unconst.h"
#include "log4stdc.h"

static int init_real_int_pair(void *buf, size_t bufsize, struct mempool *pool);

struct bfc_pair_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_pair_class *,
		struct bfc_number_pair *, const struct bfc_number_pair *,
		bfc_object_t)
};

const struct bfc_pair_class bfc_real_int_pair_class = {
	.super 	= (const struct bfc_pair_class *) &bfc_int_pair_class,
	.name 	= "real int pair",
	.init 	= init_real_int_pair,
};

static int
init_real_int_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_number_pair *pair = (struct bfc_number_pair *) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	if (bufsize < sizeof(*pair)) {
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_int_pair_class;
	bfc_init_real_number_object(&pair->first, sizeof(pair->first), pool);
	bfc_init_integer_object(&pair->second, sizeof(pair->second), pool);

	L4SC_DEBUG(logger, "%s(%p, %ld, %p): classes %s, %s",
		__FUNCTION__, buf, (long) bufsize, pool,
		BFC_CLASS(&pair->first)->name, BFC_CLASS(&pair->second)->name);

	return (BFC_SUCCESS);
}

