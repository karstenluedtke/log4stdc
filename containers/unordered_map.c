
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/map.h"
#include "barefootc/pair.h"
#include "barefootc/vector.h"
#include "barefootc/synchronization.h"
#include "log4stdc.h"

static void destroy_map(bfc_contptr_t map);
static int begin_iterator(bfc_ccontptr_t map, bfc_iterptr_t it, size_t bufsize);

#define BFC_MAP_HASHLEN(map)	(CV2_LOG2ELEM(map))
#define BFC_MAP_HASHMASK(map)	(CV2_ELEMENTS(map)-1)

extern const bfc_class_t bfc_object_vector_class;
const struct bfc_map_class unordered_map_class = {
	.super 		= (void *) &bfc_object_vector_class,
	.name 		= "map",
	.destroy	= destroy_map,
	.ibegin		= begin_iterator,
};

int
bfc_init_map_class(void *buf, size_t bufsize, int estimate,
		   bfc_classptr_t pairclass, struct mempool *pool)
{
	int rc;
	long boundary = 4 * estimate;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) buf;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if ((rc = bfc_init_object_vector_class(buf, bufsize, pool)) < 0) {
		L4SC_ERROR(logger,"%s(%p, %ld, estimate %d, pair %p, pool %p):"
				  " init vector error %d", __FUNCTION__,
			   vec, (long) bufsize, estimate, pairclass, pool, rc);
		return(rc);
	}
	vec->vptr = (bfc_vector_class_ptr_t) &unordered_map_class;
	vec->elem_class = pairclass;
	vec->log2_double_indirect = 1;
	while (boundary > (long) CV2_ELEMENTS(vec)) {
		if (vec->log2_double_indirect < 20) {
			vec->log2_double_indirect++;
		} else {
			L4SC_ERROR(logger,
				"%s(%p, %ld, estimate %d, pair %p, pool %p):"
				  " too many entries", __FUNCTION__,
				vec, (long)bufsize, estimate, pairclass, pool);
			bfc_destroy(vec);
			return(-EINVAL);
		}
	}
	L4SC_DEBUG(logger,"%s(%p, %ld, estimate %d, pair %p, pool %p):"
		" hash size %d, mask 0x%08X, elem_class %p", __FUNCTION__,
		vec, (long) bufsize, estimate, pairclass, pool,
		BFC_MAP_HASHLEN(vec), BFC_MAP_HASHMASK(vec), vec->elem_class);
	return (rc);
}

int
bfc_init_map_copy(void *buf, size_t bufsize, struct mempool *pool,
		  bfc_ccontptr_t src)
{
	bfc_char_vector_t *vec = (bfc_char_vector_t *) buf;
	const bfc_char_vector_t *svec = (const bfc_char_vector_t *) src;
	size_t elem_size = bfc_container_element_size(src);
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_DEBUG(logger, "%s(%p, %ld, pool %p, src %p)",
		__FUNCTION__, buf, (long)bufsize, pool, src);

	rc = bfc_init_vector_by_element_size(buf, bufsize, pool, elem_size);
	if (rc >= 0) {
		vec->vptr = (bfc_vector_class_ptr_t) &unordered_map_class;
		vec->elem_class = svec->elem_class;
		vec->log2_indirect = svec->log2_indirect;
		vec->log2_double_indirect = svec->log2_double_indirect;
		rc = bfc_container_assign_copy((bfc_contptr_t) vec, src);
	}
	return (rc);
}

static void
destroy_map(bfc_contptr_t map)
{
	bfc_classptr_t cls;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;

	if (vec && (cls = (bfc_classptr_t) BFC_CLASS(vec))) {
		BFC_DESTROY_EPILOGUE(vec, cls);
	}
}

static int
begin_iterator(bfc_ccontptr_t map, bfc_iterptr_t it, size_t bufsize)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	int rc = bfc_init_object_vector_iterator(it, bufsize,
						(bfc_cobjptr_t)map, 0);

	L4SC_DEBUG(logger, "%s(map %p, it %p, bufsize %ld)",
		__FUNCTION__, map, it, (long) bufsize);
	if (rc >= 0) {
		bfc_cobjptr_t pair = bfc_iterator_index(it);
		if (pair && (BFC_CLASS(pair) == NULL)) {
			bfc_iterator_advance(it, 1);
		}
	}
	L4SC_DEBUG(logger, "%s(map %p, it %p, bufsize %ld): %d",
		__FUNCTION__, map, it, (long) bufsize, rc);
	bfc_object_dump(it, 2, logger);
	return (rc);
}

