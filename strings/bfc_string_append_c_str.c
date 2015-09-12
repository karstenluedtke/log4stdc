/**
 * @file bfc_string_append_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "string_private.h"
#include "log4stdc.h"

/**
 * @brief    bfc_string_append_c_str
 */
int
bfc_string_append_c_str(bfc_strptr_t s, const void *s2)
{
	size_t n = (*STRING_TRAITS(s)->szlen)(s2);
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %p, %ld): %s, %s", __FUNCTION__,
		s, s2, (long) n, s->vptr->name, STRING_TRAITS(s)->name);

	RETURN_METHCALL(bfc_string_classptr_t, s, append_buffer, (s, s2, n),
			bfc_string_append_buffer(s, s2, n));
}

