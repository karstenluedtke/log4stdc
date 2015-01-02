
#include <errno.h>

#include "barefootc/object.h"

int
bfc_decr_refcount(void *obj)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, decrrefc, (o), -ENOSYS);
}

