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
bfc_string_append_range(bfc_strptr_t s,
			 bfc_iterptr_t first, bfc_iterptr_t last)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	const size_t len = bfc_strlen(s);
	bfc_iterator_t end;

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, s, first, last);

	bfc_string_end_iterator(s, &end, sizeof(end));

	return (bfc_string_append_iter_range(s, &end, first, last));
}

