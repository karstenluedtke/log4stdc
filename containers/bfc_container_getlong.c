
#include "barefootc/object.h"

long
bfc_container_getlong(const void *obj, size_t offs)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, getl, (o, offs), 0);
}

