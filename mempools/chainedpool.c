
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/mempool.h"
#include "barefootc/linkedlist.h"
#include "barefootc/synchronization.h"

#include "log4stdc.h"

struct largeitem {
	struct largeitem *prev;
	struct largeitem *next;
	unsigned	item_size;
	unsigned	total_size;
	unsigned	line;
#define I386RETADDR	0x36383369	/* "i386" in little endian */
	const char *	file;
	const char *	func;
	uint64_t 	item[1];
};

struct chainedpool {
	BFC_CHAINEDPOOLHDR(bfc_mempool_classptr_t, struct mempool *)
};

static int  init_chainedpool(void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pool(struct mempool *pool);
static int  clone_pool(const struct mempool *pool, void *buf, size_t bufsize,
							struct mempool *other);
static size_t get_pool_object_size(const struct mempool *pool);
static unsigned get_pool_hashcode(const struct mempool *pool);
static int is_equal_pool(const struct mempool *obj,const struct mempool *other);
static size_t get_pool_size(const struct mempool *pool);
static int pool_tostring(const struct mempool *pool, char *buf, size_t bufsize);

const struct bfc_mempool_class bfc_chainedpool_class = {
	.super = NULL,
	.name = "chained mempool",
	.init = init_chainedpool,
	.destroy = destroy_pool,
	.clone = clone_pool,
	.clonesize = get_pool_object_size,
	.hashcode = get_pool_hashcode,
	.equals = is_equal_pool,
	.length = get_pool_size,
	.tostring = pool_tostring,
	.dump = bfc_chainedpool_dump,
	.alloc = bfc_chainedpool_malloc,
	.calloc = bfc_chainedpool_calloc,
	.realloc = bfc_chainedpool_realloc,
	.free = bfc_chainedpool_free,
};

static int
init_chainedpool(void *buf, size_t bufsize, struct mempool *pool)
{
	struct mempool *parent = pool? pool: get_stdc_mempool();
	BFC_INIT_PROLOGUE(bfc_mempool_class_ptr_t,
			  struct chainedpool *, p, buf, bufsize, parent,
			  &bfc_chainedpool_class);
	bfc_incr_refcount(parent);
	p->pool = p->parent_pool = parent;
	bfc_new_mutex(&p->lock, parent);
	bfc_mempool_add_pool((struct mempool *)p, parent);
	return (BFC_SUCCESS);
}

static void
destroy_pool(struct mempool *pool)
{
	struct chainedpool *obj = (struct chainedpool *) pool;
	struct mempool *parent = obj->parent_pool;
	bfc_mutex_ptr_t lock;
	bfc_mempool_class_ptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		bfc_mempool_remove_pool((struct mempool *) obj, parent);
		if ((lock = obj->lock) != NULL) {
			obj->lock = NULL;
			bfc_decr_refcount(lock);
			lock = NULL;
		}
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
clone_pool(const struct mempool *pool, void *buf, size_t bufsize,
						struct mempool *other)
{
	int rc;

	rc = init_chainedpool(buf, bufsize, other? other: pool->parent_pool);

	return (rc);
}

static size_t
get_pool_object_size(const struct mempool *pool)
{
	return (sizeof(struct chainedpool));
}

static unsigned
get_pool_hashcode(const struct mempool *pool)
{
	size_t x = (size_t) pool;
	return ((unsigned) x);
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
pool_tostring(const struct mempool *pool, char *buf, size_t bufsize)
{
	struct chainedpool *obj = (struct chainedpool *) pool;
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

void
bfc_chainedpool_dump(const struct mempool *pool,
			int depth, struct l4sc_logger *log)
{
	L4SC_INFO(log, "dump to be implemented");
}

void *
bfc_chainedpool_malloc(struct mempool *pool, size_t size,
		       const char *file, int line, const char *func)
{
	struct chainedpool *impl = (struct chainedpool *) pool;
	struct largeitem *lblk;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);
	
	L4SC_TRACE(logger, "%s(pool %p, size %ld)",
			__FUNCTION__, pool, (long)size);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (NULL);
	}

	lblk = bfc_mempool_alloc(pool->parent_pool,
			    offsetof(struct largeitem, item) + size);
	if (lblk == NULL) {
		L4SC_ERROR(logger, "%s: malloc failed", __FUNCTION__);
		return (NULL);
	}
	memset (lblk, 0, offsetof(struct largeitem, item));
	lblk->item_size = lblk->total_size = size;
	lblk->file = file;
	lblk->line = line;
	lblk->func = func;

	locked = bfc_mutex_lock(impl->lock);
	BFC_DLIST_INSERT_LAST(&impl->largeblks, lblk, next, prev);
	bfc_mutex_unlock(locked);

	L4SC_TRACE(logger, "%s(pool %p, size %ld): %p",
		__FUNCTION__, pool, (long) size, lblk->item);
	return (lblk->item);
}

void *
bfc_chainedpool_calloc(struct mempool *pool, int elements, size_t objsize,
		       const char *file, int line, const char *func)
{
	void *p;
	int size = elements * objsize;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	L4SC_TRACE(logger, "%s(pool %p, %d, %ld)",
		__FUNCTION__, pool, elements, (long)objsize);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (NULL);
	}

	if ((elements > 0) && (objsize > 0)) {
		p = bfc_chainedpool_malloc(pool, size, file, line, func);
		if (p != NULL) {
			memset(p, 0, size);
			return (p);
		}
	}
	return (NULL);
}

void *
bfc_chainedpool_realloc(struct mempool *pool, void *ptr, size_t size,
		        const char *file, int line, const char *func)
{
	struct chainedpool *impl = (struct chainedpool *) pool;
	struct largeitem *lblk;
	void *p;
	size_t cplen;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	L4SC_TRACE(logger, "%s(pool %p, ptr %p, size %ld)",
			__FUNCTION__, pool, ptr, (long) size);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (NULL);
	}

	if (ptr == NULL) {
		if (size > 0) {
			return (bfc_chainedpool_malloc(pool, size,
							file, line, func));
		} else {
			return (NULL);
		}
	}

	locked = bfc_mutex_lock(impl->lock);
	BFC_LIST_FOREACH(lblk, &impl->largeblks, next) {
		if (ptr == &lblk->item) {
			if (size > (size_t) lblk->total_size) {
				BFC_DLIST_REMOVE(&impl->largeblks,
						 lblk, next, prev);
				bfc_mutex_unlock(locked);
				cplen = (size_t) lblk->item_size;
				if (cplen > (size_t) lblk->total_size) {
					cplen = (size_t) lblk->total_size;
				}
				p = bfc_chainedpool_malloc(pool, size,
							   file, line, func);
				if (p != NULL) {
					memcpy(p, ptr, cplen);
				}
				bfc_mempool_free(pool->parent_pool, lblk);
				return (p);
			} else if (size > 0) {
				lblk->item_size = size;
				bfc_mutex_unlock(locked);
				return (ptr);
			} else {
				BFC_DLIST_REMOVE(&impl->largeblks,
						 lblk, next, prev);
				bfc_mutex_unlock(locked);
				bfc_mempool_free(pool->parent_pool, lblk);
				return (NULL);
			}
		}
	}
	bfc_mutex_unlock(locked);
	L4SC_ERROR(logger, "%s: unmatched item @%p", __FUNCTION__, ptr);
	if (size > 0) {
		p = bfc_chainedpool_malloc(pool->parent_pool, size,
					   file, line, func);
		if (p != NULL) {
			memcpy(p, ptr, size);
			return (p);
		}
	}
	return (NULL);
}

