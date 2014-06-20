
#include "barefootc/object.h"

bfc_objptr_t
bfc_clone_object(const void *obj, void *buf, size_t bufsize)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;
	bfc_objptr_t newobj;

	newobj = VMETHCALL(o, clone, (o, buf, bufsize), NULL);
	return (newobj);
}

