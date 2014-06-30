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

struct mempool;
struct bfc_mutex;
struct l4sc_logger;

struct bfc_string_class;
typedef const struct bfc_string_class *bfc_string_classptr_t;

#define BFC_STRINGHDR(classptrT,charT) \
	classptrT	vptr;	  /**< virtual methods */	\
	charT *		buf;					\
	size_t 		len;					\
	unsigned 	offs;					\
	unsigned 	bufsize;

struct bfc_string {
	BFC_STRINGHDR(bfc_string_classptr_t, char)
};

typedef struct bfc_string bfc_string_t;
typedef struct bfc_string *bfc_strptr_t;
typedef const struct bfc_string *bfc_cstrptr_t;

struct bfc_wstring {
	BFC_STRINGHDR(bfc_string_classptr_t, wchar_t)
};

typedef struct bfc_wstring bfc_wstring_t;
typedef struct bfc_wstring *bfc_wstrptr_t;
typedef const struct bfc_wstring *bfc_cwstrptr_t;

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

#define BFC_STRING_METHODS(strptrT,cstrptrT,charT,iterptrT) \
	/* Additional allocators */					\
	int	(*init_bfstr)(void *buf, size_t bufsize,		\
				struct mempool *pool, cstrptrT str);	\
	int	(*init_move)(void *buf, size_t bufsize,			\
				struct mempool *pool, strptrT str);	\
	int	(*init_substr)(void *buf, size_t bufsize,		\
				struct mempool *pool,			\
				cstrptrT str, size_t pos, size_t n);	\
	int	(*init_buffer)(void *buf, size_t bufsize,		\
				struct mempool *pool,			\
				const charT* s, size_t n);		\
	int	(*init_c_str)(void *buf, size_t bufsize,		\
				struct mempool *pool, const charT* s);	\
	int	(*init_fill)(void *buf, size_t bufsize,			\
				struct mempool *pool,size_t n,charT c);	\
	int	(*init_range)(void *buf, size_t bufsize,		\
				struct mempool *pool,			\
				iterptrT begin, iterptrT end);		\
	/* Capacity */							\
	size_t	(*size)(cstrptrT s);		/* == length */		\
	size_t	(*max_size)(cstrptrT s);				\
	int	(*resize)(strptrT s, size_t n, charT c);		\
	size_t	(*capacity)(cstrptrT s);				\
	int	(*reserve)(strptrT s, size_t n);			\
	/* Element access */						\
	charT	(*at)(cstrptrT s, size_t pos);				\
	charT *	(*ref)(strptrT s, size_t pos);				\
	const charT* (*data)(cstrptrT s);  /* not zero terminated */	\
	/* Modifiers */							\
	strptrT	(*assign_bfstr)(strptrT s, cstrptrT s2);		\
	strptrT	(*assign_substr)(strptrT s, cstrptrT s2,		\
				 size_t subpos, size_t sublen);		\
	strptrT	(*assign_c_str)(strptrT s, const charT *s2);		\
	strptrT	(*assign_buffer)(strptrT s, const charT *s2, size_t n);	\
	strptrT	(*assign_fill)(strptrT s, size_t n, charT c);		\
	strptrT	(*assign_range)(strptrT s,iterptrT first,iterptrT last);\
	strptrT	(*append_bfstr)(strptrT s, cstrptrT s2);		\
	strptrT	(*append_substr)(strptrT s, cstrptrT s2,		\
				 size_t subpos, size_t sublen);		\
	strptrT	(*append_c_str)(strptrT s, const charT *s2);		\
	strptrT	(*append_buffer)(strptrT s, const charT *s2, size_t n);	\
	strptrT	(*append_fill)(strptrT s, size_t n, charT c);		\
	strptrT	(*append_range)(strptrT s,iterptrT first,iterptrT last);\
	int	(*push_back)(strptrT s, charT c);			\
	strptrT	(*insert_bfstr)(strptrT s, size_t pos, cstrptrT s2);	\
	strptrT	(*insert_substr)(strptrT s, size_t pos, cstrptrT s2,	\
				 size_t subpos, size_t sublen);		\
	strptrT	(*insert_c_str)(strptrT s, size_t pos, const charT *s2);\
	strptrT	(*insert_buffer)(strptrT s, size_t pos,			\
				 const charT *s2, size_t n);		\
	strptrT	(*insert_fill)(strptrT s,size_t pos, size_t n,charT c);	\
	strptrT	(*insert_fillit)(strptrT s,iterptrT p,size_t n,charT c);\
	iterptrT(*insert_char)(strptrT s, iterptrT p, charT c);		\
	strptrT	(*insert_range)(strptrT s, iterptrT p,			\
				iterptrT first, iterptrT last);		\
	strptrT	(*erase_seq)(strptrT s, size_t pos, size_t len);	\
	strptrT	(*erase_tail)(strptrT s, size_t pos);			\
	iterptrT(*erase_char)(strptrT s, iterptrT p);			\
	strptrT	(*erase_range)(strptrT s,iterptrT first,iterptrT last);	\
	void	(*pop_back)(strptrT s);					\
	strptrT (*replace_bfstr)(strptrT s, size_t pos1, size_t n1,	\
				cstrptrT str);				\
	strptrT (*replace_substr)(strptrT s, size_t pos1, size_t n1,	\
		 		cstrptrT str, size_t pos2, size_t n2);	\
	strptrT (*replace_buffer)(strptrT s, size_t pos, size_t n1,	\
				const charT* s2, size_t n2);		\
	strptrT (*replace_c_str)(strptrT s, size_t pos, size_t n1,	\
				const charT* s2);			\
	strptrT (*replace_fill)(strptrT s, size_t pos, size_t n1,	\
				size_t n2, charT c);			\
	strptrT (*replace_range_bfstr)(strptrT s, iterptrT i1,		\
				iterptrT i2, cstrptrT s2);		\
	strptrT (*replace_range_buffer)(strptrT s, iterptrT i1,		\
				iterptrT i2, const charT* s2, size_t n);\
	strptrT (*replace_range_c_str)(strptrT s, iterptrT i1,		\
				iterptrT i2, const charT* s2);		\
	strptrT (*replace_range_fill)(strptrT s, iterptrT i1,		\
				iterptrT i2, size_t n, charT c);	\
	strptrT (*replace_ranges)(strptrT s, iterptrT i1, iterptrT i2,	\
				iterptrT j1, iterptrT j2);		\
	size_t	(*copy)(strptrT s, charT* s2, size_t n, size_t pos);	\
	void	(*swap)(strptrT s, strptrT str);			\
	/* String operations */						\
	size_t	(*find_bfstr)(cstrptrT s, cstrptrT str, size_t pos);	\
	size_t	(*find_buffer)(cstrptrT s, const charT* s2,		\
				size_t pos, size_t n);			\
	size_t	(*find_c_str)(cstrptrT s, const charT* s2, size_t pos);	\
	size_t	(*find_char)(cstrptrT s, charT c, size_t pos);		\
	size_t	(*rfind_bfstr)(cstrptrT s, cstrptrT str, size_t pos);	\
	size_t	(*rfind_buffer)(cstrptrT s, const charT* s2,		\
				size_t pos, size_t n);			\
	size_t	(*rfind_c_str)(cstrptrT s, const charT* s2,size_t pos);	\
	size_t	(*rfind_char)(cstrptrT s, charT c, size_t pos);		\
	size_t	(*find_first_of_bfstr)(cstrptrT s, cstrptrT str,	\
				size_t pos);				\
	size_t	(*find_first_of_buffer)(cstrptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_first_of_c_str)(cstrptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_first_of_char)(cstrptrT s, charT c, size_t pos);	\
	size_t	(*find_last_of_bfstr)(cstrptrT s, cstrptrT str,		\
				size_t pos);				\
	size_t	(*find_last_of_buffer)(cstrptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_last_of_c_str)(cstrptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_last_of_char)(cstrptrT s, charT c, size_t pos);	\
	size_t	(*find_first_not_of_bfstr)(cstrptrT s, cstrptrT str,	\
				size_t pos);				\
	size_t	(*find_first_not_of_buffer)(cstrptrT s,const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_first_not_of_c_str)(cstrptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_first_not_of_char)(cstrptrT s, charT c,		\
				size_t pos);				\
	size_t	(*find_last_not_of_bfstr)(cstrptrT s, cstrptrT str,	\
				size_t pos);				\
	size_t	(*find_last_not_of_buffer)(cstrptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_last_not_of_c_str)(cstrptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_last_not_of_char)(cstrptrT s, charT c,		\
				size_t pos);				\
	strptrT	(*substr)(cstrptrT s, size_t pos,size_t n, strptrT buf);\
	int	(*compare_bfstr)(cstrptrT s, cstrptrT str);		\
	int	(*compare_substr)(cstrptrT s, size_t pos1, size_t n1,	\
				cstrptrT str);				\
	int	(*compare_substrs)(cstrptrT s, size_t pos1, size_t n1,	\
				cstrptrT str, size_t pos2, size_t n2);	\
	int	(*compare_c_str)(cstrptrT s, const charT* s2);		\
	int	(*compare_substr_c_str)(cstrptrT s, size_t pos1,	\
				size_t n1, const charT* s2);		\
	int	(*compare_buffer)(cstrptrT s, size_t pos1, size_t n1,	\
				const charT* s2, size_t n2);		\
	/* Check nothing is missing */					\
	void	(*last_method)(void);

