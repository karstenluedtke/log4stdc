
#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"

struct bfc_container {
	BFC_OBJHDR(bfc_container_classptr_t, bfc_contptr_t)
};

int
bfc_container_insert_fill(bfc_contptr_t c, struct bfc_iterator *it,
			  size_t n, const void *p)
{
	RETURN_METHCALL(bfc_container_classptr_t, c,
			insert_fill, (c, it, n, p),
			-ENOSYS);
}

