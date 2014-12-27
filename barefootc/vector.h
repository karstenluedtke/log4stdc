/**
 * @file      vector.h
 *
 * @brief     Macros for vectors in "C".
 *
 * @author    Karsten Luedtke
 *
 * @date      2011-10-22
 *
 * Copyright (c)  2011  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_VECTOR_H_
#define _BFC_VECTOR_H_

#include "barefootc/object.h"
#include "barefootc/mempool.h"

/** @addtogroup vector_configuration */
/** @{ */
#ifndef BFC_VECTOR_ENABLE_TRIPLE_INDIRECT
#define BFC_VECTOR_ENABLE_TRIPLE_INDIRECT	0
#endif

#ifndef BFC_VECTOR_ENABLE_DOUBLE_INDIRECT
#define BFC_VECTOR_ENABLE_DOUBLE_INDIRECT	1
#endif

#ifndef BFC_VECTOR_ENABLE_SINGLE_INDIRECT
#define BFC_VECTOR_ENABLE_SINGLE_INDIRECT	1
#endif

#ifndef BFC_VECTOR_GET_ERROR_RETURN
#define BFC_VECTOR_GET_ERROR_RETURN(vec)	((vec)->zero_element[0])
#endif

#ifndef BFC_VECTOR_REF_ERROR_RETURN
#define BFC_VECTOR_REF_ERROR_RETURN(vec)	NULL
#endif

#ifndef BFC_VECTOR_ALLOC_ERROR_HANDLER
#define BFC_VECTOR_ALLOC_ERROR_HANDLER(vec)	/* None */
#endif
/** @} */

/** @addtogroup class_definitions */
struct bfc_vector_class;
typedef const struct bfc_vector_class *bfc_vector_class_ptr_t;
/** @} */

/** @addtogroup vector_definition */
/** @{ */

/**
 * @brief    Vector definition.
 *
 * use: typedef BFC_VECTOR (my_vector, my_element_t, 16) my_vector_t;
 *
 * @param[in]   vecname   Defines the name of the struct. May be empty.
 * @param[in]   elemtype  Type of the elements to be stored within the vector.
 * @param[in]   ndirect   Number of elements stored directly within the vector.
 */

#define BFC_VECTOR(vecname,elemtype,ndirect) \
struct vecname {							\
	BFC_OBJHDR(bfc_vector_class_ptr_t, struct vecname *)		\
									\
	/**								\
	 * Number of elements currently stored.				\
	 */								\
	unsigned	size;						\
									\
	/**								\
	 * Number of elements stored direcly within the vector.		\
	 */								\
	unsigned	elem_direct;					\
									\
	/**								\
	 * Element class.						\
	 */								\
	unsigned	elem_class;					\
									\
	/**								\
	 * Size of each element.					\
	 */								\
	unsigned short	elem_size;					\
									\
	/**								\
	 * Number of elements stored in each indirect block.		\
	 * Stored as the logarithm to base 2.				\
	 */								\
	unsigned char	log2_indirect;					\
									\
	/**								\
	 * Number of pointers stored in each double and triple indirect \
	 * block. Stored as the logarithm to base 2.			\
	 */								\
	unsigned char	log2_double_indirect;				\
									\
	/**								\
	 * One element of all zeros.					\
	 */								\
	elemtype *	zero_element;					\
									\
	/**								\
	 * Pointer to one block of 2**log2_indirect elements 		\
	 * of indirect data.						\
	 */								\
	elemtype *	indirect;					\
									\
	/**								\
	 * Pointer to (2**log2_double_indirect) * (2**log2_indirect) 	\
	 * elements of indirect data.					\
	 */								\
	elemtype **	double_indirect;				\
									\
	/**								\
	 * Pointer to (2**log2_double_indirect)**2 * (2**log2_indirect) \
	 * of indirect data.						\
	 */								\
	elemtype ***	triple_indirect;				\
									\
	/**								\
	 * Storage space for elem_direct elements of direct data.	\
	 */								\
	elemtype	direct[ndirect];				\
}

typedef BFC_VECTOR(bfc_ptr_vector,  void *,   16) bfc_ptr_vector_t;
typedef BFC_VECTOR(bfc_int_vector,  int,      16) bfc_int_vector_t;
typedef BFC_VECTOR(bfc_uint_vector, unsigned, 16) bfc_uint_vector_t;
typedef BFC_VECTOR(bfc_char_vector, char    , 16) bfc_char_vector_t;
/** @} */

