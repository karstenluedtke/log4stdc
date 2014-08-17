/**
 * @file bfc_string_append_range.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"
#include "log4stdc.h"
#define  LOGGERNAME  "barefootc.string", 16

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, char)
};

/**
 * @brief    bfc_string_append_range
 */
int
bfc_string_append_range(bfc_strptr_t s,
			 bfc_iterptr_t first, bfc_iterptr_t last)
{
	l4sc_logger_ptr_t logger = l4sc_get_logger(LOGGERNAME);
	const size_t len = bfc_strlen(s);
	bfc_iterator_t end;

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, s, first, last);

	bfc_init_iterator(&end, sizeof(end), (bfc_objptr_t) s, len);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &end, &end, first, last),
			bfc_string_replace_ranges(s, &end, &end, first, last));
}

