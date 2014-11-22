
#include "barefootc/object.h"
#include "barefootc/iterator.h"

void *
bfc_iterator_index(bfc_iterptr_t it)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, it, index, (it, 0), NULL);
}

