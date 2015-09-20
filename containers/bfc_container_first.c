
#include "barefootc/object.h"

const void *
bfc_container_first(bfc_cobjptr_t cont)
{
	RETURN_METHCALL(bfc_classptr_t, cont, first, (cont), NULL);
}

