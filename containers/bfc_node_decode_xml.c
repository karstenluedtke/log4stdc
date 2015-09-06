
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/xmltag.h"
#include "barefootc/iterator.h"
#include "barefootc/treenode.h"

#include "log4stdc.h"

struct bfc_node_xml_parse_state {
	BFC_VECTOR(bfc_node_parse_stack, bfc_nodeptr_t, 4) stack;
	bfc_xmltag_t	prev;
	bfc_xmltag_t	curr;
};

extern const struct bfc_xmltag_iterator_class bfc_xmltag_forward_iterator_class;

static int set_node_attributes(bfc_nodeptr_t node, bfc_ctagptr_t tag);
static int new_child_node(bfc_nodeptr_t parent, bfc_nodeptr_t *childpp,
			  bfc_ctagptr_t tag);
static int new_text_node(bfc_nodeptr_t parent, bfc_strptr_t *strpp,
			 bfc_cstrptr_t text);

static int
init_parse_state(void *buf, size_t bufsize, struct mempool *pool,
		 bfc_cstrptr_t s, size_t offs)
{
	struct bfc_node_xml_parse_state *st;
	int rc;

	memset(buf, 0, bufsize);
	st = (struct bfc_node_xml_parse_state *) buf;
	BFC_VECTOR_INIT_POOL(&st->stack, pool);
	rc = bfc_init_objref_vector_class(&st->stack, sizeof(st->stack), pool);
	bfc_init_xmltag(&st->prev, sizeof(st->prev), s, offs);
	st->curr = st->prev;

	return (rc);
}

static void
destroy_parse_state(struct bfc_node_xml_parse_state *st)
{
	if (st->stack.vptr) {
		bfc_destroy(&st->prev);
		bfc_destroy(&st->curr);
		bfc_destroy(&st->stack);
		st->stack.vptr = NULL;
	}
}

int
bfc_node_decode_xml(bfc_nodeptr_t node, bfc_cstrptr_t s, size_t offs)
{
	int rc;
	struct bfc_node_xml_parse_state st;

	if ((rc = init_parse_state(&st,sizeof(st),node->vec.pool,s,offs)) < 0) {
		return (rc);
	}
	rc = bfc_node_parse_xmltags(node, s, &st);
	destroy_parse_state(&st);
	if (rc > 0) {
		rc = -EPIPE;
	}
	return (rc);
}

int
bfc_node_parse_xmltags(bfc_nodeptr_t rootnode, bfc_cstrptr_t s,
			struct bfc_node_xml_parse_state *st)
{
	int rc, level, iterations = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %p)", __FUNCTION__, s, st);

	if (BFC_CLASS(&st->prev) != &bfc_xmltag_forward_iterator_class) {
		init_parse_state(st, sizeof(*st), rootnode->vec.pool, s, 0);
	}
	level = st->prev.level;
	st->curr = st->prev;
	bfc_container_place(&st->stack, level, rootnode, st->stack.pool);

	while ((rc = bfc_iterator_advance((bfc_iterptr_t) &st->curr, 1)) >= 0) {
		bfc_ctagptr_t tag = &st->curr;
		bfc_nodeptr_t node = bfc_container_index(&st->stack, level);
		size_t offs = bfc_iterator_position((bfc_citerptr_t)tag);
		size_t off1 = bfc_iterator_position((bfc_citerptr_t)&st->prev);
		size_t text = off1 + st->prev.length;
							
		iterations++;

		L4SC_DEBUG(logger,
			"%s: iteration #%d, found %d@%ld, level %d, node %p",
			__FUNCTION__, iterations, rc, (long)offs, level, node);

		if ((offs > text) && (level > 0)) {
			bfc_string_t str;
			bfc_strptr_t newnode;
			bfc_string_shared_substr(s, text, offs-text,
						 &str, sizeof(str));
			if ((rc = new_text_node(node, &newnode, &str)) < 0) {
				return(rc);
			}
			bfc_decr_refcount(newnode); /* ref'd by parent */
		}

		L4SC_DEBUG(logger, "%s: iteration #%d, tag %.*s",
			__FUNCTION__, iterations,(int) st->curr.length,
			(char *)bfc_iterator_index((bfc_iterptr_t)&st->curr));

		if (tag->tagtype == BFC_XML_START_TAG) {
			int nextlevel = tag->level+1;
			if (level == 0) {
				if ((rc = set_node_attributes(node, tag)) < 0) {
					return (rc);
				}
				bfc_container_place(&st->stack, nextlevel,
						    rootnode, st->stack.pool);
			} else {
				bfc_nodeptr_t nn = NULL;
				if ((rc = new_child_node(node, &nn, tag)) < 0) {
					return (rc);
				}
				bfc_container_place(&st->stack, nextlevel,
						    nn, st->stack.pool);
				bfc_decr_refcount(nn); /* ref'd by st->stack */
			}
			level = nextlevel;
			L4SC_DEBUG(logger, "%s: nodestack[%d] = %p",
				__FUNCTION__, level,
				bfc_container_index(&st->stack,level));

		} else if (tag->tagtype == BFC_XML_END_TAG) {
			level = tag->level;
			bfc_container_resize((bfc_contptr_t)&st->stack,
							     level+1, NULL);

		} else if (tag->tagtype == BFC_XML_EMPTY_TAG) {
			if (level == 0) {
				if ((rc = set_node_attributes(node, tag)) < 0) {
					return (rc);
				}
			} else {
				bfc_nodeptr_t nn = NULL;
				if ((rc = new_child_node(node, &nn, tag)) < 0) {
					return(rc);
				}
				bfc_decr_refcount(nn); /* ref'd by parent */
			}
		}
		st->prev = st->curr;
	}
	return (st->prev.level);
}

