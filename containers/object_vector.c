
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#define BFC_VECTOR_ENABLE_TRIPLE_INDIRECT	1
#define CV_ELEMSIZE_FACTOR(vec)		((vec)->elem_size / sizeof(char))

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "log4stdc.h"

/* for the remainder of this file ... */
typedef struct bfc_char_vector *bfc_vecptr_t;
typedef const struct bfc_char_vector *bfc_cvecptr_t;

static int clone_vector(bfc_cvecptr_t vec, void *buf, size_t bufsize);
static void destroy_vector(bfc_vecptr_t vec);

static int vector_equals(bfc_cvecptr_t vec, bfc_cvecptr_t other);
static unsigned vector_hashcode(bfc_cvecptr_t vec);
static void dump_vector(bfc_cvecptr_t vec, int depth, struct l4sc_logger *log);

static long vector_getlong(bfc_cvecptr_t vec, size_t pos);
static int vector_setlong(bfc_vecptr_t vec, size_t pos, long val);

static int vector_push_back(bfc_vecptr_t vec, const void *p);
static void vector_pop_back(bfc_vecptr_t vec);

static int vector_insert_fill(bfc_vecptr_t vec, bfc_iterptr_t position,
				size_t n, const void *p);
static int vector_insert_range(bfc_vecptr_t vec, bfc_iterptr_t position,
				bfc_iterptr_t first,bfc_iterptr_t last);
static int vector_erase_range(bfc_vecptr_t vec,
				bfc_iterptr_t first,bfc_iterptr_t last);

struct bfc_vector_class {
	BFC_CONTAINER_CLASS_DEF(const struct bfc_vector_class *,
				bfc_vecptr_t, bfc_cvecptr_t, void)
};

extern struct bfc_vector_class bfc_vector_class;

struct bfc_vector_class bfc_object_vector_class = {
	.super 		= (void *) &bfc_vector_class,
	.name 		= "object vector",
	.init 		= bfc_init_object_vector_class,
	.destroy 	= destroy_vector,
	.clone 		= clone_vector,
	.hashcode 	= vector_hashcode,
	.equals 	= vector_equals,
	.dump	 	= dump_vector,
	/* Element access */
	.getl		= vector_getlong,
	.setl		= vector_setlong,
	/* Modifiers */
	.push_back	= vector_push_back,
	.pop_back	= vector_pop_back,
	.insert_fill	= vector_insert_fill,
	.insert_range	= vector_insert_range,
	.erase_range	= vector_erase_range,
};

int
bfc_init_object_vector_class(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_vecptr_t vec = (bfc_vecptr_t) buf;
	BFC_INIT_PROLOGUE(const struct bfc_vector_class *,
			  bfc_objptr_t, obj, buf, bufsize, pool,
			  (bfc_classptr_t) &bfc_object_vector_class);
	if (bufsize < bfc_object_size(vec)) {
		return (-ENOSPC);
	}
	vec->pool = pool;
	return (BFC_SUCCESS);
}

int
bfc_init_object_vector_copy(void *buf, size_t bufsize, struct mempool *pool,
		     const struct bfc_container *src)
{
	bfc_vecptr_t vec = (bfc_vecptr_t) buf;
	bfc_objptr_t ref;
	bfc_cobjptr_t obj;
	unsigned idx;
	void *elem;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, src @%p)",
		__FUNCTION__, vec, (long) bufsize, pool, src);

	if ((rc = bfc_init_vector_copy(buf, bufsize, pool, src)) >= 0) {
		vec->vptr = &bfc_object_vector_class;
		dump_vector(vec, 1, logger);
		BFC_VECTOR_FOREACH(elem, idx, vec) {
			ref = (bfc_objptr_t) elem;
			obj = (bfc_cobjptr_t) bfc_container_index(
					(bfc_contptr_t)(uintptr_t)src, idx);
			if (obj && BFC_CLASS(obj)) {
				bfc_clone_object(obj, ref, vec->elem_size);
				bfc_object_dump(ref, 1, logger);
			}
		} END_BFC_VECTOR_FOREACH;
	}
	return (rc);
}

static void
destroy_vector(bfc_vecptr_t vec)
{
	const struct bfc_vector_class *cls = BFC_CLASS(vec);
	bfc_objptr_t obj;
	unsigned idx;
	char *elem;

	BFC_VECTOR_FOREACH(elem, idx, vec) {
		obj = (bfc_objptr_t) elem;
		if (obj && BFC_CLASS(obj)) {
			bfc_destroy(obj);
		}
	} END_BFC_VECTOR_FOREACH;
	BFC_VECTOR_DESTROY(vec);
	BFC_DESTROY_EPILOGUE(vec, cls);
}

