
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "log4stdc.h"

typedef BFC_VECTOR(bfc_generic_vector, size_t, 16) bfc_generic_vector_t;
typedef struct bfc_generic_vector *bfc_vecptr_t;
typedef const struct bfc_generic_vector *bfc_cvecptr_t;

static int clone_vector(bfc_cvecptr_t vec, void *buf, size_t bufsize);
static void destroy_vector(bfc_vecptr_t vec);

static size_t vector_objsize(bfc_cvecptr_t vec);
static size_t vector_size(bfc_cvecptr_t vec);
static int vector_equals(bfc_cvecptr_t vec, bfc_cvecptr_t other);
static unsigned vector_hashcode(bfc_cvecptr_t vec);
static void dump_vector(bfc_cvecptr_t vec,int depth,struct l4sc_logger*log);
static int vector_tostring(bfc_cvecptr_t vec, char *buf, size_t bufsize);

static int begin_iterator(bfc_cvecptr_t vec, bfc_iterptr_t it, size_t bufsize);
static int limit_iterator(bfc_cvecptr_t vec, bfc_iterptr_t it, size_t bufsize);
static int rbegin_iterator(bfc_cvecptr_t vec,bfc_iterptr_t it, size_t bufsize);
static int rlimit_iterator(bfc_cvecptr_t vec,bfc_iterptr_t it, size_t bufsize);

static size_t vector_max_size(bfc_cvecptr_t vec);
static int vector_resize(bfc_vecptr_t vec, size_t n, const void *p);
static size_t vector_capacity(bfc_cvecptr_t vec);
static int vector_reserve(bfc_vecptr_t vec, size_t n);

static int vector_assign_fill(bfc_vecptr_t vec, size_t n, const void *p);
static int vector_assign_range(bfc_vecptr_t vec,
				bfc_iterptr_t first,bfc_iterptr_t last);
static int vector_push_front(bfc_vecptr_t vec, const void *p);
static void vector_pop_front(bfc_vecptr_t vec);
static int vector_push_back(bfc_vecptr_t vec, const void *p);
static void vector_pop_back(bfc_vecptr_t vec);
static int vector_insert_element(bfc_vecptr_t vec, bfc_iterptr_t position,
				const void *p);
static int vector_insert_fill(bfc_vecptr_t vec, bfc_iterptr_t position,
				size_t n, const void *p);
static int vector_insert_range(bfc_vecptr_t vec, bfc_iterptr_t position,
				bfc_iterptr_t first,bfc_iterptr_t last);
static int vector_erase_element(bfc_vecptr_t vec, bfc_iterptr_t position);
static int vector_erase_range(bfc_vecptr_t vec,
				bfc_iterptr_t first,bfc_iterptr_t last);
static size_t vector_copy_out(bfc_cvecptr_t vec,void* buf,size_t n,size_t pos);
static void vector_swap(bfc_vecptr_t vec, bfc_vecptr_t other);

static void last_method(void) { }

struct bfc_vector_class {
	BFC_CONTAINER_CLASS_DEF(const struct bfc_vector_class *,
				bfc_vecptr_t, bfc_cvecptr_t, void)
};

struct bfc_vector_class bfc_vector_class = {
	.super 		= (void *) &bfc_object_class,
	.name 		= "generic vector",
	.init 		= bfc_init_vector_class,
	.destroy 	= destroy_vector,
	.clone 		= clone_vector,
	.clonesize 	= vector_objsize,
	.hashcode 	= vector_hashcode,
	.equals 	= vector_equals,
	.length 	= vector_size,
	.tostring 	= vector_tostring,
	.dump 		= dump_vector,
	/* Element access */
	.first		= NULL,
	.index		= NULL,
	.getl		= NULL,
	.setl		= NULL,
	.ibegin		= begin_iterator,
	.ilimit		= limit_iterator,
	.rbegin		= rbegin_iterator,
	.rlimit		= rlimit_iterator,
	/* Capacity */
	.size		= vector_size,
	.max_size	= vector_max_size,
	.resize		= vector_resize,
	.capacity	= vector_capacity,
	.reserve	= vector_reserve,
	/* Modifiers */
	.assign_fill	= vector_assign_fill,
	.assign_range	= vector_assign_range,
	.push_front	= vector_push_front,
	.pop_front	= vector_pop_front,
	.push_back	= vector_push_back,
	.pop_back	= vector_pop_back,
	.insert_element	= vector_insert_element,
	.insert_fill	= vector_insert_fill,
	.insert_range	= vector_insert_range,
	.erase_element	= vector_erase_element,
	.erase_range	= vector_erase_range,
	.copy_out	= vector_copy_out,
	.swap		= vector_swap,
	.last_method	= last_method
};

int
bfc_init_vector_class(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_vecptr_t vec = (bfc_vecptr_t) buf;
	BFC_INIT_PROLOGUE(const struct bfc_vector_class *,
			  bfc_objptr_t, obj, buf, bufsize, pool,
			  (bfc_classptr_t) &bfc_vector_class);
	if (bufsize < bfc_object_size(vec)) {
		return (-ENOSPC);
	}
	return (BFC_SUCCESS);
}

int
bfc_init_vector_copy(void *buf, size_t bufsize, const void *src)
{
	size_t size = bfc_object_size(src);
	bfc_vecptr_t vec = (bfc_vecptr_t) buf;
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(vec, src, size);
	vec->vptr = &bfc_vector_class;
	return (BFC_SUCCESS);
}

