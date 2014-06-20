
#include "barefootc/object.h"

void
bfc_object_dump(const void *obj, int depth, struct l4sc_logger *logger)
{
	bfc_cobjptr_t o = (bfc_cobjptr_t) obj;

	VMETHCALL(o, dump, (o, depth, logger), (void) 0);
}

