/**
 * @file      iterator.h
 *
 * @brief     A generic iterator implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-08-02
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_ITERATOR_H_
#define _BFC_ITERATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"

#define BFC_NPOS	((size_t) -1)

struct bfc_iterator_class;
typedef const struct bfc_iterator_class *bfc_iterator_classptr_t;

#define BFC_ITERATORHDR(classptrT,objptrT) \
	classptrT	vptr;	  /**< virtual methods */	\
	objptrT		obj;					\
	size_t 		pos;

struct bfc_iterator {
	BFC_ITERATORHDR(bfc_iterator_classptr_t, bfc_objptr_t)
};

typedef struct bfc_iterator bfc_iterator_t;
typedef struct bfc_iterator *bfc_iterptr_t;
typedef const struct bfc_iterator *bfc_citerptr_t;

#define BFC_ITERATOR_METHODS(iterptrT,citerptrT,objptrT,cobjptrT,elemT) \
	/* Additional allocators */					\
	int	(*initialize)(void *buf, size_t bufsize,		\
					cobjptrT obj, size_t pos);	\
	/* Iterator functions */					\
	int	(*advance)(iterptrT it, ptrdiff_t n);			\
	ptrdiff_t (*distance)(citerptrT first, citerptrT last);		\
	/* Check nothing is missing */					\
	void	(*last_method)(void);

#define BFC_ITERATOR_CLASS_DEF(classptrT,iterptrT,citerptrT,objptrT,cobjptrT,elemT) \
	BFC_CONTAINER_CLASSHDR(classptrT, iterptrT, citerptrT, elemT)	\
	BFC_ITERATOR_METHODS(iterptrT, citerptrT, objptrT, cobjptrT, elemT)

struct bfc_iterator_class {
	BFC_ITERATOR_CLASS_DEF(bfc_iterator_classptr_t,
				bfc_iterptr_t, bfc_citerptr_t,
				bfc_objptr_t, bfc_cobjptr_t, void)
};

int	bfc_init_iterator(void *buf, size_t bufsize,
			  bfc_cobjptr_t obj, size_t pos);
int	bfc_init_reverse_iterator(void *buf, size_t bufsize,
			  bfc_cobjptr_t obj, size_t pos);
void	bfc_destroy_iterator(bfc_iterptr_t it);
int	bfc_clone_iterator(bfc_citerptr_t obj, void *buf, size_t bufsize);
size_t	bfc_iterator_objsize(bfc_citerptr_t it);
int	bfc_iterator_equals(bfc_citerptr_t it, bfc_citerptr_t other);
size_t	bfc_iterator_length(bfc_citerptr_t it);
size_t	bfc_iterator_position(bfc_citerptr_t it);
long	bfc_iterator_getlong(bfc_citerptr_t it);
int	bfc_iterator_setlong(bfc_iterptr_t it, long val);
ptrdiff_t bfc_iterator_distance(bfc_citerptr_t first, bfc_citerptr_t limit);
int	bfc_iterator_advance(bfc_iterptr_t it, ptrdiff_t n);

int	bfc_init_cstr_iterator(void *buf, size_t bufsize,
					const char *s, size_t pos);
int	bfc_init_cstr_reverse_iterator(void *buf, size_t bufsize,
					const char *s, size_t pos);

int	bfc_init_wstr_iterator(void *buf, size_t bufsize,
					const wchar_t *s, size_t pos);
int	bfc_init_wstr_reverse_iterator(void *buf, size_t bufsize,
					const wchar_t *s, size_t pos);
#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_ITERATOR_H_ */
