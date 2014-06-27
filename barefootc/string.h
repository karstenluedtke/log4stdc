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
		BFC_CHAR_TRAITS_METHODS(char, int)			\
	} *	traits;

#define BFC_STRING_METHODS(strptrT,cstrptrT,charT,iterptrT) \
	size_t	(*size)(cstrptrT s);		/* == length */		\
	size_t	(*max_size)(cstrptrT s);				\
	int	(*resize)(strptrT s, size_t n, charT c);		\
	size_t	(*capacity)(cstrptrT s);				\
	int	(*reserve)(strptrT s, size_t n);			\
	charT	(*at)(cstrptrT s, size_t pos);				\
	charT *	(*ref)(strptrT s, size_t pos);				\
	const charT* (*data)(cstrptrT s);  /* not zero terminated */	\
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
	size_t	(*find_bfstr)(strptrT s, cstrptrT str, size_t pos);	\
	size_t	(*find_buffer)(strptrT s, const charT* s2,		\
				size_t pos, size_t n);			\
	size_t	(*find_c_str)(strptrT s, const charT* s2, size_t pos);	\
	size_t	(*find_char)(strptrT s, charT c, size_t pos);		\
	size_t	(*rfind_bfstr)(strptrT s, cstrptrT str, size_t pos);	\
	size_t	(*rfind_buffer)(strptrT s, const charT* s2,		\
				size_t pos, size_t n);			\
	size_t	(*rfind_c_str)(strptrT s, const charT* s2, size_t pos);	\
	size_t	(*rfind_char)(strptrT s, charT c, size_t pos);		\
	size_t	(*find_first_of_bfstr)(strptrT s, cstrptrT str,		\
				size_t pos);				\
	size_t	(*find_first_of_buffer)(strptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_first_of_c_str)(strptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_first_of_char)(strptrT s, charT c, size_t pos);	\
	size_t	(*find_last_of_bfstr)(strptrT s, cstrptrT str,		\
				size_t pos);				\
	size_t	(*find_last_of_buffer)(strptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_last_of_c_str)(strptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_last_of_char)(strptrT s, charT c, size_t pos);	\
	size_t	(*find_first_not_of_bfstr)(strptrT s, cstrptrT str,	\
				size_t pos);				\
	size_t	(*find_first_not_of_buffer)(strptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_first_not_of_c_str)(strptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_first_not_of_char)(strptrT s, charT c,		\
				size_t pos);				\
	size_t	(*find_last_not_of_bfstr)(strptrT s, cstrptrT str,	\
				size_t pos);				\
	size_t	(*find_last_not_of_buffer)(strptrT s, const charT* s2,	\
				size_t pos, size_t n);			\
	size_t	(*find_last_not_of_c_str)(strptrT s, const charT* s2,	\
				size_t pos);				\
	size_t	(*find_last_not_of_char)(strptrT s, charT c,		\
				size_t pos);				\
	strptrT	(*substr)(strptrT s, size_t pos, size_t n, strptrT buf);\
	int	(*compare_bfstr)(strptrT s, cstrptrT str);		\
	int	(*compare_substr)(strptrT s, size_t pos1, size_t n1,	\
				cstrptrT str);				\
	int	(*compare_substrs)(strptrT s, size_t pos1, size_t n1,	\
				cstrptrT str, size_t pos2, size_t n2);	\
	int	(*compare_c_str)(strptrT s, const charT* s2);		\
	int	(*compare_substr_c_str)(strptrT s, size_t pos1,		\
				size_t n1, const charT* s2);		\
	int	(*compare_buffer)(strptrT s, size_t pos1, size_t n1,	\
				const charT* s2, size_t n2);

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

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_STRING_H_ */
