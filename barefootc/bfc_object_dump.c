
#include "barefootc/object.h"

void
bfc_object_dump(bfc_cobjptr_t obj, int depth, struct l4sc_logger *logger)
{
	VOID_METHCALL(bfc_classptr_t, obj, dump, (obj, depth, logger));
}