size_t
bfc_map_size(bfc_ccontptr_t map)
{
	size_t n = 0;
	ptrdiff_t dist;
	bfc_cobjptr_t obj;
	bfc_mutex_ptr_t locked;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_iterator_t it, limit;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		bfc_container_begin_iterator(vec, &it, sizeof(it));
		bfc_container_end_iterator(vec, &limit, sizeof(limit));
		while ((dist = bfc_iterator_distance(&it, &limit)) > 0) {
			obj = bfc_iterator_index(&it);
			if (obj && BFC_CLASS(obj)) {
				L4SC_DEBUG(logger, "%s: pair @%p, dist %ld",
						__FUNCTION__, obj, (long)dist);
				n++;
			}
			bfc_iterator_advance(&it, 1);
		}
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p) cannot lock", __FUNCTION__, map);
	}
	L4SC_DEBUG(logger, "%s(%p): %ld", __FUNCTION__, map, (long) n);

	return (n);
}

size_t
bfc_map_load_limit(const void *map)
{
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	return ((size_t) 1 << BFC_MAP_HASHLEN(vec));
}

size_t
bfc_map_load_percent(const void *map)
{
	// (100 * bfc_map_size(map) / bfc_map_load_limit(map));
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	return ((bfc_map_size(map) * 100) >> BFC_MAP_HASHLEN(vec));
}

int
bfc_map_reserve(bfc_contptr_t map, size_t n)
{
	size_t oldsize, limit;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		oldsize = BFC_VECTOR_GET_SIZE(vec);
		if (n > oldsize) {
			limit = bfc_map_load_limit(map);
			L4SC_DEBUG(logger, "%s(%p): %lu -> %lu, limit %lu",
				__FUNCTION__, map,
				(long) oldsize, (long) n, (long) limit);
			if (n < limit/2) {
				/* ok */
			} else if ((vec->double_indirect == NULL)
				&& (vec->triple_indirect == NULL)
				&& ((oldsize==0) || (bfc_map_size(map)==0))) {
				while ((2*n > (size_t) CV2_ELEMENTS(vec))
				    && (vec->log2_double_indirect < 20)) {
					vec->log2_double_indirect++;
				}
				L4SC_DEBUG(logger,
					"%s(%p): %lu -> %lu, new limit %lu",
					__FUNCTION__, map, (long)oldsize,
					(long)n, (long)bfc_map_load_limit(map));
			} else {
				L4SC_WARN(logger,
					"%s(%p): %lu -> %lu, limit %lu:"
					" should rehash!", __FUNCTION__, map,
					(long)oldsize, (long)n, (long)limit);
			}
			BFC_VECTOR_SET_SIZE(vec, n);
		}
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p) cannot lock", __FUNCTION__, map);
		rc = -EBUSY;
	}
	return (rc);
}

unsigned
bfc_map_keyhashcode(const void *map, const void *key)
{
	unsigned mask, code = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if (map && key && BFC_CLASS((bfc_cobjptr_t)key)) {
		char kbuf[200];
		bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
		code = bfc_object_hashcode(key, BFC_MAP_HASHLEN(vec));
		mask = BFC_MAP_HASHMASK(vec);
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_TRACE(logger, "%s: key %s hashcode 0x%x & 0x%x",
					__FUNCTION__, kbuf, code, mask);
		code &= mask;
	}
	return (code);
}

