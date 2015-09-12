
#include "barefootc/object.h"
#include "barefootc/mempool.h"

int
bfc_new(void **objpp, bfc_classptr_t cls, struct mempool *pool)
{
	bfc_objptr_t newobj = NULL;
	size_t size;
	int rc;

	RETVAR_CMETHCALL(size, bfc_classptr_t, cls,
			 clonesize, (NULL),
			 4*sizeof(struct bfc_basic_object));
	newobj = mempool_alloc(pool, size);
	if (newobj == NULL) {
		return (-ENOMEM);
	}
	rc = bfc_init_object(cls, newobj, size, pool);
	if (rc < 0) {
		mempool_free(pool, newobj);
		return (rc);
	}
	newobj->parent_pool = pool;
	bfc_init_refcount(newobj, 1);
	*objpp = newobj;
	return (BFC_SUCCESS);
}

