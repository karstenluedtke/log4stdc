
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/number.h"

bfc_objptr_t
bfc_node_new_integer_element(bfc_cobjptr_t ancestor, const char *name, long val)
{
	bfc_mempool_t pool = bfc_container_pool(ancestor);
	bfc_objptr_t number = NULL;
	bfc_objptr_t node = NULL;

	if ((bfc_new(&number, &bfc_number_class, pool) >= 0)
	 && (number != NULL)) {
		bfc_object_setlong(number, val);
		node = bfc_node_new_element(ancestor, "nO", name, number);
	}
	return (node);
}
