/**
 * @file      treenode.h
 *
 * @brief     Tree nodes for representing XML documents and others.
 *
 * @author    Karsten Luedtke
 *
 * @date      2015-05-30
 *
 * Copyright (c)  2015  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_TREENODE_H_
#define _BFC_TREENODE_H_

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/vector.h"
#include "barefootc/map.h"
#include "barefootc/container.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup treenode_definition */
/** @{ */
/**
 * @brief    Tree node definition.
 */
struct bfc_treenode;
typedef struct bfc_treenode bfc_node_t;
typedef struct bfc_treenode *bfc_nodeptr_t;
typedef const struct bfc_treenode *bfc_cnodeptr_t;

struct bfc_treenode {
	BFC_VECTOR(bfc_node_vector, bfc_nodeptr_t, 4) vec;
	bfc_string_t  tagname;  /**< tag name including namespace prefix */
	bfc_objptr_t attributes;
};
/** @} */

int bfc_init_treenode(void *buf, size_t bufsize, bfc_mempool_t pool);
int bfc_node_set_name(bfc_objptr_t node, bfc_cobjptr_t name);

bfc_objptr_t bfc_node_new_element(bfc_cobjptr_t ancestor,
				   const char *chldtypes, ...);
bfc_objptr_t bfc_node_new_integer_element(bfc_cobjptr_t ancestor,
				   const char *name, long val);
bfc_objptr_t bfc_node_new_boolean_element(bfc_cobjptr_t ancestor,
				   const char *name, int val);

int bfc_node_set_xml_attribute(bfc_objptr_t node,
			       bfc_cobjptr_t name, bfc_cobjptr_t val);
bfc_cobjptr_t bfc_node_get_attribute_string(bfc_cobjptr_t node,
					    bfc_cobjptr_t name);
int bfc_node_new_attribute_map(bfc_objptr_t, bfc_objptr_t *attrpp);

int bfc_node_append(bfc_objptr_t node, const char *chldtypes, ...);
int bfc_node_append_child(bfc_objptr_t node, bfc_objptr_t child);
int bfc_node_append_text (bfc_objptr_t node, bfc_cobjptr_t text);
int bfc_node_append_c_str(bfc_objptr_t node, const char *cp);

int bfc_node_encode_xml(bfc_cobjptr_t node,
			char *buf, size_t bufsize, int level);
int bfc_node_decode_xml(bfc_objptr_t node,
			bfc_cobjptr_t s, size_t offs, int flags);
#define BFC_DECODE_INSPACE	0x0100
#define BFC_PRESERVE_WHITESPACE	0x0200

struct bfc_node_xml_parse_state;
int bfc_node_parse_xmltags(bfc_objptr_t rootnode, bfc_cobjptr_t s,
			   struct bfc_node_xml_parse_state *st);


#ifdef __cplusplus
};
#endif
#endif /* _BFC_TREENODE_H_ */
