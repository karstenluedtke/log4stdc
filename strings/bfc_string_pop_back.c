/**
 * @file bfc_string_pop_back.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_pop_back
 */
void
bfc_string_pop_back(bfc_objptr_t s)
{
	VOID_METHCALL(bfc_string_classptr_t, s, pop_back, (s));
}
