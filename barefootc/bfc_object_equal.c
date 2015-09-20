
#include "barefootc/object.h"

int
bfc_object_equals(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	RETURN_METHCALL(bfc_classptr_t, obj, equals, (obj, other), 0);
}

