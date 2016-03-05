
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/mempool.h"

#include "log4stdc.h"

static int  init_pool(void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pool(struct mempool *pool);
static int  clone_pool(const struct mempool *pool, void *buf, size_t bufsize,
							struct mempool *other);
static size_t get_pool_object_size(const struct mempool *pool);
static unsigned get_pool_hashcode(const struct mempool *pool, int hashlen);
static int is_equal_pool(const struct mempool *obj,const struct mempool *other);
static size_t get_pool_size(const struct mempool *pool);
static int pool_tostring(const struct mempool *pool,
			 char *buf, size_t bufsize, const char *fmt);

/*
 *  Standard "C" mempool.
 *  malloc(3), free(3), ...
 */

static void *
stdc_pool_malloc (struct mempool *pool, size_t size,
		  const char *file, int line, const char *func);
static void *
stdc_pool_calloc (struct mempool *pool, int elements, size_t objsize,
		  const char *file, int line, const char *func);
static void *
stdc_pool_realloc (struct mempool *pool, void *ptr, size_t size,
		  const char *file, int line, const char *func);
static void
stdc_pool_free (struct mempool *pool, void *ptr,
		  const char *file, int line, const char *func);
static void
stdc_pool_dump (const struct mempool *pool, int depth, struct l4sc_logger *log);

struct stdc_mempool {
	BFC_CHAINEDPOOLHDR(bfc_mempool_class_ptr_t, struct mempool *)
};

const struct bfc_mempool_class l4sc_stdc_mempool_class = {
	/* .super 	*/ NULL,
	/* .name 	*/ "stdc_mempool",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_pool,
	/* .initrefc 	*/ NULL,
	/* .incrrefc 	*/ NULL,
	/* .decrrefc 	*/ NULL,
	/* .destroy 	*/ destroy_pool,
	/* .clone 	*/ clone_pool,
	/* .clonesize 	*/ get_pool_object_size,
	/* .hashcode 	*/ get_pool_hashcode,
	/* .equals 	*/ is_equal_pool,
	/* .length 	*/ get_pool_size,
	/* .tostring 	*/ pool_tostring,
	/* .dump 	*/ stdc_pool_dump,
	/* .alloc	*/ stdc_pool_malloc,
	/* .calloc	*/ stdc_pool_calloc,
	/* .realloc	*/ stdc_pool_realloc,
	/* .free	*/ stdc_pool_free
};

struct stdc_mempool l4sc_stdc_mempool = {
	/* .vptr	*/ &l4sc_stdc_mempool_class,
	/* .name	*/ "stdc_mempool",
	/* .refc	*/ 20000, 
	/* .lock	*/ NULL,
	/* .parent_pool	*/ NULL,
	/* .next	*/ NULL,
	/* .prev	*/ NULL,
	/* .line	*/ 0,
	/* .file	*/ "libc",
	/* .func	*/ "malloc",
	/* .subpools	*/ { NULL, NULL }
};

struct mempool *l4sc_default_mempool = (struct mempool *) &l4sc_stdc_mempool;

static int
init_pool(void *buf, size_t bufsize, struct mempool *pool)
{
	struct stdc_mempool *p = (struct stdc_mempool *) buf;
	BFC_INIT_PROLOGUE(bfc_mempool_class_ptr_t,
			  struct stdc_mempool *, stdcpool, buf, bufsize, pool,
			  &l4sc_stdc_mempool_class);
	p->refc = 20000;
	p->file = __FILE__;
	p->line = __LINE__;
	p->func = __FUNCTION__;
	return (BFC_SUCCESS);
}

static void
destroy_pool(struct mempool *pool)
{
	struct stdc_mempool *obj = (struct stdc_mempool *) pool;
	bfc_mempool_class_ptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
clone_pool(const struct mempool *pool, void *buf, size_t bufsize,
						struct mempool *other)
{
	struct mempool *newpool = (struct mempool *) buf;
	size_t size;
	RETVAR_METHCALL(size, bfc_mempool_class_ptr_t, pool, clonesize, (pool),
			sizeof(struct stdc_mempool));
	if (bufsize < size) {
		return (-ENOSPC);
	}
	memcpy(newpool, pool, size);
	newpool->next = NULL;
	newpool->prev = NULL;
	memset(&newpool->sub_pools, 0, sizeof(newpool->sub_pools));
	
	return (BFC_SUCCESS);
}

static size_t
get_pool_object_size(const struct mempool *pool)
{
	return (sizeof(struct stdc_mempool));
}

static unsigned
get_pool_hashcode(const struct mempool *pool, int hashlen)
{
	return ((unsigned)((size_t)pool >> 3));
}

static int
is_equal_pool(const struct mempool *obj, const struct mempool *other)
{
	return (obj == other);
}

static size_t
get_pool_size(const struct mempool *pool)
{
	return (0);
}

static int
pool_tostring(const struct mempool *pool,
	      char *buf, size_t bufsize, const char *fmt)
{
	struct stdc_mempool *obj = (struct stdc_mempool *) pool;
	if (obj && obj->name && buf) {
		size_t namelen = strlen(obj->name);
		if (bufsize > namelen) {
			memcpy(buf, obj->name, namelen);
			buf[namelen] = 0;
			return((int) namelen);
		}
	}
	return (0);
}

static void
stdc_pool_dump (const struct mempool *pool, int depth, struct l4sc_logger *log)
{
	L4SC_INFO(log, "Cannot dump standard \"C\" mempool");
}

/*
 *  Standard "C" mempool.
 *  malloc(3), free(3), ...
 */

static void *
stdc_pool_malloc (struct mempool *pool, size_t size,
		  const char *file, int line, const char *func)
{
	return (malloc(size));
}

static void *
stdc_pool_calloc (struct mempool *pool, int elements, size_t objsize,
		  const char *file, int line, const char *func)
{
	return (calloc(elements, objsize));
}

static void *
stdc_pool_realloc (struct mempool *pool, void *ptr, size_t size,
		  const char *file, int line, const char *func)
{
	return (realloc(ptr, size));
}

static void
stdc_pool_free (struct mempool *pool, void *ptr,
		  const char *file, int line, const char *func)
{
	free(ptr);
}

struct mempool *
l4sc_get_stdc_mempool(const char *file, int line, const char *func)
{
	return ((struct mempool *) &l4sc_stdc_mempool);
}

struct mempool *
l4sc_get_default_mempool(const char *file, int line, const char *func)
{
	return (l4sc_default_mempool);
}


