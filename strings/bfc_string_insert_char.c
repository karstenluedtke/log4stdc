/**
 * @file bfc_string_insert_char.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_insert_char
 */
int
bfc_string_insert_char(bfc_objptr_t s, bfc_iterptr_t p, int c)
{
	bfc_iterator_t j1, j2;

	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, 1);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, p, p, &j1, &j2),
			-ENOSYS);
}