/** @addtogroup vector_internals */
/** @{ */
/*
 * Number of elements per indirection and resulting boundaries.
 *
 * Direct elements.
 */
#define CV0_ELEMENTS(vec)	((vec)->elem_direct)
#define CV0_BOUNDARY(vec)	CV0_ELEMENTS(vec)

/*
 * Indirect elements.
 */
#define CV1_LOG2ELEM(vec)	((vec)->log2_indirect)
#define CV1_ELEMENTS(vec)	(1u << CV1_LOG2ELEM(vec))
#define CV1_BOUNDARY(vec)	(CV0_BOUNDARY(vec) + CV1_ELEMENTS(vec))

/*
 * Double indirect elements.
 * Numbers are multiplied by adding their logarithms.
 */
#define CV2_LOG2PTRS(vec)	((vec)->log2_double_indirect)
#define CV2_POINTERS(vec)	(1u << CV2_LOG2PTRS(vec))
#define CV2_LOG2ELEM(vec)	(CV2_LOG2PTRS(vec) + CV1_LOG2ELEM(vec))
#define CV2_ELEMENTS(vec)	(1u << CV2_LOG2ELEM(vec))
#define CV2_BOUNDARY(vec)	(CV1_BOUNDARY(vec) + CV2_ELEMENTS(vec))

/*
 * Triple indirect elements.
 */
#define CV3_LOG2PTRS(vec)	((vec)->log2_double_indirect)
#define CV3_POINTERS(vec)	(1u << CV3_LOG2PTRS(vec))
#define CV3_LOG2ELEM(vec)	(CV3_LOG2PTRS(vec) + CV2_LOG2ELEM(vec))
#define CV3_ELEMENTS(vec)	(1u << CV3_LOG2ELEM(vec))
#define CV3_BOUNDARY(vec)	(CV2_BOUNDARY(vec) + CV3_ELEMENTS(vec))

/*
 * Actual element size = CV_ELEMEMT_FACTOR * sizeof(vec->direct[0])
 * Relevant when casting between element types only.
 */
#ifndef CV_ELEMSIZE_FACTOR
#define CV_ELEMSIZE_FACTOR(vec)		1
#endif
#define	CV1_ELEMSIZE(vec) \
	(CV_ELEMSIZE_FACTOR(vec) * sizeof((vec)->indirect[0]))

/*
 * Memory allocation.
 */
#define BFC_VECTOR_ALLOC_BLOCK(vec,num,size) \
	mempool_calloc((vec)->pool, num, size)

#define BFC_VECTOR_FREE_BLOCK(vec,ptr) \
do {									\
	if (ptr) {							\
		mempool_free((vec)->pool, ptr);				\
	}								\
} while (0)

/*
 * Index calculation for each level of indirection.
 * Here the preceeding boundary is already subtracted from the original index.
 */
#define	CV0_IDX1(v,i)	(CV_ELEMSIZE_FACTOR(v) * (i))
#define	CV1_IDX1(v,i)	(CV_ELEMSIZE_FACTOR(v) * (i))
#define	CV2_IDX1(v,i)	( (i) >> CV1_LOG2ELEM(v))
#define	CV2_IDX2(v,i)	(CV_ELEMSIZE_FACTOR(v) * ((i) & (CV1_ELEMENTS(v)-1)))
#define	CV3_IDX1(v,i)	( (i) >> CV2_LOG2ELEM(v))
#define	CV3_IDX2(v,i)	(((i) >> CV1_LOG2ELEM(v))     & (CV2_POINTERS(v)-1) )
#define	CV3_IDX3(v,i)	(CV_ELEMSIZE_FACTOR(v) * ((i) & (CV1_ELEMENTS(v)-1)))

/*
 * Direct elements.
 */
#define CV0_SET(vec,idx,val)	\
	(vec)->direct[CV0_IDX1(vec,idx)] = (val)

#define CV0_GET(vec,idx)	\
	((vec)->direct[CV0_IDX1(vec,idx)])

#define CV0_REF(vec,idx)	\
	(&((vec)->direct[CV0_IDX1(vec,idx)]))

/*
 * Indirect elements.
 */
#if (BFC_VECTOR_ENABLE_SINGLE_INDIRECT != 0)

