/**
 * @file bfc_string_reverse_end_iterator.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_reverse_end_iterator
 */
int
bfc_string_reverse_end_iterator(bfc_cstrptr_t s,
				bfc_iterptr_t it, size_t bufsize)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			rlimit, (s, it, bufsize),
			-ENOSYS);
}
