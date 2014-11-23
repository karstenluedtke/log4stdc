/**
 * @file      string.h
 *
 * @brief     A generic string implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-06-26
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_STRING_H_
#define _BFC_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"

struct mempool;
struct bfc_mutex;
struct l4sc_logger;

struct bfc_string_class;
typedef const struct bfc_string_class *bfc_string_classptr_t;

extern struct bfc_string_class bfc_string_class;
extern struct bfc_string_class bfc_basic_string_class;
extern struct bfc_string_class bfc_shared_string_class;

extern struct bfc_string_class bfc_wstring_class;
extern struct bfc_string_class bfc_basic_wstring_class;
extern struct bfc_string_class bfc_shared_wstring_class;

#define BFC_STRINGHDR(classptrT,charT) \
	classptrT	vptr;	  /**< virtual methods */	\
	charT *		buf;					\
	size_t 		len;					\
	unsigned 	offs;					\
	unsigned 	bufsize;				\
	struct mempool *pool;

struct bfc_string {
	BFC_STRINGHDR(bfc_string_classptr_t, void)
};

typedef struct bfc_string bfc_string_t;
typedef struct bfc_string *bfc_strptr_t;
typedef const struct bfc_string *bfc_cstrptr_t;

#define BFCSTR(s) \
 {&bfc_shared_string_class, s, sizeof(s)-1, 0, (unsigned)(sizeof(s)-1), NULL, }

#define BFCWSTR(s) \
 {&bfc_shared_wstring_class, s, (sizeof(s)-1)/sizeof(wchar_t), 0, \
		     (unsigned)((sizeof(s)-1)/sizeof(wchar_t)), NULL, }

#define BFC_STRING_LOGGER	"barefootc.string",16

#define BFC_CHAR_TRAITS_METHODS(charT,intT) \
	int	(*eq)(charT c, charT d);	/* c == d */		\
	int	(*lt)(charT c, charT d);	/* c <  d */		\
	size_t	(*szlen)(const charT *s);	/* strlen, wcslen */	\
	charT	(*assign)(charT *p, size_t n, charT c); /* memset */	\
	int	(*compare)(const charT *p, const charT *q, size_t n);	\
	const charT* (*find)(const charT *p, size_t n, charT c);	\
	charT *	(*move)(charT *dest, const charT *src, size_t n);	\
	charT *	(*copy)(charT *dest, const charT *src, size_t n);	\
	intT	(*eof)(void);			/* EOF, WEOF */		\
	intT	(*not_eof)(intT c);					\
	charT	(*to_char)(intT c);					\
	intT	(*to_int)(charT c);					\
	int	(*eq_int)(intT x, intT y);

#define BFC_STRING_CLASS_DATA(charT) \
	const struct {							\
		BFC_CLASSHDR(struct bfc_classhdr*, void*, const void*)	\
		BFC_CHAR_TRAITS_METHODS(charT, int)			\
	} *	traits;

