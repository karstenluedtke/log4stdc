
#include "barefootc/object.h"

unsigned
bfc_object_hashcode(bfc_cobjptr_t obj, int hashlen)
{
	RETURN_METHCALL(bfc_classptr_t, obj, hashcode, (obj, hashlen), 0);
}

