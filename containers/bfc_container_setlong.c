
#include "barefootc/object.h"

int
bfc_container_setlong(void *obj, size_t offs, long val)
{
	bfc_objptr_t o = (bfc_objptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, setl, (o, offs, val), 0);
}

