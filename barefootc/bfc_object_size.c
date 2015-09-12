
#include "barefootc/object.h"

size_t
bfc_object_size(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, clonesize, (o),
			sizeof(struct bfc_basic_object));
}

