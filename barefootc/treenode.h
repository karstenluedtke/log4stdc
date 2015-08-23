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
	bfc_contptr_t attributes;
};
/** @} */

int bfc_init_treenode(void *buf, size_t bufsize, struct mempool *pool);
int bfc_node_set_name(bfc_nodeptr_t node, bfc_cstrptr_t name);

bfc_nodeptr_t bfc_node_new_element(bfc_cnodeptr_t ancestor,
				   const char *chldtypes, ...);
bfc_nodeptr_t bfc_node_new_integer_element(bfc_cnodeptr_t ancestor,
				   const char *name, long val);
bfc_nodeptr_t bfc_node_new_boolean_element(bfc_cnodeptr_t ancestor,
				   const char *name, int val);

int bfc_node_set_xml_attribute(bfc_nodeptr_t node,
			       bfc_cstrptr_t name, bfc_cstrptr_t val);
bfc_cstrptr_t bfc_node_get_attribute_string(bfc_nodeptr_t node,
					    bfc_cstrptr_t name);

int bfc_node_encode_xml(bfc_cnodeptr_t node,
			char *buf, size_t bufsize, int level);

#ifdef __cplusplus
};
#endif
#endif /* _BFC_TREENODE_H_ */
