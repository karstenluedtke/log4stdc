
#include "barefootc/object.h"
#include "barefootc/mempool.h"

bfc_objptr_t
bfc_new(bfc_classptr_t cls, struct mempool *pool)
{
	bfc_objptr_t newobj = NULL;
	size_t size;
	void *buf;

	size = CMETHCALL(cls, clonesize, (NULL), 4*sizeof(struct bfc_objhdr));
	buf  = mempool_alloc(pool, size);
	if (buf == NULL) {
		return (NULL);
	}
	newobj = bfc_init_object(cls, buf, size, pool);
	if (newobj == NULL) {
		mempool_free(pool, buf);
		return (NULL);
	}
	newobj->pool = pool;
	return (newobj);
}

