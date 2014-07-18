
#include "barefootc/object.h"

int
bfc_object_tostring(const void *obj, char *buf, size_t bufsize)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, tostring, (o, buf, bufsize), 0);
}

