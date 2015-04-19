
#include "barefootc/object.h"
#include "barefootc/mempool.h"

void *
bfc_container_create_element(void *obj, size_t offs,
			     void *elem, struct mempool *pool)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o,
			create, (o, offs, elem, pool),
			(void *) 0);
}

