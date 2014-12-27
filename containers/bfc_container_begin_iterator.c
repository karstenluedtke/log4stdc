
#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"

int
bfc_container_begin_iterator(const void *obj,
			     struct bfc_iterator *it, size_t bufsize)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	RETURN_METHCALL(bfc_classptr_t, o, ibegin, (o, it, bufsize), -ENOSYS);
}

