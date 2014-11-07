
#include "barefootc/object.h"

void *
bfc_container_index(void *obj, size_t offs)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, index, (o, offs), NULL);
}

