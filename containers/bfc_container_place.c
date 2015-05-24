
#include "barefootc/object.h"
#include "barefootc/mempool.h"

void *
bfc_container_place(void *obj, size_t offs,
		    void *elem, struct mempool *pool)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o,
			place, (o, offs, elem, pool),
			(void *) 0);
}