#define CV1_HAVE_OR_BREAK(vec,idx)	\
	if ((vec)->indirect == NULL) {					\
		if (((vec)->indirect = BFC_VECTOR_ALLOC_BLOCK(vec,		\
					CV1_ELEMENTS(vec),		\
					CV1_ELEMSIZE(vec))) == NULL) {	\
			BFC_VECTOR_ALLOC_ERROR_HANDLER(vec);		\
			break;						\
		}							\
	}

#define CV1_HAVE(vec,idx)	\
do {									\
	CV1_HAVE_OR_BREAK(vec,idx)					\
} while (0);

#define CV1_SET(vec,idx,val)	\
do {									\
	CV1_HAVE_OR_BREAK(vec,idx)					\
	(vec)->indirect[CV1_IDX1(vec,idx)] = (val);			\
} while (0);

#define CV1_GET(vec,idx)	\
	(((vec)->indirect)?						\
		(vec)->indirect[CV1_IDX1(vec,idx)]:			\
		BFC_VECTOR_GET_ERROR_RETURN(vec))

#define CV1_REF(vec,idx)	\
	(((vec)->indirect)?						\
		&((vec)->indirect[CV1_IDX1(vec,idx)]):			\
		BFC_VECTOR_REF_ERROR_RETURN(vec))

#else /* (BFC_VECTOR_ENABLE_SINGLE_INDIRECT == 0) */

#define CV1_SET(vec,idx,val)	/* Nothing */
#define CV1_GET(vec,idx)	BFC_VECTOR_GET_ERROR_RETURN(vec)
#define CV1_REF(vec,idx)	BFC_VECTOR_REF_ERROR_RETURN(vec)
#endif

/*
 * Double indirect elements.
 */
#if (BFC_VECTOR_ENABLE_DOUBLE_INDIRECT != 0)

#define CV2_HAVE_OR_BREAK(vec,idx)	\
	if ((vec)->double_indirect == NULL) {				\
		if (((vec)->double_indirect =				\
			BFC_VECTOR_ALLOC_BLOCK(vec, CV2_POINTERS(vec),	\
				sizeof((vec)->double_indirect[0])))	\
							   == NULL) {	\
			BFC_VECTOR_ALLOC_ERROR_HANDLER(vec);		\
			break;						\
		}							\
	}								\
	if ((vec)->double_indirect[CV2_IDX1(vec,idx)] == NULL) {	\
		if (((vec)->double_indirect[CV2_IDX1(vec,idx)] =	\
			BFC_VECTOR_ALLOC_BLOCK(vec, CV1_ELEMENTS(vec),	\
						 CV1_ELEMSIZE(vec)))	\
							    == NULL) {	\
			BFC_VECTOR_ALLOC_ERROR_HANDLER(vec);		\
			break;						\
		}							\
	}

#define CV2_HAVE(vec,idx)	\
do {									\
	CV2_HAVE_OR_BREAK(vec,idx)					\
} while (0);

#define CV2_SET(vec,idx,val)	\
do {									\
	CV2_HAVE_OR_BREAK(vec,idx)					\
	((vec)->double_indirect[CV2_IDX1(vec,idx)])			\
	                       [CV2_IDX2(vec,idx)] = (val);		\
} while (0);

#define CV2_GET(vec,idx)	\
	(((vec)->double_indirect					\
	  && (vec)->double_indirect[CV2_IDX1(vec,idx)])?		\
		((vec)->double_indirect[CV2_IDX1(vec,idx)])		\
		                       [CV2_IDX2(vec,idx)]:		\
		BFC_VECTOR_GET_ERROR_RETURN(vec))

#define CV2_REF(vec,idx)	\
	(((vec)->double_indirect					\
	  && (vec)->double_indirect[CV2_IDX1(vec,idx)])?		\
		&(((vec)->double_indirect[CV2_IDX1(vec,idx)])		\
		                         [CV2_IDX2(vec,idx)]):		\
		BFC_VECTOR_REF_ERROR_RETURN(vec))

#else /* (BFC_VECTOR_ENABLE_DOUBLE_INDIRECT == 0) */

#define CV2_SET(vec,idx,val)	/* Nothing */
#define CV2_GET(vec,idx)	BFC_VECTOR_GET_ERROR_RETURN(vec)
#define CV2_REF(vec,idx)	BFC_VECTOR_REF_ERROR_RETURN(vec)
#endif

/*
 * Triple indirect elements.
 */
#if (BFC_VECTOR_ENABLE_TRIPLE_INDIRECT != 0)

