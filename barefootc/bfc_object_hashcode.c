
#include "barefootc/object.h"

unsigned
bfc_object_hashcode(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, hashcode, (o), 0);
}

