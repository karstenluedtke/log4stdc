
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

static int init_vector(void *buf, size_t bufsize, struct mempool *pool);
static int clone_vector(bfc_cvecptr_t vec, void *buf, size_t bufsize);
static void destroy_vector(bfc_vecptr_t vec);

static size_t vector_objsize(bfc_cvecptr_t vec);
static size_t vector_size(bfc_cvecptr_t vec);
static int vector_equals(bfc_cvecptr_t vec, bfc_cvecptr_t other);
static unsigned vector_hashcode(bfc_cvecptr_t vec);
static void dump_vector(bfc_cvecptr_t vec,int depth,struct l4sc_logger*log);
static int vector_tostring(bfc_cvecptr_t vec, char *buf, size_t bufsize);

static void last_method(void) { }

struct bfc_vector_class {
	BFC_CONTAINER_CLASS_DEF(const struct bfc_vector_class *,
				bfc_vecptr_t, bfc_cvecptr_t, void *)
};

struct bfc_vector_class bfc_vector_class = {
	.super 		= (void *) &bfc_object_class,
	.name 		= "generic vector",
	.init 		= init_vector,
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
	.ibegin		= NULL,
	.ilimit		= NULL,
	.rbegin		= NULL,
	.rlimit		= NULL,
	/* modifiers */	
	.last_method	= last_method
};

static int
init_vector(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_vecptr_t vec = (bfc_vecptr_t) buf;
	if (bufsize < sizeof(*vec)) {
		return (-ENOSPC);
	}
	memset(vec, 0, sizeof(*vec));
	vec->vptr = &bfc_vector_class;
	return (BFC_SUCCESS);
}

int
bfc_init_vector_copy(void *buf, size_t bufsize, const void *src)
{
	bfc_vecptr_t vec = (bfc_vecptr_t) buf;
	if (bufsize < sizeof(*vec)) {
		return (-ENOSPC);
	}
	memcpy(vec, src, sizeof(*vec));
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
	if (bfc_object_size(vec) != bfc_object_size(other)) {
		return (0);
	}
	return (memcmp (vec, other, sizeof(*vec)) == 0);
}

static size_t
vector_size(bfc_cvecptr_t vec)
{
	return (vec->size);
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

