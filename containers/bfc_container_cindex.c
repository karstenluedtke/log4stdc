
#include "barefootc/object.h"
#include "barefootc/unconst.h"

const void *
bfc_container_cindex(const void *obj, size_t offs)
{
	bfc_objptr_t o = BFC_UNCONST(bfc_objptr_t, obj);

	RETURN_METHCALL(bfc_classptr_t, o, index, (o, offs), NULL);
}

