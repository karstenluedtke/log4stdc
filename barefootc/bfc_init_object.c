
#include "barefootc/object.h"

#ifndef ELOOP
#define ELOOP EINVAL
#endif

int
bfc_init_object(bfc_classptr_t cls,
		void *buf, size_t bufsize, struct mempool *pool)
{
	int levels;
	bfc_classptr_t super;

	if (cls->init) {
		return ((*cls->init)(buf, bufsize, pool));
	}
	for (levels = 1, super = cls->super; super; super = super->super) {
		if (super->init) {
			return ((*super->init)(buf, bufsize, pool));
		} else if (levels++ > 20) {
			return (-ELOOP);
		}
	}
	return (-ENOSYS);
}

