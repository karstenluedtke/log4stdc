
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
#include "barefootc/synchronization.h"
#include "barefootc/unconst.h"
#include "log4stdc.h"

/* define struct bfc_container for the remainer of this file */
BFC_VECTOR(bfc_container, char, 0);

static int clone_vector(bfc_ccontptr_t vec, void *buf, size_t bufsize,
						struct mempool *pool);
static void destroy_vector(bfc_contptr_t vec);

static size_t vector_objsize(bfc_ccontptr_t vec);
static size_t vector_size(bfc_ccontptr_t vec);
static size_t vector_element_size(bfc_ccontptr_t vec);
static int vector_equals(bfc_ccontptr_t vec, bfc_ccontptr_t other);
static unsigned vector_hashcode(bfc_ccontptr_t vec, int hashlen);
static void dump_vector(bfc_ccontptr_t vec,int depth,struct l4sc_logger*log);
static int vector_tostring(bfc_ccontptr_t vec,
			   char *buf, size_t bufsize, const char *fmt);

static void *vector_index(bfc_contptr_t vec, size_t pos);
static const void *vector_first(bfc_ccontptr_t vec);
static long vector_getlong(bfc_ccontptr_t vec, size_t pos);
static int vector_setlong(bfc_contptr_t vec, size_t pos, long val);
static int begin_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize);
static int limit_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize);
static int rbegin_iterator(bfc_ccontptr_t vec,bfc_iterptr_t it, size_t bufsize);
static int rlimit_iterator(bfc_ccontptr_t vec,bfc_iterptr_t it, size_t bufsize);

static size_t vector_max_size(bfc_ccontptr_t vec);
static int vector_resize(bfc_contptr_t vec, size_t n, const void *p);
static size_t vector_capacity(bfc_ccontptr_t vec);
static int vector_reserve(bfc_contptr_t vec, size_t n);

static int vector_assign_fill(bfc_contptr_t vec, size_t n, const void *p);
static int vector_assign_range(bfc_contptr_t vec,
				bfc_iterptr_t first,bfc_iterptr_t last);
static int vector_push_front(bfc_contptr_t vec, const void *p);
static void vector_pop_front(bfc_contptr_t vec);
static int vector_push_back(bfc_contptr_t vec, const void *p);
static void vector_pop_back(bfc_contptr_t vec);
static int vector_insert_element(bfc_contptr_t vec, bfc_iterptr_t position,
				const void *p);
static int vector_insert_fill(bfc_contptr_t vec, bfc_iterptr_t position,
				size_t n, const void *p);
static int vector_insert_range(bfc_contptr_t vec, bfc_iterptr_t position,
				bfc_iterptr_t first,bfc_iterptr_t last);
static int vector_erase_element(bfc_contptr_t vec, bfc_iterptr_t position);
static int vector_erase_range(bfc_contptr_t vec,
				bfc_iterptr_t first,bfc_iterptr_t last);
static size_t vector_copy_out(bfc_ccontptr_t vec,void* buf,size_t n,size_t pos);
static void vector_swap(bfc_contptr_t vec, bfc_contptr_t other);

static void last_method(void) { }

struct bfc_vector_class {
	BFC_CONTAINER_CLASS_DEF(const struct bfc_vector_class *,
				bfc_contptr_t, bfc_ccontptr_t, void)
};

