
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/map.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "barefootc/unconst.h"

int
bfc_node_set_xml_attribute(bfc_nodeptr_t node,
			   bfc_cstrptr_t name, bfc_cstrptr_t val)
{
	bfc_string_map_t *map;
	struct mempool *pool = node->vec.pool;
	int rc;

	if ((map = (bfc_string_map_t *) node->attributes) == NULL) {
		map = bfc_mempool_calloc(pool, 1, sizeof(*map));
		if (map != NULL) {
			BFC_STRING_MAP_INIT(map, 16, pool);
		} else {
			return (-ENOMEM);
		}
	}
	rc = bfc_map_replace_objects((bfc_contptr_t) map,
				     BFC_UNCONST(bfc_objptr_t, name),
				     BFC_UNCONST(bfc_objptr_t, val), NULL, 0);
	return (rc);
}

