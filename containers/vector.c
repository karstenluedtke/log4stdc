
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

#define BFC_VECTOR_ENABLE_TRIPLE_INDIRECT	1
#define CV_ELEMSIZE_FACTOR(vec)		((vec)->elem_size / sizeof(char))

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/vector.h"
#include "log4stdc.h"

/* for the remainder of this file ... */
typedef struct bfc_char_vector *bfc_vecptr_t;
typedef const struct bfc_char_vector *bfc_cvecptr_t;

static int clone_vector(bfc_cvecptr_t vec, void *buf, size_t bufsize);
static void destroy_vector(bfc_vecptr_t vec);

static size_t vector_objsize(bfc_cvecptr_t vec);
static size_t vector_size(bfc_cvecptr_t vec);
static int vector_equals(bfc_cvecptr_t vec, bfc_cvecptr_t other);
static unsigned vector_hashcode(bfc_cvecptr_t vec);
static void dump_vector(bfc_cvecptr_t vec,int depth,struct l4sc_logger*log);
static int vector_tostring(bfc_cvecptr_t vec, char *buf, size_t bufsize);

static void *vector_index(bfc_vecptr_t vec, size_t pos);
static const void *vector_first(bfc_cvecptr_t vec);
static long vector_getlong(bfc_cvecptr_t vec, size_t pos);
static int vector_setlong(bfc_vecptr_t vec, size_t pos, long val);
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
	.first		= vector_first,
	.index		= vector_index,
	.getl		= vector_getlong,
	.setl		= vector_setlong,
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
	vec->pool = pool;
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
	return (offsetof(struct bfc_char_vector, direct)
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
	int di, ti;
	char *ip, **dp, ***tp;
	const int __dblcnt = CV2_POINTERS(vec);

	if (vec && BFC_CLASS(vec)) {
		L4SC_DEBUG(log, "%s @%p size %ld, pool %p",
			BFC_CLASS(vec)->name, vec,
			(long)BFC_VECTOR_GET_SIZE(vec), vec->pool);
		L4SC_DEBUG(log, "%6ld direct elements @%p size %d",
			(long)CV0_ELEMENTS(vec), vec->direct, vec->elem_size);

		if ((ip = vec->indirect) != NULL) {
			L4SC_DEBUG(log, "%6ld indirect elements @%p",
				(long)CV1_ELEMENTS(vec), ip);
		}

		if ((dp = (vec)->double_indirect) != NULL) {
			L4SC_DEBUG(log, "%6ld double indirect elements @%p",
				(long)CV2_ELEMENTS(vec), dp);
			for (di=0; di < __dblcnt; di++) {
				if ((ip = dp[di]) != NULL) {
					L4SC_DEBUG(log,
					    "%6ld double indirect elements @%p",
					    (long)CV1_ELEMENTS(vec), ip);
				}
			}
		}
	
		if ((tp = (vec)->triple_indirect) != NULL) {
			L4SC_DEBUG(log, "%6ld triple indirect elements @%p",
				(long)CV3_ELEMENTS(vec), tp);
			for (ti=0; ti < __dblcnt; ti++) {
				if ((dp = tp[ti]) != NULL) {
					L4SC_DEBUG(log,
					    "%6ld triple indirect elements @%p",
					    (long)CV2_ELEMENTS(vec), dp);
					for (di=0; di < __dblcnt; di++) {
						if ((ip = dp[di]) != NULL) {
							L4SC_DEBUG(log,
							    "%6ld triple indirect elements @%p",
							    (long)CV1_ELEMENTS(vec), ip);
						}
					}
				}
			}
		}
	}
}

// element access:
static void *
vector_index(bfc_vecptr_t vec, size_t pos)
{
	void *p = bfc_vector_ref(vec, pos);
	return (p);
}

static const void *
vector_first(bfc_cvecptr_t vec)
{
	void *p = bfc_vector_ref((void *)(uintptr_t)vec, 0);
	return (p);
}

static long
vector_getlong(bfc_cvecptr_t vec, size_t pos)
{
	void *p = bfc_vector_ref((void *)(uintptr_t)vec, 0);
	if (p) {
		if (vec->elem_size == sizeof(long)) {
			return (*(const long *)p);
		} else if (vec->elem_size == sizeof(int)) {
			return (*(const int *)p);
		} else if (vec->elem_size == sizeof(short)) {
			return (*(const short *)p);
		} else if (vec->elem_size == sizeof(char)) {
			return (*(const char *)p);
		}
	}
	return (0);
}

