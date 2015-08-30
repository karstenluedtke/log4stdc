
#include "barefootc/object.h"
#include "barefootc/mempool.h"

int
bfc_clone_object(const void *obj,
		 void *buf, size_t bufsize, struct mempool *pool)
{
	int rc;
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETVAR_METHCALL(rc, bfc_classptr_t, o,
			clone, (o, buf, bufsize, pool),
			-ENOSYS);

	bfc_init_refcount(buf, 1);

	return (rc);
}

