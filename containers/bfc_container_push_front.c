/**
 * @file bfc_container_push_front.c
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
 * @brief    bfc_container_push_front
 */
int
bfc_container_push_front(bfc_objptr_t c, const void *p)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			push_front, (c, p),
			-ENOSYS);
}
