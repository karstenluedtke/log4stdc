
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/map.h"
#include "barefootc/pair.h"
#include "barefootc/vector.h"
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

unsigned
bfc_map_keyhashcode(bfc_ccontptr_t map, bfc_cobjptr_t key)
{
	unsigned mask, code = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if (map && key && BFC_CLASS(key)) {
		char kbuf[200];
		bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
		code = bfc_object_hashcode(key);
		mask = BFC_MAP_HASHMASK(vec);
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_TRACE(logger, "%s: key %s hashcode 0x%x & 0x%x -> 0x%x",
			__FUNCTION__, kbuf, code, mask, code & mask);
		code &= mask;
	}
	return (code);
}

int
bfc_map_insert_objects(bfc_contptr_t map, void *key, void *value)
{
	int rc;
	unsigned idx, lim;
	bfc_contptr_t pair = NULL;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) map;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if ((map == NULL) || (key == NULL)) {
		L4SC_ERROR(logger, "%s(%p, key %p, value %p): null arg",
				__FUNCTION__, map, key, value);
		return (-EFAULT);
	} else if (BFC_CLASS((bfc_cobjptr_t)key) == NULL) {
		L4SC_ERROR(logger, "%s(%p, key %p, value %p): no key class",
				__FUNCTION__, map, key, value);
		return (-EINVAL);
	}
	idx = bfc_map_keyhashcode(map, (bfc_cobjptr_t)key);
	lim = 2u << BFC_MAP_HASHLEN(vec);
	do {
		pair = (bfc_contptr_t) bfc_vector_have(vec, idx);
		if (pair == NULL) {
			/* No memory */
			break;
		} else if (BFC_CLASS((bfc_cobjptr_t)pair) == NULL) {
			/* OK, empty entry */
			break;
		} else {
			bfc_cobjptr_t k = bfc_container_first(pair);
			if (k == NULL) {
				break;	
			} else if (BFC_CLASS(k) == NULL) {
				break;
			} else if (bfc_equal_object(k, key)) {
				char kbuf[200];
				bfc_object_tostring(key, kbuf, sizeof(kbuf));
				L4SC_WARN(logger, "%s(%p, key %p: %s) exists",
						__FUNCTION__, map, key, kbuf);
				bfc_object_dump(key, 1, logger);
				return (-EEXIST);
			}
			/* else try next index */
		}
	} while (++idx < lim);

	L4SC_DEBUG(logger, "%s: pair %p at index %u", __FUNCTION__, pair, idx);

	if (pair == NULL) {
		char kbuf[200];
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_ERROR(logger, "%s(%p, key %p: %s): no memory for pair",
				__FUNCTION__, map, key, kbuf);
		return (-ENOMEM);
	}

	rc = bfc_init_object(vec->elem_class, pair, vec->elem_size, vec->pool);
	if (rc < 0) {
		char kbuf[200];
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_ERROR(logger, "%s(%p, key %p: %s): pair init error %d",
				__FUNCTION__, map, key, kbuf, rc);
		return (rc);
	}
	if ((bfc_container_create_element(pair, 0, key, vec->pool) == NULL)
	 || (bfc_container_create_element(pair, 1, value, vec->pool) == NULL)) {
		char kbuf[200];
		bfc_object_tostring(key, kbuf, sizeof(kbuf));
		L4SC_ERROR(logger, "%s(%p, key %p: %s): set error %d",
				__FUNCTION__, map, key, kbuf, rc);
	}

	return(rc);
}
	
