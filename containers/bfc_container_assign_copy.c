/**
 * @file bfc_container_assign_copy.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"

struct bfc_container {
	BFC_OBJHDR(bfc_container_classptr_t, bfc_contptr_t)
};

/**
 * @brief    bfc_container_assign_copy
 */
int

bfc_container_assign_copy(bfc_contptr_t c, bfc_ccontptr_t src)
{
	bfc_iterator_t first, last;

	bfc_container_begin_iterator(src, &first, sizeof(first));
	bfc_container_end_iterator(src, &last, sizeof(last));
	bfc_iterator_set_position(&first, 0);

	bfc_container_resize(c, 0, NULL);
	RETURN_METHCALL(bfc_container_classptr_t, c,
			assign_range, (c, &first, &last),
			-ENOSYS);
}
