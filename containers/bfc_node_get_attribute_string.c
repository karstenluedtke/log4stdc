
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/map.h"
#include "barefootc/string.h"

bfc_cstrptr_t
bfc_node_get_attribute_string(bfc_cobjptr_t node, bfc_cstrptr_t name)
{
	bfc_contptr_t map = ((bfc_nodeptr_t)node)->attributes;
	bfc_cobjptr_t o = NULL;

	if (map) {
		o = bfc_map_find_value(map, (bfc_cobjptr_t) name);
	}
	return ((bfc_cstrptr_t) o);
}