static int
clone_vector(bfc_cvecptr_t vec, void *buf, size_t bufsize)
{
	return (bfc_init_object_vector_copy(buf, bufsize, vec->pool,
						(bfc_ccontptr_t) vec));
}

static unsigned  
vector_hashcode(bfc_cvecptr_t vec)
{
	return (0);
}

static int
vector_equals(bfc_cvecptr_t vec, bfc_cvecptr_t other)
{
	size_t idx, size;
	const void *p, *q;
	bfc_cobjptr_t obj1, obj2;

	if (vec == other) {
		return (1);
	}
	if (bfc_object_length(vec) != bfc_object_length(other)) {
		return (0);
	}
	size = bfc_object_length(vec);
	for (idx=0; idx < size; idx++) {
		p = bfc_container_index((bfc_contptr_t)other, idx);
		q = bfc_container_index((bfc_contptr_t)other, idx);
		if (p && q) {
			obj1 = (bfc_cobjptr_t) p;
			obj2 = (bfc_cobjptr_t) q;
			if (BFC_CLASS(obj1) || BFC_CLASS(obj2)) {
				if (BFC_CLASS(obj1) != BFC_CLASS(obj2)) {
					return (0);
				} else if (!bfc_equal_object(obj1, obj2)) {
					return (0);
				}
			}
		} else if (p != q) /* one of them != NULL */ {
			return (0);
		}
	}
	return (1);
}

static void
dump_vector(bfc_cvecptr_t vec, int depth, struct l4sc_logger *log)
{
	char *elem;
	unsigned idx;
	bfc_cobjptr_t obj;

	bfc_vector_dump_structure(vec, log);

	if (depth > 1) {
		bfc_vecptr_t v = (bfc_vecptr_t)(uintptr_t) vec;
		BFC_VECTOR_FOREACH(elem, idx, v) {
			obj = (bfc_cobjptr_t) elem;
			if (obj && BFC_CLASS(obj)) {
				bfc_object_dump(obj, depth-1, log);
			}
		} END_BFC_VECTOR_FOREACH;
	}
}

static long
vector_getlong(bfc_cvecptr_t vec, size_t pos)
{
	const void *p;
	bfc_cobjptr_t obj;

	if ((pos == BFC_NPOS) || (pos > bfc_object_length(vec))) {
		return (-ERANGE);
	}
	if ((p = bfc_vector_ref((bfc_vecptr_t)(uintptr_t)vec, pos)) == NULL) {
		return (-ENOENT);
	}
	obj = (bfc_cobjptr_t) p;
	if (BFC_CLASS(obj) == NULL) {
		return (-ENOENT);
	}
	return (bfc_container_getlong((bfc_ccontptr_t) obj, 0));
}

static int
vector_setlong(bfc_vecptr_t vec, size_t pos, long val)
{
	void *p;
	bfc_objptr_t obj;

	if ((pos == BFC_NPOS) || (pos > bfc_object_length(vec))) {
		return (-ERANGE);
	}
	if ((p = bfc_vector_ref(vec, pos)) == NULL) {
		return (-ENOENT);
	}
	obj = (bfc_objptr_t) p;
	if (BFC_CLASS(obj) == NULL) {
		return (-ENOENT);
	}
	return (bfc_container_setlong((bfc_contptr_t) obj, 0, val));
}

/* Modifiers */
static int
vector_push_back(bfc_vecptr_t vec, const void *p)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	bfc_cobjptr_t obj = (bfc_cobjptr_t) p;
	void *ref;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, elem @%p)", __FUNCTION__, vec, p);
	dump_vector(vec, 1, logger);

	if ((ref = bfc_vector_have(vec, size)) == NULL) {
		return (-ENOMEM);
	}
	size++;
	BFC_VECTOR_SET_SIZE(vec, size);
	if (obj && BFC_CLASS(obj)) {
		bfc_clone_object(obj, ref, vec->elem_size);
		bfc_object_dump(ref, 1, logger);
	} else {
		memset(ref, 0, vec->elem_size);
	}
	dump_vector(vec, 1, logger);
	return ((int) size);
}

static void
vector_pop_back(bfc_vecptr_t vec)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	bfc_objptr_t obj;
	void *ref;

	if (size > 0) {
		if ((ref = bfc_vector_ref(vec, size-1)) != NULL) {
			obj = (bfc_objptr_t) ref;
			if (obj && BFC_CLASS(obj)) {
				bfc_destroy(obj);
			}
		}
		bfc_container_resize((bfc_contptr_t) vec, size-1, NULL);
	}
}

