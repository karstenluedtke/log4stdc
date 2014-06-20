
#include "barefootc/object.h"

size_t
bfc_object_size(const void *obj)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;
	size_t size;

	size = VMETHCALL(o, clonesize, (o), sizeof(struct bfc_objhdr));
	return (size);
}

