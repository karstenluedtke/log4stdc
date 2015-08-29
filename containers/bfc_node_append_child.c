/**
 * @file bfc_node_append_child.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/container.h"
#include "barefootc/treenode.h"

#ifdef bfc_node_append_child
#undef bfc_node_append_child
#endif

/**
 * @brief    bfc_node_append_child
 */
int
bfc_node_append_child(bfc_nodeptr_t node, bfc_nodeptr_t child)
{
	return (bfc_container_push_back((bfc_contptr_t) node, child));
}