const struct bfc_vector_class bfc_vector_class = {
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
	.max_size	= vector_max_size,
	.capacity	= vector_capacity,
	.reserve	= vector_reserve,
	.element_size	= vector_element_size,
	/* Modifiers */
	.resize		= vector_resize,
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

static const size_t zeroelement[64] = { 0 };

int
bfc_init_vector_class(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_contptr_t vec = (bfc_contptr_t) buf;
	const int spare = (char *)vec + bufsize - (char *) vec->direct;
	/* be careful not to overwrite elem_size and elem_direct */
	BFC_INIT_PROLOGUE(const struct bfc_vector_class *,
			  bfc_objptr_t, obj, buf, bufsize, pool,
			  (bfc_classptr_t) &bfc_vector_class);
	if (bufsize < bfc_object_size(vec)) {
		return (-ENOSPC);
	}
	vec->pool = pool;
	bfc_new_mutex(&vec->lock, pool);
	if (spare > 0) {
		memset(vec->direct, 0, spare);
	}
	return (BFC_SUCCESS);
}

int
bfc_init_vector_by_element_size(void *buf, size_t bufsize,
				struct mempool *pool, size_t elem_size)
{
	BFC_VECTOR(minvec_s, char, 0) *vec = (struct minvec_s *) buf;
	const int spare = (char *)vec + bufsize - (char *) vec->direct;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	BFC_INIT_PROLOGUE(const struct bfc_vector_class *,
			  struct minvec_s *, obj, buf, bufsize, pool,
			  &bfc_vector_class);

	L4SC_TRACE(logger, "%s(vec @%p, size %ld, pool %p, elem_size %ld)",
			__FUNCTION__,vec,(long)bufsize,pool,(long)elem_size);

	vec->pool = pool;
	bfc_new_mutex(&vec->lock, pool);
	vec->elem_size = elem_size;
	vec->elem_direct = 0;
	if (bufsize >= bfc_object_size(vec)) {
		if (spare > 0) {
			vec->elem_direct = spare / elem_size;
			memset(vec->direct, 0, spare);
		} else {
			vec->elem_direct = 0;
		}
	} else {
		return (-ENOSPC);
	}
	vec->log2_indirect = 0;
	while (elem_size * CV1_ELEMENTS(vec) <= 4096) {
		vec->log2_indirect++;
	}
	vec->log2_double_indirect = 10; /* 1024 pointers per block */
	*((const size_t **) &vec->zero_element) = zeroelement;
	if ((vec->elem_size > sizeof(zeroelement)) && pool) {
		unsigned n = CV1_ELEMENTS(vec);
		vec->indirect = mempool_calloc(pool, n+1, vec->elem_size);
		if (vec->indirect) {
			vec->zero_element = vec->indirect + n * vec->elem_size;
		}
	}

	BFC_VECTOR_SET_SIZE(vec, 0);
	dump_vector((bfc_ccontptr_t) vec, 1, logger);
	return (BFC_SUCCESS);
}

int
bfc_init_vector_copy(void *buf, size_t bufsize, struct mempool *pool,
		     const struct bfc_container *src)
{
	int rc;
	bfc_contptr_t vec = (bfc_contptr_t) buf;
	size_t elem_size = bfc_container_element_size(src);
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, size %ld, pool %p, src @%p)",
			__FUNCTION__, buf, (long) bufsize, pool, src);

	rc = bfc_init_vector_by_element_size(buf, bufsize, pool, elem_size);
	if (rc < 0) {
		return (rc);
	}

	rc = bfc_container_assign_copy(vec, src);

	dump_vector(vec, 1, logger);
	return (rc);
}

int
bfc_init_vector_move(void *buf, size_t bufsize, struct bfc_container *src)
{
	int rc = BFC_SUCCESS;
	bfc_contptr_t vec = (bfc_contptr_t) buf;
	size_t clonesize = bfc_object_size(src);
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger,
		"%s(vec @%p, size %ld, src @%p): clonesize %ld",
		__FUNCTION__, buf, (long) bufsize, src, (long) clonesize);

	if (bufsize >= clonesize) {
		BFC_VECTOR_INIT_POOL(vec, src->pool);
		vec->vptr = &bfc_vector_class;
	} else {
		L4SC_ERROR(logger,
			"%s(vec @%p, size %ld, src @%p): need clonesize %ld",
			__FUNCTION__, buf, (long)bufsize, src, (long)clonesize);
		return (-ENOSPC);
	}

	if (src->lock && (locked = bfc_mutex_lock(src->lock))) {
		memcpy(vec, src, clonesize);
		bfc_incr_refcount(locked); /*another ref to lock in the copy*/
		BFC_VECTOR_SET_SIZE(src, 0);
		src->indirect = NULL;
		src->double_indirect = NULL;
		src->triple_indirect = NULL;
		if (CV0_ELEMENTS(src) > 0) {
			size_t offs = (char*)src->direct - (char*)src;
			if (clonesize > offs) {
				memset(src->direct, 0, clonesize - offs);
			}
		}
		dump_vector(vec, 1, logger);
		bfc_mutex_unlock(locked);
	} else {
		L4SC_ERROR(logger,
			"%s(vec @%p, size %ld, src @%p): cannot lock",
			__FUNCTION__, buf, (long)bufsize, src);
		rc = -EBUSY;
	}

	return (rc);
}

