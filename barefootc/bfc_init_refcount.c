
#include "barefootc/object.h"

void
bfc_init_refcount(void *obj, int n)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	VOID_METHCALL(bfc_classptr_t, o, initrefc, (o, n));
}

