
#include "barefootc/object.h"

void *
bfc_container_index(bfc_objptr_t cont, size_t offs)
{
	RETURN_METHCALL(bfc_classptr_t, cont, index, (cont, offs), NULL);
}

