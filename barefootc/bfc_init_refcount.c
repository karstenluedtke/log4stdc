
#include "barefootc/object.h"
#include "errno.h"

int
bfc_init_refcount(bfc_objptr_t obj, int n)
{
	RETURN_METHCALL(bfc_classptr_t, obj, initrefc, (obj, n), -ENOSYS);
}

