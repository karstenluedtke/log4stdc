/**
 * @file bfc_container_capacity.c
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
 * @brief    bfc_container_capacity
 */
size_t
bfc_container_capacity(bfc_ccontptr_t c)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			capacity, (c),
			-ENOSYS);
}
