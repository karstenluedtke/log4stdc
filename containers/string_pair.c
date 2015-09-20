
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

static int init_string_pair(void *buf, size_t bufsize, bfc_mempool_t pool);
static int clone_pair(const struct bfc_string_pair *obj,
				void *buf,size_t bufsize,bfc_mempool_t pool);
static void destroy_pair(struct bfc_string_pair *pair);

static size_t pair_clonesize(const struct bfc_string_pair *pair);
static unsigned pair_hashcode(const struct bfc_string_pair *pair, int hashlen);
static int pair_equals(const struct bfc_string_pair *pair,
			const struct bfc_string_pair *other);
static void dump_pair(const struct bfc_string_pair *pair,
			int depth, struct l4sc_logger *log);
static bfc_cobjptr_t pair_first(const struct bfc_string_pair *pair);
static bfc_objptr_t  pair_index(struct bfc_string_pair *pair, size_t pos);
static bfc_objptr_t  place_string_pair_element(struct bfc_string_pair *pair,
			size_t pos, bfc_objptr_t str, bfc_mempool_t pool);

struct bfc_string_pair_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_string_pair_class *,
		struct bfc_string_pair *, const struct bfc_string_pair *,
		bfc_string_t)
};

const struct bfc_string_pair_class bfc_string_pair_class = {
	.super 		= NULL,
	.name 		= "string pair",
	.init 		= init_string_pair,
	.destroy 	= destroy_pair,
	.clone		= clone_pair,
	.clonesize	= pair_clonesize,
	.hashcode 	= pair_hashcode,
	.equals 	= pair_equals,
	.dump	 	= dump_pair,
	.first		= pair_first,
	.index		= pair_index,
	.place		= place_string_pair_element,
};

int
bfc_init_basic_string_pair(void *buf, size_t bufsize, bfc_mempool_t pool)
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
init_string_pair(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	return (bfc_init_shared_string_pair(buf, bufsize));
}

static bfc_objptr_t
place_string_pair_element(struct bfc_string_pair *pair, size_t pos,
			   bfc_objptr_t str, bfc_mempool_t pool)
{
	bfc_objptr_t s;
	size_t bufsize;
	
	if (pos > 0) {
		s = &pair->second;
		bufsize = sizeof(pair->second);
	} else {
		s = &pair->first;
		bufsize = sizeof(pair->first);
	}

	bfc_destroy(s);

	if (str && BFC_CLASS(str) && pool) {
		bfc_init_basic_string_copy(s, bufsize, pool, str);
	} else if (str && BFC_CLASS(str)) {
		bfc_init_shared_string_substr(s, bufsize, str, 0, BFC_NPOS);
	} else if (pool) {
		bfc_init_basic_string(s, bufsize, pool);
	} else {
		bfc_init_shared_string_buffer(s, bufsize, "", 0);
	}

	return (s);
}

static int
clone_pair(const struct bfc_string_pair *obj,
	   void *buf, size_t bufsize, bfc_mempool_t pool)
{
	struct bfc_string_pair *pair = (struct bfc_string_pair *) buf;
	struct bfc_string_pair *src = BFC_UNCONST(struct bfc_string_pair*, obj);
	size_t size = bfc_object_size((bfc_cobjptr_t)obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(pair, src, size);
	memset(&pair->first, 0, sizeof(pair->first));
	memset(&pair->second, 0, sizeof(pair->second));
	place_string_pair_element(pair, 0, &src->first, pool);
	place_string_pair_element(pair, 1, &src->second, pool);
	return (BFC_SUCCESS);
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

static bfc_cobjptr_t
pair_first(const struct bfc_string_pair *pair)
{
	return (&pair->first);
}

static bfc_objptr_t
pair_index(struct bfc_string_pair *pair, size_t pos)
{
	return ((pos > 0)? &pair->second: &pair->first);
}

static unsigned  
pair_hashcode(const struct bfc_string_pair *pair, int hashlen)
{
	bfc_cobjptr_t obj = bfc_container_cindex((bfc_cobjptr_t)pair, 0);
	return (obj? bfc_object_hashcode(obj, hashlen): 0);
}

static int
pair_equals(const struct bfc_string_pair *pair,
	    const struct bfc_string_pair *other)
{
	bfc_cobjptr_t obj1, obj2, obj3, obj4;

	obj1 = bfc_container_cindex((bfc_cobjptr_t)pair, 0);
	obj2 = bfc_container_cindex((bfc_cobjptr_t)other,0);
	if (((obj1 == NULL) || (obj2 == NULL)) && (obj1 != obj2)) {
		return (0);
	}
	obj3 = bfc_container_cindex((bfc_cobjptr_t)pair, 1);
	obj4 = bfc_container_cindex((bfc_cobjptr_t)other,1);
	if (((obj3 == NULL) || (obj4 == NULL)) && (obj3 != obj4)) {
		return (0);
	}
	return (((obj1 == obj2) || bfc_equal_object(obj1, obj2)) &&
		((obj3 == obj4) || bfc_equal_object(obj3, obj4)));
}

static void
dump_pair(const struct bfc_string_pair *pair,int depth,struct l4sc_logger *log)
{
	if (pair && BFC_CLASS(pair)) {
		L4SC_DEBUG(log, "%s @%p:",
			((bfc_classptr_t) BFC_CLASS(pair))->name, pair);
		if (depth > 1) {
			bfc_cobjptr_t obj;
			if ((obj = bfc_container_cindex((bfc_cobjptr_t)pair, 0))
								      != NULL) {
				bfc_object_dump(obj, depth-1, log);
			}
			if ((obj = bfc_container_cindex((bfc_cobjptr_t)pair, 1))
								      != NULL) {
				bfc_object_dump(obj, depth-1, log);
			}
		}
	}
}
