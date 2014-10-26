
#include "barefootc/object.h"

void
bfc_destroy(void *obj)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	if (o && BFC_CLASS(o)) {
		VOID_METHCALL(bfc_classptr_t, o, destroy, (o));
		o->vptr = NULL;
	}
}