#define BFC_STRING_CLASS_DEF(classptrT,strptrT,cstrptrT,charT) \
	BFC_CLASSHDR(classptrT, strptrT, cstrptrT)			\
	BFC_STRING_CLASS_DATA(charT)					\
	BFC_STRING_METHODS(strptrT,cstrptrT,charT,void *)

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
#define iterptrT void *
/* Allocators */
int	bfc_init_string(void *buf, size_t bufsize, struct mempool *pool);
int	bfc_init_string_bfstr(void *buf, size_t bufsize, struct mempool *pool,
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
				size_t n, char c);
int	bfc_init_string_range(void *buf, size_t bufsize, struct mempool *pool,
				iterptrT begin, iterptrT end);
void	bfc_destroy_string(bfc_strptr_t obj);
size_t	bfc_string_objsize(bfc_cstrptr_t obj);

/* Capacity */
size_t	bfc_string_length(bfc_cstrptr_t s);
size_t	bfc_string_max_size(bfc_cstrptr_t s);
int	bfc_string_resize(bfc_strptr_t s, size_t n, char c);
size_t	bfc_string_capacity(bfc_cstrptr_t s);
int	bfc_string_reserve(bfc_strptr_t s, size_t n);

/* Element access */
char	bfc_string_at(bfc_cstrptr_t s, size_t pos);
char *bfc_string_ref(bfc_strptr_t s, size_t pos);
const char *bfc_string_data(bfc_cstrptr_t s);  /* not zero terminated */

