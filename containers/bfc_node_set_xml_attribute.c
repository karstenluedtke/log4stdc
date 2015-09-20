
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
bfc_node_set_xml_attribute(bfc_objptr_t node,
			   bfc_cstrptr_t name, bfc_cstrptr_t val)
{
	bfc_contptr_t map;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(node %p, name %p, val %p)",
				__FUNCTION__, node, name, val);

	if ((map = ((bfc_nodeptr_t)node)->attributes) == NULL) {
		if ((bfc_node_new_attribute_map(node, &map) >= 0)
		 && (map != NULL)) {
			bfc_init_refcount(map, 1);
			((bfc_nodeptr_t)node)->attributes = map;
		} else {
			L4SC_ERROR(logger, "%s: no memory", __FUNCTION__);
			return (-ENOMEM);
		}
	}
	rc = bfc_map_replace_objects(map, BFC_UNCONST(bfc_objptr_t, name),
				     BFC_UNCONST(bfc_objptr_t, val), NULL, 0);

	L4SC_TRACE(logger, "%s(node %p, name %p, val %p): rc %d",
				__FUNCTION__, node, name, val, rc);

	return (rc);
}

