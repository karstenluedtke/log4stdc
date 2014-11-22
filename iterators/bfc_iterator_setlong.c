
#include "barefootc/object.h"
#include "barefootc/iterator.h"

int
bfc_iterator_setlong(bfc_iterptr_t it, long val)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, it, setl, (it, 0, val), 0);
}

