/**
 * @file bfc_strdata.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_strdata
 */
const char *
bfc_strdata(bfc_cstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, first, (s), s->buf);
}

