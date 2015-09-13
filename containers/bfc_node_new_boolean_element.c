
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/number.h"

bfc_nodeptr_t
bfc_node_new_boolean_element(bfc_cnodeptr_t ancestor, const char *name, int val)
{
	struct mempool *pool = ancestor->vec.pool;
	bfc_numptr_t number = NULL;
	bfc_nodeptr_t node = NULL;

	if ((bfc_new((void **) &number, &bfc_boolean_class, pool) >= 0)
	 && (number != NULL)) {
		bfc_object_setlong(number, val);
		node = bfc_node_new_element(ancestor, "nO", name, number);
	}
	return (node);
}
