
#include "barefootc/object.h"
#include "barefootc/mempool.h"

bfc_objptr_t
bfc_container_place(bfc_objptr_t cont, size_t offs,
		    bfc_objptr_t elem, bfc_mempool_t pool)
{
	RETURN_METHCALL(bfc_classptr_t, cont,
			place, (cont, offs, elem, pool),
			(bfc_objptr_t) 0);
}

