/**
 * @file bfc_string_data.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_data
 */
const char *
bfc_string_data(bfc_cobjptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			first, (s),
			s->name);
}
