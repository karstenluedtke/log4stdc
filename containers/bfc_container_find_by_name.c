/**
 * @file bfc_container_find_by_name.c
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
 * @brief    bfc_container_find_by_name
 */
int
bfc_container_find_by_name(bfc_ccontptr_t c, const void *p, int depth,
                           bfc_iterptr_t position)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			find_by_name, (c, p, depth, position),
			-ENOSYS);
}