/* Modifiers */
bfc_strptr_t bfc_string_assign_bfstr(bfc_strptr_t s, bfc_cstrptr_t s2);
bfc_strptr_t bfc_string_assign_substr(bfc_strptr_t s, bfc_cstrptr_t s2,
			 		size_t subpos, size_t sublen);
bfc_strptr_t bfc_string_assign_c_str(bfc_strptr_t s, const char *s2);
bfc_strptr_t bfc_string_assign_buffer(bfc_strptr_t s,
					const char *s2, size_t n);
bfc_strptr_t bfc_string_assign_fill(bfc_strptr_t s, size_t n, char c);
bfc_strptr_t bfc_string_assign_range(bfc_strptr_t s,
					iterptrT first, iterptrT last);
bfc_strptr_t bfc_string_append_bfstr(bfc_strptr_t s, bfc_cstrptr_t s2);
bfc_strptr_t bfc_string_append_substr(bfc_strptr_t s, bfc_cstrptr_t s2,
			 		size_t subpos, size_t sublen);
bfc_strptr_t bfc_string_append_c_str(bfc_strptr_t s, const char *s2);
bfc_strptr_t bfc_string_append_buffer(bfc_strptr_t s,
					const char *s2, size_t n);
bfc_strptr_t bfc_string_append_fill(bfc_strptr_t s, size_t n, char c);
bfc_strptr_t bfc_string_append_range(bfc_strptr_t s,
					iterptrT first, iterptrT last);
int	bfc_string_push_back(bfc_strptr_t s, char c);
bfc_strptr_t bfc_string_insert_bfstr(bfc_strptr_t s, size_t pos,
					bfc_cstrptr_t s2);
