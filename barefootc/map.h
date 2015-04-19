/**
 * @file      map.h
 *
 * @brief     Definitions for maps in "C".
 *
 * @author    Karsten Luedtke
 *
 * @date      2015-04-04
 *
 * Copyright (c)  2015  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_MAP_H_
#define _BFC_MAP_H_

#include "barefootc/object.h"
#include "barefootc/container.h"
#include "barefootc/pair.h"
#include "barefootc/vector.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup class_definitions */
struct bfc_map_class {
	BFC_CONTAINER_CLASSHDR(const struct bfc_map_class *,
		bfc_contptr_t, bfc_ccontptr_t, bfc_contptr_t)
};
typedef const struct bfc_map_class *bfc_map_class_ptr_t;
/** @} */

/** @addtogroup map_definition */
/** @{ */

/**
 * @brief    Map definition.
 *
 * use: typedef BFC_PAIR(my_pair, key_element_t, value_element_t) my_pair_t;
 *      typedef BFC_MAP (my_map, my_pair_t) my_map_t;
 *
 * @param[in]   mapname  Defines the name of the struct.
 * @param[in]   elemtype Type of the pairs to be stored within the map.
 */

#define BFC_MAP(mapname,elemtype) \
	BFC_VECTOR(mapname##_pair,elemtype,0)

typedef BFC_MAP(bfc_string_map, bfc_string_pair_t) bfc_string_map_t;
typedef BFC_MAP(bfc_strref_map, bfc_strref_pair_t) bfc_strref_map_t;
typedef BFC_MAP(bfc_object_map, bfc_string_object_pair_t) bfc_object_map_t;
typedef BFC_MAP(bfc_objref_map, bfc_string_objref_pair_t) bfc_objref_map_t;

/**
 * @brief    Initialize a map.
 *
 * The map shall be defined using the BFC_MAP macro.
 * Sets internal data.
 *
 * @param[out]   map   		Pointer to a new map.
 * @param[in]    pairclass 	Class of the pairs (bfc_classptr_t).
 * @param[in]    mpool 		Memory pool to use.
 * @return       None.
 */
 
#define BFC_MAP_INIT(map,estimate,pairclass,mpool) \
do {									\
	BFC_VECTOR_INIT_POOL(map,mpool);				\
	bfc_init_map_class(map,sizeof(*map),estimate,pairclass,mpool);	\
} while (0 /*just once*/)

#define BFC_STRING_MAP_INIT(map,estimate,mpool) \
do {									\
	extern const bfc_class_t bfc_string_pair_class;			\
	BFC_MAP_INIT(map, estimate, &bfc_string_pair_class, mpool);	\
} while (0 /*just once*/)

#define BFC_STRPTR_MAP_INIT(map,estimate,mpool) \
do {									\
	extern const bfc_class_t bfc_strref_pair_class;			\
	BFC_MAP_INIT(map, estimate, &bfc_strref_pair_class, mpool);	\
} while (0 /*just once*/)

#define BFC_OBJECT_MAP_INIT(map,estimate,mpool) \
do {									\
	extern const bfc_class_t bfc_string_object_pair_class;		\
	BFC_MAP_INIT(map,estimate,&bfc_string_object_pair_class,mpool);	\
} while (0 /*just once*/)

#define BFC_OBJPTR_MAP_INIT(map,estimate,mpool) \
do {									\
	extern const bfc_class_t bfc_string_objref_pair_class;		\
	BFC_MAP_INIT(map,estimate,&bfc_string_objref_pair_class,mpool);	\
} while (0 /*just once*/)

#define BFC_INT_MAP_INIT(map,estimate,mpool) \
do {									\
	extern const bfc_class_t bfc_int_pair_class;			\
	BFC_MAP_INIT(map, estimate, &bfc_int_pair_class, mpool);	\
} while (0 /*just once*/)

#define BFC_INT_STRING_MAP_INIT(map,estimate,mpool) \
do {									\
	extern const bfc_class_t bfc_int_string_pair_class;		\
	BFC_MAP_INIT(map, estimate, &bfc_int_string_pair_class, mpool);	\
} while (0 /*just once*/)
/** @} */

int bfc_init_map_class(void *, size_t, int, bfc_classptr_t, struct mempool *);
size_t bfc_map_size(bfc_ccontptr_t);

int bfc_map_insert_objects(void *, void *, void *);
int bfc_map_find_index(void *, const void *, void **);
void *bfc_map_find_pair(void *, const void *);
void *bfc_map_find_value(void *, const void *);
void *bfc_map_index_value(void *, size_t);
int bfc_map_remove_index(void *, size_t);
int bfc_map_remove_key(void *, const void *);

#ifdef __cplusplus
};
#endif
#endif /* _BFC_MAP_H_ */
