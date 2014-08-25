/**
 * @file bfc_string_index.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_index
 */
char *
bfc_string_index(bfc_strptr_t s, size_t pos)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			index, (s, pos),
			s->buf);
}