#define CV3_HAVE_OR_BREAK(vec,idx)	\
	if ((vec)->triple_indirect == NULL) {				\
		if (((vec)->triple_indirect =				\
			BFC_VECTOR_ALLOC_BLOCK(vec, CV3_POINTERS(vec),	\
				sizeof((vec)->triple_indirect[0])))	\
							   == NULL) {	\
			BFC_VECTOR_ALLOC_ERROR_HANDLER(vec);		\
			break;						\
		}							\
	}								\
	if ((vec)->triple_indirect[CV3_IDX1(vec,idx)] == NULL) {	\
		if (((vec)->triple_indirect[CV3_IDX1(vec,idx)] =	\
			BFC_VECTOR_ALLOC_BLOCK(vec, CV2_POINTERS(vec),	\
				sizeof((vec)->double_indirect[0])))	\
							   == NULL) {	\
			BFC_VECTOR_ALLOC_ERROR_HANDLER(vec);		\
			break;						\
		}							\
	}								\
	if (((vec)->triple_indirect[CV3_IDX1(vec,idx)])			\
				   [CV3_IDX2(vec,idx)] == NULL) {	\
		if ((((vec)->triple_indirect[CV3_IDX1(vec,idx)])	\
					    [CV3_IDX2(vec,idx)] =	\
			BFC_VECTOR_ALLOC_BLOCK(vec, CV1_ELEMENTS(vec),	\
						 CV1_ELEMSIZE(vec)))	\
							    == NULL) {	\
			BFC_VECTOR_ALLOC_ERROR_HANDLER(vec);		\
			break;						\
		}							\
	}	

#define CV3_HAVE(vec,idx)	\
do {									\
	CV3_HAVE_OR_BREAK(vec,idx)					\
} while (0);

#define CV3_SET(vec,idx,val)	\
do {									\
	CV3_HAVE_OR_BREAK(vec,idx)					\
	(((vec)->triple_indirect[CV3_IDX1(vec,idx)])			\
	                        [CV3_IDX2(vec,idx)])			\
	                        [CV3_IDX3(vec,idx)] = (val);		\
} while (0);

#define CV3_GET(vec,idx)	\
	(((vec)->triple_indirect					\
	  && (vec)->triple_indirect[CV3_IDX1(vec,idx)]			\
	  && ((vec)->triple_indirect[CV3_IDX1(vec,idx)])		\
	                            [CV3_IDX2(vec,idx)])?		\
		(((vec)->triple_indirect[CV3_IDX1(vec,idx)])		\
		                        [CV3_IDX2(vec,idx)])		\
		                        [CV3_IDX3(vec,idx)]:		\
		BFC_VECTOR_GET_ERROR_RETURN(vec))

#define CV3_REF(vec,idx)	\
	(((vec)->triple_indirect					\
	  && (vec)->triple_indirect[CV3_IDX1(vec,idx)]			\
	  && ((vec)->triple_indirect[CV3_IDX1(vec,idx)])		\
	                            [CV3_IDX2(vec,idx)])?		\
		&((((vec)->triple_indirect[CV3_IDX1(vec,idx)])		\
		                          [CV3_IDX2(vec,idx)])		\
		                          [CV3_IDX3(vec,idx)]):		\
		BFC_VECTOR_REF_ERROR_RETURN(vec))

#else /* (BFC_VECTOR_ENABLE_TRIPLE_INDIRECT == 0) */

#define CV3_SET(vec,idx,val)	/* Nothing */
#define CV3_GET(vec,idx)	BFC_VECTOR_GET_ERROR_RETURN(vec)
#define CV3_REF(vec,idx)	BFC_VECTOR_REF_ERROR_RETURN(vec)

#endif
/** @} */

/** @addtogroup vector_public */
/** @{ */

/**
 * @brief    Initialize a vector.
 *
 * The vector shall be defined using the BFC_VECTOR macro.
 * Sets internal data. Zeros direct elements.
 *
 * @param[out]   vec   Pointer to a new vector.
 * @param[in]    mpool Memory pool to use.
 * @return       None.
 */
 
