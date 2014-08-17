/**
 * @file bfc_strdata.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, char)
};

/**
 * @brief    bfc_strdata
 */
const char *
bfc_strdata(bfc_cstrptr_t s)
{
	RETURN_METHCALL(bfc_string_classptr_t, s, first, (s), s->buf);
}

