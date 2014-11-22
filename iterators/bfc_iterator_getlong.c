
#include "barefootc/object.h"
#include "barefootc/iterator.h"

long
bfc_iterator_getlong(bfc_citerptr_t it)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, it, getl, (it, 0), 0);
}

