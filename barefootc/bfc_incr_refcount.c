
#include "barefootc/object.h"

void
bfc_incr_refcount(void *obj)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	VOID_METHCALL(bfc_classptr_t, o, incrrefc, (o));
}