#define BFC_STRING_METHODS(strptrT,cstrptrT,charT,fillT,bfc_iterptr_t) \
	/* Capacity */							\
	size_t	(*size)(cstrptrT s);		/* == length */		\
	size_t	(*max_size)(cstrptrT s);				\
	int	(*resize)(strptrT s, size_t n, fillT c);		\
	size_t	(*capacity)(cstrptrT s);				\
	int	(*reserve)(strptrT s, size_t n);			\
	/* Modifiers */							\
	int	(*assign_buffer)(strptrT s, const charT *s2, size_t n);	\
	int	(*assign_fill)(strptrT s, size_t n, fillT c);		\
	int	(*append_buffer)(strptrT s, const charT *s2, size_t n);	\
	int	(*append_fill)(strptrT s, size_t n, fillT c);		\
	int	(*push_back)(strptrT s, fillT c);			\
	void	(*pop_back)(strptrT s);					\
	int	(*replace_buffer)(strptrT s, size_t pos, size_t n1,	\
				const charT* s2, size_t n2);		\
	int	(*replace_range_buffer)(strptrT s, bfc_iterptr_t i1,	\
				bfc_iterptr_t i2, const charT* s2, size_t n);\
	int	(*replace_ranges)(strptrT s,				\
				bfc_iterptr_t i1, bfc_iterptr_t i2,	\
				bfc_iterptr_t j1, bfc_iterptr_t j2);	\
	size_t	(*copy)(cstrptrT s, charT* s2, size_t n, size_t pos);	\
	void	(*swap)(strptrT s, strptrT str);			\
	/* String operations */						\
	size_t	(*find_buffer)(cstrptrT s, const charT* s2,		\
				size_t pos, size_t n);			\
	size_t	(*find_char)(cstrptrT s, fillT c, size_t pos);		\
	size_t	(*rfind_buffer)(cstrptrT s, const charT* s2,		\
				size_t pos, size_t n);			\
	size_t	(*rfind_char)(cstrptrT s, fillT c, size_t pos);		\
	size_t	(*find_first_of_buffer)(cstrptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_first_of_char)(cstrptrT s, fillT c, size_t pos);	\
	size_t	(*find_last_of_buffer)(cstrptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_last_of_char)(cstrptrT s, fillT c, size_t pos);	\
	size_t	(*find_first_not_of_buffer)(cstrptrT s,const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_first_not_of_char)(cstrptrT s, fillT c,		\
				size_t pos);				\
	size_t	(*find_last_not_of_buffer)(cstrptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_last_not_of_char)(cstrptrT s, fillT c,		\
				size_t pos);				\
	int	(*substr)(cstrptrT s, size_t pos, size_t n,		\
				void *buf, size_t bufsize);		\
	int	(*mutable_substr)(cstrptrT s, size_t pos, size_t n,	\
				void *buf, size_t bufsize);		\
	int	(*buffered_substr)(cstrptrT s, size_t pos, size_t n,	\
				void *buf, size_t bufsize,		\
				void *databuf, size_t dbufsize);	\
	int	(*compare_buffer)(cstrptrT s, size_t pos1, size_t n1,	\
				const charT* s2, size_t n2);		\
	/* Check nothing is missing */					\
	void	(*last_method)(void);

#define BFC_STRING_CLASS_DEF(classptrT,strptrT,cstrptrT,charT) \
	BFC_CONTAINER_CLASSHDR(classptrT, strptrT, cstrptrT, charT)	\
	BFC_STRING_CLASS_DATA(charT)					\
	BFC_STRING_METHODS(strptrT,cstrptrT,charT,int,bfc_iterptr_t)

#ifdef NEED_BFC_STRING_CLASS
struct bfc_string_class {
	BFC_STRING_CLASS_DEF(bfc_string_classptr_t,
			     bfc_strptr_t, bfc_cstrptr_t, char)
};
#endif

#define BFC_STRING_INIT_PROLOGUE(classptrT,objptrT,obj,buf,size,pool,cls)\
	bfc_classptr_t super = (bfc_classptr_t) (cls)->super;		\
	objptrT obj = (objptrT) (buf);					\
	if (size < sizeof(*obj)) {					\
		return (-ENOSPC);					\
	} else {							\
		memset(obj, 0, sizeof(*obj));				\
		if (super) {						\
			bfc_init_object(super, obj, size, pool);	\
		}							\
		obj->vptr = (cls);					\
	}

/*
 * bfc_string_t
 */
/* Constructors */
int	bfc_init_string(void *buf, size_t bufsize, struct mempool *pool);
int	bfc_init_string_copy(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str);
int	bfc_init_string_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_strptr_t str);
int	bfc_init_string_substr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str, size_t pos, size_t n);
int	bfc_init_string_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const char* s, size_t n);
int	bfc_init_string_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const char* s);
int	bfc_init_string_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, int c);
int	bfc_init_string_range(void *buf, size_t bufsize, struct mempool *pool,
				bfc_iterptr_t begin, bfc_iterptr_t end);
void	bfc_destroy_string(bfc_strptr_t obj);
size_t	bfc_string_objsize(bfc_cstrptr_t obj);

/* Capacity */
size_t	bfc_strlen(bfc_cstrptr_t s);
size_t	bfc_string_length(bfc_cstrptr_t s);
size_t	bfc_string_max_size(bfc_cstrptr_t s);
int	bfc_string_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_string_capacity(bfc_cstrptr_t s);
int	bfc_string_reserve(bfc_strptr_t s, size_t n);
size_t	bfc_string_sublen(bfc_cstrptr_t s, size_t pos, size_t n);

/* Element access */
const char *bfc_strdata(bfc_cstrptr_t s);
const char *bfc_string_subdata(bfc_cstrptr_t s, size_t pos);
char *bfc_string_index(bfc_strptr_t s, size_t pos);
const char *bfc_string_data(bfc_cstrptr_t s);  /* not zero terminated */
long bfc_string_get_char(bfc_cstrptr_t s, size_t pos);
int bfc_string_set_char(bfc_strptr_t s, size_t pos, long c);

