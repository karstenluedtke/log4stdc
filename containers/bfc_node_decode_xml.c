
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

static int new_child_node(bfc_nodeptr_t parent, bfc_nodeptr_t *childpp,
			  struct bfc_node_xml_parse_state *st);
static int new_text_node(bfc_nodeptr_t parent, bfc_strptr_t *strpp,
			 char *text, size_t len);

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
	return (rc);
}

int
bfc_node_parse_xmltags(bfc_nodeptr_t rootnode, bfc_cstrptr_t s,
			struct bfc_node_xml_parse_state *st)
{
	int rc, level, iterations = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(%p, %p)", __FUNCTION__, s, st);

	if (BFC_CLASS(&st->prev) != &bfc_xmltag_forward_iterator_class) {
		init_parse_state(st, sizeof(*st), rootnode->vec.pool, s, 0);
	}
	level = st->prev.level;
	st->curr = st->prev;
	bfc_container_place(&st->stack, level, rootnode, st->stack.pool);

	while ((rc = bfc_iterator_advance((bfc_iterptr_t) &st->curr, 1)) >= 0) {
		bfc_nodeptr_t node = bfc_container_index(&st->stack, level);
		char *tend = bfc_iterator_index((bfc_iterptr_t) &st->curr);
		char *text = bfc_iterator_index((bfc_iterptr_t) &st->prev)
						+ st->prev.length;
		iterations++;

		L4SC_DEBUG(logger, "%s: iteration #%d, found %d, node %p",
				__FUNCTION__, iterations, rc, node);

		if (tend > text) {
			bfc_strptr_t str;
			if ((rc=new_text_node(node,&str,text,tend-text)) < 0) {
				return(rc);
			}
			bfc_decr_refcount(&str); /* ref'd by parent */
		}

		if (rc == BFC_XML_START_TAG) {
			bfc_nodeptr_t newnode = NULL;
			if ((rc = new_child_node(node, &newnode, st)) < 0) {
				return(rc);
			}
			level++;
			bfc_container_place(&st->stack, level,
					    newnode, st->stack.pool);
			bfc_decr_refcount(&newnode); /* ref'd by st->stack */

		} else if (rc == BFC_XML_END_TAG) {
			if (level > 0) {
				bfc_container_resize((bfc_contptr_t)&st->stack,
						     level, NULL);
				level--;
			} else {
				level = 0;
			}

		} else if (rc == BFC_XML_EMPTY_TAG) {
			bfc_nodeptr_t newnode;
			if ((rc = new_child_node(node, &newnode, st)) < 0) {
				return(rc);
			}
			bfc_decr_refcount(&newnode); /* ref'd by parent */
		}
		st->prev = st->curr;
	}
	return (st->prev.level);
}

static int
new_child_node(bfc_nodeptr_t parent, bfc_nodeptr_t *childpp,
		struct bfc_node_xml_parse_state *st)
{
	int i, rc, nattrs;
	bfc_nodeptr_t node;
	bfc_ctagptr_t tag = &st->curr;
	void *buf;
	size_t bufsize;
	bfc_string_t namestr, tmp;
	bfc_string_t attrs[40];

	if ((rc = bfc_new((void **) childpp,
			  BFC_CLASS((bfc_cobjptr_t)parent),
			  parent->vec.pool)) < 0) {
		return (rc);
	}
	node = *childpp;
	if ((rc = bfc_container_push_back((bfc_contptr_t)parent, node)) < 0) {
		return(rc);
	}

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

	return (rc);
}

static int
new_text_node(bfc_nodeptr_t parent, bfc_strptr_t *strpp, char *text, size_t len)
{
	int rc;
	bfc_strptr_t s;
	struct mempool *pool = parent->vec.pool;

	if (((s = bfc_mempool_alloc(pool, sizeof(*s))) != NULL)
	 && ((rc = bfc_init_basic_string_buffer(s, sizeof(*s),
						pool, text, len)) >= 0)) {
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