static void
destroy_vector(bfc_contptr_t vec)
{
	const struct bfc_vector_class *cls;
	bfc_mutex_ptr_t lock, locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if (vec && (cls = BFC_CLASS(vec))) {
		L4SC_DEBUG(logger, "%s: vec %p, lock %p, class %p",
			__FUNCTION__, vec, vec->lock,
			(vec->lock)? BFC_CLASS(vec->lock): 0);
		if ((lock = vec->lock) != NULL) {
			locked = bfc_mutex_lock(lock);
			vec->lock = NULL;
			BFC_VECTOR_DESTROY(vec);
			bfc_mutex_unlock(locked);
			bfc_decr_refcount(lock);
		}
		BFC_DESTROY_EPILOGUE(vec, cls);
	}
}

static int
clone_vector(bfc_ccontptr_t vec, void *buf,size_t bufsize,struct mempool *pool)
{
	bfc_contptr_t v = buf;
	struct mempool *newpool = pool? pool: vec->pool;
	const size_t elemsize = vec->elem_size;
	bfc_mutex_ptr_t locked;
	int rc;

	if (!vec->lock || !(locked = bfc_mutex_lock(vec->lock))) {
		return (-EBUSY);
	}
	rc = bfc_init_vector_by_element_size(buf, bufsize, newpool, elemsize);
	if (rc < 0) {
		bfc_mutex_unlock(locked);
		return (rc);
	}
	v->vptr = vec->vptr;
	v->elem_class = vec->elem_class;
	rc = bfc_container_assign_copy(v, vec);
	bfc_mutex_unlock(locked);
	return (rc);
}

static size_t
vector_objsize(bfc_ccontptr_t vec)
{
	size_t minsize = offsetof(struct bfc_char_vector, direct), extra = 0;
	if (vec && BFC_CLASS(vec)) {
		extra = vec->elem_direct * vec->elem_size;
	}
	return (minsize + extra);
}

static unsigned  
vector_hashcode(bfc_ccontptr_t vec, int hashlen)
{
	return (0);
}

static int
vector_equals(bfc_ccontptr_t vec, bfc_ccontptr_t other)
{
	int eq;
	size_t idx, size;
	const void *p, *q;
	bfc_mutex_ptr_t lock1 = NULL, lock2 = NULL;

	if (vec == other) {
		return (1);
	}
	if (BFC_VECTOR_GET_SIZE(vec) != bfc_object_length(other)) {
		return (0);
	}
	if (vec->lock) {
		lock1 = bfc_mutex_lock(vec->lock);
	}
	if (other->lock) {
		lock2 = bfc_mutex_lock(other->lock);
	}
	size = BFC_VECTOR_GET_SIZE(vec);
	for (idx=0, eq=1; idx < size; idx++) {
		p = bfc_container_index(BFC_UNCONST(bfc_contptr_t,vec), idx);
		q = bfc_container_index(BFC_UNCONST(bfc_contptr_t,other), idx);
		if (p && q) {
			if (memcmp(p, q, vec->elem_size) != 0) {
				eq = 0;
				break;
			}
		} else if (p != q) /* one of them != NULL */ {
			eq = 0;
			break;
		}
	}
	if (lock2) {
		bfc_mutex_unlock(lock2);
	}
	if (lock1) {
		bfc_mutex_unlock(lock1);
	}
	return (eq);
}

static size_t
vector_size(bfc_ccontptr_t vec)
{
	return (BFC_VECTOR_GET_SIZE(vec));
}