bfc_strptr_t bfc_string_insert_substr(bfc_strptr_t s, size_t pos,
			bfc_cstrptr_t s2, size_t subpos, size_t sublen);
bfc_strptr_t bfc_string_insert_c_str(bfc_strptr_t s, size_t pos,
					const char *s2);
bfc_strptr_t bfc_string_insert_buffer(bfc_strptr_t s, size_t pos,
			 		const char *s2, size_t n);
bfc_strptr_t bfc_string_insert_fill(bfc_strptr_t s, size_t pos,
					size_t n, char c);
bfc_strptr_t bfc_string_insert_fillit(bfc_strptr_t s, iterptrT p,
					size_t n, char c);
iterptrT      bfc_string_insert_char(bfc_strptr_t s, iterptrT p, char c);
bfc_strptr_t bfc_string_insert_range(bfc_strptr_t s, iterptrT p,
					iterptrT first, iterptrT last);
bfc_strptr_t bfc_string_erase_seq(bfc_strptr_t s, size_t pos, size_t len);
bfc_strptr_t bfc_string_erase_tail(bfc_strptr_t s, size_t pos);
iterptrT      bfc_string_erase_char(bfc_strptr_t s, iterptrT p);
bfc_strptr_t bfc_string_erase_range(bfc_strptr_t s,
					iterptrT first, iterptrT last);
void          bfc_string_pop_back(bfc_strptr_t s);
bfc_strptr_t bfc_string_replace_bfstr(bfc_strptr_t s, size_t pos1,size_t n1,
					bfc_cstrptr_t str);
bfc_strptr_t bfc_string_replace_substr(bfc_strptr_t s,size_t pos1,size_t n1,
	 			bfc_cstrptr_t str, size_t pos2, size_t n2);
bfc_strptr_t bfc_string_replace_buffer(bfc_strptr_t s, size_t pos,size_t n1,
					const char* s2, size_t n2);
bfc_strptr_t bfc_string_replace_c_str(bfc_strptr_t s, size_t pos, size_t n1,
					const char* s2);
bfc_strptr_t bfc_string_replace_fill(bfc_strptr_t s, size_t pos, size_t n1,
					size_t n2, char c);
bfc_strptr_t bfc_string_replace_range_bfstr(bfc_strptr_t s, iterptrT i1,
					iterptrT i2, bfc_cstrptr_t s2);
bfc_strptr_t bfc_string_replace_range_buffer(bfc_strptr_t s, iterptrT i1,
				iterptrT i2, const char* s2, size_t n);
bfc_strptr_t bfc_string_replace_range_c_str(bfc_strptr_t s, iterptrT i1,
					iterptrT i2, const char* s2);
bfc_strptr_t bfc_string_replace_range_fill(bfc_strptr_t s, iterptrT i1,
					iterptrT i2, size_t n, char c);
bfc_strptr_t bfc_string_replace_ranges(bfc_strptr_t s, iterptrT i1,
					iterptrT i2, iterptrT j1, iterptrT j2);
size_t	bfc_string_copy(bfc_strptr_t s, char* s2, size_t n, size_t pos);
void	bfc_string_swap(bfc_strptr_t s, bfc_strptr_t str);

/* String operations */
size_t	bfc_string_find_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str, size_t pos);
size_t	bfc_string_find_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_string_find_c_str(bfc_cstrptr_t s, const char* s2, size_t pos);
size_t	bfc_string_find_char(bfc_cstrptr_t s, char c, size_t pos);
size_t	bfc_string_rfind_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str,size_t pos);
size_t	bfc_string_rfind_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_string_rfind_c_str(bfc_cstrptr_t s, const char* s2,size_t pos);
size_t	bfc_string_rfind_char(bfc_cstrptr_t s, char c, size_t pos);
size_t	bfc_string_find_first_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_first_of_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_string_find_first_of_c_str(bfc_cstrptr_t s, const char* s2,
					size_t pos);
size_t	bfc_string_find_first_of_char(bfc_cstrptr_t s, char c, size_t pos);
size_t	bfc_string_find_last_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_last_of_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_string_find_last_of_c_str(bfc_cstrptr_t s, const char* s2,
					size_t pos);
