
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "log4stdc.h"

static int init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static int advance_forward(bfc_iterptr_t it, ptrdiff_t n);

extern struct bfc_iterator_class bfc_forward_iterator_class;
extern struct bfc_iterator_class bfc_reverse_iterator_class;

#define FORWARD_CLASS_NAME	"hole-skipping vector forward iterator"

struct bfc_iterator_class bfc_vector_forward_iterator_class = {
	.super	= &bfc_forward_iterator_class,
	.name	= FORWARD_CLASS_NAME,
	.init 	= init_iterator,
	/* Iterator functions */
	.advance    = advance_forward,
};

static int
init_iterator(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_iterptr_t it = (bfc_iterptr_t) buf;
	if (bufsize < sizeof(*it)) {
		return (-ENOSPC);
	}
	memset(it, 0, sizeof(*it));
	it->vptr = &bfc_vector_forward_iterator_class;
	return (BFC_SUCCESS);
}

int
bfc_init_vector_iterator(void *buf, size_t bufsize,
			 bfc_cobjptr_t obj, size_t pos)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	L4SC_TRACE(logger, "%s(%p, %ld, %ld)",
		__FUNCTION__, buf, (long) bufsize, (long) pos);

	if ((rc = bfc_init_iterator(buf, bufsize, obj, pos)) == BFC_SUCCESS) {
		bfc_iterptr_t it = (bfc_iterptr_t) buf;
		it->vptr = &bfc_vector_forward_iterator_class;
	}
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

static size_t
next_allocated_index(bfc_objptr_t obj, size_t idx)
{
	BFC_VECTOR(minvec1, char, 0) *vec = (struct minvec1 *) obj;
	unsigned stage = 0;
	unsigned i=0, j=0, k=0;
	unsigned cont_elems = 0;
	const unsigned cont_ptrs = CV2_POINTERS(vec);
	const size_t size = BFC_VECTOR_GET_SIZE(vec);
	char *ptr;
	l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	
	if ((idx == 0) || (idx < CV0_BOUNDARY(vec))) {
		stage = 0;
		k = idx;
		cont_elems = CV0_ELEMENTS(vec);
		L4SC_DEBUG(log, "%s: current idx %ld is vec.direct[%u]",
					__FUNCTION__, (long) idx, k);
	} else if (idx < CV1_BOUNDARY(vec)) {
		stage = 1;
		k = idx - CV0_BOUNDARY(vec);
		if (vec->indirect) {
			cont_elems = CV1_ELEMENTS(vec);
			L4SC_DEBUG(log, "%s: current idx %ld "
					" is vec.indirect[%u]",
					__FUNCTION__, (long) idx, k);
		} else {
			L4SC_ERROR(log, "%s: current idx %ld does not exist"
					" as vec.indirect[%u]",
					__FUNCTION__, (long) idx, k);
		}
	} else if (idx < CV2_BOUNDARY(vec)) {
		stage = 2;
		k = idx - CV1_BOUNDARY(vec);
		j = CV2_IDX1(vec,k);
		k &= (CV1_ELEMENTS(vec)-1);
		if (vec->double_indirect && vec->double_indirect[j]) {
			cont_elems = CV1_ELEMENTS(vec);
			L4SC_DEBUG(log, "%s: current idx %ld "
					" is vec.double_indirect[%u][%u]",
					__FUNCTION__, (long) idx, j, k);
		} else {
			L4SC_ERROR(log, "%s: current idx %ld does not exist"
					" as vec.double_indirect[%u][%u]",
					__FUNCTION__, (long) idx, j, k);
		}
	} else if (idx < size) {
		stage = 3;
		k = idx - CV2_BOUNDARY(vec);
		i = CV3_IDX1(vec,k);
		j = CV3_IDX2(vec,k);
		k &= (CV1_ELEMENTS(vec)-1);
		if (vec->triple_indirect && vec->triple_indirect[i]
					 && (vec->triple_indirect[i])[j]) {
			cont_elems = CV1_ELEMENTS(vec);
			L4SC_DEBUG(log, "%s: current idx %ld "
					" is vec.triple_indirect[%u][%u][%u]",
					__FUNCTION__, (long) idx, i, j, k);
		} else {
			L4SC_ERROR(log, "%s: current idx %ld does not exist"
					" as vec.triple_indirect[%u][%u][%u]",
					__FUNCTION__, (long) idx, i, j, k);
		}
	}
	while (++idx < size) {
		++k;
		ptr = ""; /* don't care, just needs to be != NULL */
		CV_FOREACH_NEXT(vec,idx,ptr,stage,i,j,k,cont_ptrs,cont_elems);
		if (ptr != NULL) {
			break;
		}
	}

	return (idx);
}

static int
advance_forward(bfc_iterptr_t it, ptrdiff_t n)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	L4SC_TRACE(logger, "%s(%p, %ld)", __FUNCTION__, it, (long) n);

	if (n > 0) {
		size_t objlen = bfc_object_length(it->obj);
		size_t pos = it->pos;
		size_t idx = (pos == BFC_NPOS)? 0: pos;
		while ((idx < objlen) && (n > (ptrdiff_t)(idx - pos))) {
			idx = next_allocated_index(it->obj, idx);
		}
		it->pos = (idx < objlen)? idx: objlen;
	} else if (n < 0) {
		if ((it->pos != BFC_NPOS) && (it->pos >= (-n))) {
			it->pos += n;
		} else {
			it->pos = BFC_NPOS;
		}
	}
	bfc_object_dump(it, 1, logger);
	return (BFC_SUCCESS);
}