int
bfc_map_insert_objects(bfc_contptr_t map, bfc_objptr_t key, bfc_objptr_t value,
		       bfc_iterptr_t position, size_t possize)
{
	int rc = -ENOENT;
	unsigned idx, lim;
	bfc_cobjptr_t pkey;
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;

	if ((map == NULL) || (key == NULL)) {
		L4SC_ERROR(logger, "%s(%p, key %p, value %p): null arg",
				__FUNCTION__, map, key, value);
		return (-EFAULT);
	} else if (BFC_CLASS((bfc_cobjptr_t)key) == NULL) {
		L4SC_ERROR(logger, "%s(%p, key %p, value %p): no key class",
				__FUNCTION__, map, key, value);
		return (-EINVAL);
	}

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		idx = bfc_map_keyhashcode(map, key);
		lim = 2u << BFC_MAP_HASHLEN(vec);
		do {
			pair = (bfc_contptr_t) bfc_vector_have(vec, idx);
			if (pair == NULL) {
				rc = -ENOMEM;
				break;
			} else if (BFC_CLASS((bfc_cobjptr_t)pair) == NULL) {
				/* OK, empty entry */
				rc = BFC_SUCCESS;
				break;
			} else if ((pkey = bfc_container_first(pair))== NULL) {
				rc = BFC_SUCCESS;
				break;	
			} else if (BFC_CLASS(pkey) == NULL) {
				rc = BFC_SUCCESS;
				break;
			} else if (bfc_equal_object(pkey, key)) {
				rc = -EEXIST;
				break;
			}
			/* else try next index */
		} while (++idx < lim);

		L4SC_DEBUG(logger, "%s: pair %p at #%u",__FUNCTION__,pair,idx);

		if ((rc == BFC_SUCCESS) && (pair != NULL)
		 && ((rc = bfc_init_object(vec->elem_class, pair,
					   vec->elem_size, vec->pool)) >= 0)) {
			if ((bfc_container_create_element(pair, 0,
						key, vec->pool) != NULL)
			 && (bfc_container_create_element(pair, 1,
						value, vec->pool) != NULL)) {
				rc = (int) idx;
			} else {
				rc = -ENOMEM;
			}
		}
		if (position != NULL) {
			if ((pair != NULL)
			 && (bfc_container_begin_iterator(vec, position,
							  possize) >= 0)) {
				bfc_iterator_set_position(position, idx);
			} else {
				bfc_container_end_iterator(vec, position,
								possize);
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		rc = -EBUSY;
	}

	if (rc == -EEXIST) {
		char kbuf[200];
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_WARN(logger, "%s(%p, key %p: %s): entry exists",
					__FUNCTION__, map, key, kbuf);
	} else if ((rc < 0) || (pair == NULL)) {
		char kbuf[200];
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_ERROR(logger, "%s(%p, key %p: %s): error %d %s",
					__FUNCTION__, map, key, kbuf, rc,
					(rc == -ENOMEM)? "no memory":
					(rc == -EBUSY)?  "cannot lock": "");
	}
	return(rc);
}

int
bfc_map_replace_objects(bfc_contptr_t map, bfc_objptr_t key, bfc_objptr_t value,
		        bfc_iterptr_t position, size_t possize)
{
	int rc = -ENOENT;
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;
	bfc_iterptr_t it;
	size_t itsize;
	bfc_iterator_t iterbuf;

	if ((map == NULL) || (key == NULL)) {
		L4SC_ERROR(logger, "%s(%p, key %p, value %p): null arg",
				__FUNCTION__, map, key, value);
		return (-EFAULT);
	} else if (BFC_CLASS((bfc_cobjptr_t)key) == NULL) {
		L4SC_ERROR(logger, "%s(%p, key %p, value %p): no key class",
				__FUNCTION__, map, key, value);
		return (-EINVAL);
	}

	if (position && (possize >= sizeof(iterbuf))) {
		it = position;
		itsize = possize;
	} else {
		it = &iterbuf;
		itsize = sizeof(iterbuf);
	}

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		rc = bfc_map_insert_objects(map, key, value, it, itsize);
		if ((rc == -EEXIST)
		 && ((pair = bfc_iterator_index(it)) != NULL)) {
			if ((bfc_container_create_element(pair, 0,
						key, vec->pool) != NULL)
			 && (bfc_container_create_element(pair, 1,
						value, vec->pool) != NULL)) {
				rc = (int) bfc_iterator_position(it);
			} else {
				rc = -ENOMEM;
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p) cannot lock", __FUNCTION__, map);
		rc = -EBUSY;
	}
	return (rc);
}

int
bfc_map_find_index(bfc_contptr_t map, bfc_cobjptr_t key, bfc_contptr_t *pairpp)
{
	int rc = -ENOENT;
	unsigned idx, lim;
	bfc_cobjptr_t pkey;
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;
	char kbuf[200];

	if ((map == NULL) || (key == NULL)) {
		L4SC_ERROR(logger, "%s(%p, key %p): null arg",
						__FUNCTION__, map, key);
		return (-EFAULT);
	} else if (BFC_CLASS((bfc_cobjptr_t)key) == NULL) {
		L4SC_ERROR(logger, "%s(%p, key %p): no key class",
						__FUNCTION__, map, key);
		return (-EINVAL);
	}

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		idx = bfc_map_keyhashcode(map, key);
		lim = BFC_VECTOR_GET_SIZE(vec);
		do {
			pair = (bfc_contptr_t) bfc_vector_ref(vec, idx);
			if (pair == NULL) {
				/* entry never allocated */
				break;
			} else if ((BFC_CLASS((bfc_cobjptr_t)pair) != NULL)
			    && ((pkey = bfc_container_first(pair)) != NULL)
			    && BFC_CLASS(pkey) && bfc_equal_object(pkey,key)) {
				rc = idx;
				if (pairpp) {
					*pairpp = (void *) pair;
				}
				break;
			}
			/* else try next index */
		} while (++idx < lim);
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
		rc = -EBUSY;
	}

	bfc_object_tostring(key, kbuf, sizeof(kbuf));
	L4SC_DEBUG(logger, "%s(%p, key %p: %s) %s%d",
		__FUNCTION__, map, key, kbuf, (rc < 0)? "error ": "at #", rc);

	return (rc);
}

