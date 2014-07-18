
#include "barefootc/object.h"

int
bfc_object_length(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, length, (o), 0);
}

