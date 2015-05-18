
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

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "barefootc/synchronization.h"
#include "barefootc/unconst.h"
#include "log4stdc.h"

/* define struct bfc_container for the remainder of this file ... */
BFC_VECTOR(bfc_container, bfc_objptr_t, 0);

static void destroy_vector(bfc_contptr_t vec);

static int vector_equals(bfc_ccontptr_t vec, bfc_ccontptr_t other);
static unsigned vector_hashcode(bfc_ccontptr_t vec, int hashlen);
static void dump_vector(bfc_ccontptr_t vec, int depth, struct l4sc_logger *log);

static bfc_objptr_t vector_index(bfc_contptr_t vec, size_t pos);
static bfc_cobjptr_t vector_first(bfc_ccontptr_t vec);
static long vector_getlong(bfc_ccontptr_t vec, size_t pos);
static int vector_setlong(bfc_contptr_t vec, size_t pos, long val);
static bfc_objptr_t vector_create_ref(bfc_contptr_t vec, size_t pos,
				      bfc_objptr_t elem, struct mempool *pool);

static int begin_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize);

static int vector_resize(bfc_contptr_t vec, size_t n, bfc_cobjptr_t);
static int vector_push_back(bfc_contptr_t vec, bfc_cobjptr_t);
static void vector_pop_back(bfc_contptr_t vec);

static int vector_insert_fill(bfc_contptr_t vec, bfc_iterptr_t position,
				size_t n, bfc_cobjptr_t);
static int vector_insert_range(bfc_contptr_t vec, bfc_iterptr_t position,
				bfc_iterptr_t first,bfc_iterptr_t last);
static int vector_erase_range(bfc_contptr_t vec,
				bfc_iterptr_t first,bfc_iterptr_t last);

struct bfc_vector_class {
	BFC_CONTAINER_CLASS_DEF(const struct bfc_vector_class *,
				bfc_contptr_t, bfc_ccontptr_t, bfc_object_t)
};

extern const struct bfc_vector_class bfc_vector_class;

const struct bfc_vector_class bfc_objref_vector_class = {
	.super 		= (void *) &bfc_vector_class,
	.name 		= "object reference vector",
	.init 		= bfc_init_objref_vector_class,
	.destroy 	= destroy_vector,
	.hashcode 	= vector_hashcode,
	.equals 	= vector_equals,
	.dump	 	= dump_vector,
	/* Element access */
	.first		= vector_first,
	.index		= vector_index,
	.getl		= vector_getlong,
	.setl		= vector_setlong,
	.create		= vector_create_ref,
	.ibegin		= begin_iterator,
	/* Modifiers */
	.resize		= vector_resize,
	.push_back	= vector_push_back,
	.pop_back	= vector_pop_back,
	.insert_fill	= vector_insert_fill,
	.insert_range	= vector_insert_range,
	.erase_range	= vector_erase_range,
};

int
bfc_init_objref_vector_class(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_contptr_t vec = (bfc_contptr_t) buf;
	BFC_INIT_PROLOGUE(const struct bfc_vector_class *,
			  bfc_objptr_t, obj, buf, bufsize, pool,
			  (bfc_classptr_t) &bfc_objref_vector_class);
	if (bufsize < bfc_object_size(vec)) {
		return (-ENOSPC);
	}
	vec->pool = pool;
	return (BFC_SUCCESS);
}

int
bfc_init_objref_vector_copy(void *buf, size_t bufsize, struct mempool *pool,
			    const struct bfc_container *src)
{
	bfc_contptr_t vec = (bfc_contptr_t) buf;
	size_t elem_size = sizeof(bfc_objptr_t);
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, pool %p, src @%p)",
		__FUNCTION__, vec, (long) bufsize, pool, src);

	rc = bfc_init_vector_by_element_size(buf, bufsize, pool, elem_size);
	if (rc < 0) {
		return (rc);
	}
	vec->vptr = &bfc_objref_vector_class;
	rc = bfc_container_assign_copy(vec, src);
	return (rc);
}