/* Iterators */
int bfc_string_begin_iterator(bfc_cstrptr_t s,bfc_iterptr_t it,size_t bufsize);
int bfc_string_end_iterator(bfc_cstrptr_t s, bfc_iterptr_t it, size_t bufsize);
int bfc_string_reverse_begin_iterator(bfc_cstrptr_t s,
					bfc_iterptr_t it, size_t bufsize);
int bfc_string_reverse_end_iterator(bfc_cstrptr_t s,
					bfc_iterptr_t it, size_t bufsize);

/* Modifiers */
int     bfc_string_assign(bfc_strptr_t s, bfc_cstrptr_t s2);
int     bfc_string_assign_substr(bfc_strptr_t s, bfc_cstrptr_t s2,
			 		size_t subpos, size_t sublen);
int     bfc_string_assign_c_str(bfc_strptr_t s, const void *s2);
int     bfc_string_assign_buffer(bfc_strptr_t s, const void *s2, size_t n);
int     bfc_string_assign_fill(bfc_strptr_t s, size_t n, int c);
int     bfc_string_assign_range(bfc_strptr_t s,
				bfc_iterptr_t first, bfc_iterptr_t last);
int     bfc_string_append(bfc_strptr_t s, bfc_cstrptr_t s2);
int     bfc_string_append_substr(bfc_strptr_t s, bfc_cstrptr_t s2,
			 		size_t subpos, size_t sublen);
int     bfc_string_append_c_str(bfc_strptr_t s, const void *s2);
int     bfc_string_append_buffer(bfc_strptr_t s, const void *s2, size_t n);
int     bfc_string_append_fill(bfc_strptr_t s, size_t n, int c);
int     bfc_string_append_range(bfc_strptr_t s,
				bfc_iterptr_t first, bfc_iterptr_t last);
int     bfc_string_append_iter_range(bfc_strptr_t s, bfc_iterptr_t it,
				bfc_iterptr_t first, bfc_iterptr_t last);
int	bfc_string_push_back(bfc_strptr_t s, int c);
int     bfc_string_insert(bfc_strptr_t s, size_t pos, bfc_cstrptr_t s2);
int     bfc_string_insert_substr(bfc_strptr_t s, size_t pos,
			bfc_cstrptr_t s2, size_t subpos, size_t sublen);
int     bfc_string_insert_c_str(bfc_strptr_t s, size_t pos, const void *s2);
int     bfc_string_insert_buffer(bfc_strptr_t s, size_t pos,
			 		const void *s2, size_t n);
int     bfc_string_insert_fill(bfc_strptr_t s, size_t pos, size_t n, int c);
int     bfc_string_insert_fillit(bfc_strptr_t s,bfc_iterptr_t p,size_t n,int c);
int     bfc_string_insert_char(bfc_strptr_t s, bfc_iterptr_t p, int c);
int     bfc_string_insert_range(bfc_strptr_t s, bfc_iterptr_t p,
				bfc_iterptr_t first, bfc_iterptr_t last);
int     bfc_string_erase_seq(bfc_strptr_t s, size_t pos, size_t len);
int     bfc_string_erase_tail(bfc_strptr_t s, size_t pos);
int     bfc_string_erase_char(bfc_strptr_t s, bfc_iterptr_t p);
int     bfc_string_erase_range(bfc_strptr_t s,
				bfc_iterptr_t first, bfc_iterptr_t last);
void    bfc_string_pop_back(bfc_strptr_t s);
int     bfc_string_replace(bfc_strptr_t s, size_t pos1,size_t n1,
					bfc_cstrptr_t str);
int     bfc_string_replace_substr(bfc_strptr_t s,size_t pos1,size_t n1,
	 			bfc_cstrptr_t str, size_t pos2, size_t n2);
int     bfc_string_replace_buffer(bfc_strptr_t s, size_t pos,size_t n1,
					const void *s2, size_t n2);
int     bfc_string_replace_c_str(bfc_strptr_t s, size_t pos, size_t n1,
					const void *s2);
int     bfc_string_replace_fill(bfc_strptr_t s, size_t pos, size_t n1,
					size_t n2, int c);
