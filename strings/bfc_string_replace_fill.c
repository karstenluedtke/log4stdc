/**
 * @file bfc_string_replace_fill.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_replace_fill
 */
int
bfc_string_replace_fill(bfc_objptr_t s, size_t pos, size_t n1, size_t n2, int c)
{
	bfc_iterator_t i1, i2, j1, j2;

	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	bfc_string_begin_iterator(s, &i1, sizeof(i1));
	bfc_string_begin_iterator(s, &i2, sizeof(i2));
	bfc_iterator_set_position(&i1, pos);
	bfc_iterator_set_position(&i2, pos + bfc_string_sublen(s, pos, n1));
	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n2);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, &i1, &i2, &j1, &j2),
			-ENOSYS);
}

