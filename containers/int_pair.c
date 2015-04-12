
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

static int init_int_pair(void *buf, size_t bufsize, struct mempool *pool);
static void destroy_pair(struct bfc_int_pair *pair);

static size_t pair_clonesize(const struct bfc_int_pair *pair);
static unsigned pair_hashcode(const struct bfc_int_pair *pair, int hashlen);
static int pair_equals(const struct bfc_int_pair *pair,
			const struct bfc_int_pair *other);
static void dump_pair(const struct bfc_int_pair *pair,
			int depth, struct l4sc_logger *log);
static const int *pair_first(const struct bfc_int_pair *pair);
static int *pair_index(struct bfc_int_pair *pair, size_t pos);
static int *create_int_pair_element(struct bfc_int_pair *pair,
			size_t pos, int *valp, struct mempool *pool);

struct bfc_pair_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_int_pair_class *,
		struct bfc_int_pair *, const struct bfc_int_pair *,
		int)
};

const struct bfc_pair_class bfc_int_pair_class = {
	.super 		= NULL,
	.name 		= "int pair",
	.init 		= init_int_pair,
	.destroy 	= destroy_pair,
	.clonesize	= pair_clonesize,
	.hashcode 	= pair_hashcode,
	.equals 	= pair_equals,
	.dump	 	= dump_pair,
	.first		= pair_first,
	.index		= pair_index,
	.create		= create_int_pair_element,
};

static int
init_int_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_int_pair *pair = (struct bfc_int_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_int_pair_class;
	pair->first = pair->second = 0;
	return (BFC_SUCCESS);
}

static int *
create_int_pair_element(struct bfc_int_pair *pair, size_t pos,
			   int *valp, struct mempool *pool)
{
	int *p;
	
	if (pos > 0) {
		p = &pair->second;
	} else {
		p = &pair->first;
	}

	*p = valp? *valp: 0;
	return (p);
}

size_t
pair_clonesize(const struct bfc_int_pair *pair)
{
	return (sizeof(struct bfc_int_pair));
}

static void
destroy_pair(struct bfc_int_pair *pair)
{
	if (BFC_CLASS(pair)) {
		pair->vptr = NULL;
	}
}

static const int *
pair_first(const struct bfc_int_pair *pair)
{
	return (&pair->first);
}

static int *
pair_index(struct bfc_int_pair *pair, size_t pos)
{
	return ((pos > 0)? &pair->second: &pair->first);
}

static unsigned  
pair_hashcode(const struct bfc_int_pair *pair, int hashlen)
{
	return (bfc_reduce_hashcode(pair->first,
				    8*sizeof(pair->first), hashlen));
}

static int
pair_equals(const struct bfc_int_pair *pair,
	    const struct bfc_int_pair *other)
{
	return ((pair->first == other->first) &&
		(pair->second == other->second));
}

static void
dump_pair(const struct bfc_int_pair *pair,int depth,struct l4sc_logger *log)
{
	if (pair && BFC_CLASS(pair)) {
		L4SC_DEBUG(log, "%s @%p: %d, %d",
			((bfc_classptr_t) BFC_CLASS(pair))->name, pair,
			pair->first, pair->second);
	}
}
