/**
 * @file bfc_string_insert_c_str.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "log4stdc.h"
#define  BFC_STRING_LOGGER  "barefootc.string", 16

/**
 * @brief    bfc_string_insert_c_str
 */
int
bfc_string_insert_c_str(bfc_strptr_t s, size_t pos, const void *s2)
{
	size_t n = (*s->vptr->traits->szlen)(s2);
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %p, %ld): %s, %s", __FUNCTION__,
		s, s2, (long) n, s->vptr->name, s->vptr->traits->name);

	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_buffer, (s, pos, 0, s2, n),
			bfc_string_replace_buffer(s, pos, 0, s2, n));
}

