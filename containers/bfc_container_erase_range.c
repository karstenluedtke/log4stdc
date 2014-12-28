/**
 * @file bfc_container_erase_range.c
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
 * @brief    bfc_container_erase_range
 */
int
bfc_container_erase_range(bfc_contptr_t c, bfc_iterptr_t first,bfc_iterptr_t last)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			erase_range, (c, first,last),
			-ENOSYS);
}