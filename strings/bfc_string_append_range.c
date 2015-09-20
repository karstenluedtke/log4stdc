/**
 * @file bfc_string_append_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

/**
 * @brief    bfc_string_append_range
 */
int
bfc_string_append_range(bfc_objptr_t s,
			 bfc_iterptr_t first, bfc_iterptr_t last)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	bfc_iterator_t it;

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, s, first, last);

	bfc_string_end_iterator(s, &it, sizeof(it));

	return (bfc_string_append_iter_range(s, &it, first, last));
}

