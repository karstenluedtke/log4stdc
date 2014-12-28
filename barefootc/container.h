/**
 * @file      container.h
 *
 * @brief     A generic representation of containers.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-12-27
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_CONTAINER_H_
#define _BFC_CONTAINER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"

#define BFC_CONTAINER_LOGGER	"barefootc.container",19

struct bfc_container_class;
typedef const struct bfc_container_class *bfc_container_classptr_t;

struct bfc_container;
typedef struct bfc_container *bfc_contptr_t;
typedef const struct bfc_container *bfc_ccontptr_t;

#define BFC_CONTAINER_METHODS(contptrT,ccontptrT,elemT)			\
	/* Capacity */							\
	size_t	(*size)(ccontptrT c);					\
	size_t	(*max_size)(ccontptrT c);				\
	int	(*resize)(contptrT c, size_t n, const elemT *p);	\
	size_t	(*capacity)(ccontptrT c);				\
	int	(*reserve)(contptrT c, size_t n);			\
	/* Modifiers */							\
	int	(*assign_fill)(contptrT c, size_t n, const elemT *p);	\
	int	(*assign_range)(contptrT c,				\
				bfc_iterptr_t first,bfc_iterptr_t last);\
	int	(*push_front)(contptrT c, const elemT *p);		\
	void	(*pop_front)(contptrT c);				\
	int	(*push_back)(contptrT c, const elemT *p);		\
	void	(*pop_back)(contptrT c);				\
	int	(*insert_element)(contptrT c, bfc_iterptr_t position,	\
				const elemT *p);			\
	int	(*insert_fill)(contptrT c, bfc_iterptr_t position,	\
				size_t n, const elemT *p);		\
	int	(*insert_range)(contptrT c, bfc_iterptr_t position,	\
				bfc_iterptr_t first,bfc_iterptr_t last);\
	int	(*erase_element)(contptrT c, bfc_iterptr_t position);	\
	int	(*erase_range)(contptrT c,				\
				bfc_iterptr_t first,bfc_iterptr_t last);\
	size_t	(*copy_out)(ccontptrT c,elemT* buf,size_t n,size_t pos);\
	void	(*swap)(contptrT c, contptrT other);			\
	/* Check nothing is missing */					\
	void	(*last_method)(void);

#define BFC_CONTAINER_CLASS_DEF(classptrT,contptrT,ccontptrT,elemT) \
	BFC_CONTAINER_CLASSHDR(classptrT, contptrT,ccontptrT,elemT)	\
	BFC_CONTAINER_METHODS(contptrT, ccontptrT, elemT)

struct bfc_container_class {
	BFC_CONTAINER_CLASS_DEF(bfc_container_classptr_t,
				bfc_contptr_t, bfc_ccontptr_t, void)
};

void bfc_destroy_container(bfc_contptr_t date);
int  bfc_clone_container(bfc_ccontptr_t obj, void *buf, size_t bufsize);

size_t bfc_container_max_size(bfc_ccontptr_t c);
size_t bfc_container_capacity(bfc_ccontptr_t c);
int  bfc_container_reserve(bfc_contptr_t c, size_t n);
int  bfc_container_resize(bfc_contptr_t c, size_t n, const void *p);

int  bfc_container_assign_fill(bfc_contptr_t c, size_t n, const void *p);
int  bfc_container_assign_range(bfc_contptr_t c,
				bfc_iterptr_t first,bfc_iterptr_t last);
int  bfc_container_push_front(bfc_contptr_t c, const void *p);
void bfc_container_pop_front(bfc_contptr_t c);
int  bfc_container_push_back(bfc_contptr_t c, const void *p);
void bfc_container_pop_back(bfc_contptr_t c);
int  bfc_container_insert_element(bfc_contptr_t c, bfc_iterptr_t position,
				const void *p);
int  bfc_container_insert_fill(bfc_contptr_t c, bfc_iterptr_t position,
				size_t n, const void *p);
int  bfc_container_insert_range(bfc_contptr_t c, bfc_iterptr_t position,
				bfc_iterptr_t first,bfc_iterptr_t last);
int  bfc_container_erase_element(bfc_contptr_t c, bfc_iterptr_t position);
int  bfc_container_erase_range(bfc_contptr_t c,
				bfc_iterptr_t first,bfc_iterptr_t last);
size_t bfc_container_copy_out(bfc_ccontptr_t c,void* buf,size_t n,size_t pos);
void bfc_container_swap(bfc_contptr_t c, bfc_contptr_t other);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_CONTAINER_H_ */