size_t	bfc_string_find_last_of_char(bfc_cstrptr_t s, char c, size_t pos);
size_t	bfc_string_find_first_not_of_bfstr(bfc_cstrptr_t s,bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_first_not_of_buffer(bfc_cstrptr_t s,const char* s2,
					size_t pos, size_t n);
size_t	bfc_string_find_first_not_of_c_str(bfc_cstrptr_t s, const char* s2,
					size_t pos);
size_t	bfc_string_find_first_not_of_char(bfc_cstrptr_t s, char c,
					size_t pos);
size_t	bfc_string_find_last_not_of_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str,
					size_t pos);
size_t	bfc_string_find_last_not_of_buffer(bfc_cstrptr_t s, const char* s2,
					size_t pos, size_t n);
size_t	bfc_string_find_last_not_of_c_str(bfc_cstrptr_t s, const char* s2,
					size_t pos);
size_t	bfc_string_find_last_not_of_char(bfc_cstrptr_t s, char c,
					size_t pos);
bfc_strptr_t bfc_string_substr(bfc_cstrptr_t s, size_t pos, size_t n,
					bfc_strptr_t buf);
int	bfc_string_compare_bfstr(bfc_cstrptr_t s, bfc_cstrptr_t str);
int	bfc_string_compare_substr(bfc_cstrptr_t s, size_t pos1, size_t n1,
					bfc_cstrptr_t str);
int	bfc_string_compare_substrs(bfc_cstrptr_t s, size_t pos1, size_t n1,
				 bfc_cstrptr_t str, size_t pos2, size_t n2);
int	bfc_string_compare_c_str(bfc_cstrptr_t s, const char* s2);
int	bfc_string_compare_substr_c_str(bfc_cstrptr_t s, size_t pos1,
					size_t n1, const char* s2);
int	bfc_string_compare_buffer(bfc_cstrptr_t s, size_t pos1, size_t n1,
					const char* s2, size_t n2);

/*
 * bfc_wstring_t
 */
/* Allocators */
int	bfc_init_wstring(void *buf, size_t bufsize, struct mempool *pool);
int	bfc_init_wstring_bfstr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str);
int	bfc_init_wstring_move(void *buf, size_t bufsize, struct mempool *pool,
				bfc_wstrptr_t str);
int	bfc_init_wstring_substr(void *buf, size_t bufsize, struct mempool *pool,
				bfc_cwstrptr_t str, size_t pos, size_t n);
int	bfc_init_wstring_buffer(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s, size_t n);
int	bfc_init_wstring_c_str(void *buf, size_t bufsize, struct mempool *pool,
				const wchar_t* s);
int	bfc_init_wstring_fill(void *buf, size_t bufsize, struct mempool *pool,
				size_t n, wchar_t c);
int	bfc_init_wstring_range(void *buf, size_t bufsize, struct mempool *pool,
				iterptrT begin, iterptrT end);
void	bfc_destroy_wstring(bfc_wstrptr_t obj);
size_t	bfc_wstring_objsize(bfc_cwstrptr_t obj);

/* Capacity */
size_t	bfc_wstring_length(bfc_cwstrptr_t s);
size_t	bfc_wstring_max_size(bfc_cwstrptr_t s);
int	bfc_wstring_resize(bfc_wstrptr_t s, size_t n, wchar_t c);
size_t	bfc_wstring_capacity(bfc_cwstrptr_t s);
int	bfc_wstring_reserve(bfc_wstrptr_t s, size_t n);

/* Element access */
wchar_t	bfc_wstring_at(bfc_cwstrptr_t s, size_t pos);
wchar_t *bfc_wstring_ref(bfc_wstrptr_t s, size_t pos);
const wchar_t *bfc_wstring_data(bfc_cwstrptr_t s);  /* not zero terminated */

/* Modifiers */
bfc_wstrptr_t bfc_wstring_assign_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2);
bfc_wstrptr_t bfc_wstring_assign_substr(bfc_wstrptr_t s, bfc_cwstrptr_t s2,
			 		size_t subpos, size_t sublen);
bfc_wstrptr_t bfc_wstring_assign_c_str(bfc_wstrptr_t s, const wchar_t *s2);
bfc_wstrptr_t bfc_wstring_assign_buffer(bfc_wstrptr_t s,
					const wchar_t *s2, size_t n);