static void
destroy_vector(bfc_contptr_t vec)
{
	const struct bfc_vector_class *cls;

	if (vec && (cls = BFC_CLASS(vec))) {
		vector_resize(vec, 0, NULL);
		BFC_VECTOR_DESTROY(vec);
		BFC_DESTROY_EPILOGUE(vec, cls);
	}
}

static unsigned  
vector_hashcode(bfc_ccontptr_t vec, int hashlen)
{
	return (0);
}

static int
vector_equals(bfc_ccontptr_t vec, bfc_ccontptr_t other)
{
	int eq;
	size_t idx, size;
	const void *p, *q;
	bfc_cobjptr_t obj1, obj2;
	bfc_mutex_ptr_t lock1 = NULL, lock2 = NULL;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if (vec == other) {
		return (1);
	}
	if (BFC_VECTOR_GET_SIZE(vec) != bfc_object_length(other)) {
		return (0);
	}
	if (vec->lock) {
		lock1 = bfc_mutex_lock(vec->lock);
	}
	if (other->lock) {
		lock2 = bfc_mutex_lock(other->lock);
	}
	size = BFC_VECTOR_GET_SIZE(vec);
	for (idx=0, eq=1; idx < size; idx++) {
		p = bfc_container_index(BFC_UNCONST(bfc_contptr_t,vec), idx);
		q = bfc_container_index(BFC_UNCONST(bfc_contptr_t,other), idx);
		if (p && q) {
			obj1 = (bfc_cobjptr_t) p;
			obj2 = (bfc_cobjptr_t) q;
			if (BFC_CLASS(obj1) || BFC_CLASS(obj2)) {
				if (BFC_CLASS(obj1) != BFC_CLASS(obj2)) {
					L4SC_TRACE(logger, "%s: #%ld: class"
						"(%p): %p != class(%p): %p",
						__FUNCTION__, (long)idx,
						obj1, BFC_CLASS(obj1),
						obj2, BFC_CLASS(obj2));
					eq = 0;
					break;
				} else if (!bfc_equal_object(obj1, obj2)) {
					L4SC_TRACE(logger,
						"%s: #%ld: object %p != %p",
						__FUNCTION__, (long)idx, p, q);
					bfc_object_dump(obj1, 1, logger);
					bfc_object_dump(obj2, 1, logger);
					eq = 0;
					break;
				}
			}
		} else if (p) {
			obj1 = (bfc_cobjptr_t) p;
			if (obj1 && BFC_CLASS(obj1)) {
				L4SC_TRACE(logger, "%s: #%ld: ref %p(%p) != %p",
					__FUNCTION__, (long)idx,
					obj1, BFC_CLASS(obj1), q);
				eq = 0;
				break;
			}
		} else if (q) {
			obj2 = (bfc_cobjptr_t) q;
			if (obj2 && BFC_CLASS(obj2)) {
				L4SC_TRACE(logger, "%s: #%ld: ref %p != %p(%p)",
					__FUNCTION__, (long)idx,
					p, obj2, BFC_CLASS(obj2));
				eq = 0;
				break;
			}
		}
	}
	if (lock2) {
		bfc_mutex_unlock(lock2);
	}
	if (lock1) {
		bfc_mutex_unlock(lock1);
	}
	return (eq);
}

static void
dump_vector(bfc_ccontptr_t vec, int depth, struct l4sc_logger *log)
{
	bfc_mutex_ptr_t locked = NULL;

	if (vec && BFC_CLASS(vec)) {
		if (vec->lock) {
			locked = bfc_mutex_lock(vec->lock);
		}
		bfc_vector_dump_structure(vec, log);
		if (depth > 1) {
			bfc_contptr_t v = BFC_UNCONST(bfc_contptr_t, vec);
			bfc_objptr_t obj, *p;
			unsigned idx;
			BFC_VECTOR_FOREACH(p, idx, v) {
				obj = *p;
				if (obj && BFC_CLASS(obj)) {
					L4SC_DEBUG(log, "%s: #%u @%p",
						__FUNCTION__, idx, obj);
					bfc_object_dump(obj, depth-1, log);
				}
			} END_BFC_VECTOR_FOREACH;
		}
		if (locked) {
			bfc_mutex_unlock(locked);
		}
	}
}

