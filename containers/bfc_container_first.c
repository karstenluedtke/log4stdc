
#include "barefootc/object.h"

const void *
bfc_container_first(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, first, (o), NULL);
}

