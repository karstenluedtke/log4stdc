
#include "barefootc/object.h"
#include "barefootc/mempool.h"

int
bfc_clone_new(bfc_cobjptr_t obj, bfc_objptr_t *newpp, bfc_mempool_t pool)
{
	bfc_cobjptr_t srcobj = (bfc_cobjptr_t) obj;
	bfc_objptr_t newobj = NULL;
	size_t size;
	int rc;

	RETVAR_METHCALL(size, bfc_classptr_t, srcobj,
			clonesize, (srcobj),
			4*sizeof(struct bfc_basic_object));
	newobj = mempool_alloc(pool, size);
	if (newobj == NULL) {
		return (-ENOMEM);
	}
	rc = bfc_clone_object(srcobj, newobj, size, pool);
	if (rc < 0) {
		mempool_free(pool, newobj);
		return (rc);
	}
	newobj->parent_pool = pool;
	bfc_init_refcount(newobj, 1);
	*newpp = newobj;
	return (BFC_SUCCESS);
}

