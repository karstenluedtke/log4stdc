
#include "barefootc/object.h"
#include "errno.h"

int
bfc_incr_refcount(bfc_objptr_t obj)
{
	RETURN_METHCALL(bfc_classptr_t, obj, incrrefc, (obj), -ENOSYS);
}