int
bfc_map_find_iter(bfc_contptr_t map, bfc_cobjptr_t key,
		  bfc_iterptr_t it, size_t bufsize)
{
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = 0;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if (((rc = bfc_map_find_index(map, key, NULL)) >= 0)
		 && (bfc_container_begin_iterator(vec, it, bufsize) >= 0)) {
			bfc_iterator_set_position(it, rc);
		} else {
			bfc_container_end_iterator(vec, it, bufsize);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
		rc = -EBUSY;
	}
	return (rc);
}

bfc_contptr_t
bfc_map_find_pair(bfc_contptr_t map, bfc_cobjptr_t key)
{
	bfc_contptr_t pair = NULL;
	if (bfc_map_find_index(map, key, &pair) < 0) {
		return (NULL);
	}
	return (pair);
}

bfc_objptr_t
bfc_map_find_value(bfc_contptr_t map, bfc_cobjptr_t key)
{
	bfc_contptr_t pair = NULL;
	bfc_objptr_t  pval = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if ((bfc_map_find_index(map, key, &pair) >= 0) && pair) {
			pval = bfc_container_index(pair, 1);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
	}
	return (pval);
}

bfc_objptr_t
bfc_map_index_value(bfc_contptr_t map, size_t idx)
{
	bfc_contptr_t pair = NULL;
	bfc_objptr_t  pval = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		if ((pair = bfc_vector_ref(vec, (unsigned)idx)) != NULL) {
			pval = bfc_container_index(pair, 1);
		}
		bfc_mutex_unlock(locked);
		L4SC_DEBUG(log, "%s(%p, %ld): @%p",
					__FUNCTION__, map, (long)idx, pval);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld) cannot lock",
						__FUNCTION__, map, (long)idx);
	}
	return (pval);
}

int
bfc_map_count(bfc_contptr_t map, bfc_cobjptr_t key)
{
	int count = 0;
	bfc_cobjptr_t pkey;
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;
	bfc_iterator_t it, limit;

	if ((map == NULL) || (key == NULL)) {
		L4SC_ERROR(logger, "%s(%p, key %p): null arg",
						__FUNCTION__, map, key);
		return (-EFAULT);
	} else if (BFC_CLASS((bfc_cobjptr_t)key) == NULL) {
		L4SC_ERROR(logger, "%s(%p, key %p): no key class",
						__FUNCTION__, map, key);
		return (-EINVAL);
	}

	L4SC_TRACE(logger, "%s(%p, key %p) vec %p, lock %p",
			__FUNCTION__, map, key, vec, vec->lock);

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		L4SC_TRACE(logger, "%s: locked", __FUNCTION__);
		bfc_container_begin_iterator(vec, &it, sizeof(it));
		L4SC_TRACE(logger, "%s: begin iter", __FUNCTION__);
		bfc_object_dump(&it, 1, logger);
		bfc_container_end_iterator(vec, &limit, sizeof(limit));
		L4SC_TRACE(logger, "%s: end iter", __FUNCTION__);
		bfc_object_dump(&limit, 1, logger);
		while (bfc_iterator_distance(&it, &limit) > 0) {
			bfc_object_dump(&it, 1, logger);
			pair = (bfc_contptr_t) bfc_iterator_index(&it);
			if (pair && (BFC_CLASS((bfc_cobjptr_t)pair) != NULL)
			    && (bfc_object_dump(pair, 2, logger), 1)
			    && ((pkey = bfc_container_first(pair)) != NULL)
			    && BFC_CLASS(pkey) && bfc_equal_object(pkey,key)) {
				count++;
			}
			bfc_iterator_advance(&it, 1);
		}
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
	}

	return (count);
}

