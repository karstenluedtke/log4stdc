/**
 * @file bfc_container_find_element.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"

struct bfc_container {
	BFC_OBJHDR(bfc_container_classptr_t, bfc_objptr_t)
};

/**
 * @brief    bfc_container_find_element
 */
int
bfc_container_find_element(bfc_cobjptr_t c, const void *p, int depth,
                           bfc_iterptr_t position)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			find_element, (c, p, depth, position),
			-ENOSYS);
}
