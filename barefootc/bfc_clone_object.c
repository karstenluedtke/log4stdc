
#include "barefootc/object.h"
#include "barefootc/mempool.h"

int
bfc_clone_object(bfc_cobjptr_t obj,
		 void *buf, size_t bufsize, struct mempool *pool)
{
	int rc;

	RETVAR_METHCALL(rc, bfc_classptr_t, obj,
			clone, (obj, buf, bufsize, pool),
			-ENOSYS);

	bfc_init_refcount(buf, 1);

	return (rc);
}

