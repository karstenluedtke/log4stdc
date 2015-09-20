
#include "barefootc/object.h"

int
bfc_object_tostring(bfc_cobjptr_t obj,
		    char *buf, size_t bufsize, const char *fmt)
{
	RETURN_METHCALL(bfc_classptr_t, obj,
			tostring, (obj, buf, bufsize, fmt),
			0);
}

