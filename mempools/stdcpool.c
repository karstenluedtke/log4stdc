
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/mempool.h"

static struct mempool *init_pool(void *buf, size_t bufsize);
static void destroy_pool(struct mempool *pool);
static struct mempool *clone_pool(const struct mempool *pool,
				  void *buf, size_t bufsize);
static size_t get_pool_object_size(const struct mempool *pool);
static unsigned get_pool_hashcode(const struct mempool *pool);
static int is_equal_pool(const struct mempool *obj,const struct mempool *other);
static int get_pool_size(const struct mempool *pool);
static int pool_tostring(const struct mempool *pool, char *buf, size_t bufsize);

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

struct stdc_mempool {
	BFC_OBJHDR(bfc_mempool_class_ptr_t, struct mempool *)
};

const struct bfc_mempool_class bfc_stdc_mempool_class = {
	.super = NULL,
	.name = "stdc_mempool",
	.init = init_pool,
	.destroy = destroy_pool,
	.clone = clone_pool,
	.clonesize = get_pool_object_size,
	.hashcode = get_pool_hashcode,
	.equals = is_equal_pool,
	.length = get_pool_size,
	.tostring = pool_tostring,
	.alloc = stdc_pool_malloc,
	.calloc = stdc_pool_calloc,
	.realloc = stdc_pool_realloc,
	.free = stdc_pool_free,
};

struct stdc_mempool bfc_stdc_mempool = {
	.vptr = &bfc_stdc_mempool_class,
	.name = "stdc_mempool",
	.refc = 20000, 
};

static struct mempool *
init_pool(void *buf, size_t bufsize)
{
	BFC_INIT_PROLOGUE(bfc_mempool_class_ptr_t,
			  struct stdc_mempool *, stdcpool, buf, bufsize,
			  &bfc_stdc_mempool_class);
	return ((struct mempool *) stdcpool);
}

static void
destroy_pool(struct mempool *pool)
{
	struct stdc_mempool *obj = (struct stdc_mempool *) pool;
	bfc_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static struct mempool *
clone_pool(const struct mempool *pool, void *buf, size_t bufsize)
{
	struct stdc_mempool *obj = (struct stdc_mempool *) pool;
	bfc_objptr_t object = (bfc_objptr_t) buf;
	size_t size = VMETHCALL(obj, clonesize, (obj), sizeof(*object));
	if (bufsize < size) {
		return (NULL);
	}
	memcpy(object, pool, size);
	return ((struct mempool *) object);
}

static size_t
get_pool_object_size(const struct mempool *pool)
{
	return (sizeof(struct stdc_mempool));
}

static unsigned
get_pool_hashcode(const struct mempool *pool)
{
	size_t x = (size_t) pool;
	if (sizeof(x) > sizeof(unsigned)) {
		x ^= x >> (8*sizeof(unsigned));
	}
	return ((unsigned) x);
}

static int
is_equal_pool(const struct mempool *obj, const struct mempool *other)
{
	return (obj == other);
}

static int
get_pool_size(const struct mempool *pool)
{
	return (0);
}

static int
pool_tostring(const struct mempool *pool, char *buf, size_t bufsize)
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
bfc_get_stdc_mempool(const char *file, int line, const char *func)
{
	return ((struct mempool *) &bfc_stdc_mempool);
}
