/**
 * @file bfc_container_swap.c
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
 * @brief    bfc_container_swap
 */
void
bfc_container_swap(bfc_contptr_t c, bfc_contptr_t other)
{
	VOID_METHCALL(bfc_container_classptr_t, c, swap, (c, other));
}