// element access:
static bfc_objptr_t
vector_index(bfc_contptr_t vec, size_t pos)
{
	bfc_mutex_ptr_t locked;
	bfc_objptr_t *p = NULL;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref(vec, pos);
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld): cannot lock",
				__FUNCTION__, vec, (long) pos);
	}
	return (p? *p: NULL);
}

static bfc_cobjptr_t
vector_first(bfc_ccontptr_t vec)
{
	bfc_mutex_ptr_t locked;
	bfc_objptr_t *p = NULL;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref(BFC_UNCONST(bfc_contptr_t, vec), 0);
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p): cannot lock", __FUNCTION__, vec);
	}
	return (p? *p: NULL);
}

static long
vector_getlong(bfc_ccontptr_t vec, size_t pos)
{
	bfc_cobjptr_t obj, *p;
	bfc_mutex_ptr_t locked;
	long v = 0;

	if ((pos == BFC_NPOS) || (pos > BFC_VECTOR_GET_SIZE(vec))) {
		return (-ERANGE);
	}
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref(BFC_UNCONST(bfc_contptr_t,vec), pos);
		if (p && (obj = *p) && BFC_CLASS(obj)) {
			v = bfc_container_getlong((bfc_ccontptr_t) obj, 0);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld): cannot lock",
				__FUNCTION__, vec, (long) pos);
	}
	return (v);
}

static int
vector_setlong(bfc_contptr_t vec, size_t pos, long val)
{
	bfc_objptr_t obj, *p;
	bfc_mutex_ptr_t locked;
	int rc = -ENOENT;

	if ((pos == BFC_NPOS) || (pos > BFC_VECTOR_GET_SIZE(vec))) {
		return (-ERANGE);
	}
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref(vec, pos);
		if (p && (obj = *p) && BFC_CLASS(obj)) {
			rc = bfc_container_setlong((bfc_contptr_t)obj, 0, val);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld, %ld): cannot lock",
				__FUNCTION__, vec, (long) pos, val);
	}
	return (rc);
}

static bfc_objptr_t
vector_create_ref(bfc_contptr_t vec, size_t pos,
		  bfc_objptr_t elem, struct mempool *pool)
{
	bfc_objptr_t *p = NULL;
	bfc_objptr_t old = NULL;
	bfc_mutex_ptr_t locked;

	if (elem && BFC_CLASS(elem)) {
		bfc_incr_refcount(elem);
	}
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if (pos == BFC_NPOS) {
			p = bfc_vector_have(vec, BFC_VECTOR_GET_SIZE(vec));
		} else {
			p = bfc_vector_have(vec, pos);
		}
		if (p != NULL) {
			old = *p;
			*p = elem;
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld, %p): cannot lock",
				__FUNCTION__, vec, (long) pos, elem);
	}
	if ((p == NULL) && elem && BFC_CLASS(elem)) {
		bfc_decr_refcount(elem);
	} else if (old && BFC_CLASS(old)) {
		bfc_decr_refcount(old);
	}
	return (p? *p: NULL);
}

static int
begin_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	return (bfc_init_vector_iterator(it, bufsize,
					(bfc_cobjptr_t)vec, 0));
}

