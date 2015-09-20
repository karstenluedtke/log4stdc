/**
 * @file bfc_string_insert_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_insert_fill
 */
int
bfc_string_insert_fill(bfc_objptr_t s, size_t pos, size_t n, int c)
{
	bfc_iterator_t i1, j1, j2;

	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	bfc_string_begin_iterator(s, &i1, sizeof(i1));
	bfc_iterator_set_position(&i1, pos);
	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &i1, &i1, &j1, &j2),
			-ENOSYS);
}