static size_t
vector_element_size(bfc_ccontptr_t vec)
{
	return (vec->elem_size);
}

static int
vector_tostring(bfc_ccontptr_t vec, char *buf, size_t bufsize, const char *fmt)
{
	if (vec && BFC_CLASS(vec)) {
		snprintf(buf, bufsize, "%s @%p",
			 BFC_CLASS(vec)->name, vec);
	}
	return (0);
}

static void
dump_double_loop(bfc_ccontptr_t vec, char **dp,
		 const char *label, struct l4sc_logger *log)
{
	int di;
	char *ip;
	const int __dblcnt = CV2_POINTERS(vec);

	L4SC_DEBUG(log, "%6ld %s elements @%p",
			(long)CV2_ELEMENTS(vec), label, dp);
	for (di=0; di < __dblcnt; di++) {
		if ((ip = dp[di]) != NULL) {
			L4SC_DEBUG(log, "%6ld %s elements @%p",
				    (long)CV1_ELEMENTS(vec), label, ip);
		}
	}
}

void
bfc_vector_dump_structure(const void *obj, struct l4sc_logger *log)
{
	bfc_ccontptr_t vec = (bfc_ccontptr_t) obj;
	int ti;
	char *ip, **dp, ***tp;
	const int __dblcnt = CV2_POINTERS(vec);

	if (vec && BFC_CLASS(vec)) {
		L4SC_DEBUG(log, "%s @%p size %ld, pool %p, lock %p",
			BFC_CLASS(vec)->name, vec, 
			(long)BFC_VECTOR_GET_SIZE(vec), vec->pool, vec->lock);

		L4SC_DEBUG(log, "%6ld direct elements @%p size %d",
			(long)CV0_ELEMENTS(vec), vec->direct, vec->elem_size);

		if ((ip = vec->indirect) != NULL) {
			L4SC_DEBUG(log, "%6ld indirect elements @%p",
				(long)CV1_ELEMENTS(vec), ip);
		}

		if ((dp = vec->double_indirect) != NULL) {
			dump_double_loop(vec, dp, "double indirect", log);
		}
	
		if ((tp = vec->triple_indirect) != NULL) {
			L4SC_DEBUG(log, "%6ld triple indirect elements @%p",
				(long)CV3_ELEMENTS(vec), tp);
			for (ti=0; ti < __dblcnt; ti++) {
				if ((dp = tp[ti]) != NULL) {
					dump_double_loop(vec, dp,
						"triple indirect", log);
				}
			}
		}
	}
}

static void
dump_vector(bfc_ccontptr_t vec, int depth, struct l4sc_logger *log)
{
	bfc_mutex_ptr_t locked = NULL;

	if (vec && BFC_CLASS(vec)) {
		if (vec->lock) {
			locked = bfc_mutex_lock(vec->lock);
		}
		bfc_vector_dump_structure(vec, log);
		if (locked) {
			bfc_mutex_unlock(locked);
		}
	}
}

// element access:
static void *
vector_index(bfc_contptr_t vec, size_t pos)
{
	bfc_mutex_ptr_t locked;
	void *p = NULL;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref(vec, pos);
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld): cannot lock",
				__FUNCTION__, vec, (long) pos);
	}
	return (p);
}

static const void *
vector_first(bfc_ccontptr_t vec)
{
	bfc_mutex_ptr_t locked;
	void *p = NULL;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref(BFC_UNCONST(bfc_contptr_t, vec), 0);
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p): cannot lock", __FUNCTION__, vec);
	}
	return (p);
}

static long
vector_getlong(bfc_ccontptr_t vec, size_t pos)
{
	void *p;
	long v = 0;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		p = bfc_vector_ref((void *)(uintptr_t)vec, 0);
		if (p) {
			if (vec->elem_size == sizeof(long)) {
				v = *(const long *)p;
			} else if (vec->elem_size == sizeof(int)) {
				v = *(const int *)p;
			} else if (vec->elem_size == sizeof(short)) {
				v = *(const short *)p;
			} else if (vec->elem_size == sizeof(char)) {
				v = *(const char *)p;
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld): cannot lock",
				__FUNCTION__, vec, (long) pos);
	}
	return (v);
}

