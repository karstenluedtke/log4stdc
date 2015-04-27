/**
 * @file      pair.h
 *
 * @brief     Macros for pairs in "C".
 *
 * @author    Karsten Luedtke
 *
 * @date      2015-03-22
 *
 * Copyright (c)  2015  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_PAIR_H_
#define _BFC_PAIR_H_

#include "barefootc/object.h"
#include "barefootc/number.h"
#include "barefootc/string.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup class_definitions */
struct bfc_pair_class;
typedef const struct bfc_pair_class *bfc_pair_class_ptr_t;
extern const struct bfc_pair_class bfc_int_pair_class;
extern const struct bfc_pair_class bfc_int_string_pair_class;
extern const struct bfc_pair_class bfc_real_int_pair_class;
/** @} */

/** @addtogroup pair_definition */
/** @{ */

/**
 * @brief    Pair definition.
 *
 * use: typedef BFC_PAIR (my_pair, first_element_t, second_element_t) my_pair_t;
 *
 * @param[in]   pairname  Defines the name of the struct.
 * @param[in]   elemtype  Type of the elements to be stored within the pair.
 */

#define BFC_PAIR(pairname,first_elemtype,second_elemtype) \
struct pairname {							\
	bfc_pair_class_ptr_t vptr;					\
	first_elemtype	first;						\
	second_elemtype	second;						\
}

typedef BFC_PAIR(bfc_number_pair, bfc_number_t, bfc_number_t) bfc_number_pair_t;
typedef BFC_PAIR(bfc_number_string_pair, bfc_number_t, bfc_string_t)
						       bfc_number_string_pair_t;

typedef BFC_PAIR(bfc_ptr_pair, void *, void *) bfc_ptr_pair_t;
typedef BFC_PAIR(bfc_object_pair, bfc_object_t, bfc_object_t) bfc_object_pair_t;
typedef BFC_PAIR(bfc_objref_pair, bfc_objptr_t, bfc_objptr_t) bfc_objref_pair_t;
typedef BFC_PAIR(bfc_string_pair, bfc_string_t, bfc_string_t) bfc_string_pair_t;
typedef BFC_PAIR(bfc_string_object_pair, bfc_string_t, bfc_object_t)
						       bfc_string_object_pair_t;
typedef BFC_PAIR(bfc_string_objref_pair, bfc_string_t, bfc_objptr_t)
						       bfc_string_objref_pair_t;
typedef BFC_PAIR(bfc_strref_pair, bfc_strptr_t, bfc_strptr_t) bfc_strref_pair_t;
/** @} */

#ifdef __cplusplus
};
#endif
#endif /* _BFC_PAIR_H_ */
