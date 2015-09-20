
#include "barefootc/object.h"

int
bfc_object_length(bfc_cobjptr_t obj)
{
	RETURN_METHCALL(bfc_classptr_t, obj, length, (obj), 0);
}

