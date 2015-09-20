
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/map.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "barefootc/unconst.h"

static int
init_node(bfc_nodeptr_t node, size_t space, bfc_cnodeptr_t ancestor)
{
	bfc_cobjptr_t obj = (bfc_cobjptr_t) ancestor;
	struct mempool *pool = ancestor->vec.pool;

	RETURN_METHCALL(bfc_classptr_t, obj,
			init, (node, space, pool), 0);
}

bfc_objptr_t
bfc_node_new_element(bfc_cobjptr_t ancestor, const char *chldtypes, ...)
{
	bfc_nodeptr_t node;
	size_t space = bfc_object_size(ancestor);
	struct mempool *pool = ((bfc_cnodeptr_t)ancestor)->vec.pool;
	const char *cp, *currtype;
	bfc_objptr_t obj;
	bfc_strptr_t s;
	va_list ap;

	if ((node = bfc_mempool_calloc(pool, 1, sizeof(*node))) == NULL) {
		return (NULL);
	}

	init_node(node, sizeof(*node), (bfc_cnodeptr_t) ancestor);

	va_start(ap, chldtypes);
	for (currtype = chldtypes; currtype && *currtype; currtype++) {
		switch (*currtype) {
		case 'N':
			s = va_arg(ap, bfc_strptr_t);
			bfc_node_set_name((bfc_objptr_t)node, s);
			break;
		case 'n':
			cp = va_arg(ap, const char *);
			space = sizeof(bfc_string_t);
			if ((s = bfc_mempool_alloc(pool, space)) != NULL) {
				bfc_init_basic_string_c_str(s, space, pool, cp);
				bfc_node_set_name((bfc_objptr_t)node, s);
			}
			break;
		case 's': case 't':
			cp = va_arg(ap, const char *);
			space = sizeof(bfc_string_t);
			if ((s = bfc_mempool_alloc(pool, space)) != NULL) {
				bfc_init_basic_string_c_str(s, space, pool, cp);
				bfc_container_push_back((bfc_contptr_t)node, s);
			}
			break;
		case 'S': case 'T':
		case 'E': case 'O':
		default:
			obj = va_arg(ap, bfc_objptr_t);
			bfc_container_push_back((bfc_contptr_t)node, obj);
		}
	}
	va_end(ap);

	return ((bfc_objptr_t)node);
}

