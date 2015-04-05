
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
			  " hash size %d, mask 0x%08X", __FUNCTION__,
			vec, (long) bufsize, estimate, pairclass, pool,
			BFC_MAP_HASHLEN(vec), BFC_MAP_HASHMASK(vec));
	return (rc);
}
