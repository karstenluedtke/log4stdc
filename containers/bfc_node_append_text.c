/**
 * @file bfc_node_append_text.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/container.h"
#include "barefootc/treenode.h"
#include "barefootc/string.h"

#ifdef bfc_node_append_text
#undef bfc_node_append_text
#endif

/**
 * @brief    bfc_node_append_text
 */
int
bfc_node_append_text(bfc_objptr_t node, bfc_cobjptr_t text)
{
	return (bfc_container_push_back(node, text));
}
