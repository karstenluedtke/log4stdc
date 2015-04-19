
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
#include "barefootc/container.h"
#include "barefootc/string.h"
#include "barefootc/pair.h"
#include "log4stdc.h"

static int init_int_string_pair(void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pair(struct bfc_int_string_pair *pair);

static size_t pair_clonesize(const struct bfc_int_string_pair *pair);
static unsigned pair_hashcode(const struct bfc_int_string_pair *pair, int hashlen);
static int pair_equals(const struct bfc_int_string_pair *pair,
			const struct bfc_int_string_pair *other);
static void dump_pair(const struct bfc_int_string_pair *pair,
			int depth, struct l4sc_logger *log);
static bfc_cobjptr_t pair_first(const struct bfc_int_string_pair *pair);
static bfc_objptr_t pair_index(struct bfc_int_string_pair *pair, size_t pos);
static bfc_objptr_t create_int_string_pair_element(
			struct bfc_int_string_pair *pair,
			size_t pos, bfc_objptr_t val, struct mempool *pool);

struct bfc_pair_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_int_pair_class *,
		struct bfc_int_string_pair*, const struct bfc_int_string_pair*,
		bfc_object_t)
};

const struct bfc_pair_class bfc_int_string_pair_class = {
	.super 		= NULL,
	.name 		= "integer string pair",
	.init 		= init_int_string_pair,
	.destroy 	= destroy_pair,
	.clonesize	= pair_clonesize,
	.hashcode 	= pair_hashcode,
	.equals 	= pair_equals,
	.dump	 	= dump_pair,
	.first		= pair_first,
	.index		= pair_index,
	.create		= create_int_string_pair_element,
};

int
bfc_init_int_basic_string_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_int_string_pair *pair = (struct bfc_int_string_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s: bufsize %d, need %d",
			__FUNCTION__, (int) bufsize, (int) sizeof(*pair));
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_int_string_pair_class;
	bfc_init_integer_object(&pair->first, sizeof(pair->first), pool);
	bfc_init_basic_string(&pair->second, sizeof(pair->second), pool);
	return (BFC_SUCCESS);
}

int
bfc_init_int_shared_string_pair(void *buf, size_t bufsize)
{
	struct bfc_int_string_pair *pair = (struct bfc_int_string_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s: bufsize %d, need %d",
			__FUNCTION__, (int) bufsize, (int) sizeof(*pair));
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_int_string_pair_class;
	bfc_init_integer_object(&pair->first, sizeof(pair->first), NULL);
	bfc_init_shared_string_buffer(&pair->second,sizeof(pair->second),"",0);
	return (BFC_SUCCESS);
}

static int
init_int_string_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	return (bfc_init_int_shared_string_pair(buf, bufsize));
}

static bfc_objptr_t
create_int_string_pair_element(struct bfc_int_string_pair *pair, size_t pos,
				bfc_objptr_t val, struct mempool *pool)
{
	bfc_objptr_t p = (bfc_objptr_t) &pair->first;

	if (pos > 0) {
		bfc_strptr_t str = (bfc_strptr_t) val;
		size_t bufsize = sizeof(pair->second);

		p = (bfc_objptr_t) &pair->second;

		bfc_destroy(p);

		if (str && BFC_CLASS(str) && pool) {
			bfc_init_basic_string_copy(p, bufsize, pool, str);
		} else if (str && BFC_CLASS(str)) {
			bfc_init_shared_string_substr(p, bufsize,
						      str, 0, BFC_NPOS);
		} else {
			bfc_init_shared_string_buffer(p, bufsize, "", 0);
		}
	} else {
		bfc_clone_object(val, p, sizeof(pair->first), pool);
	}

	return (p);
}

size_t
pair_clonesize(const struct bfc_int_string_pair *pair)
{
	return (sizeof(struct bfc_int_string_pair));
}

static void
destroy_pair(struct bfc_int_string_pair *pair)
{
	if (BFC_CLASS(pair)) {
		bfc_destroy(&pair->second);
		pair->vptr = NULL;
	}
}

static bfc_cobjptr_t
pair_first(const struct bfc_int_string_pair *pair)
{
	return ((bfc_cobjptr_t) &pair->first);
}

static bfc_objptr_t
pair_index(struct bfc_int_string_pair *pair, size_t pos)
{
	return ((pos > 0)? (bfc_objptr_t) &pair->second:
			   (bfc_objptr_t) &pair->first);
}

static unsigned  
pair_hashcode(const struct bfc_int_string_pair *pair, int hashlen)
{
	return (bfc_object_hashcode(&pair->first, hashlen));
}

static int
pair_equals(const struct bfc_int_string_pair *pair,
	    const struct bfc_int_string_pair *other)
{
	return (bfc_equal_object(&pair->first, &other->first) &&
		bfc_equal_object(&pair->second, &other->second));
}

static void
dump_pair(const struct bfc_int_string_pair *pair, int depth,
	  struct l4sc_logger *log)
{
	if (pair && BFC_CLASS(pair)) {
		L4SC_DEBUG(log, "%s @%p: %ld",
			((bfc_classptr_t) BFC_CLASS(pair))->name, pair,
			(long) pair->first.u.n);
		if (depth > 1) {
			bfc_cobjptr_t obj;
			if ((obj = bfc_container_cindex(pair, 1)) != NULL) {
				bfc_object_dump(obj, depth-1, log);
			}
		}
	}
}
