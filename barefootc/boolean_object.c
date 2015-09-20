
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/number.h"
#include "log4stdc.h"

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

const struct bfc_classhdr bfc_boolean_class = {
	.super	  = &bfc_number_class,
	.name 	  = "real",
	.init 	  = bfc_init_boolean_object,
	.initrefc = bfc_default_init_refcount,
	.incrrefc = bfc_default_incr_refcount,
	.decrrefc = bfc_default_decr_refcount,
	.destroy  = bfc_number_destroy_object,
	.clone 	  = bfc_number_clone_object,
	.clonesize= bfc_get_boolean_object_size,
	.hashcode = bfc_boolean_get_hashcode,
	.equals	  = bfc_boolean_is_equal,
	.length	  = bfc_number_get_object_length,
	.tostring = bfc_boolean_object_tostring,
	.dump	  = bfc_boolean_dump_object,
};

#define BOOLEAN_VAL(obj)	(obj)->private_6
#define GET_BOOLEAN(obj)	(BOOLEAN_VAL(obj) != 0)

int
bfc_init_boolean_object(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	bfc_objptr_t obj = (bfc_objptr_t) buf;
	if (bufsize < sizeof(*obj)) {
		return (-ENOSPC);
	} else {
		memset(obj, 0, sizeof(*obj));
		obj->vptr = (bfc_classptr_t) &bfc_boolean_class;
	}
	return (BFC_SUCCESS);
}

size_t
bfc_get_boolean_object_size(bfc_cobjptr_t obj)
{
	return (sizeof(bfc_number_t));
}

unsigned  
bfc_boolean_get_hashcode(bfc_cobjptr_t obj, int hashlen)
{
	bfc_cobjptr_t p = (bfc_cobjptr_t) obj;
	return ((GET_BOOLEAN(p))? 1 : 0);
}

int
bfc_boolean_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	return ((GET_BOOLEAN(obj) && GET_BOOLEAN(other))
	     || (!GET_BOOLEAN(obj) && !GET_BOOLEAN(other)));
}

int
bfc_boolean_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize,
				const char *fmt)
{
	int rc = 0;
	char *dp = buf;
	char tmp[20];
		
	if (obj == NULL) {
		return (-EFAULT);
	} else if ((buf == NULL) || (bufsize == 0)) {
		dp = tmp;
	}

	if (fmt == NULL) {
		const char *s = (GET_BOOLEAN(obj))? "true": "false";
		if ((rc = snprintf(dp, bufsize, "%s", s)) < 0) {
			rc = snprintf(tmp, sizeof(tmp), "%s", s);
		}
	} else if (strchr(fmt, 's')) {
		const char *s = (GET_BOOLEAN(obj))? "true": "false";
		if ((rc = snprintf(dp, bufsize, "%s", s)) < 0) {
			rc = snprintf(tmp, sizeof(tmp), "%s", s);
		}
	} else {
		if ((rc = snprintf(dp, bufsize, fmt, GET_BOOLEAN(obj))) < 0) {
			rc = snprintf(tmp, sizeof(tmp), fmt, GET_BOOLEAN(obj));
		}
	}
	return (rc);
}

void
bfc_boolean_dump_object(bfc_cobjptr_t obj,int depth,struct l4sc_logger *log)
{
	if (obj && BFC_CLASS(obj)) {
		L4SC_DEBUG(log, "%s object @%p: %d",
			BFC_CLASS(obj)->name, obj, GET_BOOLEAN(obj));
	}
}

