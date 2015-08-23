
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/map.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "barefootc/unconst.h"

#include "log4stdc.h"

int
bfc_node_set_xml_attribute(bfc_nodeptr_t node,
			   bfc_cstrptr_t name, bfc_cstrptr_t val)
{
	bfc_string_map_t *map;
	struct mempool *pool = node->vec.pool;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(node %p, name %p, val %p)",
				__FUNCTION__, node, name, val);

	if ((map = (bfc_string_map_t *) node->attributes) == NULL) {
		map = bfc_mempool_calloc(pool, 1, sizeof(*map));
		if (map != NULL) {
			BFC_STRING_MAP_INIT(map, 16, pool);
			bfc_init_refcount(map, 1);
			node->attributes = (bfc_contptr_t) map;
		} else {
			L4SC_ERROR(logger, "%s: no memory", __FUNCTION__);
			return (-ENOMEM);
		}
	}
	rc = bfc_map_replace_objects((bfc_contptr_t) map,
				     BFC_UNCONST(bfc_objptr_t, name),
				     BFC_UNCONST(bfc_objptr_t, val), NULL, 0);

	L4SC_TRACE(logger, "%s(node %p, name %p, val %p): rc %d",
				__FUNCTION__, node, name, val, rc);

	return (rc);
}