static int
vector_setlong(bfc_contptr_t vec, size_t pos, long val)
{
	bfc_mutex_ptr_t locked;
	void *p;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		if ((pos == BFC_NPOS) || (pos > BFC_VECTOR_GET_SIZE(vec))) {
			rc = -ERANGE;
		} else if ((p = bfc_vector_have(vec, pos)) == NULL) {
			rc = -ENOMEM;
		} else if (val == 0) {
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
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld, %ld): cannot lock",
				__FUNCTION__, vec, (long) pos, val);
		rc = -EBUSY;
	}
	return (rc);
}

static int
begin_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	return (bfc_init_vector_iterator(it, bufsize, (bfc_cobjptr_t)vec, 0));
}

static int
limit_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	int n = BFC_VECTOR_GET_SIZE(vec);
	size_t pos = (n > 0)? n: 0;
	return (bfc_init_iterator(it, bufsize, (bfc_cobjptr_t)vec, pos));
}

static int
rbegin_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	int n = BFC_VECTOR_GET_SIZE(vec);
	size_t pos = (n > 0)? (n-1): BFC_NPOS;
	return (bfc_init_reverse_iterator(it, bufsize,
					  (bfc_cobjptr_t)vec, pos));
}

static int
rlimit_iterator(bfc_ccontptr_t vec, bfc_iterptr_t it, size_t bufsize)
{
	return (bfc_init_reverse_iterator(it, bufsize,
					  (bfc_cobjptr_t)vec, BFC_NPOS));
}

static size_t
vector_max_size(bfc_ccontptr_t vec)
{
	return (BFC_VECTOR_MAX_SIZE(vec));
}

static int vector_resize(bfc_contptr_t vec, size_t n, const void *p)
{
	size_t size;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (n < size) {
			BFC_VECTOR_SET_SIZE(vec, n);
		} else if (n > size) {
			while (size < n) {
				bfc_container_push_back(vec, p);
				size++;
			}
			BFC_VECTOR_SET_SIZE(vec, n);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld, %p): cannot lock",
				__FUNCTION__, vec, (long) n, p);
		rc = -EBUSY;
	}
	return (rc);
}

static size_t
vector_capacity(bfc_ccontptr_t vec)
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
vector_reserve(bfc_contptr_t vec, size_t n)
{
	size_t size;
	bfc_mutex_ptr_t locked;
	int rc = BFC_SUCCESS;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if ((n > size) && (n > vector_capacity(vec))) {
			bfc_vector_have(vec, n-1);
			BFC_VECTOR_SET_SIZE(vec, size);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld): cannot lock",
				__FUNCTION__, vec, (long) n);
		rc = -EBUSY;
	}
	return (rc);
}

/* Modifiers */
static int
vector_assign_fill(bfc_contptr_t vec, size_t n, const void *p)
{
	int rc = 0, count = 0;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		bfc_container_resize(vec, 0, NULL);
		if ((rc = bfc_container_reserve(vec, n)) >= 0) {
			while (count < (int) n) {
				bfc_container_push_back(vec, p);
				count++;
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %ld, %p): cannot lock",
				__FUNCTION__, vec, (long) n, p);
		rc = -EBUSY;
	}
	return ((rc < 0)? rc: count);
}

static int
vector_assign_range(bfc_contptr_t vec, bfc_iterptr_t first, bfc_iterptr_t last)
{
	int rc;
	bfc_iterator_t it;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		bfc_container_resize(vec, 0, NULL);
		bfc_container_end_iterator(vec, &it, sizeof(it));
		rc = bfc_container_insert_range(vec, &it, first, last);
		if (rc >= 0) {
			rc = BFC_VECTOR_GET_SIZE(vec);
			if (rc < 0) {
				rc = BFC_SUCCESS;
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %p): cannot lock",
				__FUNCTION__, vec, first, last);
		rc = -EBUSY;
	}
	return (rc);
}

