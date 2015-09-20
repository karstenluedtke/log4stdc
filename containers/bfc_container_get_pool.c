/**
 * @file bfc_container_get_pool.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"

/**
 * @brief    bfc_container_pool
 */
bfc_mempool_t
bfc_container_pool(bfc_cobjptr_t obj)
{
	RETURN_METHCALL(bfc_classptr_t, obj,
			getpool, (obj),
			(bfc_mempool_t) 0);
}
