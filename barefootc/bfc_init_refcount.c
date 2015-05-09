
#include "barefootc/object.h"
#include "errno.h"

int
bfc_init_refcount(void *obj, int n)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, initrefc, (o, n), -ENOSYS);
}

