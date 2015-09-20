/**
 * @file bfc_string_append_iter_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

/**
 * @brief    bfc_string_append_iter_range
 */
int
bfc_string_append_iter_range(bfc_strptr_t s, bfc_iterptr_t it,
			     bfc_iterptr_t first, bfc_iterptr_t last)
{
	int rc;
	size_t newsize;
	ptrdiff_t d, n;
	long c;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	bfc_iterator_t si, di;

	L4SC_TRACE(logger, "%s(%p, %p, %p, %p)",__FUNCTION__,s,it,first,last);

	if ((d = bfc_iterator_distance(first, last)) < 0) {
		L4SC_ERROR(logger, "%s: negative distance %ld",
						__FUNCTION__, (long)d);
		return (-EINVAL);
	} else if (d == 0) {
		return (bfc_string_resize(s, bfc_iterator_position(it), 0));
	}

	di = *it;
	si = *first;
#define MAKESPACE(extra) \
	newsize = bfc_iterator_position(&di) + (extra);			\
	L4SC_DEBUG(logger, "%s: resizing to %ld", 			\
				__FUNCTION__, (long)newsize);		\
	if ((rc = bfc_string_resize(s, newsize, 0)) < 0) {		\
		L4SC_ERROR(logger, "%s: resizing to %ld error %d",	\
				__FUNCTION__, (long)newsize, rc);	\
		return (rc);						\
	}
	MAKESPACE(d + 20);
	for (n=0; (n < (size_t) d) && !bfc_iterator_equals(&si, last); n++) {
		if ((c = bfc_iterator_getlong(&si)) < 0) {
			break;
		}
		bfc_iterator_advance(&si, 1);

		if (bfc_strlen(s) < bfc_iterator_position(&di) + 8) {
			MAKESPACE(40);
		}
		bfc_iterator_setlong(&di, c);
		bfc_iterator_dump(&di, 1, logger);
		bfc_object_dump(s, 1, logger);
		bfc_iterator_advance(&di, 1);
	}
	MAKESPACE(0);
	bfc_object_dump(s, 1, logger);

	n = bfc_iterator_distance(it, &di);

	return ((int) n);
}

