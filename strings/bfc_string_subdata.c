/**
 * @file bfc_string_subdata.c
 */

#include <stddef.h>
#include <errno.h>
#include <inttypes.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/unconst.h"

/**
 * @brief    bfc_string_subdata
 */
const char *
bfc_string_subdata(bfc_cstrptr_t s, size_t pos)
{
	bfc_strptr_t s1 = BFC_UNCONST(bfc_strptr_t, s);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			index,(s1,pos),
			s->name + pos);
}

