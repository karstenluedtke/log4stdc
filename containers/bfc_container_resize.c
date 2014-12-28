/**
 * @file bfc_container_resize.c
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
 * @brief    bfc_container_resize
 */
int
bfc_container_resize(bfc_contptr_t c, size_t n, const void *p)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			resize, (c, n, p),
			-ENOSYS);
}
