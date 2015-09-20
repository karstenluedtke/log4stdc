/**
 * @file bfc_node_append_c_str.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/container.h"
#include "barefootc/treenode.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"

#ifdef bfc_node_append_c_str
#undef bfc_node_append_c_str
#endif

/**
 * @brief    bfc_node_append_c_str
 */
int
bfc_node_append_c_str(bfc_objptr_t node, const char *cp)
{
	bfc_strptr_t str;
	struct mempool *pool = ((bfc_nodeptr_t)node)->vec.pool;
	int rc = BFC_SUCCESS;

	if ((str = bfc_mempool_alloc(pool, sizeof(*str))) != NULL) {
		bfc_init_basic_string_c_str(str, sizeof(*str), pool, cp);
		rc = bfc_container_push_back(node, &str);
	} else {
		rc = -ENOMEM;
	}
	return (rc);
}