bfc_wstrptr_t bfc_wstring_assign_fill(bfc_wstrptr_t s, size_t n, wchar_t c);
bfc_wstrptr_t bfc_wstring_assign_range(bfc_wstrptr_t s,
					iterptrT first, iterptrT last);
bfc_wstrptr_t bfc_wstring_append_bfstr(bfc_wstrptr_t s, bfc_cwstrptr_t s2);
bfc_wstrptr_t bfc_wstring_append_substr(bfc_wstrptr_t s, bfc_cwstrptr_t s2,
			 		size_t subpos, size_t sublen);
bfc_wstrptr_t bfc_wstring_append_c_str(bfc_wstrptr_t s, const wchar_t *s2);
bfc_wstrptr_t bfc_wstring_append_buffer(bfc_wstrptr_t s,
					const wchar_t *s2, size_t n);
bfc_wstrptr_t bfc_wstring_append_fill(bfc_wstrptr_t s, size_t n, wchar_t c);
bfc_wstrptr_t bfc_wstring_append_range(bfc_wstrptr_t s,
					iterptrT first, iterptrT last);
int	bfc_wstring_push_back(bfc_wstrptr_t s, wchar_t c);
bfc_wstrptr_t bfc_wstring_insert_bfstr(bfc_wstrptr_t s, size_t pos,
					bfc_cwstrptr_t s2);
bfc_wstrptr_t bfc_wstring_insert_substr(bfc_wstrptr_t s, size_t pos,
			bfc_cwstrptr_t s2, size_t subpos, size_t sublen);
bfc_wstrptr_t bfc_wstring_insert_c_str(bfc_wstrptr_t s, size_t pos,
					const wchar_t *s2);
bfc_wstrptr_t bfc_wstring_insert_buffer(bfc_wstrptr_t s, size_t pos,
			 		const wchar_t *s2, size_t n);
bfc_wstrptr_t bfc_wstring_insert_fill(bfc_wstrptr_t s, size_t pos,
					size_t n, wchar_t c);
bfc_wstrptr_t bfc_wstring_insert_fillit(bfc_wstrptr_t s, iterptrT p,
					size_t n, wchar_t c);
iterptrT      bfc_wstring_insert_char(bfc_wstrptr_t s, iterptrT p, wchar_t c);
bfc_wstrptr_t bfc_wstring_insert_range(bfc_wstrptr_t s, iterptrT p,
					iterptrT first, iterptrT last);
bfc_wstrptr_t bfc_wstring_erase_seq(bfc_wstrptr_t s, size_t pos, size_t len);
bfc_wstrptr_t bfc_wstring_erase_tail(bfc_wstrptr_t s, size_t pos);
iterptrT      bfc_wstring_erase_char(bfc_wstrptr_t s, iterptrT p);
bfc_wstrptr_t bfc_wstring_erase_range(bfc_wstrptr_t s,
					iterptrT first, iterptrT last);
void          bfc_wstring_pop_back(bfc_wstrptr_t s);
bfc_wstrptr_t bfc_wstring_replace_bfstr(bfc_wstrptr_t s, size_t pos1,size_t n1,
					bfc_cwstrptr_t str);
bfc_wstrptr_t bfc_wstring_replace_substr(bfc_wstrptr_t s,size_t pos1,size_t n1,
	 			bfc_cwstrptr_t str, size_t pos2, size_t n2);
bfc_wstrptr_t bfc_wstring_replace_buffer(bfc_wstrptr_t s, size_t pos,size_t n1,
					const wchar_t* s2, size_t n2);
bfc_wstrptr_t bfc_wstring_replace_c_str(bfc_wstrptr_t s, size_t pos, size_t n1,
					const wchar_t* s2);
bfc_wstrptr_t bfc_wstring_replace_fill(bfc_wstrptr_t s, size_t pos, size_t n1,
					size_t n2, wchar_t c);
bfc_wstrptr_t bfc_wstring_replace_range_bfstr(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, bfc_cwstrptr_t s2);
bfc_wstrptr_t bfc_wstring_replace_range_buffer(bfc_wstrptr_t s, iterptrT i1,
				iterptrT i2, const wchar_t* s2, size_t n);
