/**
 * @file bfc_init_shared_string_range.c
 */

#include <stddef.h>
#include <errno.h>
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_init_shared_string_range
 */
int
bfc_init_shared_string_range(void *buf, size_t bufsize,
				bfc_iterptr_t start, bfc_iterptr_t limit)
{
	char *p;
	ptrdiff_t d;

	if ((d = bfc_iterator_distance(start, limit)) < 0) {
		return (-EINVAL);
	}

	if ((p = (char*) bfc_iterator_index(start)) == NULL) {
		return (-EFAULT);
	}

	return (bfc_init_shared_string_buffer(buf, bufsize, p, (size_t) d));
}