static void
destroy_vector(bfc_vecptr_t vec)
{
	vec->vptr = NULL;
}

static int
clone_vector(bfc_cvecptr_t vec, void *buf, size_t bufsize)
{
	size_t size = bfc_object_size(vec);
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(buf, vec, size);
	return (BFC_SUCCESS);
}

static size_t
vector_objsize(bfc_cvecptr_t vec)
{
	return (offsetof(struct bfc_generic_vector, direct)
		+ (vec->elem_direct * vec->elem_size));
}

static unsigned  
vector_hashcode(bfc_cvecptr_t vec)
{
	return (0);
}

static int
vector_equals(bfc_cvecptr_t vec, bfc_cvecptr_t other)
{
	if (vec == other) {
		return (1);
	}
	if (bfc_object_length(vec) != bfc_object_length(other)) {
		return (0);
	}
	return (memcmp (vec, other, sizeof(*vec)) == 0);
}

static size_t
vector_size(bfc_cvecptr_t vec)
{
	return (BFC_VECTOR_GET_SIZE(vec));
}

static int
vector_tostring(bfc_cvecptr_t vec, char *buf, size_t bufsize)
{
	if (vec && BFC_CLASS(vec)) {
		snprintf(buf, bufsize, "%s @%p",
			 BFC_CLASS(vec)->name, vec);
	}
	return (0);
}

static void
dump_vector(bfc_cvecptr_t vec, int depth, struct l4sc_logger *log)
{
	if (vec && BFC_CLASS(vec)) {
		L4SC_DEBUG(log, "%s @%p",
			 BFC_CLASS(vec)->name, vec);
	}
}

static int
begin_iterator(bfc_cvecptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	return (bfc_init_iterator(it, bufsize, (bfc_cobjptr_t)vec, 0));
}

static int
limit_iterator(bfc_cvecptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	int n = bfc_object_length(vec);
	size_t pos = (n > 0)? n: 0;
	return (bfc_init_iterator(it, bufsize, (bfc_cobjptr_t)vec, pos));
}

static int
rbegin_iterator(bfc_cvecptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	int n = bfc_object_length(vec);
	size_t pos = (n > 0)? (n-1): BFC_NPOS;
	return (bfc_init_reverse_iterator(it, bufsize,
					  (bfc_cobjptr_t)vec, pos));
}

static int
rlimit_iterator(bfc_cvecptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	return (bfc_init_reverse_iterator(it, bufsize,
					  (bfc_cobjptr_t)vec, BFC_NPOS));
}

static size_t
vector_max_size(bfc_cvecptr_t vec)
{
	return (BFC_VECTOR_MAX_SIZE(vec));
}

static int vector_resize(bfc_vecptr_t vec, size_t n, const void *p)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	if (n < size) {
		BFC_VECTOR_SET_SIZE(vec, n);
	} else if (n > size) {
		bfc_iterator_t it;
		bfc_container_end_iterator(vec, &it, sizeof(it));
		bfc_container_insert_fill((bfc_contptr_t) vec, &it, n-size, p);
		BFC_VECTOR_SET_SIZE(vec, n);
	}
	return (BFC_SUCCESS);
}

static size_t
vector_capacity(bfc_cvecptr_t vec)
{
	if (vec->triple_indirect) {
		return (CV3_BOUNDARY(vec));
	} else if (vec->double_indirect) {
		return (CV2_BOUNDARY(vec));
	} else if (vec->indirect) {
		return (CV1_BOUNDARY(vec));
	}
	return (CV0_BOUNDARY(vec));
}

static int
vector_reserve(bfc_vecptr_t vec, size_t n)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);

	if ((n > size) && (n > vector_capacity(vec))) {
		bfc_vector_have(vec, n-1);
		BFC_VECTOR_SET_SIZE(vec, size);
	}
	return (BFC_SUCCESS);
}

/* Modifiers */
static int vector_assign_fill(bfc_vecptr_t vec, size_t n, const void *p)
{
	return (-ENOSYS);
}
static int vector_assign_range(bfc_vecptr_t vec, bfc_iterptr_t first,bfc_iterptr_t last)
{
	return (-ENOSYS);
}
static int vector_push_front(bfc_vecptr_t vec, const void *p)
{
	return (-ENOSYS);
}
static void
vector_pop_front(bfc_vecptr_t vec)
{
}

static int
vector_push_back(bfc_vecptr_t vec, const void *p)
{
	return (-ENOSYS);
}

static void vector_pop_back(bfc_vecptr_t vec)
{
}

static int vector_insert_element(bfc_vecptr_t vec, bfc_iterptr_t position, const void *p)
{
	return (-ENOSYS);
}
static int vector_insert_fill(bfc_vecptr_t vec, bfc_iterptr_t position, size_t n, const void *p)
{
	return (-ENOSYS);
}
static int vector_insert_range(bfc_vecptr_t vec, bfc_iterptr_t position, bfc_iterptr_t first,bfc_iterptr_t last)
{
	return (-ENOSYS);
}
static int vector_erase_element(bfc_vecptr_t vec, bfc_iterptr_t position)
{
	return (-ENOSYS);
}
static int vector_erase_range(bfc_vecptr_t vec, bfc_iterptr_t first,bfc_iterptr_t last)
{
	return (-ENOSYS);
}
static size_t vector_copy_out(bfc_cvecptr_t vec,void* buf,size_t n,size_t pos)
{
	return (-ENOSYS);
}

static void
vector_swap(bfc_vecptr_t vec, bfc_vecptr_t other)
{
}
