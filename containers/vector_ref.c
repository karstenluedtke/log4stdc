/**
 * @file    vector_ref.c
 *
 * @brief   Generic vector reference function
 *
 * @author  Karsten Luedtke
 *
 * @date    2011-10-25
 */

#include <stddef.h>

#define BFC_VECTOR_ENABLE_TRIPLE_INDIRECT	1
#define CV_ELEMSIZE_FACTOR(vec)		((vec)->elem_size / sizeof(char))
#include "barefootc/vector.h"

void *
bfc_vector_ref (void *vector, unsigned index)
{
	bfc_char_vector_t *vec = (bfc_char_vector_t *) vector;
	char *ref;

	ref = BFC_VECTOR_REF (vec, index);

	return ((void *) ref);
}

