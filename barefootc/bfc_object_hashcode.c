
#include "barefootc/object.h"

unsigned
bfc_object_hashcode(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;
	unsigned code;

	code = VMETHCALL(o, hashcode, (o), 0);
	return (code);
}

