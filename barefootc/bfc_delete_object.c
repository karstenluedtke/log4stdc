
#include <stddef.h>

#include "barefootc/object.h"
#include "barefootc/mempool.h"

void
bfc_delete(bfc_objptr_t obj)
{
	bfc_mempool_t pool;

	if (obj && BFC_CLASS(obj)) {
		pool = obj->parent_pool;
		bfc_destroy(obj);
		if (pool) {
			obj->parent_pool = NULL;
			mempool_free(pool, obj);
		}
	}
}