static int
vector_push_front(bfc_contptr_t vec, const void *p)
{
	return (-ENOSYS);
}

static void
vector_pop_front(bfc_contptr_t vec)
{
}

static int
vector_push_back(bfc_contptr_t vec, const void *p)
{
	size_t size;
	void *ref;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	int rc = 0;

	L4SC_TRACE(logger, "%s(vec @%p, elem @%p)", __FUNCTION__, vec, p);

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if ((ref = bfc_vector_have(vec, size)) != NULL) {
			size++;
			BFC_VECTOR_SET_SIZE(vec, size);
			if (p) {
				memcpy(ref, p, vec->elem_size);
			} else {
				memset(ref, 0, vec->elem_size);
			}
			rc = (int) size;
			if (rc < 0) {
				rc = BFC_SUCCESS;
			}
		} else {
			rc = -ENOMEM;
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p): cannot lock",__FUNCTION__,vec,p);
		rc = -EBUSY;
	}
	return (rc);
}

static void
vector_pop_back(bfc_contptr_t vec)
{
	size_t size;
	bfc_mutex_ptr_t locked;

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (size > 0) {
			bfc_container_resize(vec, size-1, NULL);
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p): cannot lock",__FUNCTION__, vec);
	}
}

static int
vector_insert_element(bfc_contptr_t vec, bfc_iterptr_t pos, const void *p)
{
	return (vector_insert_fill(vec, pos, 1, p));
}

static void
zero_vector_elements(bfc_contptr_t vec, size_t pos, size_t limit)
{
	size_t idx;
	void *ref;

	for (idx = pos; idx < limit; idx++) {
		if ((ref = bfc_vector_ref(vec, idx)) != NULL) {
			memset(ref, 0, vec->elem_size);
		}
	}
}

static int
init_vector_elements(bfc_contptr_t vec, size_t pos, size_t limit, const void *p)
{
	size_t idx;
	void *ref;
	int rc = BFC_SUCCESS;

	for (idx = pos; idx < limit; idx++) {
		if ((ref = bfc_vector_have(vec, idx)) != NULL) {
			memcpy(ref, p, vec->elem_size);
		} else {
			rc = -ENOMEM;
			break;
		}
	}
	return (rc);
}

static int
move_vector_elements(bfc_contptr_t vec, size_t pos, size_t limit, size_t dest)
{
	size_t idx, n;
	void *cp, *dp;
	int rc = BFC_SUCCESS;

	if (dest < pos) {
		n = pos - dest;
		for (idx = pos; idx < limit; idx++) {
			if ((cp = bfc_vector_ref(vec, idx)) != NULL) {
				if ((dp = bfc_vector_have(vec, idx-n))!= NULL) {
					memcpy(dp, cp, vec->elem_size);
					memset(cp,  0, vec->elem_size);
				} else {
					rc = -ENOMEM;
					break;
				}
			}
		}
	} else if (dest > pos) {
		n = dest - pos;
		for (idx = limit; (idx--) > pos; ) {
			if ((cp = bfc_vector_ref(vec, idx)) != NULL) {
				if ((dp = bfc_vector_have(vec, idx+n))!= NULL) {
					memcpy(dp, cp, vec->elem_size);
					memset(cp,  0, vec->elem_size);
				} else {
					rc = -ENOMEM;
					break;
				}
			}
		}
	}
	return (rc);
}

static int
vector_insert_fill(bfc_contptr_t vec, bfc_iterptr_t position, size_t n,
		   const void *p)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t pos = bfc_iterator_position(position);
	int rc = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_mutex_ptr_t locked;

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %ld, %p): pos %ld/%ld",
		__FUNCTION__, vec, position, (long)n, p, (long)pos, (long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);

	if (n < 1) {
		return (BFC_SUCCESS);
	}

	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (((rc = bfc_container_reserve(vec, size+n)) >= 0)
		 && ((rc = move_vector_elements(vec, pos, size, pos+n)) >= 0)) {
			size += n;
			BFC_VECTOR_SET_SIZE(vec, size);
			if (p) {
				init_vector_elements(vec, pos, pos+n, p);
			} else {
				zero_vector_elements(vec, pos, pos+n);
			}
			rc = (int) size;
			if (rc < 0) {
				rc = BFC_SUCCESS;
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %ld, %p): cannot lock",
				__FUNCTION__, vec, position, (long) n, p);
		rc = -EBUSY;
	}
	return (rc);
}