#define BFC_VECTOR_INIT_POOL(vec,mpool) \
do {									\
	unsigned __vec_nelem, __vec_log2;				\
	static double __vec_zero[1 + (sizeof((vec)->direct[0])		\
						/ sizeof(double))];	\
									\
	memset (vec, 0, sizeof(*vec));					\
	memset (__vec_zero, 0, sizeof (__vec_zero));			\
									\
	(vec)->pool = (mpool);						\
	(vec)->elem_size = sizeof((vec)->direct[0]);			\
	(vec)->zero_element = (void *) __vec_zero;			\
									\
	__vec_nelem = sizeof((vec)->direct)				\
			/ sizeof((vec)->direct[0]);			\
	(vec)->elem_direct = __vec_nelem;				\
									\
	__vec_nelem = 4096 / sizeof((vec)->direct[0]);			\
	for (__vec_log2=0; (__vec_nelem >>= 1) > 0; __vec_log2++);	\
	(vec)->log2_indirect = __vec_log2;				\
	(vec)->log2_double_indirect = 10; /* 1024 pointers per block */	\
} while (0 /* just once */)

/**
 * @brief    Initialize a vector with default pool.
 *
 * The vector shall be defined using the BFC_VECTOR macro.
 * Sets internal data. Zeros direct elements.
 *
 * @param[out]   vec   Pointer to a new vector.
 * @return       None.
 */
 
#define BFC_VECTOR_INIT(vec) \
	BFC_VECTOR_INIT_POOL(vec,get_default_mempool())

/**
 * @brief    Set a custom memory allocation functions for a vector.
 * 
 * @param[in,out]   vec   Pointer to a vector.
 * @return          None.
 */

#define BFC_VECTOR_SET_POOL(vec,pool) \
do {									\
	(vec)->pool = (pool);						\
} while (0)
 
/**
 * @brief    Get the maximum size of a vector.
 *
 * Optional.
 * The size is also set when setting elements beyond the current size.
 * This function does neither intialize any elements, not does it allocate
 * any memory.
 * 
 * @param[in]       vec      Pointer to a vector.
 * @return          Maximum size of the vector.
 */

#if (BFC_VECTOR_ENABLE_SINGLE_INDIRECT == 0)
#define BFC_VECTOR_MAX_SIZE(vec)		 	CV0_BOUNDARY(vec)

#elif (BFC_VECTOR_ENABLE_DOUBLE_INDIRECT == 0)
#define BFC_VECTOR_MAX_SIZE(vec)		 	CV1_BOUNDARY(vec)

#elif (BFC_VECTOR_ENABLE_TRIPLE_INDIRECT == 0)
#define BFC_VECTOR_MAX_SIZE(vec)		 	CV2_BOUNDARY(vec)
#else

#define BFC_VECTOR_MAX_SIZE(vec)		 	CV3_BOUNDARY(vec)
#endif

/**
 * @brief    Get the size of a vector.
 *
 * Optional.
 * The size is also set when setting elements beyond the current size.
 * This function does neither intialize any elements, not does it allocate
 * any memory.
 * 
 * @param[in]       vec      Pointer to a vector.
 * @return          Current size of the vector.
 */

#define BFC_VECTOR_GET_SIZE(vec)	 	((vec)->size)
 
/**
 * @brief    Set the size of a vector.
 *
 * Optional.
 * The size is also set when setting elements beyond the current size.
 * This function does neither intialize any elements, not does it allocate
 * any memory.
 * 
 * @param[in,out]   vec      Pointer to a vector.
 * @param[in]       newsize  New size of the vector.
 * @return          None.
 */

#define BFC_VECTOR_SET_SIZE(vec,newsize)	 (vec)->size = (newsize)
 
/**
 * @brief    Set an element of a vector to the value supplied.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       idx   Index of the element. Between 0 and (size-1).
 * @param[in]       val   New value for the element.
 * @return          None.
 */
 
#define BFC_VECTOR_SET(vec,idx,val)	\
do {									\
	if ((idx) < CV0_BOUNDARY(vec)) {				\
		CV0_SET(vec, idx, val);			    		\
	} else if ((idx) < CV1_BOUNDARY(vec)) {				\
		CV1_SET(vec, (idx)-CV0_BOUNDARY(vec), val);		\
	} else if ((idx) < CV2_BOUNDARY(vec)) {				\
		CV2_SET(vec, (idx)-CV1_BOUNDARY(vec), val);		\
	} else if ((idx) < CV3_BOUNDARY(vec)) {				\
		CV3_SET(vec, (idx)-CV2_BOUNDARY(vec), val);		\
	}								\
	if ((idx) >= (vec)->size) {					\
		(vec)->size = (idx + 1);				\
	}								\
} while (0)

