
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
#include "barefootc/unconst.h"
#include "log4stdc.h"

static int init_string_ref_pair(void *buf, size_t bufsize,
				   struct mempool *pool);
static int clone_pair(const struct bfc_string_ref_pair *obj,
				void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pair(struct bfc_string_ref_pair *pair);

static size_t pair_clonesize(const struct bfc_string_ref_pair *pair);
static unsigned pair_hashcode(const struct bfc_string_ref_pair *pair,
								int hashlen);
static int pair_equals(const struct bfc_string_ref_pair *pair,
			const struct bfc_string_ref_pair *other);
static int pair_tostring(const struct bfc_string_ref_pair *pair,
			 char *buf, size_t bufsize, const char *fmt);
static void dump_pair(const struct bfc_string_ref_pair *pair,
			int depth, struct l4sc_logger *log);
static bfc_cobjptr_t pair_first(const struct bfc_string_ref_pair *pair);
static bfc_objptr_t pair_index(struct bfc_string_ref_pair *pair, size_t pos);
static bfc_objptr_t place_string_ref_pair_element(
			struct bfc_string_ref_pair *pair,
			size_t pos, bfc_objptr_t val, struct mempool *pool);

struct bfc_pair_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_pair_class *,
		struct bfc_string_ref_pair*,
		const struct bfc_string_ref_pair*,
		bfc_object_t)
};

const struct bfc_pair_class bfc_string_ref_pair_class = {
	.super 		= NULL,
	.name 		= "string reference pair",
	.init 		= init_string_ref_pair,
	.destroy 	= destroy_pair,
	.clone		= clone_pair,
	.clonesize	= pair_clonesize,
	.hashcode 	= pair_hashcode,
	.equals 	= pair_equals,
	.tostring	= pair_tostring,
	.dump	 	= dump_pair,
	.first		= pair_first,
	.index		= pair_index,
	.place		= place_string_ref_pair_element,
};

int
bfc_init_basic_string_ref_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_string_ref_pair *pair = (struct bfc_string_ref_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s: bufsize %d, need %d",
			__FUNCTION__, (int) bufsize, (int) sizeof(*pair));
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_string_ref_pair_class;
	bfc_init_basic_string(&pair->first, sizeof(pair->first), pool);
	pair->second = NULL;
	return (BFC_SUCCESS);
}

int
bfc_init_shared_string_ref_pair(void *buf, size_t bufsize)
{
	struct bfc_string_ref_pair *pair = (struct bfc_string_ref_pair *) buf;
	if (bufsize < sizeof(*pair)) {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s: bufsize %d, need %d",
			__FUNCTION__, (int) bufsize, (int) sizeof(*pair));
		return (-ENOSPC);
	}
	pair->vptr = (bfc_pair_class_ptr_t) &bfc_string_ref_pair_class;
	bfc_init_shared_string_buffer(&pair->first, sizeof(pair->first), "",0);
	pair->second = NULL;
	return (BFC_SUCCESS);
}

static int
init_string_ref_pair(void *buf, size_t bufsize, struct mempool *pool)
{
	return (bfc_init_shared_string_ref_pair(buf, bufsize));
}

static bfc_objptr_t
place_string_ref_pair_element(struct bfc_string_ref_pair *pair,
				  size_t pos, bfc_objptr_t val,
				  struct mempool *pool)
{
	bfc_objptr_t p = (bfc_objptr_t) &pair->first;

	if (pos == 0) {
		bfc_strptr_t str = (bfc_strptr_t) val;
		size_t bufsize = sizeof(pair->first);

		p = (bfc_objptr_t) &pair->first;

		bfc_destroy(p);

		if (str && BFC_CLASS(str) && pool) {
			bfc_init_basic_string_copy(p, bufsize, pool, str);
		} else if (str && BFC_CLASS(str)) {
			bfc_init_shared_string_substr(p, bufsize,
						      str, 0, BFC_NPOS);
		} else if (pool) {
			bfc_init_basic_string(p, bufsize, pool);
		} else {
			bfc_init_shared_string_buffer(p, bufsize, "", 0);
		}
	} else {
		if (val && BFC_CLASS(val)) {
			bfc_incr_refcount(val);
		}
		p = pair->second;
		pair->second = val;
		if (p && BFC_CLASS(p)) {
			bfc_decr_refcount(p);
		}
		p = pair->second;
	}

