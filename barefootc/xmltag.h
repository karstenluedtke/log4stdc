/**
 * @file      xmltag.h
 *
 * @brief     An iterator for XML tags.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-11-09
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_XMLTAG_H_
#define _BFC_XMLTAG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

#define BFC_XML_LOGGER		"barefootc.xml",13

struct bfc_xmltag_iterator_class;
typedef const struct bfc_xmltag_iterator_class *bfc_xmltag_classptr_t;

#define BFC_XMLTAGHDR(classptrT) \
	BFC_ITERATORHDR(classptrT, bfc_strptr_t)			\
	unsigned short	tagtype;					\
	unsigned short	level;						\
	unsigned short	nameoffs;					\
	unsigned short	namelen;					\
	unsigned	length;

struct bfc_xmltag {
	BFC_XMLTAGHDR(bfc_xmltag_classptr_t)
};

typedef struct bfc_xmltag bfc_xmltag_t;
typedef struct bfc_xmltag *bfc_tagptr_t;
typedef const struct bfc_xmltag *bfc_ctagptr_t;

/* tag type */
#define	BFC_XML_HEADER_TAG	0
#define	BFC_XML_START_TAG	1
#define	BFC_XML_END_TAG  	2
#define	BFC_XML_EMPTY_TAG	3
#define	BFC_XML_COMMENT_TAG	4

#define BFC_XMLTAG_METHODS(tagptrT,ctagptrT)				\
	int (*find_nexttag)(bfc_ctagptr_t, bfc_tagptr_t, size_t);	\
	int (*find_endtag)(bfc_ctagptr_t, bfc_tagptr_t, size_t);	\
	int (*get_name)(bfc_ctagptr_t, bfc_strptr_t, size_t);		\
	int (*get_attrs)(bfc_ctagptr_t, bfc_strptr_t, size_t);		\
	int (*get_namespace_prefix)(bfc_ctagptr_t,bfc_strptr_t,size_t);

#define BFC_XMLTAG_CLASS_DEF(classptrT,tagptrT,ctagptrT)		\
	BFC_CONTAINER_CLASSHDR(classptrT,tagptrT,ctagptrT,void)		\
	BFC_ITERATOR_METHODS(tagptrT,ctagptrT,bfc_strptr_t,bfc_cstrptr_t,void)\
	BFC_XMLTAG_METHODS(tagptrT, ctagptrT)				\
	void	(*last_method)(void);

struct bfc_xmltag_iterator_class {
	BFC_XMLTAG_CLASS_DEF(bfc_xmltag_classptr_t,bfc_tagptr_t,bfc_ctagptr_t)
};

int bfc_init_xmltag(void *buf,size_t bufsize, bfc_cstrptr_t doc,size_t offset);
int bfc_find_next_xmltag(bfc_ctagptr_t tag, bfc_tagptr_t next, size_t bufsize);
int bfc_find_xml_endtag(bfc_ctagptr_t starttag,
				bfc_tagptr_t endtag, size_t bufsize);
int bfc_tag_get_name(bfc_ctagptr_t tag, bfc_strptr_t buf, size_t bufsize);
int bfc_tag_get_attrs(bfc_ctagptr_t tag, bfc_strptr_t buf, size_t bufsize);
int bfc_tag_get_namespace_prefix(bfc_ctagptr_t tag,
					bfc_strptr_t buf, size_t bufsize);

struct bfc_tag_parse_state {
	void *userdata;
	int (*on_content)  (struct bfc_tag_parse_state *state,
			    bfc_ctagptr_t starttag, bfc_cstrptr_t content);
	int (*on_start_tag)(struct bfc_tag_parse_state *state,
			    bfc_ctagptr_t starttag, bfc_cstrptr_t name,
			    int numattrs, bfc_string_t attributes[]);
	int (*on_end_tag)  (struct bfc_tag_parse_state *state,
			    bfc_ctagptr_t starttag, bfc_ctagptr_t endtag,
			    bfc_cstrptr_t name);
	int (*on_empty_tag)(struct bfc_tag_parse_state *state,
			    bfc_ctagptr_t starttag, bfc_cstrptr_t name,
			    int numattrs, bfc_string_t attributes[]);
	unsigned	maxlevels;
	bfc_xmltag_t	prev;
	bfc_xmltag_t	curr;
	bfc_xmltag_t	tags[16 /* or more */];
};

int bfc_parse_xmltags(bfc_cstrptr_t s, struct bfc_tag_parse_state *st);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_XMLTAG_H_ */
