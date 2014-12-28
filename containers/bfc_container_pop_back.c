/**
 * @file bfc_container_pop_back.c
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
 * @brief    bfc_container_pop_back
 */
void
bfc_container_pop_back(bfc_contptr_t c)
{
	VOID_METHCALL(bfc_container_classptr_t, c, pop_back, (c));
}
