/**
 * @file bfc_container_reserve.c
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
 * @brief    bfc_container_reserve
 */
int
bfc_container_reserve(bfc_objptr_t c, size_t n)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			reserve, (c, n),
			-ENOSYS);
}
