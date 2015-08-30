
#include "barefootc/object.h"

int
bfc_equal_object(const void *obj, const void *other)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;
	bfc_cobjptr_t p = (bfc_cobjptr_t) other;

	RETURN_METHCALL(bfc_classptr_t, o, equals, (o, p), 0);
}

int
bfc_object_equals(const void *obj, const void *other)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;
	bfc_cobjptr_t p = (bfc_cobjptr_t) other;

	RETURN_METHCALL(bfc_classptr_t, o, equals, (o, p), 0);
}