/**
 * @brief    Makes sure an element is available in a vector
 *           without changing its value.
 *
 * Initially its value will be all zeros.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       idx   Index of the element. Between 0 and (size-1).
 * @return          None.
 */
 
#define BFC_VECTOR_HAVE(vec,idx)	\
do {									\
	if ((idx) < CV0_BOUNDARY(vec)) {				\
		/* ok */;						\
	} else if ((idx) < CV1_BOUNDARY(vec)) {				\
		CV1_HAVE(vec, (idx)-CV0_BOUNDARY(vec));			\
	} else if ((idx) < CV2_BOUNDARY(vec)) {				\
		CV2_HAVE(vec, (idx)-CV1_BOUNDARY(vec));			\
	} else if ((idx) < CV3_BOUNDARY(vec)) {				\
		CV3_HAVE(vec, (idx)-CV2_BOUNDARY(vec));			\
	}								\
	if ((idx) >= (vec)->size) {					\
		(vec)->size = (idx + 1);				\
	}								\
} while (0)

/**
 * @brief    Retrieve an element from a vector.
 *
 * The value returned is all zeros, when the element has never been set.
 *
 * The return value for the case that no memory has ben allocated yet for
 * the element can be defined by defining the BFC_VECTOR_GET_ERROR_RETURN(vec)
 * macro before including this file.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       idx   Index of the element. Between 0 and (size-1).
 * @return          The value stored in the element.
 *                  Undefined, if the element has never been set.
 */

#define BFC_VECTOR_GET(vec,idx)	\
	(((idx) >= (vec)->size)? BFC_VECTOR_GET_ERROR_RETURN(vec):	    \
	 ((idx) < CV0_BOUNDARY(vec))? CV0_GET(vec,idx):			    \
	 ((idx) < CV1_BOUNDARY(vec))? CV1_GET(vec,(idx)-CV0_BOUNDARY(vec)): \
	 ((idx) < CV2_BOUNDARY(vec))? CV2_GET(vec,(idx)-CV1_BOUNDARY(vec)): \
	 ((idx) < CV3_BOUNDARY(vec))? CV3_GET(vec,(idx)-CV2_BOUNDARY(vec)): \
	 BFC_VECTOR_GET_ERROR_RETURN(vec))

/**
 * @brief    Retrieve a reference to an element in a vector.
 *
 * To make sure that there is memory alocated for the element,
 * the element must be set using BFC_VECTOR_SET first.
 *
 * The return value for the case that no memory has ben allocated yet for
 * the element can be defined by defining the BFC_VECTOR_REF_ERROR_RETURN(vec)
 * macro before including this file.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       idx   Index of the element. Between 0 and (size-1).
 * @return          The address of the element.
 *                  NULL, if there is no memory allocated for the element yet.
 */

#define BFC_VECTOR_REF(vec,idx)	\
	(((idx) >= (vec)->size)? BFC_VECTOR_REF_ERROR_RETURN(vec):	    \
	 ((idx) < CV0_BOUNDARY(vec))? CV0_REF(vec,idx):			    \
	 ((idx) < CV1_BOUNDARY(vec))? CV1_REF(vec,(idx)-CV0_BOUNDARY(vec)): \
	 ((idx) < CV2_BOUNDARY(vec))? CV2_REF(vec,(idx)-CV1_BOUNDARY(vec)): \
	 ((idx) < CV3_BOUNDARY(vec))? CV3_REF(vec,(idx)-CV2_BOUNDARY(vec)): \
	 BFC_VECTOR_REF_ERROR_RETURN(vec))

/**
 * @brief    Add an element to the end of the vector.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       val   New value for the element.
 * @return          None.
 */
 
#define BFC_VECTOR_PUSH_BACK(vec,val)	\
	BFC_VECTOR_SET(vec, BFC_VECTOR_GET_SIZE(vec), val)

/**
 * @brief    Delete the last element of a vector.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @return          None.
 */
 
#define BFC_VECTOR_POP_BACK(vec)	\
	if (BFC_VECTOR_GET_SIZE(vec) > 0)				\
		BFC_VECTOR_SET_SIZE(vec, BFC_VECTOR_GET_SIZE(vec)-1)