void
bfc_chainedpool_free(struct mempool *pool, void *ptr,
		     const char *file, int line, const char *func)
{
	struct chainedpool *impl = (struct chainedpool *) pool;
	struct largeitem *lblk;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	L4SC_TRACE(logger, "%s(pool %p, ptr %p)", __FUNCTION__, pool, ptr);

	if (ptr == NULL) {
		return;
	}

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return;
	}

	locked = bfc_mutex_lock(impl->lock);
	BFC_LIST_FOREACH(lblk, &impl->largeblks, next) {
		if (ptr == &lblk->item) {
			BFC_DLIST_REMOVE(&impl->largeblks, lblk, next, prev);
			bfc_mutex_unlock(locked);
			mempool_free(pool->parent_pool, lblk);
			return;
		}
	}
	bfc_mutex_unlock(locked);
	if (line == I386RETADDR) {
		L4SC_ERROR(logger, "%s: unmatched item @%p from %p via %s",
				 	__FUNCTION__, ptr, file, func);
	} else {
		L4SC_ERROR(logger, "%s: unmatched item @%p from %s:%d in %s",
				 	__FUNCTION__, ptr, file, line, func);
	}
	L4SC_ERROR(logger, " %p: %08X %08X %08X %08X %08X %08X", ptr,
		((unsigned*)ptr)[0], ((unsigned*)ptr)[1], ((unsigned*)ptr)[2],
		((unsigned*)ptr)[3], ((unsigned*)ptr)[4], ((unsigned*)ptr)[5]);
}

