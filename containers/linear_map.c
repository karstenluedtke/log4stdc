
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "barefootc/map.h"
#include "barefootc/pair.h"
#include "barefootc/vector.h"
#include "barefootc/synchronization.h"
#include "barefootc/unconst.h"
#include "log4stdc.h"

static void destroy_map(bfc_contptr_t map);
static int find_by_name(bfc_ccontptr_t map, bfc_cobjptr_t key, int depth,
			bfc_iterptr_t it);

extern const struct bfc_map_class bfc_unordered_map_class;

const struct bfc_map_class linear_map_class = {
	.super 		= &bfc_unordered_map_class,
	.name 		= "linear_map",
	.destroy	= destroy_map,
	.find_by_name	= find_by_name,
};

int
bfc_init_linear_map(void *buf, size_t bufsize,
		    bfc_classptr_t pairclass, struct mempool *pool)
{
	int rc;
	bfc_char_vector_t *vec = (bfc_char_vector_t *) buf;
	const size_t elem_size = (*pairclass->clonesize)(NULL);
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	rc = bfc_init_vector_by_element_size(buf, bufsize, pool, elem_size);
	if (rc < 0) {
		L4SC_ERROR(logger,"%s(%p, %ld, pair %p, pool %p):"
				  " init vector error %d", __FUNCTION__,
			   vec, (long) bufsize, pairclass, pool, rc);
		return(rc);
	}

	vec->vptr = (bfc_vector_class_ptr_t) &linear_map_class;
	vec->elem_class = pairclass;

	L4SC_DEBUG(logger,"%s(%p, %ld, pair %p, pool %p): elem_size %ld",
		__FUNCTION__,vec,(long)bufsize,pairclass,pool,(long)elem_size);
	return (rc);
}

int
bfc_init_linear_map_copy(void *buf, size_t bufsize, struct mempool *pool,
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
		vec->vptr = (bfc_vector_class_ptr_t) &linear_map_class;
		vec->elem_class = svec->elem_class;
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

/**
 * @brief  Find the position of a pair in the map by means of the key.
 *
 * @param[in]	map	The map.
 * @param[in]	key	The key.
 * @param[in]	depth	Ignored, there is only one level in the map.
 * @param[out]	it	An iterator which is set to the position of the pair.
 *			If no pair is found, -ENOENT is returend and the
 *			iterator is set to the position where the pair shall be
 *			placed.
 * @return		Index of the pair in the map, or negative on error.
 */
static int
find_by_name(bfc_ccontptr_t map, bfc_cobjptr_t key, int depth, bfc_iterptr_t it)
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
		idx = 0;
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
				if (it != NULL) {
					bfc_container_begin_iterator(map,
							it, sizeof(*it));
					bfc_iterator_set_position(it, idx);
				}
				break;
			}
			/* else try next index */
		} while (++idx < lim);
		if ((rc == -ENOENT) && (it != NULL)) {
			bfc_container_end_iterator(map, it, sizeof(*it));
			/* rc remains -ENOENT */
		}
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger, "%s(%p, key %p) cannot lock",
						__FUNCTION__, map, key);
		rc = -EBUSY;
	}

	bfc_object_tostring(key, kbuf, sizeof(kbuf), NULL);
	L4SC_DEBUG(logger, "%s(%p, key %p: %s) %s%d",
		__FUNCTION__, map, key, kbuf, (rc < 0)? "error ": "at #", rc);
	return (rc);
}

