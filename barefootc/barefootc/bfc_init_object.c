
#include "barefootc/object.h"

bfc_objptr_t
bfc_init_object(bfc_classptr_t cls,
		void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_objptr_t newobj;

	newobj = CMETHCALL(cls, init, (buf, bufsize, pool), NULL);
	return (newobj);
}