static int
vector_insert_fill(bfc_vecptr_t vec, bfc_iterptr_t position, size_t n,
		   const void *p)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(position);
	bfc_cobjptr_t obj = (bfc_cobjptr_t) p;
	void *ref, *src;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %ld, %p): pos %ld/%ld",
		__FUNCTION__, vec, position, (long)n, p, (long)pos, (long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);

	if (n < 1) {
		return (BFC_SUCCESS);
	}
	if ((rc = bfc_container_reserve((bfc_contptr_t) vec, size+n)) < 0) {
		return (rc);
	}
	for (idx = size-1; idx >= pos; idx--) {
		if (((src = bfc_vector_ref(vec, idx)) != NULL)
		 && ((ref = bfc_vector_have(vec, idx+n)) != NULL)) {
			memcpy(ref, src, vec->elem_size);
		}
		if (idx == 0) {
			break;
		}
	}
	size += n;
	BFC_VECTOR_SET_SIZE(vec, size);
	if (obj && BFC_CLASS(obj)) {
		for (idx = pos; idx < pos+n; idx++) {
			if ((ref = bfc_vector_have(vec, idx)) != NULL) {
				bfc_clone_object(obj, ref, vec->elem_size);
			}
		}
	} else {
		for (idx = pos; idx < pos+n; idx++) {
			if ((ref = bfc_vector_have(vec, idx)) != NULL) {
				memset(ref, 0, vec->elem_size);
			}
		}
	}
	rc = (int) size;
	return ((rc > 0)? rc: BFC_SUCCESS);
}

static int
vector_insert_range(bfc_vecptr_t vec, bfc_iterptr_t position,
		    bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(position);
	int n = bfc_iterator_distance(first, last);
	bfc_objptr_t obj;
	void *ref, *src;
	int rc;
	bfc_iterator_t it;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %p, %p): pos %ld/%ld",
		__FUNCTION__, vec, position, first, last,(long)pos,(long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);
	bfc_object_dump(first, 1, logger);
	bfc_object_dump(last, 1, logger);

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	if ((rc = bfc_container_reserve((bfc_contptr_t) vec, size+n)) < 0) {
		return (rc);
	}
	for (idx = size-1; idx >= pos; idx--) {
		if (((src = bfc_vector_ref(vec, idx)) != NULL)
		 && ((ref = bfc_vector_have(vec, idx+n)) != NULL)) {
			memcpy(ref, src, vec->elem_size);
		}
		if (idx == 0) {
			break;
		}
	}
	size += n;
	BFC_VECTOR_SET_SIZE(vec, size);

	it = *first;
	for (idx = pos; bfc_iterator_distance(&it, last) > 0; idx++) {
		src = bfc_iterator_index(&it);
		bfc_iterator_advance(&it, 1);
		if (src && ((ref = bfc_vector_have(vec, idx)) != NULL)) {
			obj = (bfc_objptr_t) src;
			if (obj && BFC_CLASS(obj)) {
				bfc_clone_object(obj, ref, vec->elem_size);
			} else {
				memset(ref, 0, vec->elem_size);
			}
		}
	}
	return (BFC_SUCCESS);
}

static int
vector_erase_range(bfc_vecptr_t vec, bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(first);
	int n = bfc_iterator_distance(first, last);
	void *ref, *src;
	bfc_objptr_t obj;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_iterator_t it;

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %p): pos %ld+%ld/%ld",
		__FUNCTION__, vec, first, last, (long)pos, (long)n, (long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(first, 1, logger);
	bfc_object_dump(last, 1, logger);

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	n = 0;
	it = *first;
	while (bfc_iterator_distance(&it, last) > 0) {
		obj = (bfc_objptr_t) bfc_iterator_index(&it);
		bfc_iterator_advance(&it, 1);
		if (obj && BFC_CLASS(obj)) {
			bfc_destroy(obj);
		}
		n++;
	}
	if (pos+n < size) {
		for (idx = pos; idx+n < size; idx++) {
			L4SC_TRACE(logger, "%s: idx %ld", __FUNCTION__, (long) idx);
			if ((int) idx < 0) {
				return (-EFAULT);
			}
			if (((src = bfc_vector_ref(vec, idx+n)) != NULL)
			 && ((ref = bfc_vector_have(vec, idx)) != NULL)) {
				memcpy(ref, src, vec->elem_size);
			}
		}
		size -= n;
	} else {
		size = pos;
	}
	return (bfc_container_resize((bfc_contptr_t) vec, size, NULL));
}

