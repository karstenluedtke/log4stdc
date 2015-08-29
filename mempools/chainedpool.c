
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

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

struct chainedpool_mark {
	uint32_t 	magic;
#define MARK_MAGIC	0x4B52414D	/* "MARK" in little endian */
	unsigned	line;
	const char *	file;
	const char *	func;
	struct mempool *pool;
};

static int  init_chainedpool(void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pool(struct mempool *pool);
static int  clone_pool(const struct mempool *pool, void *buf, size_t bufsize,
							struct mempool *other);
static size_t get_pool_object_size(const struct mempool *pool);
static unsigned get_pool_hashcode(const struct mempool *pool, int hashlen);
static int is_equal_pool(const struct mempool *obj,const struct mempool *other);
static int pool_tostring(const struct mempool *pool,
			 char *buf, size_t bufsize, const char *fmt);

size_t bfc_get_chainedpool_size(const struct mempool *pool);

const struct bfc_mempool_class bfc_chainedpool_class = {
	.super = NULL,
	.name = "chained mempool",
	.init = init_chainedpool,
	.initrefc = (void *) bfc_default_init_refcount,
	.incrrefc = (void *) bfc_default_incr_refcount,
	.decrrefc = (void *) bfc_default_decr_refcount,
	.destroy = destroy_pool,
	.clone = clone_pool,
	.clonesize = get_pool_object_size,
	.hashcode = get_pool_hashcode,
	.equals = is_equal_pool,
	.length = bfc_get_chainedpool_size,
	.tostring = pool_tostring,
	.dump = bfc_chainedpool_dump,
	.alloc = bfc_chainedpool_malloc,
	.calloc = bfc_chainedpool_calloc,
	.realloc = bfc_chainedpool_realloc,
	.free = bfc_chainedpool_free,
	.mark = bfc_chainedpool_mark,
	.reset = bfc_chainedpool_reset,
	.info = bfc_chainedpool_info,
};

struct mempool *
bfc_new_chained_mempool(struct mempool *parent,
		     const char *file, int line, const char *func)
{
	int rc = -ENOMEM;
	struct chainedpool *pool;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);
	
	L4SC_INFO(logger, "%s(parent %p): from %s in %s:%d",
				__FUNCTION__, parent, func, file, line);

	if ((pool = bfc_mempool_alloc(parent, sizeof(*pool))) != NULL) {
		rc = init_chainedpool(pool, sizeof(*pool), parent);
	}
	if (rc >= 0) {
		pool->file = file;
		pool->line = line;
		pool->func = func;
	} else {
		L4SC_ERROR(logger, "%s(parent %p): from %s in %s:%d: error %d",
				__FUNCTION__, parent, func, file, line, rc);
	}
	return ((struct mempool *) pool);
}

static int
init_chainedpool(void *buf, size_t bufsize, struct mempool *pool)
{
	struct mempool *parent = pool? pool: get_stdc_mempool();
	BFC_INIT_PROLOGUE(bfc_mempool_class_ptr_t,
			  struct chainedpool *, p, buf, bufsize, parent,
			  &bfc_chainedpool_class);
	bfc_incr_refcount(parent);
	p->parent_pool = parent;
	bfc_new_mutex(&p->lock, parent);
	p->file = __FILE__;
	p->line = __LINE__;
	p->func = __FUNCTION__;
	bfc_mempool_add_pool((struct mempool *)p, parent);
	bfc_init_refcount(p, 1);
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
get_pool_hashcode(const struct mempool *pool, int hashlen)
{
	return (bfc_reduce_hashcode((size_t) pool, 8*sizeof(pool), hashlen));
}

static int
is_equal_pool(const struct mempool *obj, const struct mempool *other)
{
	return (obj == other);
}

size_t
bfc_get_chainedpool_size(const struct mempool *pool)
{
	struct chainedpool *impl = (struct chainedpool *) pool;
	bfc_mutex_ptr_t locked;
	struct largeitem *lblk;
	size_t netto = 0;

	if (impl->lock && (locked = bfc_mutex_lock(impl->lock))) {
		BFC_LIST_FOREACH(lblk, &impl->largeblks, next) {
			netto += lblk->item_size;
		}
		bfc_mutex_unlock(locked);
	}
	return (netto);
}

static int
pool_tostring(const struct mempool *pool,
	      char *buf, size_t bufsize, const char *fmt)
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

const struct mempool_mark *
bfc_chainedpool_mark(struct mempool *pool,
		     const char *file, int line, const char *func)
{
	struct chainedpool_mark *m;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (NULL);
	}
	if ((m = bfc_chainedpool_malloc(pool, sizeof(*m),
					file, line, __FUNCTION__)) != NULL) {
		m->magic = MARK_MAGIC;
		m->line  = line;
		m->file  = file;
		m->func  = func;
		m->pool  = pool;
	}
	L4SC_DEBUG(logger, "%s: pool @%p marked at %p", __FUNCTION__, pool, m);
	return ((const struct mempool_mark *) m);
}

