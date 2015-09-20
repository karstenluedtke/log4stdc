
#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"

int
bfc_container_end_iterator(bfc_cobjptr_t cont,
			   struct bfc_iterator *it, size_t bufsize)
{
	RETURN_METHCALL(bfc_classptr_t, cont,
			ilimit, (cont, it, bufsize),
			-ENOSYS);
}

