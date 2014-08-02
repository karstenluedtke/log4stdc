/**
 * @file      iterator.h
 *
 * @brief     A generic iterator implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-06-26
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

struct bfc_iterator_class;
typedef const struct bfc_iterator_class *bfc_iterator_classptr_t;

#define BFC_ITERATORHDR(classptrT,objectT) \
	classptrT	vptr;	  /**< virtual methods */	\
	objectT *	obj;					\
	size_t 		pos;

struct bfc_iterator {
	BFC_ITERATORHDR(bfc_iterator_classptr_t, bfc_objptr_t)
};

typedef struct bfc_iterator bfc_iterator_t;
typedef struct bfc_iterator *bfc_iterptr_t;

#define BFC_ITERATOR_METHODS(iterptrT,objptrT,cobjptrT,elemT) \
	/* Additional allocators */					\
	int	(*init_pos)(void *buf, size_t bufsize,			\
				struct mempool *pool,			\
				objptrT obj, size_t pos);		\
	int	(*init_iter)(void *buf, size_t bufsize,			\
				struct mempool *pool, iterptrT it);	\
	int	(*init_move)(void *buf, size_t bufsize,			\
				struct mempool *pool, iterptrT it);	\
	/* Iterator functions */					\
	int	(*advance)(iterptrT it, ptrdiff_t n);			\
	ptrdiff_t (*distance)(iterptrT first, iterptrT last);		\
	/* Check nothing is missing */					\
	void	(*last_method)(void);

#define BFC_ITERATOR_CLASS_DEF(classptrT,iterptrT,objptrT,cobjptrT,elemT) \
	BFC_CONTAINER_CLASSHDR(classptrT, iterptrT, iterptrT, elemT)	\
	BFC_ITERATOR_METHODS(iterptrT, objptrT, cobjptrT, elemT)

struct bfc_iterator_class {
	BFC_ITERATOR_CLASS_DEF(bfc_iterator_classptr_t,
		bfc_iterptr_t, bfc_objptr_t, bfc_cobjptr_t, void)
};

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_ITERATOR_H_ */