int     bfc_string_replace_range(bfc_strptr_t s, bfc_iterptr_t i1,
					bfc_iterptr_t i2, bfc_cstrptr_t s2);
int     bfc_string_replace_range_buffer(bfc_strptr_t s, bfc_iterptr_t i1,
				bfc_iterptr_t i2, const void *s2, size_t n);
int     bfc_string_replace_range_c_str(bfc_strptr_t s, bfc_iterptr_t i1,
					bfc_iterptr_t i2, const void *s2);
int     bfc_string_replace_range_fill(bfc_strptr_t s, bfc_iterptr_t i1,
					bfc_iterptr_t i2, size_t n, int c);
int     bfc_string_replace_ranges(bfc_strptr_t s,
				  bfc_iterptr_t i1, bfc_iterptr_t i2,
				  bfc_iterptr_t j1, bfc_iterptr_t j2);
size_t	bfc_string_copy(bfc_cstrptr_t s, void *s2, size_t n, size_t pos);
void	bfc_string_swap(bfc_strptr_t s, bfc_strptr_t str);

int	bfc_string_trim(bfc_strptr_t s);

/* String operations */
size_t	bfc_string_find_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str, size_t pos);
size_t	bfc_string_find_buffer(bfc_cstrptr_t s, const void *s2,
					size_t pos, size_t n);
size_t	bfc_string_find_c_str(bfc_cstrptr_t s, const void *s2, size_t pos);
size_t	bfc_string_find_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_string_find_range(bfc_cstrptr_t s, bfc_citerptr_t pattern,
					bfc_citerptr_t pattend, size_t pos);
size_t	bfc_string_rfind_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str,size_t pos);
size_t	bfc_string_rfind_buffer(bfc_cstrptr_t s, const void *s2,
					size_t pos, size_t n);
size_t	bfc_string_rfind_c_str(bfc_cstrptr_t s, const void *s2,size_t pos);
size_t	bfc_string_rfind_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_string_rfind_range(bfc_cstrptr_t s, bfc_citerptr_t pattern,
					bfc_citerptr_t pattend, size_t pos);
size_t	bfc_string_find_first_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_first_of_buffer(bfc_cstrptr_t s, const void *s2,
					size_t pos, size_t n);
size_t	bfc_string_find_first_of_c_str(bfc_cstrptr_t s, const void *s2,
					size_t pos);
size_t	bfc_string_find_first_of_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_string_find_first_of_range(bfc_cstrptr_t s, bfc_citerptr_t pattern,
					bfc_citerptr_t pattend, size_t pos);
size_t	bfc_string_find_last_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_last_of_buffer(bfc_cstrptr_t s, const void *s2,
					size_t pos, size_t n);
size_t	bfc_string_find_last_of_c_str(bfc_cstrptr_t s, const void *s2,
					size_t pos);
size_t	bfc_string_find_last_of_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_string_find_last_of_range(bfc_cstrptr_t s, bfc_citerptr_t pattern,
					bfc_citerptr_t pattend, size_t pos);
size_t	bfc_string_find_first_not_of_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_first_not_of_buffer(bfc_cstrptr_t s,const void *s2,
					size_t pos, size_t n);
size_t	bfc_string_find_first_not_of_c_str(bfc_cstrptr_t s, const void *s2,
					size_t pos);
size_t	bfc_string_find_first_not_of_char(bfc_cstrptr_t s, int c,
					size_t pos);
size_t	bfc_string_find_first_not_of_range(bfc_cstrptr_t s,
					bfc_citerptr_t pattern,
					bfc_citerptr_t pattend, size_t pos);
size_t	bfc_string_find_last_not_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_last_not_of_buffer(bfc_cstrptr_t s, const void *s2,
					size_t pos, size_t n);
size_t	bfc_string_find_last_not_of_c_str(bfc_cstrptr_t s, const void *s2,
					size_t pos);
size_t	bfc_string_find_last_not_of_char(bfc_cstrptr_t s, int c,
					size_t pos);
size_t	bfc_string_find_last_not_of_range(bfc_cstrptr_t s,
					bfc_citerptr_t pattern,
					bfc_citerptr_t pattend, size_t pos);
int	bfc_string_shared_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);
int	bfc_string_mutable_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);
int	bfc_string_buffered_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize,
					void *databuf, size_t dbufsize);
int	bfc_string_compare_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str);
int	bfc_string_compare_substr(bfc_cstrptr_t s, size_t pos1, size_t n1,
					bfc_cstrptr_t str);