static int
vector_setlong(bfc_vecptr_t vec, size_t pos, long val)
{
	void *p;

	if ((pos == BFC_NPOS) || (pos > bfc_object_length(vec))) {
		return (-ERANGE);
	}
	if ((p = bfc_vector_have(vec, pos)) == NULL) {
		return (-ENOMEM);
	}
	if (val == 0) {
		memset(p, 0, vec->elem_size);
	} else if (vec->elem_size == sizeof(long)) {
		*(long *)p = val;
	} else if (vec->elem_size == sizeof(int)) {
		*(int *)p = (int) val;
	} else if (vec->elem_size == sizeof(short)) {
		*(short *)p = (short) val;
	} else if (vec->elem_size == sizeof(char)) {
		*(char *)p = (char) val;
	} else {
		signed long x = val;
		char *cp = (char *) p;
		char *ep = cp + vec->elem_size;
		static const long endiantest = 0x12;
		if (*(const char *)&endiantest == 0x12) {
			while (cp < ep) {	/* little endian */
				*(cp++) = (char) x;
				x >>= sizeof(char);
			}
		} else {
			while (ep > cp) {	/* big endian */
				*(--ep) = (char) x;
				x >>= sizeof(char);
			}
		}
	}
	return (BFC_SUCCESS);
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
		while (size < n) {
			bfc_container_push_back((bfc_contptr_t) vec, p);
			size++;
		}
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
static int
vector_assign_fill(bfc_vecptr_t vec, size_t n, const void *p)
{
	int rc, count = 0;

	bfc_container_resize((bfc_contptr_t) vec, 0, NULL);
	if ((rc = bfc_container_reserve((bfc_contptr_t) vec, n)) < 0) {
		return (rc);
	}
	while (count < (int) n) {
		bfc_container_push_back((bfc_contptr_t) vec, p);
		count++;
	}
	return (count);
}

static int
vector_assign_range(bfc_vecptr_t vec, bfc_iterptr_t first, bfc_iterptr_t last)
{
	int count = 0;
	bfc_iterator_t it = *first;

	bfc_container_resize((bfc_contptr_t) vec, 0, NULL);
	while (bfc_iterator_distance(&it, last) > 0) {
		const void *p = bfc_iterator_index(&it);
		bfc_iterator_advance(&it, 1);
		bfc_container_push_back((bfc_contptr_t) vec, p);
		count++;
	}
	return (count);
}

static int
vector_push_front(bfc_vecptr_t vec, const void *p)
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
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	void *ref;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, elem @%p)", __FUNCTION__, vec, p);
	dump_vector(vec, 1, logger);

	if ((ref = bfc_vector_have(vec, size)) == NULL) {
		return (-ENOMEM);
	}
	size++;
	BFC_VECTOR_SET_SIZE(vec, size);
	if (p) {
		memcpy(ref, p, vec->elem_size);
	} else {
		memset(ref, 0, vec->elem_size);
	}
	dump_vector(vec, 1, logger);
	return ((int) size);
}

static void
vector_pop_back(bfc_vecptr_t vec)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);

	if (size > 0) {
		bfc_container_resize((bfc_contptr_t) vec, size-1, NULL);
	}
}

static int
vector_insert_element(bfc_vecptr_t vec, bfc_iterptr_t pos, const void *p)
{
	return (vector_insert_fill(vec, pos, 1, p));
}

static int
vector_insert_fill(bfc_vecptr_t vec, bfc_iterptr_t position, size_t n,
		   const void *p)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(position);
	void *ref, *src;
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %ld, %p): pos %ld/%ld",
		__FUNCTION__, vec, position, (long)n, p, (long)pos, (long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);

	if (n < 1) {
		return (BFC_SUCCESS);
	}
	if ((rc = bfc_container_reserve((bfc_contptr_t) vec, size+n)) < 0) {
		return (rc);
	}
	for (idx = pos; idx < size; idx++) {
		if (((src = bfc_vector_ref(vec, idx)) != NULL)
		 && ((ref = bfc_vector_have(vec, idx+n)) != NULL)) {
			memcpy(ref, src, vec->elem_size);
		}
	}
	size += n;
	BFC_VECTOR_SET_SIZE(vec, size);
	if (p) {
		for (idx = pos; idx < pos+n; idx++) {
			if ((ref = bfc_vector_have(vec, idx)) != NULL) {
				memcpy(ref, p, vec->elem_size);
			}
		}
	} else {
		for (idx = pos; idx < pos+n; idx++) {
			if ((ref = bfc_vector_have(vec, idx)) != NULL) {
				memset(ref, 0, vec->elem_size);
			}
		}
	}
	rc = (int) size;
	return ((rc > 0)? rc: BFC_SUCCESS);
}

static int
vector_insert_range(bfc_vecptr_t vec, bfc_iterptr_t position,
		    bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(position);
	int n = bfc_iterator_distance(first, last);
	void *ref, *src;
	int rc;
	bfc_iterator_t it;

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	if ((rc = bfc_container_reserve((bfc_contptr_t) vec, size+n)) < 0) {
		return (rc);
	}
	for (idx = pos; idx < size; idx++) {
		if (((src = bfc_vector_ref(vec, idx)) != NULL)
		 && ((ref = bfc_vector_have(vec, idx+n)) != NULL)) {
			memcpy(ref, src, vec->elem_size);
		}
	}
	size += n;
	BFC_VECTOR_SET_SIZE(vec, size);

	it = *first;
	for (idx = pos; bfc_iterator_distance(&it, last) > 0; idx++) {
		src = bfc_iterator_index(&it);
		bfc_iterator_advance(&it, 1);
		if (src && ((ref = bfc_vector_have(vec, idx)) != NULL)) {
			memcpy(ref, src, vec->elem_size);
		}
	}
	return (-ENOSYS);
}

static int
vector_erase_element(bfc_vecptr_t vec, bfc_iterptr_t position)
{
	bfc_iterator_t limit = *position;
	bfc_iterator_advance(&limit, 1);
	return (vector_erase_range(vec, position, &limit));
}

static int
vector_erase_range(bfc_vecptr_t vec, bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(first);
	int n = bfc_iterator_distance(first, last);
	void *ref, *src;

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	if (n < (int) size) {
		for (idx = size-n-1; idx >= pos; idx--) {
			if (((src = bfc_vector_ref(vec, idx+n)) != NULL)
			 && ((ref = bfc_vector_have(vec, idx)) != NULL)) {
				memcpy(ref, src, vec->elem_size);
			}
		}
		size -= n;
	} else {
		size = 0;
	}
	return (bfc_container_resize((bfc_contptr_t) vec, size, NULL));
}

static size_t vector_copy_out(bfc_cvecptr_t vec,void* buf,size_t n,size_t pos)
{
	return (-ENOSYS);
}

static void
vector_swap(bfc_vecptr_t vec, bfc_vecptr_t other)
{
}
