
#include "barefootc/object.h"
#include "barefootc/unconst.h"

const void *
bfc_container_cindex(bfc_cobjptr_t cont, size_t offs)
{
	bfc_objptr_t o = BFC_UNCONST(bfc_objptr_t, cont);

	RETURN_METHCALL(bfc_classptr_t, o, index, (o, offs), NULL);
}

