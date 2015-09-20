
#include "barefootc/object.h"

void
bfc_destroy(bfc_objptr_t obj)
{
	if (obj && BFC_CLASS(obj)) {
		VOID_METHCALL(bfc_classptr_t, obj, destroy, (obj));
		obj->vptr = NULL;
	}
}

