
#include "barefootc/object.h"

int
bfc_container_setlong(bfc_objptr_t cont, size_t offs, long val)
{
	RETURN_METHCALL(bfc_classptr_t, cont, setl, (cont, offs, val), 0);
}

