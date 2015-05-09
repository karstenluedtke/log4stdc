
#include "barefootc/object.h"
#include "errno.h"

int
bfc_incr_refcount(void *obj)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, incrrefc, (o), -ENOSYS);
}

