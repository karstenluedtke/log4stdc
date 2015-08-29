
#include <stddef.h>

#include "barefootc/object.h"
#include "barefootc/mempool.h"

void
bfc_delete(void *obj)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;
	struct mempool *pool;

	if (o && BFC_CLASS(o)) {
		pool = o->parent_pool;
		bfc_destroy(o);
		if (pool) {
			o->parent_pool = NULL;
			mempool_free(pool, o);
		}
	}
}

