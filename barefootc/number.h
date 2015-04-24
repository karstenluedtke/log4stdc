/**
 * @file      number.h
 *
 * @brief     A generic number implementation.
 *
 * @author    Karsten Luedtke
 *
 * @date      2015-04-12
 *
 * Copyright (c)  2015  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_NUMBER_H_
#define _BFC_NUMBER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"

struct bfc_number_class;
typedef const struct bfc_number_class *bfc_number_classptr_t;

extern const struct bfc_classhdr bfc_integer_class;
extern const struct bfc_classhdr bfc_natural_class;

#define BFC_NUMBERHDR(classptrT) \
	classptrT	vptr;	  /**< virtual methods */

struct bfc_number {
	BFC_NUMBERHDR(bfc_classptr_t)
	union bfc_number_un {
		ptrdiff_t	n;
		double		f;
	} 			u;
};

typedef struct bfc_number bfc_number_t;
typedef struct bfc_number *bfc_numptr_t;
typedef const struct bfc_number *bfc_cnumptr_t;

#define BFC_SIGNED_NUMBER(x)	{ &bfc_integer_class, { (ptrdiff_t)(x) } }

#define BFC_NUMBER_LOGGER	"barefootc.number",16

int  bfc_init_integer_object(void *buf, size_t bufsize, struct mempool *pool);
void bfc_number_init_refcount(bfc_objptr_t obj, int n);
void bfc_number_incr_refcount(bfc_objptr_t obj);
int  bfc_number_decr_refcount(bfc_objptr_t obj);
void bfc_number_destroy_object(bfc_objptr_t obj);
int  bfc_number_clone_object(bfc_cobjptr_t obj,
			void *buf, size_t bufsize, struct mempool *pool);
size_t bfc_get_integer_object_size(bfc_cobjptr_t obj);
unsigned bfc_integer_get_hashcode(bfc_cobjptr_t obj, int hashlen);
int bfc_integer_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other);
size_t bfc_number_get_object_length(bfc_cobjptr_t obj);
int bfc_integer_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize);
void bfc_integer_dump_object(bfc_cobjptr_t obj, int depth,
					struct l4sc_logger *log);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_NUMBER_H_ */
