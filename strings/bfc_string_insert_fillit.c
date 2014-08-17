/**
 * @file bfc_string_insert_fillit.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_insert_fillit
 */
int
bfc_string_insert_fillit(bfc_strptr_t s, bfc_iterptr_t p, size_t n, int c)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			insert_fillit, (s, p, n, c),
			-ENOSYS);
}
