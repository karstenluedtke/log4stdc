
#include "barefootc/object.h"

void
bfc_destroy(void *obj)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	if (o && BFC_CLASS(o)) {
		VMETHCALL(o, destroy, (o), (void) 0);
		o->vptr = NULL;
	}
}

