
#include "barefootc/object.h"

size_t
bfc_object_size(bfc_cobjptr_t obj)
{
	RETURN_METHCALL(bfc_classptr_t, obj, clonesize, (obj),
			sizeof(struct bfc_basic_object));
}

