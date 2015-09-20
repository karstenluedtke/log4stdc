
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "barefootc/treenode.h"
#include "barefootc/map.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"

int
bfc_node_append(bfc_objptr_t node, const char *chldtypes, ...)
{
	struct mempool *pool = bfc_container_pool(node);
	const char *cp, *currtype;
	bfc_objptr_t obj;
	bfc_strptr_t s;
	size_t space;
	va_list ap;

	va_start(ap, chldtypes);
	for (currtype = chldtypes; currtype && *currtype; currtype++) {
		switch (*currtype) {
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

	return ((int) bfc_object_length(node));
}