int	bfc_string_compare_substrs(bfc_cstrptr_t s, size_t pos1, size_t n1,
				 bfc_cstrptr_t str, size_t pos2, size_t n2);
int	bfc_string_compare_c_str(bfc_cstrptr_t s, const void *s2);
int	bfc_string_compare_substr_c_str(bfc_cstrptr_t s, size_t pos1,
					size_t n1, const void *s2);
int	bfc_string_compare_buffer(bfc_cstrptr_t s, size_t pos1, size_t n1,
					const void *s2, size_t n2);
int	bfc_string_compare_ranges(bfc_citerptr_t i1, bfc_citerptr_t i2,
					bfc_citerptr_t j1, bfc_citerptr_t j2);
int	bfc_string_compare_next_range(bfc_citerptr_t it, bfc_citerptr_t limit,
				bfc_citerptr_t pattern, bfc_citerptr_t pattend);

int	bfc_string_decode_html_entities(bfc_strptr_t s);

/*
 * Specific functions for char strings
 */
/* Capacity */
size_t	bfc_cstring_length(bfc_cstrptr_t s);
size_t	bfc_cstring_max_size(bfc_cstrptr_t s);
int	bfc_cstring_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_cstring_capacity(bfc_cstrptr_t s);
int	bfc_cstring_reserve(bfc_strptr_t s, size_t n);

/* Element access */
char *bfc_cstring_index(bfc_strptr_t s, size_t pos);
const char *bfc_cstring_data(bfc_cstrptr_t s);  /* not zero terminated */

/* Modifiers */
int     bfc_cstring_assign_buffer(bfc_strptr_t s, const char *s2,size_t n);
int     bfc_cstring_assign_fill(bfc_strptr_t s, size_t n, int c);
int     bfc_cstring_append_buffer(bfc_strptr_t s, const char *s2, size_t n);
int     bfc_cstring_append_fill(bfc_strptr_t s, size_t n, int c);
int	bfc_cstring_push_back(bfc_strptr_t s, int c);
void    bfc_cstring_pop_back(bfc_strptr_t s);
int     bfc_cstring_replace_buffer(bfc_strptr_t s, size_t pos,size_t n1,
					const char* s2, size_t n2);
int     bfc_cstring_replace_range_buffer(bfc_strptr_t s, bfc_iterptr_t i1,
				bfc_iterptr_t i2, const char* s2, size_t n);
int     bfc_cstring_replace_ranges(bfc_strptr_t s, bfc_iterptr_t i1,
			bfc_iterptr_t i2, bfc_iterptr_t j1, bfc_iterptr_t j2);
size_t	bfc_cstring_copy(bfc_cstrptr_t s, char* s2, size_t n, size_t pos);
void	bfc_cstring_swap(bfc_strptr_t s, bfc_strptr_t str);

/* String operations */
size_t	bfc_cstring_find_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_cstring_find_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_cstring_rfind_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_cstring_rfind_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_cstring_find_first_of_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_cstring_find_first_of_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_cstring_find_last_of_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_cstring_find_last_of_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_cstring_find_first_not_of_char(bfc_cstrptr_t s, int c,
					size_t pos);
size_t	bfc_cstring_find_first_not_of_buffer(bfc_cstrptr_t s,const char* s2,
					size_t pos, size_t n);
size_t	bfc_cstring_find_last_not_of_char(bfc_cstrptr_t s, int c,
					size_t pos);
size_t	bfc_cstring_find_last_not_of_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
int	bfc_cstring_shared_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);
int	bfc_cstring_mutable_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);
int	bfc_cstring_buffered_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize,
					void *databuf, size_t dbufsize);
int	bfc_cstring_compare_buffer(bfc_cstrptr_t s, size_t pos1, size_t n1,
					const char* s2, size_t n2);

/*
 * bfc_wstring_t
 */
/* Constructors */
int	bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool);
int	bfc_init_wstring_copy(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str);
int	bfc_init_wstring_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_strptr_t str);
int	bfc_init_wstring_substr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cstrptr_t str, size_t pos, size_t n);
int	bfc_init_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n);
int	bfc_init_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s);
int	bfc_init_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, int c);
int	bfc_init_wstring_range(void *buf, size_t bufsize, struct mempool *pool,
				bfc_iterptr_t begin, bfc_iterptr_t end);
void	bfc_destroy_wstring(bfc_strptr_t obj);
size_t	bfc_wstring_objsize(bfc_cstrptr_t obj);

