/**
 * @file bfc_container_copy_out.c
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
 * @brief    bfc_container_copy_out
 */
size_t
bfc_container_copy_out(bfc_cobjptr_t c,void* buf,size_t n,size_t pos)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			copy_out, (c, buf,n,pos),
			-ENOSYS);
}