int
bfc_map_bucket_size(bfc_contptr_t map, bfc_cobjptr_t key)
{
	unsigned idx;
	int count = 0;
	bfc_cobjptr_t pkey;
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;

	if ((map == NULL) || (key == NULL)) {
		L4SC_ERROR(logger, "%s(%p, key %p): null arg",
						__FUNCTION__, map, key);
		return (-EFAULT);
	} else if (BFC_CLASS((bfc_cobjptr_t)key) == NULL) {
		L4SC_ERROR(logger, "%s(%p, key %p): no key class",
						__FUNCTION__, map, key);
		return (-EINVAL);
	}

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		idx = bfc_map_keyhashcode(map, key);
		do {
			pair = (bfc_contptr_t) bfc_vector_ref(vec, idx);
			if (pair == NULL) {
				/* entry never allocated */
				break;
			} else if ((BFC_CLASS((bfc_cobjptr_t)pair) != NULL)
			    && ((pkey = bfc_container_first(pair)) != NULL)
			    && BFC_CLASS(pkey) && bfc_equal_object(pkey,key)) {
				count++;
				break;
			}
		} while (0);
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
	}
	return (count);
}

int
bfc_map_erase_index(bfc_contptr_t map, size_t idx)
{
	bfc_objptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if ((pair = bfc_vector_ref(vec, (unsigned)idx)) != NULL) {
			bfc_destroy(pair);
			pair->vptr = NULL;
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld) cannot lock",
						__FUNCTION__, map, (long)idx);
		rc = -EBUSY;
	}
	return (rc);
}

int
bfc_map_erase_key(bfc_contptr_t map, bfc_cobjptr_t key)
{
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = -ENOENT;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if (((rc = bfc_map_find_index(map, key, &pair)) >= 0) && pair) {
			bfc_destroy(pair);
			((bfc_objptr_t)pair)->vptr = NULL;
			rc = BFC_SUCCESS;
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
		rc = -EBUSY;
	}
	return (rc);
}

int
bfc_map_erase_iter(bfc_contptr_t map, bfc_iterptr_t iter)
{
	bfc_objptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if ((pair = bfc_iterator_index(iter)) != NULL) {
			bfc_destroy(pair);
			pair->vptr = NULL;
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p) cannot lock",
						__FUNCTION__, map, iter);
		rc = -EBUSY;
	}
	return (rc);
}

int
bfc_map_rehash(bfc_contptr_t map, size_t n)
{
	int rc = BFC_SUCCESS;
	size_t clonesize;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_char_vector_t *tmp;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_iterator_t it, limit;

	clonesize = bfc_object_size(vec);
	L4SC_INFO(logger, "%s(%p, %ld): clonesize %ld",
		__FUNCTION__, map, (long) n, (long) clonesize);

	tmp = alloca(clonesize);
	tmp->vptr = NULL;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		/* bfc_init_vector_move clears the source vector */
		rc = bfc_init_vector_move(tmp, clonesize, (bfc_contptr_t)vec);
		if (rc >= 0) {
			vec->log2_double_indirect = 1;
			while (n > (size_t) CV2_ELEMENTS(vec)) {
				vec->log2_double_indirect++;
			}
			bfc_container_begin_iterator(tmp, &it, sizeof(it));
			L4SC_TRACE(logger, "%s: begin iter", __FUNCTION__);
			bfc_object_dump(&it, 1, logger);
			bfc_container_end_iterator(tmp, &limit, sizeof(limit));
			L4SC_TRACE(logger, "%s: end iter", __FUNCTION__);
			bfc_object_dump(&limit, 1, logger);
			while (bfc_iterator_distance(&it, &limit) > 0) {
				bfc_objptr_t sp;
				bfc_cobjptr_t key;
				bfc_object_dump(&it, 1, logger);
				sp = (bfc_objptr_t) bfc_iterator_index(&it);
				if (sp && (BFC_CLASS(sp) != NULL)
				 && ((key = bfc_container_first(sp)) != NULL)) {
					size_t idx;
					bfc_objptr_t dp;
					idx = bfc_map_keyhashcode(map, key);
					dp = (bfc_objptr_t)
					       bfc_vector_have(vec, idx);
					while (dp && (BFC_CLASS(dp) != NULL)) {
						dp = (bfc_objptr_t)
						   bfc_vector_have(vec, ++idx);
					}
					if (dp) {
						L4SC_DEBUG(logger,
							"%s: pair %p at #%lu",
							__FUNCTION__, dp,
							(unsigned long) idx);
						memcpy(dp, sp, vec->elem_size);
						memset(sp,  0, vec->elem_size);
					} else {
						rc = -ENOMEM;
						break;
					}
				}
				bfc_iterator_advance(&it, 1);
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p) cannot lock", __FUNCTION__, map);
		rc = -EBUSY;
	}
	bfc_destroy(tmp);
	return (rc);
}

