
#include "barefootc/object.h"

long
bfc_container_getlong(bfc_cobjptr_t cont, size_t offs)
{
	RETURN_METHCALL(bfc_classptr_t, cont, getl, (cont, offs), 0);
}