static int
vector_insert_range(bfc_contptr_t vec, bfc_iterptr_t position,
		    bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t idx, pos = bfc_iterator_position(position);
	ptrdiff_t offs, n = bfc_iterator_distance(first, last);
	bfc_mutex_ptr_t locked;
	void *ref, *src;
	int rc;
	bfc_iterator_t it;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %p, %p): pos %ld/%ld",
		__FUNCTION__, vec, position, first, last,(long)pos,(long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(position, 1, logger);
	bfc_object_dump(first, 1, logger);
	bfc_object_dump(last, 1, logger);

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (((rc = bfc_container_reserve(vec, size + n)) >= 0)
		 && ((rc = move_vector_elements(vec, pos, size, pos+n)) >= 0)) {
			size += n;
			BFC_VECTOR_SET_SIZE(vec, size);
			rc = (int) size;
			if (rc < 0) {
				rc = BFC_SUCCESS;
			}
			it = *first;
			idx = pos;
			for ( ; bfc_iterator_distance(&it, last) > 0; idx++) {
				src = bfc_iterator_index(&it);
				offs = bfc_iterator_distance(first, &it);
				bfc_iterator_advance(&it, 1);
				if ((offs > 0) && (idx < (size_t)(pos+offs))) {
					idx = pos + offs;
				}
				if (src) {
					ref = bfc_vector_have(vec, idx);
					if (ref) {
						memcpy(ref,src,vec->elem_size);
					} else {
						rc = -ENOMEM;
						break;
					}
				}
			}
		}
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %p, %p): cannot lock",
				__FUNCTION__, vec, position, first, last);
		rc = -EBUSY;
	}
	return (rc);
}

static int
vector_erase_element(bfc_contptr_t vec, bfc_iterptr_t position)
{
	bfc_iterator_t limit = *position;
	bfc_iterator_advance(&limit, 1);
	return (vector_erase_range(vec, position, &limit));
}

static int
vector_erase_range(bfc_contptr_t vec, bfc_iterptr_t first, bfc_iterptr_t last)
{
	size_t size = BFC_VECTOR_GET_SIZE(vec);
	size_t pos = bfc_iterator_position(first);
	int n = bfc_iterator_distance(first, last);
	int rc = BFC_SUCCESS;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(vec @%p, it @%p, %p): pos %ld+%ld/%ld",
		__FUNCTION__, vec, first, last, (long)pos, (long)n, (long)size);
	bfc_object_dump(vec, 1, logger);
	bfc_object_dump(first, 1, logger);
	bfc_object_dump(last, 1, logger);

	if (n < 1) {
		return ((n == 0)? BFC_SUCCESS: -EINVAL);
	}
	if (vec->lock && (locked = bfc_mutex_lock(vec->lock))) {
		size = BFC_VECTOR_GET_SIZE(vec);
		if (pos+n < size) {
			move_vector_elements(vec, pos+n, size, pos);
			size -= n;
		} else {
			size = pos;
		}
		rc = bfc_container_resize(vec, size, NULL);
		bfc_mutex_unlock(locked);
	} else {
		l4sc_logger_ptr_t log = l4sc_get_logger(BFC_CONTAINER_LOGGER);
		L4SC_ERROR(log, "%s(%p, %p, %p): cannot lock",
				__FUNCTION__, vec, first, last);
		rc = -EBUSY;
	}
	return (rc);
}

static size_t vector_copy_out(bfc_ccontptr_t vec,void* buf,size_t n,size_t pos)
{
	return (-ENOSYS);
}

static void
vector_swap(bfc_contptr_t vec, bfc_contptr_t other)
{
}