	return (p);
}

static int
clone_pair(const struct bfc_string_ref_pair *obj,
	   void *buf, size_t bufsize, struct mempool *pool)
{
	struct bfc_string_ref_pair *pair = (struct bfc_string_ref_pair *) buf;
	struct bfc_string_ref_pair *src =
				BFC_UNCONST(struct bfc_string_ref_pair *, obj);
	size_t size = bfc_object_size(obj);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(pair, src, size);
	memset(&pair->first, 0, sizeof(pair->first));
	pair->second = NULL;
	place_string_ref_pair_element(pair, 0,(bfc_objptr_t)&src->first,pool);
	place_string_ref_pair_element(pair, 1, src->second, pool);
	return (BFC_SUCCESS);
}

size_t
pair_clonesize(const struct bfc_string_ref_pair *pair)
{
	return (sizeof(struct bfc_string_ref_pair));
}

static void
destroy_pair(struct bfc_string_ref_pair *pair)
{
	bfc_objptr_t p;

	if (BFC_CLASS(pair)) {
		pair->vptr = NULL;
		bfc_destroy(&pair->first);
		if (((p = pair->second) != NULL) && BFC_CLASS(p)) {
			bfc_decr_refcount(p);
			p = NULL;
		}
	}
}

static bfc_cobjptr_t
pair_first(const struct bfc_string_ref_pair *pair)
{
	return ((bfc_cobjptr_t) &pair->first);
}

static bfc_objptr_t
pair_index(struct bfc_string_ref_pair *pair, size_t pos)
{
	return ((pos > 0)? pair->second: (bfc_objptr_t) &pair->first);
}

static unsigned  
pair_hashcode(const struct bfc_string_ref_pair *pair, int hashlen)
{
	return (bfc_object_hashcode(&pair->first, hashlen));
}

static int
pair_equals(const struct bfc_string_ref_pair *pair,
	    const struct bfc_string_ref_pair *other)
{
	return (bfc_equal_object(&pair->first, &other->first) &&
		bfc_equal_object(pair->second, other->second));
}

static int
pair_tostring(const struct bfc_string_ref_pair *pair,
	      char *buf, size_t bufsize, const char *fmt)
{
	int rc = bfc_object_tostring(&pair->first, buf, bufsize, NULL);
	if ((rc >= 0) && (rc+3 < bufsize)) {
		int rc2, spare;
		memcpy(buf+rc, ": ", 3);
		spare = bufsize - rc - 2;
		rc2 = bfc_object_tostring(pair->second, buf+rc+2, spare, NULL);
		if ((rc2 >= 0) && (rc < spare)) {
			rc += rc2 + 2;
		} else {
			buf[rc] = '\0';
		}
	}
	return (rc);
}

static void
dump_pair(const struct bfc_string_ref_pair *pair, int depth,
	  struct l4sc_logger *log)
{
	char buf[80];

	if (pair && BFC_CLASS(pair)) {
		if (BFC_CLASS(&pair->first)) {
			bfc_object_tostring(&pair->first,buf,sizeof(buf),NULL);
		} else {
			memcpy(buf, "(missing key)", 14);
		}
		L4SC_DEBUG(log, "%s @%p: %s",
			((bfc_classptr_t) BFC_CLASS(pair))->name, pair, buf);
		if (depth > 1) {
			bfc_cobjptr_t obj;
			if ((obj = bfc_container_cindex(pair, 1)) != NULL) {
				bfc_object_dump(obj, depth-1, log);
			}
		}
	}
}
