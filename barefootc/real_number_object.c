
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

const struct bfc_classhdr bfc_real_number_class = {
	.super	  = &bfc_real_number_class,
	.name 	  = "real number",
	.init 	  = bfc_init_real_number_object,
	.initrefc = bfc_number_init_refcount,
	.incrrefc = bfc_number_incr_refcount,
	.decrrefc = bfc_number_decr_refcount,
	.destroy  = bfc_number_destroy_object,
	.clone 	  = bfc_number_clone_object,
	.clonesize= bfc_get_real_number_object_size,
	.hashcode = bfc_real_number_get_hashcode,
	.equals	  = bfc_real_number_is_equal,
	.length	  = bfc_number_get_object_length,
	.tostring = bfc_real_number_object_tostring,
	.dump	  = bfc_real_number_dump_object,
};

int
bfc_init_real_number_object(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_numptr_t obj = (bfc_numptr_t) buf;
	if (bufsize < sizeof(*obj)) {
		return (-ENOSPC);
	} else {
		memset(obj, 0, sizeof(*obj));
		obj->vptr = (bfc_classptr_t) &bfc_real_number_class;
	}
	return (BFC_SUCCESS);
}

size_t
bfc_get_real_number_object_size(bfc_cobjptr_t obj)
{
	return (sizeof(bfc_number_t));
}

unsigned  
bfc_real_number_get_hashcode(bfc_cobjptr_t obj, int hashlen)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;
	return (bfc_reduce_hashcode(p->u.f, 8*sizeof(p->u.f), hashlen));
}

int
bfc_real_number_is_equal(bfc_cobjptr_t obj, bfc_cobjptr_t other)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;
	bfc_cnumptr_t q = (bfc_cnumptr_t) other;
	return (p->u.f == q->u.f);
}

int
bfc_real_number_object_tostring(bfc_cobjptr_t obj, char *buf, size_t bufsize,
				const char *fmt)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;

	if (p && buf) {
		if (fmt == NULL) {
			snprintf(buf, bufsize, "%.3f", p->u.f);
		} else {
			snprintf(buf, bufsize, fmt, p->u.f);
		}
	}
	return (0);
}

void
bfc_real_number_dump_object(bfc_cobjptr_t obj,int depth,struct l4sc_logger *log)
{
	bfc_cnumptr_t p = (bfc_cnumptr_t) obj;

	if (obj && BFC_CLASS(obj) && p) {
		L4SC_DEBUG(log, "%s object @%p: %.3f",
			BFC_CLASS(obj)->name, obj, p->u.f);
	}
}

