
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
#include "log4stdc.h"

static int init_string_pair(void *buf, size_t bufsize, struct mempool *pool);
static void destroy_pair(struct bfc_string_pair *pair);

static size_t pair_clonesize(const struct bfc_string_pair *pair);
static unsigned pair_hashcode(const struct bfc_string_pair *pair);
static int pair_equals(const struct bfc_string_pair *pair,
			const struct bfc_string_pair *other);
static void dump_pair(const struct bfc_string_pair *pair,
			int depth, struct l4sc_logger *log);

struct bfc_string_pair_class {
	BFC_CLASSHDR(const struct bfc_string_pair_class *,
		     struct bfc_string_pair *,
		     const struct bfc_string_pair *)
};

const struct bfc_string_pair_class bfc_string_pair_class = {
	.super 		= NULL,
	.name 		= "string pair",
	.init 		= init_string_pair,
	.destroy 	= destroy_pair,
	.clonesize	= pair_clonesize,
	.hashcode 	= pair_hashcode,
	.equals 	= pair_equals,
	.dump	 	= dump_pair,
};

int
bfc_init_basic_string_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_string_pair *pair = (struct bfc_string_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_string_pair_class;
	bfc_init_basic_string(&pair->first, sizeof(pair->first), pool);
	bfc_init_basic_string(&pair->second, sizeof(pair->second), pool);
	return (BFC_SUCCESS);
}

int
bfc_init_shared_string_pair(void *buf, size_t bufsize)
{
	struct bfc_string_pair *pair = (struct bfc_string_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_string_pair_class;
	bfc_init_shared_string_buffer(&pair->first, sizeof(pair->first), "",0);
	bfc_init_shared_string_buffer(&pair->second,sizeof(pair->second),"",0);
	return (BFC_SUCCESS);
}

static int
init_string_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	return (bfc_init_shared_string_pair(buf, bufsize));
}

size_t
pair_clonesize(const struct bfc_string_pair *pair)
{
	return (sizeof(struct bfc_string_pair));
}

static void
destroy_pair(struct bfc_string_pair *pair)
{
	if (BFC_CLASS(pair)) {
		bfc_destroy(&pair->first);
		bfc_destroy(&pair->second);
		pair->vptr = NULL;
	}
}

static unsigned  
pair_hashcode(const struct bfc_string_pair *pair)
{
	return (bfc_object_hashcode(&pair->first));
}

static int
pair_equals(const struct bfc_string_pair *pair,
	    const struct bfc_string_pair *other)
{
	return (bfc_equal_object(&pair->first, &other->first) &&
		bfc_equal_object(&pair->second, &other->second));
}

static void
dump_pair(const struct bfc_string_pair *pair,int depth,struct l4sc_logger *log)
{
	if (pair && BFC_CLASS(pair)) {
		L4SC_DEBUG(log, "bfc_string_pair @%p:", pair);
		if (depth > 1) {
			bfc_object_dump(&pair->first, depth-1, log);
			bfc_object_dump(&pair->second, depth-1, log);
		}
	}
}

