/**
 * @file bfc_string_buffered_substr.c
 */

#include <stddef.h>
#include <errno.h>
#define NEED_BFC_STRING_CLASS 1
#include "barefootc/object.h"
#include "barefootc/string.h"

/**
 * @brief    bfc_string_buffered_substr
 */
int
bfc_string_buffered_substr(bfc_cobjptr_t s, size_t pos, size_t n,
			   void *buf, size_t bufsize,
			   void *databuf, size_t dbufsize)
{
	RETURN_METHCALL(bfc_string_classptr_t, s,
			buffered_substr,(s,pos,n,buf,bufsize,databuf,dbufsize),
			-ENOSYS);
}
