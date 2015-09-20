/**
 * @file bfc_container_assign_fill.c
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
 * @brief    bfc_container_assign_fill
 */
int
bfc_container_assign_fill(bfc_objptr_t c, size_t n, const void *p)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			assign_fill, (c, n, p),
			-ENOSYS);
}