/* Capacity */
size_t	bfc_wstrlen(bfc_cstrptr_t s);
size_t	bfc_wstring_length(bfc_cstrptr_t s);
size_t	bfc_wstring_max_size(bfc_cstrptr_t s);
int	bfc_wstring_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_wstring_capacity(bfc_cstrptr_t s);
int	bfc_wstring_reserve(bfc_strptr_t s, size_t n);
size_t	bfc_wstring_sublen(bfc_cstrptr_t s, size_t pos, size_t n);

/* Element access */
wchar_t *bfc_wstring_index(bfc_strptr_t s, size_t pos);
const wchar_t *bfc_wstring_data(bfc_cstrptr_t s);  /* not zero terminated */
const wchar_t *bfc_wstrdata(bfc_cstrptr_t s);
const wchar_t *bfc_wstring_subdata(bfc_cstrptr_t s, size_t pos);

/* Modifiers */
int     bfc_wstring_assign_buffer(bfc_strptr_t s, const wchar_t *s2,size_t n);
int     bfc_wstring_assign_fill(bfc_strptr_t s, size_t n, int c);
int     bfc_wstring_append_buffer(bfc_strptr_t s, const wchar_t *s2, size_t n);
int     bfc_wstring_append_fill(bfc_strptr_t s, size_t n, int c);
int	bfc_wstring_push_back(bfc_strptr_t s, int c);
void    bfc_wstring_pop_back(bfc_strptr_t s);
int     bfc_wstring_replace_buffer(bfc_strptr_t s, size_t pos,size_t n1,
					const wchar_t* s2, size_t n2);
int     bfc_wstring_replace_range_buffer(bfc_strptr_t s, bfc_iterptr_t i1,
				bfc_iterptr_t i2, const wchar_t* s2, size_t n);
int     bfc_wstring_replace_ranges(bfc_strptr_t s, bfc_iterptr_t i1,
			bfc_iterptr_t i2, bfc_iterptr_t j1, bfc_iterptr_t j2);
size_t	bfc_wstring_copy(bfc_cstrptr_t s, wchar_t* s2, size_t n, size_t pos);
void	bfc_wstring_swap(bfc_strptr_t s, bfc_strptr_t str);

/* String operations */
size_t	bfc_wstring_find_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_wstring_find_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_rfind_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_wstring_rfind_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_first_of_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_wstring_find_first_of_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_last_of_char(bfc_cstrptr_t s, int c, size_t pos);
size_t	bfc_wstring_find_last_of_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_first_not_of_char(bfc_cstrptr_t s, int c,
					size_t pos);
size_t	bfc_wstring_find_first_not_of_buffer(bfc_cstrptr_t s,const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_last_not_of_char(bfc_cstrptr_t s, int c,
					size_t pos);
size_t	bfc_wstring_find_last_not_of_buffer(bfc_cstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
int	bfc_wstring_shared_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);
int	bfc_wstring_mutable_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize);
int	bfc_wstring_buffered_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					void *buf, size_t bufsize,
					void *databuf, size_t dbufsize);
int	bfc_wstring_compare_buffer(bfc_cstrptr_t s, size_t pos1, size_t n1,
					const wchar_t* s2, size_t n2);

/*
 * bfc_basic_string_t
 */
/* Constructors */
int	bfc_init_basic_string(void *buf, size_t bufsize, struct mempool *pool);
int	bfc_init_basic_string_copy(void *buf, size_t bufsize,
		struct mempool *pool, bfc_cstrptr_t str);
int	bfc_init_basic_string_move(void *buf, size_t bufsize,
		struct mempool *pool, bfc_strptr_t str);
int	bfc_init_basic_string_substr(void *buf, size_t bufsize,
		struct mempool *pool, bfc_cstrptr_t str, size_t pos, size_t n);
int	bfc_init_basic_string_buffer(void *buf, size_t bufsize,
		struct mempool *pool, const char* s, size_t n);
int	bfc_init_basic_string_c_str(void *buf, size_t bufsize,
		struct mempool *pool, const char* s);
int	bfc_init_basic_string_fill(void *buf, size_t bufsize,
		struct mempool *pool, size_t n, int c);
int	bfc_init_basic_string_range(void *buf, size_t bufsize,
		struct mempool *pool, bfc_iterptr_t begin, bfc_iterptr_t end);