/**
 * @brief    Insert an element at the given position.
 *
 * After calling this function, all references retrieved by BFC_VECTOR_REF
 * beyond idx are invalid.
 *
 * This is a large macro and should be better implemented using functions.
 * Caution when using with large vectors. Time and memory consuming.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       idx   Position of the element. Between 0 and (size-1).
 * @param[in]       val   Value for the element inserted.
 * @return          None.
 */
 
#define BFC_VECTOR_INSERT(vec,idx,val)	\
do {									\
	unsigned __copy_idx = BFC_VECTOR_GET_SIZE(vec);			\
	for (;__copy_idx > (idx); __copy_idx--) {			\
		BFC_VECTOR_SET(vec, __copy_idx,				\
			    BFC_VECTOR_GET(vec, __copy_idx-1));		\
	}								\
	BFC_VECTOR_SET(vec, idx, val);					\
} while (0)

/**
 * @brief    Delete an element at the given position.
 *
 * After calling this function, all references retrieved by BFC_VECTOR_REF
 * beyond idx are invalid.
 *
 * This is a large macro and should be better implemented using functions.
 * Caution when using with large vectors. Time and memory consuming.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @param[in]       idx   Position of the element. Between 0 and (size-1).
 * @return          None.
 */
 
#define BFC_VECTOR_DELETE(vec,idx)	\
do {									\
	unsigned __copy_idx = idx;					\
	for (;__copy_idx < (BFC_VECTOR_GET_SIZE(vec)-1); __copy_idx++) {\
		BFC_VECTOR_SET(vec, __copy_idx,				\
			    BFC_VECTOR_GET(vec, __copy_idx+1));		\
	}								\
	BFC_VECTOR_POP_BACK(vec);					\
} while (0)
/** @} */

/** @addtogroup vector_internals */
/** @{ */
#define CV_FOREACH_NEXT(vec,idx,ptr,stage,i,j,k,cont_ptrs,cont_elems)	     \
while ((k >= cont_elems) && (idx < (vec)->size)) {			     \
	cont_elems = 0;							     \
	ptr = NULL;							     \
	if (stage == 0) {						     \
		stage = 1;						     \
		k = 0;							     \
		if ((ptr = (vec)->indirect) != NULL) {			     \
			cont_elems = CV1_ELEMENTS(vec);			     \
		}							     \
									     \
	} else if (stage == 1) {					     \
		stage = 2;						     \
		j = k = 0;						     \
		idx = CV1_BOUNDARY(vec);				     \
		if ((vec)->double_indirect				     \
		 && (ptr = (vec)->double_indirect[0])) {		     \
			cont_elems = CV1_ELEMENTS(vec);			     \
		}							     \
		/* otherwise iterate while-loop once more */		     \
									     \
	} else if (stage == 2) {					     \
		j++; k=0;						     \
		if (!(vec)->double_indirect || (j >= cont_ptrs)) {	     \
			stage = 3;					     \
			i = j = k = 0;					     \
			idx = CV2_BOUNDARY(vec);			     \
			if ((vec)->triple_indirect			     \
			 && (vec)->triple_indirect[0]			     \
			 && (ptr = ((vec)->triple_indirect[0])[0])) {	     \
				cont_elems = CV1_ELEMENTS(vec);		     \
			}						     \
		} else if ((ptr = (vec)->double_indirect[j]) != NULL) {	     \
			idx = CV1_BOUNDARY(vec)				     \
				+ (j << CV1_LOG2ELEM(vec));		     \
			cont_elems = CV1_ELEMENTS(vec);			     \
		}							     \
		/* otherwise iterate while-loop once more */		     \
									     \
	} else if (stage == 3) {					     \
		j++; k=0;						     \
		if (j >= cont_ptrs) {					     \
			i++;						     \
			j = 0;						     \
		}							     \
		if (!(vec)->triple_indirect || (i >= cont_ptrs)) {	     \
			stage = 9;					     \
			idx = (vec)->size;				     \
		} else if ((vec)->triple_indirect[i] == NULL) {		     \
			j |= cont_ptrs-1;				     \
			/* next i in next iteration */			     \
		} else if ((ptr = ((vec)->triple_indirect[i])[j]) != NULL) { \
			idx = CV2_BOUNDARY(vec)				     \
				+ (i << CV2_LOG2ELEM(vec))		     \
				+ (j << CV1_LOG2ELEM(vec));		     \
			cont_elems = CV1_ELEMENTS(vec);			     \
		}							     \
		/* otherwise iterate while-loop once more */		     \
	}								     \
}
/** @} */

