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
			      bfc_objptr_t, bfc_cobjptr_t, bfc_objptr_t)
	BFC_CONTAINER_METHODS(bfc_objptr_t, bfc_cobjptr_t, bfc_objptr_t)
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

#ifdef _MSC_VER /* msc does not allow zero-sized arrays */
#define BFC_MAP(mapname,elemtype) \
	BFC_VECTOR(mapname##_pair_vector,elemtype,1)
#else
#define BFC_MAP(mapname,elemtype) \
	BFC_VECTOR(mapname##_pair_vector,elemtype,0)
#endif

#define BFC_LINEAR_MAP(mapname,elemtype) \
	BFC_VECTOR(mapname##_pair_vector,elemtype,4)

typedef BFC_MAP(bfc_string_map, bfc_string_pair_t) bfc_string_map_t;
typedef BFC_MAP(bfc_strref_map, bfc_strref_pair_t) bfc_strref_map_t;
typedef BFC_MAP(bfc_object_map, bfc_string_object_pair_t) bfc_object_map_t;
typedef BFC_MAP(bfc_objref_map, bfc_string_objref_pair_t) bfc_objref_map_t;
typedef BFC_MAP(bfc_string_objref_map, bfc_string_objref_pair_t)
							bfc_string_objref_map_t;

typedef BFC_LINEAR_MAP(bfc_linear_string_map, bfc_string_pair_t)
							bfc_linear_string_map_t;

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

#define BFC_STRING_OBJREF_MAP_INIT(map,estimate,mpool) \
do {									\
	BFC_MAP_INIT(map, estimate,					\
		(bfc_classptr_t)&bfc_string_ref_pair_class, mpool);	\
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

/**
 * @brief    Initialize a linear map.
 *
 * The map shall be defined using the BFC_LINEAR_MAP macro.
 * Sets internal data.
 *
 * @param[out]   map   		Pointer to a new map.
 * @param[in]    pairclass 	Class of the pairs (bfc_classptr_t).
 * @param[in]    mpool 		Memory pool to use.
 * @return       None.
 */
 
#define BFC_LINEAR_MAP_INIT(map,pairclass,mpool) \
do {									\
	bfc_init_linear_map(map,sizeof(*map),pairclass,mpool);		\
} while (0 /*just once*/)

#define BFC_LINEAR_STRING_MAP_INIT(map,mpool) \
do {									\
	extern const bfc_class_t bfc_string_pair_class;			\
	BFC_LINEAR_MAP_INIT(map,&bfc_string_pair_class,mpool);		\
} while (0 /*just once*/)

/** @} */

int bfc_init_map_class(void *, size_t, int, bfc_classptr_t, bfc_mempool_t );
int bfc_init_map_copy(void *, size_t, bfc_mempool_t , bfc_cobjptr_t);
int bfc_map_reserve(bfc_objptr_t map, size_t n);
size_t bfc_map_size(bfc_cobjptr_t);
size_t bfc_map_load_limit(const void *map);
size_t bfc_map_load_percent(const void *map);

int bfc_init_linear_map(void *, size_t, bfc_classptr_t, bfc_mempool_t );
int bfc_init_linear_map_copy(void *, size_t, bfc_mempool_t , bfc_cobjptr_t);

int bfc_map_insert_objects(bfc_objptr_t, bfc_objptr_t, bfc_objptr_t,
			   bfc_iterptr_t position, size_t possize);
int bfc_map_replace_objects(bfc_objptr_t, bfc_objptr_t, bfc_objptr_t,
			    bfc_iterptr_t position, size_t possize);
int bfc_map_erase_key(bfc_objptr_t, bfc_cobjptr_t);
int bfc_map_erase_iter(bfc_objptr_t, bfc_iterptr_t);
int bfc_map_erase_index(bfc_objptr_t, size_t);

bfc_objptr_t bfc_map_find_pair(bfc_objptr_t, bfc_cobjptr_t);
bfc_objptr_t  bfc_map_find_value(bfc_objptr_t, bfc_cobjptr_t);
bfc_objptr_t  bfc_map_index_value(bfc_objptr_t, size_t);
int bfc_map_count(bfc_objptr_t, bfc_cobjptr_t);
int bfc_map_bucket_size(bfc_objptr_t, bfc_cobjptr_t);
unsigned bfc_map_keyhashcode(const void *, const void *); /* c++ bucket() */
int bfc_map_rehash(bfc_objptr_t, size_t);

#ifdef __cplusplus
};
#endif
#endif /* _BFC_MAP_H_ */