int
bfc_chainedpool_reset(struct mempool *pool, const struct mempool_mark *mark,
		      const char *file, int line, const char *func)
{
	struct chainedpool *impl = (struct chainedpool *) pool;
	struct chainedpool_mark *m = (struct chainedpool_mark *) mark;
	struct largeitem *lblk, *tofree = NULL;
	int n = 0;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	L4SC_DEBUG(logger, "%s(pool %p, mark %p)", __FUNCTION__, pool, mark);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (-EINVAL);
	}
	if (m && (m->pool != pool)) {
		L4SC_ERROR(logger, "%s: marker pool %p != %p",
					__FUNCTION__, m->pool, pool);	
		return (-EINVAL);
	}
	locked = bfc_mutex_lock(impl->lock);
	if (m == NULL) {
		tofree = impl->largeblks.first;
		impl->largeblks.last = impl->largeblks.first = NULL;
	} else {
		BFC_LIST_FOREACH(lblk, &impl->largeblks, next) {
			if (m == (struct chainedpool_mark *) &lblk->item) {
				impl->largeblks.last = lblk;
				tofree = lblk->next;
				lblk->next = NULL;
				break;
			}
		}
	}
	bfc_mutex_unlock(locked);

	while (tofree != NULL) {
		lblk = tofree;
		tofree = lblk->next;
		mempool_free(pool->parent_pool, lblk);
		n++;
	}
	L4SC_DEBUG(logger, "%s(%p, %p) released %d items",
				__FUNCTION__, pool, mark, n);
	return (n);
}

unsigned
bfc_chainedpool_overhead_per_block(const struct mempool *pool)
{
	return (offsetof(struct largeitem,item));
}

/*
 * To be called with incremented pool refcount, but not necessarily locked.
 */
int
bfc_chainedpool_info(const struct mempool *pool,
		struct mempool_info *buf, int maxentries)
{
	struct chainedpool *impl = (struct chainedpool *) pool;
	bfc_mutex_ptr_t locked;
	struct largeitem *lblk;
	int n = 0;

	locked = bfc_mutex_lock(impl->lock);
	if (buf && (maxentries > 0)) {
		BFC_LIST_FOREACH(lblk, &impl->largeblks, next) {
			if (n < maxentries) {
				buf[n].ptr = lblk;
				buf[n].file = lblk->file;
				buf[n].func = lblk->func;
				buf[n].line = lblk->line;
				buf[n].item_cap = 1;
				buf[n].item_size = lblk->item_size;
				buf[n].total_size = lblk->total_size;
				buf[n].item_usage = 1;
				n++;
			} else {
				break;
			}
		}
	} else {
		BFC_LIST_FOREACH(lblk, &impl->largeblks, next) {
			n++;
		}
	}
	bfc_mutex_unlock(locked);

	return (n);
}

/*
 * To be called with incremented pool refcount, but preferably unlocked.
 */
void
bfc_chainedpool_dump(const struct mempool *pool, int depth,
		     struct l4sc_logger *log)
{
	int i, count;
	struct mempool_info *info;
	long netto = 0, capacity = 0, brutto = sizeof(struct chainedpool);
	unsigned overhead = bfc_chainedpool_overhead_per_block(pool);

	if (bfc_mempool_validate_pool(pool,__FILE__,__LINE__,__FUNCTION__)!=0) {
		L4SC_ERROR(log, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return;
	}

	count = bfc_chainedpool_info(pool, NULL, 0);
	L4SC_DEBUG(log, "%s(%p): count %d, anchor bfc_chainedpool_dump @%p",
			__FUNCTION__, pool, count, bfc_chainedpool_dump);

	info = alloca((count+10)*sizeof(info[0]));
	if (count > 0) {
		count = bfc_chainedpool_info(pool, info, count+10);
	}

	for (i=0; i < count; i++) {
		if (info[i].line == I386RETADDR) {
			L4SC_DEBUG(log, "%s: blk @%p: "
				"%u/%u of %u bytes, total %lu from %p via %s",
				__FUNCTION__, info[i].ptr,
				info[i].item_usage, info[i].item_cap,
				info[i].item_size,  info[i].total_size,
				info[i].file, info[i].func);
		} else {
			L4SC_DEBUG(log, "%s: blk @%p: "
				"%u/%u of %u bytes, total %lu from %s:%u in %s",
				__FUNCTION__, info[i].ptr,
				info[i].item_usage, info[i].item_cap,
				info[i].item_size,  info[i].total_size,
				info[i].file, info[i].line, info[i].func);
		}
		netto	+= info[i].item_size;
		capacity+= info[i].total_size;
		brutto	+= info[i].total_size + overhead;
	}
	L4SC_INFO(log,
		"pool %p: %ld/%ldkB in %d blks, brt %ldkB, refc %d, %s:%d %s",
		pool, netto/1000, capacity/1000, count, brutto/1000,
		pool->refc-1, pool->file, pool->line, pool->func);

	if (depth > 1) {
		bfc_mempool_dump_subpools(pool, depth-1, log);
	}
}
