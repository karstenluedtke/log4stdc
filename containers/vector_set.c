/**
 * @file    vector_set.c
 *
 * @brief   Generic vector setting function
 *
 * @author  Karsten Luedtke
 *
 * @date    2011-10-25
 */

#include <stddef.h>
#include <string.h>

#define BFC_VECTOR_ENABLE_TRIPLE_INDIRECT	1
#define CV_ELEMSIZE_FACTOR(vec)		((vec)->elem_size / sizeof(char))
#include "barefootc/vector.h"

void *
bfc_vector_have (void *vector, unsigned index)
{
	bfc_char_vector_t *vec = (bfc_char_vector_t *) vector;
	char *ref;

	BFC_VECTOR_HAVE (vec, index);
	ref = BFC_VECTOR_REF (vec, index);

	return ((void *) ref);
}

void *
bfc_vector_set (void *vector, unsigned index, void *value)
{
	bfc_char_vector_t *vec = (bfc_char_vector_t *) vector;
	void *ref;

	if ((ref = bfc_vector_have (vec, index)) != NULL) {
		memcpy (ref, value, vec->elem_size);
	}

	return (ref);
}

