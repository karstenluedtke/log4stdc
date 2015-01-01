/**
 * @file bfc_iterator_element_size.c
 */

#include <stddef.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"

/**
 * @brief    bfc_iterator_element_size
 */
size_t
bfc_iterator_element_size(bfc_citerptr_t it)
{
	RETURN_METHCALL(bfc_iterator_classptr_t, it,
			element_size, (it),
			0);
}
