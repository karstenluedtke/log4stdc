
#include <errno.h>

#include "barefootc/object.h"

int
bfc_decr_refcount(bfc_objptr_t obj)
{
	RETURN_METHCALL(bfc_classptr_t, obj, decrrefc, (obj), -ENOSYS);
}

