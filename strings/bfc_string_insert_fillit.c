/**
 * @file bfc_string_insert_fillit.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_string_insert_fillit
 */
int
bfc_string_insert_fillit(bfc_strptr_t s, bfc_iterptr_t p, size_t n, int c)
{
	size_t pos;
	bfc_iterator_t j1, j2;

	if (p->obj != (bfc_objptr_t) s) {
		return (-EFAULT);
	}
	pos = bfc_iterator_position(p);
	if ((pos == BFC_NPOS) || (pos > bfc_strlen(s))) {
		return (-ERANGE);
	}
	bfc_init_source_iterator(&j1, sizeof(j1), c, 0);
	bfc_init_source_iterator(&j2, sizeof(j2), c, n);
	RETURN_METHCALL(bfc_string_classptr_t, s,
			replace_ranges, (s, p, p, &j1, &j2),
			-ENOSYS);
}