bfc_wstrptr_t bfc_wstring_replace_range_c_str(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, const wchar_t* s2);
bfc_wstrptr_t bfc_wstring_replace_range_fill(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, size_t n, wchar_t c);
bfc_wstrptr_t bfc_wstring_replace_ranges(bfc_wstrptr_t s, iterptrT i1,
					iterptrT i2, iterptrT j1, iterptrT j2);
size_t	bfc_wstring_copy(bfc_wstrptr_t s, wchar_t* s2, size_t n, size_t pos);
void	bfc_wstring_swap(bfc_wstrptr_t s, bfc_wstrptr_t str);

/* String operations */
size_t	bfc_wstring_find_bfstr(bfc_cwstrptr_t s,bfc_cwstrptr_t str, size_t pos);
size_t	bfc_wstring_find_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_c_str(bfc_cwstrptr_t s, const wchar_t* s2, size_t pos);
size_t	bfc_wstring_find_char(bfc_cwstrptr_t s, wchar_t c, size_t pos);
size_t	bfc_wstring_rfind_bfstr(bfc_cwstrptr_t s,bfc_cwstrptr_t str,size_t pos);
size_t	bfc_wstring_rfind_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_rfind_c_str(bfc_cwstrptr_t s, const wchar_t* s2,size_t pos);
size_t	bfc_wstring_rfind_char(bfc_cwstrptr_t s, wchar_t c, size_t pos);
size_t	bfc_wstring_find_first_of_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str,
					size_t pos);
size_t	bfc_wstring_find_first_of_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_first_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos);
size_t	bfc_wstring_find_first_of_char(bfc_cwstrptr_t s, wchar_t c, size_t pos);
size_t	bfc_wstring_find_last_of_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str,
					size_t pos);
size_t	bfc_wstring_find_last_of_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_last_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos);
size_t	bfc_wstring_find_last_of_char(bfc_cwstrptr_t s, wchar_t c, size_t pos);
size_t	bfc_wstring_find_first_not_of_bfstr(bfc_cwstrptr_t s,bfc_cwstrptr_t str,
					size_t pos);
size_t	bfc_wstring_find_first_not_of_buffer(bfc_cwstrptr_t s,const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_first_not_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos);
size_t	bfc_wstring_find_first_not_of_char(bfc_cwstrptr_t s, wchar_t c,
					size_t pos);
size_t	bfc_wstring_find_last_not_of_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str,
					size_t pos);
size_t	bfc_wstring_find_last_not_of_buffer(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos, size_t n);
size_t	bfc_wstring_find_last_not_of_c_str(bfc_cwstrptr_t s, const wchar_t* s2,
					size_t pos);
size_t	bfc_wstring_find_last_not_of_char(bfc_cwstrptr_t s, wchar_t c,
					size_t pos);
bfc_wstrptr_t bfc_wstring_substr(bfc_cwstrptr_t s, size_t pos, size_t n,
					bfc_wstrptr_t buf);
int	bfc_wstring_compare_bfstr(bfc_cwstrptr_t s, bfc_cwstrptr_t str);
int	bfc_wstring_compare_substr(bfc_cwstrptr_t s, size_t pos1, size_t n1,
					bfc_cwstrptr_t str);
int	bfc_wstring_compare_substrs(bfc_cwstrptr_t s, size_t pos1, size_t n1,
				 bfc_cwstrptr_t str, size_t pos2, size_t n2);
int	bfc_wstring_compare_c_str(bfc_cwstrptr_t s, const wchar_t* s2);
int	bfc_wstring_compare_substr_c_str(bfc_cwstrptr_t s, size_t pos1,
					size_t n1, const wchar_t* s2);
int	bfc_wstring_compare_buffer(bfc_cwstrptr_t s, size_t pos1, size_t n1,
					const wchar_t* s2, size_t n2);
#ifdef __cplusplus
}	/* C++ */
namespace barefootc {
	int init_string(void *buf, size_t bufsize,
			struct mempool *pool, const char *s, size_t n);
	int init_string(void *buf, size_t bufsize,
			struct mempool *pool, const wchar_t *s, size_t n);
}
#endif
#endif /* _BFC_STRING_H_ */
