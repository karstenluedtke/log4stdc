/**
 * @file      number.h
 *
 * @brief     An implementation of "boxed" numbers.
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

extern const struct bfc_classhdr bfc_number_class;
extern const struct bfc_classhdr bfc_natural_class;
extern const struct bfc_classhdr bfc_real_number_class;
extern const struct bfc_classhdr bfc_boolean_class;

typedef struct bfc_basic_object bfc_number_t;

#define BFC_SIGNED_NUMBER(x) \
 { BFC_STATIC_OBJHDR_INITIALIZERS(bfc_number_class,"int"), \
   sizeof(ptrdiff_t), (size_t)(ptrdiff_t)(x) }

#define BFC_NUMBER_LOGGER	"barefootc.number",16

int  bfc_init_integer_object(void *buf, size_t bufsize, bfc_mempool_t pool);
void bfc_number_destroy_object(bfc_objptr_t obj);
int  bfc_number_clone_object(bfc_cobjptr_t obj,
			void *buf, size_t bufsize, bfc_mempool_t pool);
size_t bfc_get_integer_object_size(bfc_cobjptr_t obj);
unsigned bfc_integer_get_hashcode(bfc_cobjptr_t obj, int hashlen);
int bfc_integer_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other);
size_t bfc_number_get_object_length(bfc_cobjptr_t obj);
int bfc_integer_object_tostring(bfc_cobjptr_t obj,
				char *buf, size_t bufsize, const char *fmt);
void bfc_integer_dump_object(bfc_cobjptr_t obj, int depth,
					struct l4sc_logger *log);
const void *bfc_number_first(bfc_cobjptr_t obj);
void *bfc_number_index(bfc_objptr_t obj, size_t pos);
long bfc_integer_getlong(bfc_cobjptr_t obj, size_t pos);
int bfc_integer_setlong(bfc_objptr_t obj, size_t pos, long val);
bfc_objptr_t bfc_integer_place(bfc_objptr_t obj, size_t pos,
				bfc_objptr_t val, bfc_mempool_t pool);

int bfc_init_real_number_object(void *buf,size_t bufsize,bfc_mempool_t pool);
size_t bfc_get_real_number_object_size(bfc_cobjptr_t obj);
unsigned bfc_real_number_get_hashcode(bfc_cobjptr_t obj, int hashlen);
int bfc_real_number_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other);
int bfc_real_number_object_tostring(bfc_cobjptr_t obj,
				char *buf, size_t bufsize, const char *fmt);
void bfc_real_number_dump_object(bfc_cobjptr_t obj, int depth,
					struct l4sc_logger *log);
double bfc_real_number_get(bfc_cobjptr_t obj);
int bfc_real_number_set(bfc_objptr_t obj, double val);
long bfc_real_number_getlong(bfc_cobjptr_t obj, size_t pos);
int bfc_real_number_setlong(bfc_objptr_t obj, size_t pos, long val);

int bfc_init_boolean_object(void *buf, size_t bufsize, bfc_mempool_t pool);
size_t bfc_get_boolean_object_size(bfc_cobjptr_t obj);
unsigned bfc_boolean_get_hashcode(bfc_cobjptr_t obj, int hashlen);
int bfc_boolean_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other);
int bfc_boolean_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize,
				const char *fmt);
void bfc_boolean_dump_object(bfc_cobjptr_t obj, int depth,
				struct l4sc_logger *log);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_NUMBER_H_ */
