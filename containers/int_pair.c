
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
#include "barefootc/unconst.h"
#include "log4stdc.h"

static int init_int_pair(void *buf, size_t bufsize, struct mempool *pool);
static int clone_pair(const struct bfc_number_pair *obj,
				void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pair(struct bfc_number_pair *pair);

static size_t pair_clonesize(const struct bfc_number_pair *pair);
static unsigned pair_hashcode(const struct bfc_number_pair *pair, int hashlen);
static int pair_equals(const struct bfc_number_pair *pair,
			const struct bfc_number_pair *other);
static void dump_pair(const struct bfc_number_pair *pair,
			int depth, struct l4sc_logger *log);
static bfc_cobjptr_t pair_first(const struct bfc_number_pair *pair);
static bfc_objptr_t pair_index(struct bfc_number_pair *pair, size_t pos);
static bfc_objptr_t place_int_pair_element(struct bfc_number_pair *pair,
			size_t pos, bfc_objptr_t val, struct mempool *pool);

struct bfc_pair_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_number_pair_class *,
		struct bfc_number_pair *, const struct bfc_number_pair *,
		bfc_object_t)
};

const struct bfc_pair_class bfc_int_pair_class = {
	.super 		= NULL,
	.name 		= "int pair",
	.init 		= init_int_pair,
	.destroy 	= destroy_pair,
	.clone		= clone_pair,
	.clonesize	= pair_clonesize,
	.hashcode 	= pair_hashcode,
	.equals 	= pair_equals,
	.dump	 	= dump_pair,
	.first		= pair_first,
	.index		= pair_index,
	.place		= place_int_pair_element,
};

static int
init_int_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_number_pair *pair = (struct bfc_number_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_int_pair_class;
	bfc_init_integer_object(&pair->first, sizeof(pair->first), pool);
	bfc_init_integer_object(&pair->second, sizeof(pair->second), pool);
	return (BFC_SUCCESS);
}

static bfc_objptr_t
place_int_pair_element(struct bfc_number_pair *pair, size_t pos,
			   bfc_objptr_t val, struct mempool *pool)
{
	bfc_objptr_t p;
	
	if (pos > 0) {
		p = (bfc_objptr_t ) &pair->second;
		bfc_clone_object(val, p, sizeof(pair->second), pool);
	} else {
		p = (bfc_objptr_t ) &pair->first;
		bfc_clone_object(val, p, sizeof(pair->first), pool);
	}

	return (p);
}

static int
clone_pair(const struct bfc_number_pair *obj,
	   void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_number_pair *pair = (struct bfc_number_pair *) buf;
	struct bfc_number_pair *src = BFC_UNCONST(struct bfc_number_pair *, obj);
	size_t size = bfc_object_size(obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(pair, src, size);
	memset(&pair->first, 0, sizeof(pair->first));
	memset(&pair->second, 0, sizeof(pair->second));
	place_int_pair_element(pair, 0, (bfc_objptr_t)&src->first, pool);
	place_int_pair_element(pair, 1, (bfc_objptr_t)&src->second, pool);
	return (BFC_SUCCESS);
}

size_t
pair_clonesize(const struct bfc_number_pair *pair)
{
	return (sizeof(struct bfc_number_pair));
}

static void
destroy_pair(struct bfc_number_pair *pair)
{
	if (BFC_CLASS(pair)) {
		pair->vptr = NULL;
	}
}

static bfc_cobjptr_t
pair_first(const struct bfc_number_pair *pair)
{
	return ((bfc_cobjptr_t) &pair->first);
}

static bfc_objptr_t
pair_index(struct bfc_number_pair *pair, size_t pos)
{
	return ((bfc_objptr_t)((pos > 0)? &pair->second: &pair->first));
}

static unsigned  
pair_hashcode(const struct bfc_number_pair *pair, int hashlen)
{
	return (bfc_object_hashcode(&pair->first, hashlen));
}

static int
pair_equals(const struct bfc_number_pair *pair,
	    const struct bfc_number_pair *other)
{
	return (bfc_equal_object(&pair->first, &other->first) &&
		bfc_equal_object(&pair->second, &other->second));
}

static void
dump_pair(const struct bfc_number_pair *pair,int depth,struct l4sc_logger *log)
{
	if (pair && BFC_CLASS(pair)) {
		L4SC_DEBUG(log, "%s @%p: %ld, %ld",
			((bfc_classptr_t) BFC_CLASS(pair))->name, pair,
			(long) pair->first.u.n, (long) pair->second.u.n);
	}
}