/* Modifiers */
static int
vector_resize(bfc_contptr_t vec, size_t n, bfc_cobjptr_t p)
{
	size_t size;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %ld, %p)", __FUNCTION__, vec, (long) n, p);

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (n < size) {
			size_t idx;
			bfc_objptr_t obj, *ref;
			BFC_VECTOR_FOREACH(ref, idx, vec) {
				if (idx >= n) {
					obj = *ref;
					*ref = NULL;
					if (obj && BFC_CLASS(obj)) {
						L4SC_DEBUG(logger,
						  "%s: clearing #%lu @%p",
						  __FUNCTION__, (long)idx, obj);
						bfc_decr_refcount(obj);
					}
				}
			} END_BFC_VECTOR_FOREACH;
			BFC_VECTOR_SET_SIZE(vec, n);
		} else if (n > size) {
			bfc_cobjptr_t obj = (bfc_cobjptr_t) p;
			if (obj && BFC_CLASS(obj)) {
				do {
					rc = bfc_container_push_back(vec, p);
				} while ((rc >= 0) && (++size < n));
			} else {
				size = n;
			}
			BFC_VECTOR_SET_SIZE(vec, n);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld, %p): cannot lock",
				__FUNCTION__, vec, (long) n, p);
		rc = -EBUSY;
	}
	return (rc);
}

static int
vector_push_back(bfc_contptr_t vec, bfc_cobjptr_t p)
{
	size_t size;
	bfc_objptr_t obj = BFC_UNCONST(bfc_objptr_t, p);
	bfc_mutex_ptr_t locked;
	int rc = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, elem @%p)", __FUNCTION__, vec, p);
	dump_vector(vec, 1, logger);

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (vector_create_ref(vec, size, obj, vec->pool) != NULL) {
			rc = (int) size;
		} else {
			rc = -ENOMEM;
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p): cannot lock",__FUNCTION__,vec,p);
		rc = -EBUSY;
	}
	dump_vector(vec, 1, logger);
	return (rc);
}

static void
vector_pop_back(bfc_contptr_t vec)
{
	size_t size;
	bfc_objptr_t obj = NULL, *ref;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (size > 0) {
			size--;
			if ((ref = bfc_vector_ref(vec, size)) != NULL) {
				obj = *ref;
				*ref = NULL;
			}
			BFC_VECTOR_SET_SIZE(vec, size);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p): cannot lock",__FUNCTION__, vec);
	}
	if (obj && BFC_CLASS(obj)) {
		bfc_decr_refcount(obj);
	}
}

static void
zero_vector_elements(bfc_contptr_t vec, size_t pos, size_t limit)
{
	size_t idx;
	bfc_objptr_t *ref;

	for (idx = pos; idx < limit; idx++) {
		if ((ref = bfc_vector_ref(vec, idx)) != NULL) {
			*ref = NULL;
		}
	}
}

static int
init_vector_elements(bfc_contptr_t vec, size_t pos, size_t limit,
		     bfc_cobjptr_t elem)
{
	size_t idx;
	bfc_objptr_t *ref;
	bfc_objptr_t obj = BFC_UNCONST(bfc_objptr_t,elem);
	int rc = BFC_SUCCESS;

	for (idx = pos; (idx < limit) && (rc >= 0); idx++) {
		if ((ref = bfc_vector_have(vec, idx)) != NULL) {
			bfc_incr_refcount(obj);
			*ref = obj;
		} else {
			rc = -ENOMEM;
			break;
		}
	}
	return (rc);
}

static int
move_vector_elements(bfc_contptr_t vec, size_t pos, size_t limit, size_t dest)
{
	size_t idx, n;
	bfc_objptr_t *cp, *dp;
	int rc = BFC_SUCCESS;

	if (dest < pos) {
		n = pos - dest;
		for (idx = pos; idx < limit; idx++) {
			if ((cp = bfc_vector_ref(vec, idx)) != NULL) {
				if ((dp = bfc_vector_have(vec, idx-n))!= NULL) {
					*dp = *cp;
					*cp = NULL;
				} else {
					rc = -ENOMEM;
					break;
				}
			}
		}
	} else if (dest > pos) {
		n = dest - pos;
		for (idx = limit; (idx--) > pos; ) {
			if ((cp = bfc_vector_ref(vec, idx)) != NULL) {
				if ((dp = bfc_vector_have(vec, idx+n))!= NULL) {
					*dp = *cp;
					*cp = NULL;
				} else {
					rc = -ENOMEM;
					break;
				}
			}
		}
	}
	return (rc);
}

