
#include "barefootc/object.h"

int
bfc_clone_object(const void *obj, void *buf, size_t bufsize)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, clone, (o, buf, bufsize), -ENOSYS);
}