static int
new_child_node(bfc_nodeptr_t parent, bfc_nodeptr_t *childpp, bfc_ctagptr_t tag)
{
	int rc;
	bfc_nodeptr_t node;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %p, %p)",__FUNCTION__,parent, childpp, tag);

	if ((rc = bfc_new((void **) childpp,
			  BFC_CLASS((bfc_cobjptr_t)parent),
			  parent->vec.pool)) < 0) {
		return (rc);
	}
	node = *childpp;
	if ((rc = bfc_container_push_back((bfc_contptr_t)parent, node)) < 0) {
		return(rc);
	}
	rc = set_node_attributes(node, tag);
	return (rc);
}

static int
set_node_attributes(bfc_nodeptr_t node, bfc_ctagptr_t tag)
{
	int i, nattrs;
	void *buf;
	size_t bufsize;
	bfc_string_t namestr, tmp;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_string_t attrs[40];

	L4SC_TRACE(logger, "%s(%p, %p)", __FUNCTION__, node, tag);

	bfc_tag_get_name(tag, &namestr, sizeof(namestr));
	bfc_node_set_name(node, &namestr);

	nattrs = bfc_tag_get_attrs(tag, attrs, sizeof(attrs));
	for (i=0; i < 2*nattrs; i++) {
		if (bfc_string_find_char(&attrs[i], '&', 0) != BFC_NPOS) {
			bufsize = bfc_strlen(&attrs[i]) + 10;
			buf = alloca(bufsize);
			bfc_string_buffered_substr(&attrs[i], 0, BFC_NPOS,
					   &tmp, sizeof(tmp), buf, bufsize);
			bfc_string_decode_html_entities(&tmp);
			bfc_string_swap(&attrs[i], &tmp);
			bfc_destroy(&tmp);
		}
	}

	for (i=0; i < nattrs; i++) {
		bfc_node_set_xml_attribute(node, &attrs[2*i], &attrs[2*i+1]);
	}

	return (nattrs);
}

static int
new_text_node(bfc_nodeptr_t parent, bfc_strptr_t *strpp, bfc_cstrptr_t text)
{
	int rc;
	bfc_strptr_t s;
	struct mempool *pool = parent->vec.pool;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %p, %p)", __FUNCTION__, parent, strpp, text);

	if (((s = bfc_mempool_alloc(pool, sizeof(*s))) != NULL)
	 && ((rc = bfc_init_basic_string_copy(s,sizeof(*s),pool,text)) >= 0)) {
		if (bfc_string_find_char(s, '&', 0) != BFC_NPOS) {
			bfc_string_decode_html_entities(s);
		}
		rc = bfc_container_push_back((bfc_contptr_t)parent, s);
		*strpp = s;
	} else {
		rc = -ENOMEM;
	}
	return (rc);
}