static int
vector_insert_fill(bfc_contptr_t vec, bfc_iterptr_t position, size_t n,
		   bfc_cobjptr_t obj)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t pos = bfc_iterator_position(position);
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %ld, %p): pos %ld/%ld",
		__FUNCTION__, vec, position,(long)n,obj,(long)pos,(long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);

	if (n < 1) {
		return (BFC_SUCCESS);
	}

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (((rc = bfc_container_reserve(vec, size+n)) >= 0)
		 && ((rc = move_vector_elements(vec, pos, size, pos+n)) >= 0)) {
			size += n;
			BFC_VECTOR_SET_SIZE(vec, size);
			if (obj && BFC_CLASS(obj)) {
				init_vector_elements(vec, pos, pos+n, obj);
			} else {
				zero_vector_elements(vec, pos, pos+n);
			}
			rc = (int) size;
			if (rc < 0) {
				rc = BFC_SUCCESS;
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %ld, %p): cannot lock",
				__FUNCTION__, vec, position, (long) n, obj);
		rc = -EBUSY;
	}
	return (rc);
}

static int
vector_insert_range(bfc_contptr_t vec, bfc_iterptr_t position,
		    bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(position);
	ptrdiff_t offs, n = bfc_iterator_distance(first, last);
	bfc_objptr_t obj, *ref;
	int rc;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_iterator_t it;

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %p, %p): pos %ld/%ld",
		__FUNCTION__, vec, position, first, last,(long)pos,(long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);
	bfc_object_dump(first, 1, logger);
	bfc_object_dump(last, 1, logger);

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (((rc = bfc_container_reserve(vec, size + n)) >= 0)
		 && ((rc = move_vector_elements(vec, pos, size, pos+n)) >= 0)) {
			size += n;
			BFC_VECTOR_SET_SIZE(vec, size);
			rc = (int) size;
			if (rc < 0) {
				rc = BFC_SUCCESS;
			}
			it = *first;
			idx = pos;
			for ( ; bfc_iterator_distance(&it, last) > 0; idx++) {
				obj = (bfc_objptr_t) bfc_iterator_index(&it);
				offs = bfc_iterator_distance(first, &it);
				bfc_iterator_advance(&it, 1);
				if ((offs > 0) && (idx < (size_t)(pos+offs))) {
					idx = pos + offs;
				}
				if (obj && BFC_CLASS(obj)) {
					ref = bfc_vector_have(vec, idx);
					if (ref) {
						bfc_incr_refcount(obj);
						*ref = obj;
					} else {
						rc = -ENOMEM;
					}
				}
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %p, %p): cannot lock",
				__FUNCTION__, vec, position, first, last);
		rc = -EBUSY;
	}
	return (rc);
}

static int
vector_erase_range(bfc_contptr_t vec, bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t pos = bfc_iterator_position(first);
	int n = bfc_iterator_distance(first, last);
	size_t idx;
	bfc_objptr_t obj, *ref;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;
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
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		it = *first;
		while (bfc_iterator_distance(&it, last) > 0) {
			idx = bfc_iterator_position(&it);
			obj = (bfc_objptr_t) bfc_iterator_index(&it);
			bfc_iterator_advance(&it, 1);
			if ((ref = bfc_vector_ref(vec, idx)) == NULL) {
				*ref = NULL;
			}
			if (obj && BFC_CLASS(obj)) {
				bfc_decr_refcount(obj);
			}
		}
		if (pos+n < size) {
			move_vector_elements(vec, pos+n, size, pos);
			size -= n;
		} else {
			size = pos;
		}
		rc = bfc_container_resize(vec, size, NULL);
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %p): cannot lock",
				__FUNCTION__, vec, first, last);
		rc = -EBUSY;
	}
	return (rc);
}