/** @addtogroup vector_public */
/** @{ */
/**
 * @brief    Iterate over a BFC_VECTOR.
 *
 * This macro loops through all elements stored in a vector
 * between 0 and BFC_VECTOR_GET_SIZE(vec)-1,
 * skipping those elements that have no memory allocated for them yet.
 *
 * This is expected to be much faster than calling BFC_VECTOR_GET()
 * or BFC_VECTOR_REF() for each element.
 *
 * usage:
 * @code
 *	BFC_VECTOR_FOREACH(elementptr, elementidx, vec) {
 *
 * 		printf ("vec[%d] is %d\n", elementidx, *elementptr);
 *
 * 	} END_BFC_VECTOR_FOREACH;
 * @endcode
 *
 * @param[out]      ptr   Pointer variable pointing to a vector element.
 * @param[out]      idx   Integer variable holding the current index.
 * @param[in,out]   vec   Pointer to a vector.
 */

#define BFC_VECTOR_FOREACH(ptr,idx,vec) \
do {									     \
	unsigned _cvf_stage = 0;					     \
	unsigned _cvf_i=0, _cvf_j=0, _cvf_k=0;				     \
	unsigned _cvf_contiguous_elems = CV0_ELEMENTS(vec);		     \
	unsigned _cvf_contiguous_ptrs  = CV2_POINTERS(vec);		     \
									     \
	for (idx=0, ptr=(vec)->direct;					     \
	     idx < (vec)->size; idx++, ptr++, _cvf_k++) {		     \
		CV_FOREACH_NEXT(vec, idx, ptr,				     \
				_cvf_stage, _cvf_i,_cvf_j,_cvf_k,	     \
				_cvf_contiguous_ptrs,_cvf_contiguous_elems); \
		if (ptr) 

#define END_BFC_VECTOR_FOREACH \
	}								     \
} while (0)

/**
 * @brief    Free memory alloced for a vector for indirect data.
 *
 * The vector must have been initialized (or at least zeroed).
 * It is safe to call this macro multiple times.
 *
 * @param[in,out]   vec   Pointer to a vector.
 * @return          None.
 */

#define BFC_VECTOR_DESTROY(vec) \
do {									\
	int __dblidx, __tplidx;						\
	const int __dblcnt = CV2_POINTERS(vec);				\
									\
	(vec)->size = 0;						\
									\
	if ((vec)->indirect) {						\
		BFC_VECTOR_FREE_BLOCK ((vec), (vec)->indirect);		\
		(vec)->indirect = NULL;					\
	}								\
									\
	if ((vec)->double_indirect) {					\
		for (__dblidx=0; __dblidx < __dblcnt; __dblidx++) {	\
			BFC_VECTOR_FREE_BLOCK ((vec),			\
				(vec)->double_indirect[__dblidx]);	\
		}							\
		BFC_VECTOR_FREE_BLOCK ((vec), (vec)->double_indirect);	\
		(vec)->double_indirect = NULL;				\
	}								\
									\
	if ((vec)->triple_indirect) {					\
		for (__tplidx=0; __tplidx < __dblcnt; __tplidx++) {	\
			if ((vec)->triple_indirect[__tplidx]) {		\
				for (__dblidx=0;			\
				     __dblidx < __dblcnt; __dblidx++) {	\
					BFC_VECTOR_FREE_BLOCK ((vec),	\
						((vec)->triple_indirect	\
						             [__tplidx]) \
						             [__dblidx]); \
				}					\
				BFC_VECTOR_FREE_BLOCK((vec),		\
					(vec)->triple_indirect[__tplidx]);\
			}						\
		}							\
		BFC_VECTOR_FREE_BLOCK ((vec), (vec)->triple_indirect);	\
		(vec)->triple_indirect = NULL;				\
	}								\
} while (0)

/** @} */

#ifdef __cplusplus
extern "C" {
#endif

void *bfc_vector_have(void *, unsigned);
void *bfc_vector_set (void *, unsigned, void *);
void *bfc_vector_ref (void *, unsigned);

int bfc_init_vector_class(void *, size_t, struct mempool *);
#define bfc_init_vector(vec,pool)					\
do {									\
	BFC_VECTOR_INIT_POOL(vec,pool);					\
	bfc_init_vector_class(vec,sizeof(*vec),pool);			\
} while (0)

#ifdef __cplusplus
};
#endif
#endif /* _BFC_VECTOR_H_ */
