
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "barefootc/treenode.h"
#include "barefootc/synchronization.h"
#include "barefootc/unconst.h"
#include "log4stdc.h"

/* define struct bfc_container for the remainder of this file ... */
struct bfc_container {
	BFC_OBJHDR(bfc_container_classptr_t,bfc_contptr_t)
};

static int clone_treenode(bfc_ccontptr_t cont,
	       void *buf, size_t bufsize, struct mempool *pool);
static void destroy_treenode(bfc_contptr_t cont);
static size_t treenode_size(bfc_ccontptr_t cont);
static int treenode_equals(bfc_ccontptr_t cont, bfc_ccontptr_t other);
static unsigned treenode_hashcode(bfc_ccontptr_t cont, int hashlen);

struct bfc_treenode_class {
	BFC_CONTAINER_CLASS_DEF(const struct bfc_treenode_class *,
				bfc_contptr_t, bfc_ccontptr_t, bfc_object_t)
};

extern const struct bfc_vector_class bfc_objref_vector_class;

const struct bfc_container_class bfc_treenode_class = {
	.super 		= (void *) &bfc_objref_vector_class,
	.name 		= "treenode",
	.init 		= bfc_init_treenode,
	.clone 		= clone_treenode,
	.destroy 	= destroy_treenode,
	.clonesize	= treenode_size,
	.hashcode 	= treenode_hashcode,
	.equals 	= treenode_equals,
	//.dump	 	= dump_node,
};

int
bfc_init_treenode(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_nodeptr_t node = (bfc_nodeptr_t) buf;
	int rc;

	if (bufsize < sizeof(*node)) {
		return (-ENOSPC);
	}
	memset(node, 0, sizeof(*node));
	BFC_VECTOR_INIT_POOL(&node->vec, pool);
	rc = bfc_init_objref_vector_class(&node->vec, sizeof(node->vec), pool);
	bfc_init_shared_string_buffer(&node->tagname, sizeof(node->tagname),
									"", 0);
	node->vec.name = node->tagname.buf;
	node->vec.vptr = (void *) &bfc_treenode_class;
	node->vec.elem_class = (bfc_classptr_t) &bfc_treenode_class;
	return (rc);
}

int
bfc_node_set_name(bfc_nodeptr_t node, bfc_cstrptr_t name)
{
	int rc;
	size_t colon;
	bfc_strptr_t str = &node->tagname;
	struct mempool *pool = node->vec.pool;
	bfc_iterator_t it;

	rc = bfc_init_basic_string_copy(str, sizeof(node->tagname), pool, name);
	if (rc >= 0) {
		bfc_string_begin_iterator(str, &it, sizeof(it));
		colon = bfc_string_find_char(str, ':', 0);
		if ((colon != BFC_NPOS) && (colon < bfc_string_length(str))) {
			bfc_iterator_set_position(&it, colon+1);
		}
		node->vec.name = bfc_iterator_index(&it);
	}
	return (rc);
}

static int
new_attribute_map(bfc_node_t *node, bfc_ccontptr_t attrs)
{
	struct mempool *pool = node->vec.pool;
	bfc_string_map_t *map = bfc_mempool_calloc(pool, 1, sizeof(*map));
	if (map != NULL) {
		bfc_contptr_t oldattrs;
		int estimate = attrs? bfc_map_size(attrs)+4: 16;
		BFC_STRING_MAP_INIT(map, estimate, pool);
		if (attrs) {
			bfc_container_assign_copy((bfc_contptr_t)map, attrs);
		}
		bfc_init_refcount(map, 1);
		oldattrs = node->attributes;
		node->attributes = (bfc_contptr_t) map;
		if (oldattrs && BFC_CLASS(oldattrs)) {
			bfc_decr_refcount(oldattrs);
		}
	} else {
		return (-ENOMEM);
	}
	return (BFC_SUCCESS);
}

static int
clone_treenode(bfc_ccontptr_t cont,
	       void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_nodeptr_t node = (bfc_nodeptr_t) buf;
	bfc_cnodeptr_t src = (bfc_cnodeptr_t) cont;
	int rc;

	if ((rc = bfc_init_treenode(node, bufsize, pool)) >= 0) {
		bfc_node_set_name(node, &src->tagname);
		bfc_container_assign_copy((bfc_contptr_t)&node->vec, cont);
		if (src->attributes) {
			new_attribute_map(node, src->attributes);
		}
	}
	return (rc);
}

static void
destroy_treenode(bfc_contptr_t cont)
{
	bfc_container_classptr_t cls;

	if (cont && (cls = BFC_CLASS(cont))) {
		bfc_nodeptr_t node = (bfc_nodeptr_t) cont;
		bfc_contptr_t attrs = node->attributes;
		node->attributes = NULL;
		bfc_destroy(&node->tagname);
		if (attrs != NULL) {
			bfc_decr_refcount(attrs);
		}
		BFC_DESTROY_EPILOGUE(cont, cls);
	}
}

static size_t 
treenode_size(bfc_ccontptr_t cont)
{
	return (sizeof(struct bfc_treenode));
}

static int
treenode_equals(bfc_ccontptr_t cont, bfc_ccontptr_t other)
{
	if (cont == other) {
		return (1);
	}
	return (0);
}

static unsigned
treenode_hashcode(bfc_ccontptr_t cont, int hashlen)
{
	bfc_cnodeptr_t node = (bfc_cnodeptr_t) cont;
	return (bfc_object_hashcode(&node->tagname, hashlen));
}