void	bfc_destroy_basic_string(bfc_strptr_t obj);
size_t	bfc_basic_string_objsize(bfc_cstrptr_t obj);

/* Capacity */
size_t	bfc_basic_string_length(bfc_cstrptr_t s);
size_t	bfc_basic_string_max_size(bfc_cstrptr_t s);
int	bfc_basic_string_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_basic_string_capacity(bfc_cstrptr_t s);
int	bfc_basic_string_reserve(bfc_strptr_t s, size_t n);

/* Modifiers */
int     bfc_basic_string_replace_ranges(bfc_strptr_t s,
				  bfc_iterptr_t i1, bfc_iterptr_t i2,
				  bfc_iterptr_t j1, bfc_iterptr_t j2);

/*
 * bfc_basic_wstring_t
 */
/* Constructors */
int	bfc_init_basic_wstring(void *buf, size_t bufsize, struct mempool *pool);
int	bfc_init_basic_wstring_copy(void *buf, size_t bufsize,
		struct mempool *pool, bfc_cstrptr_t str);
int	bfc_init_basic_wstring_move(void *buf, size_t bufsize,
		struct mempool *pool, bfc_strptr_t str);
int	bfc_init_basic_wstring_substr(void *buf, size_t bufsize,
		struct mempool *pool, bfc_cstrptr_t str, size_t pos, size_t n);
int	bfc_init_basic_wstring_buffer(void *buf, size_t bufsize,
		struct mempool *pool, const wchar_t* s, size_t n);
int	bfc_init_basic_wstring_c_str(void *buf, size_t bufsize,
		struct mempool *pool, const wchar_t* s);
int	bfc_init_basic_wstring_fill(void *buf, size_t bufsize,
		struct mempool *pool, size_t n, int c);
int	bfc_init_basic_wstring_range(void *buf, size_t bufsize,
		struct mempool *pool, bfc_iterptr_t begin, bfc_iterptr_t end);
void	bfc_destroy_basic_wstring(bfc_strptr_t obj);
size_t	bfc_basic_wstring_objsize(bfc_cstrptr_t obj);

/* Capacity */
size_t	bfc_basic_wstring_length(bfc_cstrptr_t s);
size_t	bfc_basic_wstring_max_size(bfc_cstrptr_t s);
int	bfc_basic_wstring_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_basic_wstring_capacity(bfc_cstrptr_t s);
int	bfc_basic_wstring_reserve(bfc_strptr_t s, size_t n);

/*
 * bfc_shared_string_t
 */
/* Constructors */
int	bfc_init_shared_string_substr(void *buf, size_t bufsize,
				      bfc_cstrptr_t str, size_t pos, size_t n);
int	bfc_init_shared_string_buffer(void *buf, size_t bufsize,
				      const char* s, size_t n);
int	bfc_init_shared_string_c_str(void *buf, size_t bufsize, const char* s);
int	bfc_init_shared_string_range(void *buf, size_t bufsize,
				      bfc_iterptr_t begin, bfc_iterptr_t end);
void	bfc_destroy_shared_string(bfc_strptr_t obj);
size_t	bfc_shared_string_objsize(bfc_cstrptr_t obj);

/* Capacity */
int	bfc_shared_string_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_shared_string_capacity(bfc_cstrptr_t s);
int	bfc_shared_string_reserve(bfc_strptr_t s, size_t n);

/*
 * bfc_shared_wstring_t
 */
/* Constructors */
int	bfc_init_shared_wstring_substr(void *buf, size_t bufsize,
				      bfc_cstrptr_t str, size_t pos, size_t n);
int	bfc_init_shared_wstring_buffer(void *buf, size_t bufsize,
				      const wchar_t* s, size_t n);
int	bfc_init_shared_wstring_c_str(void *buf, size_t bufsize,
				      const wchar_t* s);
int	bfc_init_shared_wstring_range(void *buf, size_t bufsize,
				      bfc_iterptr_t begin, bfc_iterptr_t end);
void	bfc_destroy_shared_wstring(bfc_strptr_t obj);
size_t	bfc_shared_wstring_objsize(bfc_cstrptr_t obj);

/* Capacity */
int	bfc_shared_wstring_resize(bfc_strptr_t s, size_t n, int c);
size_t	bfc_shared_wstring_capacity(bfc_cstrptr_t s);
int	bfc_shared_wstring_reserve(bfc_strptr_t s, size_t n);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_STRING_H_ */
