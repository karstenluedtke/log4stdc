
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/number.h"

bfc_objptr_t
bfc_node_new_boolean_element(bfc_cobjptr_t ancestor, const char *name, int val)
{
	struct mempool *pool = ((bfc_cnodeptr_t)ancestor)->vec.pool;
	bfc_objptr_t number = NULL;
	bfc_objptr_t node = NULL;

	if ((bfc_new(&number, &bfc_boolean_class, pool) >= 0)
	 && (number != NULL)) {
		bfc_object_setlong(number, val);
		node = bfc_node_new_element(ancestor, "nO", name, number);
	}
	return (node);
}
