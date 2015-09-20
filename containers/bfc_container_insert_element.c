/**
 * @file bfc_container_insert_element.c
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
 * @brief    bfc_container_insert_element
 */
int
bfc_container_insert_element(bfc_objptr_t c, bfc_iterptr_t position, const void *p)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			insert_element, (c, position, p),
			-ENOSYS);
}
