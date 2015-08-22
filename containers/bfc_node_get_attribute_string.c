
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/map.h"
#include "barefootc/string.h"

bfc_cstrptr_t
bfc_node_get_attribute_string(bfc_nodeptr_t node, bfc_cstrptr_t name)
{
	bfc_cobjptr_t o = NULL;

	if (node->attributes) {
		o = bfc_map_find_value(node->attributes, (bfc_cobjptr_t) name);
	}
	return ((bfc_cstrptr_t) o);
}

