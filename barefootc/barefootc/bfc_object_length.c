
#include "barefootc/object.h"

int
bfc_object_length(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;
	int len;

	len = VMETHCALL(o, length, (o), 0);
	return (len);
}

