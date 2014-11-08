/**
 * @file bfc_string_assign_range.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"

/**
 * @brief    bfc_string_assign_range
 */
int
bfc_string_assign_range(bfc_strptr_t s,bfc_iterptr_t first,bfc_iterptr_t last)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	bfc_iterator_t start, limit;

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, s, first, last);

	bfc_string_begin_iterator(s, &start, sizeof(start));
	bfc_string_end_iterator  (s, &limit, sizeof(limit));

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &start, &limit, first, last),
			bfc_string_replace_ranges(s,&start,&limit,first,last));
}

