/**
 * @file bfc_container_insert_range.c
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
 * @brief    bfc_container_insert_range
 */
int
bfc_container_insert_range(bfc_objptr_t c, bfc_iterptr_t position, bfc_iterptr_t first,bfc_iterptr_t last)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			insert_range, (c, position, first,last),
			-ENOSYS);
}
