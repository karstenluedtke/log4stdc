
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/map.h"
#include "barefootc/pair.h"
#include "barefootc/vector.h"
#include "barefootc/synchronization.h"
#include "log4stdc.h"

#define BFC_MAP_HASHLEN(map)	(CV2_LOG2ELEM(map))
#define BFC_MAP_HASHMASK(map)	(CV2_ELEMENTS(map)-1)

extern const bfc_class_t bfc_object_vector_class;

const struct bfc_map_class generic_map_class = {
	.super 		= (void *) &bfc_object_vector_class,
	.name 		= "map",
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
	vec->vptr = (bfc_vector_class_ptr_t) &generic_map_class;
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

	return (n);
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
bfc_map_insert_objects(bfc_contptr_t map, bfc_objptr_t key, bfc_objptr_t value)
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
bfc_map_remove_index(bfc_contptr_t map, size_t idx)
{
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if ((pair = bfc_vector_ref(vec, (unsigned)idx)) != NULL) {
			bfc_destroy(pair);
			((bfc_objptr_t)pair)->vptr = NULL;
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
bfc_map_remove_key(bfc_contptr_t map, bfc_cobjptr_t key)
{
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if (((rc = bfc_map_find_index(map, key, &pair)) >= 0) && pair) {
			bfc_destroy(pair);
			((bfc_objptr_t)pair)->vptr = NULL;
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

