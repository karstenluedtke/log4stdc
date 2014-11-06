/**
 * @file bfc_basic_string_replace_ranges.c
 */

#include <stddef.h>
#include <string.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

/**
 * @brief    bfc_basic_string_replace_ranges
 */
int
bfc_basic_string_replace_ranges(bfc_strptr_t s,
				bfc_iterptr_t i1, bfc_iterptr_t i2,
				bfc_iterptr_t j1, bfc_iterptr_t j2)
{
	int rc=0, rc2;
	ptrdiff_t d, e;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	bfc_iterator_t it;
	bfc_string_t tail;

	L4SC_TRACE(logger, "%s(%p, %p, %p, %p, %p)",
			__FUNCTION__, s, i1, i2, j1, j2);

	if ((d = bfc_iterator_distance(j1, j2)) < 0) {
		L4SC_ERROR(logger, "%s: negative distance %ld",
						__FUNCTION__, (long)d);
		return (-EINVAL);
	}

	memset(&tail, 0, sizeof(tail));
	if (bfc_string_end_iterator(s, &it, sizeof(it)) >= 0) {
		e = bfc_iterator_distance(i2, &it);
		bfc_destroy(&it);
	} else {
		L4SC_ERROR(logger, "%s: init end iterator failed",__FUNCTION__);
		e = bfc_string_length(s);
	}
	if (e > 0) {
		RETVAR_METHCALL(rc, bfc_string_classptr_t, s,
				init, (&tail, sizeof(tail), s->pool), -ENOSYS);
		if (rc < 0) {
			L4SC_ERROR(logger, "%s: init tail error %d",
							__FUNCTION__, rc);
			return (rc);
		}	
		if ((rc = bfc_string_assign_substr(&tail,
				s, bfc_iterator_position(i2), BFC_NPOS)) < 0) {
			L4SC_ERROR(logger, "%s: saving tail error %d",
							__FUNCTION__, rc);
			bfc_destroy(&tail);
			return (rc);
		}
		bfc_object_dump(&tail, 1, logger);
	} else if (e < 0) {
		L4SC_ERROR(logger, "%s: negative tail distance %ld",
						__FUNCTION__, (long)e);
		return (-EINVAL);
	}

	bfc_string_resize(s, bfc_iterator_position(i1), 0);

	if ((d > 0) && (bfc_clone_object(i1, &it, sizeof(it)) >= 0)) {
		if ((rc = bfc_string_append_iter_range(s, &it, j1, j2)) < 0) {
			L4SC_ERROR(logger, "%s: appending range error %d",
							__FUNCTION__, rc);
		}
		bfc_destroy(&it);
	}

	if ((e > 0) && BFC_CLASS(&tail)) {
		if ((rc2 = bfc_string_append_copy(s, &tail)) < 0) {
			L4SC_ERROR(logger, "%s: appending tail error %d",
							__FUNCTION__, rc);
			rc = (rc < 0)? rc: rc2;
		}
		bfc_destroy(&tail);
	}

	bfc_object_dump(s, 1, logger);
	return (rc);
}

