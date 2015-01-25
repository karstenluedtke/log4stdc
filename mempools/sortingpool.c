
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

/**
 * Total block size 64000 Bytes.
 */
#define MAX_ITEMS_PER_BLOCK	8000u

#define MAP_BITS		MAX_ITEMS_PER_BLOCK
#define MAP_WORDS		(MAP_BITS/32)
#define MAP_LIMIT(shift)	((MAP_BITS) >> (shift))

/**
 * @li shift == 0:  item size 8 * 2**0  = 8 Bytes
 * @li shift == 1:  item size 8 * 2**1  = 16 Bytes
 * @li shift == 2:  item size 8 * 2**2  = 32 Bytes
 * @li shift == 11: item size 8 * 2**11 = 16 kB
 * @li shift == MAX_SHIFT: larger items, one per block
 */
#define MAX_SHIFT		12u

struct smallitems {
	struct smallitems *prev;
	struct smallitems *next;
	uint8_t		full;
	uint32_t 	map[MAP_WORDS];
	uint64_t 	items[MAX_ITEMS_PER_BLOCK];
};

struct largeitem; /* in chainedpool.c */

struct sortingpool {
	BFC_CHAINEDPOOLHDR(bfc_mempool_classptr_t, struct mempool *)
	struct mpsmall {
		struct smallitems *first;
		struct smallitems *last;
	}		smallblks[MAX_SHIFT];
};

static int  init_sortingpool(void *buf,size_t bufsize,struct mempool *pool);
static void destroy_pool(struct mempool *pool);
static int  clone_pool(const struct mempool *pool, void *buf, size_t bufsize,
							struct mempool *other);
static size_t get_pool_object_size(const struct mempool *pool);
static unsigned get_pool_hashcode(const struct mempool *pool);
static int is_equal_pool(const struct mempool *obj,const struct mempool *other);
static size_t get_pool_size(const struct mempool *pool);
static int pool_tostring(const struct mempool *pool, char *buf, size_t bufsize);

size_t bfc_get_chainedpool_size(const struct mempool *pool);

const struct bfc_mempool_class bfc_sortingpool_class = {
	.super = NULL,
	.name = "sorting mempool",
	.init = init_sortingpool,
	.destroy = destroy_pool,
	.clone = clone_pool,
	.clonesize = get_pool_object_size,
	.hashcode = get_pool_hashcode,
	.equals = is_equal_pool,
	.length = get_pool_size,
	.tostring = pool_tostring,
	.dump = bfc_sortingpool_dump,
	.alloc = bfc_sortingpool_malloc,
	.calloc = bfc_sortingpool_calloc,
	.realloc = bfc_sortingpool_realloc,
	.free = bfc_sortingpool_free,
};

struct mempool *
bfc_new_sorting_mempool(struct mempool *parent,
		     const char *file, int line, const char *func)
{
	int rc = -ENOMEM;
	struct sortingpool *pool;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);
	
	L4SC_INFO(logger, "%s(parent %p): from %s in %s:%d",
				__FUNCTION__, parent, func, file, line);

	if ((pool = bfc_mempool_alloc(parent, sizeof(*pool))) != NULL) {
		rc = init_sortingpool(pool, sizeof(*pool), parent);
	}
	if (rc < 0) {
		L4SC_ERROR(logger, "%s(parent %p): from %s in %s:%d: error %d",
				__FUNCTION__, parent, func, file, line, rc);
	}
	return ((struct mempool *) pool);
}

static int
init_sortingpool(void *buf, size_t bufsize, struct mempool *pool)
{
	struct mempool *parent = pool? pool: get_stdc_mempool();
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);
	BFC_INIT_PROLOGUE(bfc_mempool_class_ptr_t,
			  struct sortingpool *, p, buf, bufsize, parent,
			  &bfc_sortingpool_class);
	bfc_incr_refcount(parent);
	p->pool = p->parent_pool = parent;
	bfc_new_mutex(&p->lock, parent);
	bfc_mempool_add_pool((struct mempool *)p, parent);
	bfc_object_dump(p, 1, logger);
	return (BFC_SUCCESS);
}

static void
destroy_pool(struct mempool *pool)
{
	struct sortingpool *obj = (struct sortingpool *) pool;
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

	rc = init_sortingpool(buf, bufsize, other? other: pool->parent_pool);

	return (rc);
}

static size_t
get_pool_object_size(const struct mempool *pool)
{
	return (sizeof(struct sortingpool));
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
	struct sortingpool *impl = (struct sortingpool *) pool;
	bfc_mutex_ptr_t locked;
	struct smallitems *blk;
	unsigned i, j, k, shift, usage;
	size_t netto = 0;

	if (impl->lock && (locked = bfc_mutex_lock(impl->lock))) {
		for (shift = 0; shift < MAX_SHIFT; shift++) {
			BFC_LIST_FOREACH(blk, &impl->smallblks[shift], next) {
				usage = 0;
				for (i=0; i < MAP_LIMIT(shift); i++) {
					k = i / 32;
					j = i & 31;
					if (blk->map[k] == 0x00000000u) {
						i += 31;
						continue;
					} else if (blk->map[k] == 0xFFFFFFFFu) {
						usage += 32;
						i += 31;
						continue;
					} else if (blk->map[k] & (1u << j)) {
						usage++;
					}
				}
				/* item_size = 8u << shift = 1 << (shift+3) */
				netto += (size_t) usage << (shift+3);
			}
		}
		bfc_mutex_unlock(locked);
		netto += bfc_get_chainedpool_size((struct mempool *) impl);
	}
	return (netto);
}

static int
pool_tostring(const struct mempool *pool, char *buf, size_t bufsize)
{
	struct sortingpool *obj = (struct sortingpool *) pool;
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
bfc_sortingpool_dump(const struct mempool *pool,
			int depth, struct l4sc_logger *log)
{
	L4SC_INFO(log, "%s: dump to be implemented", __FUNCTION__);
}


void *
bfc_sortingpool_malloc(struct mempool *pool, size_t size,
		       const char *file, int line, const char *func)
{
	struct sortingpool *impl = (struct sortingpool *) pool;
	unsigned shift, map_limit;
	unsigned start, i, j=0, k=0, n=0;
	struct smallitems *blk;
	static unsigned randval = 0;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);
	
	for (shift = 0; size > (8 << shift); shift++);

	L4SC_TRACE(logger, "%s(pool %p, size %ld): shift %u",
				__FUNCTION__, pool, (long) size, shift);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (NULL);
	}

	if (shift >= MAX_SHIFT) {
		return (bfc_chainedpool_malloc(pool, size, file, line, func));
	}

	map_limit = MAP_LIMIT(shift);
	locked = bfc_mutex_lock(impl->lock);
	BFC_LIST_FOREACH(blk, &impl->smallblks[shift], next) {
		L4SC_TRACE(logger, "%s: blk @%p: %u items of %u, total %lu %s",
			__FUNCTION__, blk, map_limit, 8u << shift,
			(long) sizeof(blk->items), blk->full? "FULL": "");
		if (blk->full) {
			for (k=0; k < map_limit/32; k++) {
				if (blk->map[k] != 0xFFFFFFFFu) {
					L4SC_ERROR(logger, "%s: blk @%p: %u "
					    "items of %u %s: map[%u] = 0x%08X",
					    __FUNCTION__, blk, map_limit,
					    8u << shift, blk->full? "FULL": "",
					    k, blk->map[k]);
					goto lookup_bits;
				}
			}
			continue;
		}
	lookup_bits:
		start = randval++ % map_limit;
		for (i=0; i < map_limit; i++) {
			n = start + i;
			while (n >= map_limit) {
				n -= map_limit;
			}
			k = n / 32;
			j = n & 31;
			if (((n | 31) < map_limit)
			 && (blk->map[k] == 0xFFFFFFFFu)) {
				i += 31 - j;
				continue;

			} else if ((blk->map[k] & (1u << j)) == 0) {
				blk->map[k] |= (1u << j);
				bfc_mutex_unlock(locked);
				L4SC_TRACE(logger,
					"%s: blk @%p: n %u, k %u, j %u",
					__FUNCTION__, blk, n, k, j);
				return (&blk->items[n << shift]);
			}
		}
		blk->full = 1;
		L4SC_DEBUG(logger,
			"%s: blk @%p: %u items of %u, total %lu now %s, "
			"start %u, n %u, k%u, j %u", __FUNCTION__, blk,
			map_limit, 8u << shift, (long) sizeof(blk->items),
			blk->full? "FULL": "NOT FULL", start, n, k, j);
	}
	bfc_mutex_unlock(locked);
			
	if ((blk = bfc_mempool_alloc(pool->parent_pool, sizeof(*blk)))!= NULL) {
		L4SC_TRACE(logger, "%s: new blk @%p: %u items of %u, total %lu",
			__FUNCTION__, blk, MAP_LIMIT(shift),
			(8u << shift), (long) sizeof(blk->items));
		memset (blk, 0, offsetof(struct smallitems, items));
		blk->map[0] = 1u;

		locked = bfc_mutex_lock(impl->lock);
		BFC_DLIST_INSERT_LAST(&impl->smallblks[shift], blk, next, prev);
		bfc_mutex_unlock(locked);

		return (&blk->items[0]);
	}

	L4SC_ERROR(logger, "%s: malloc(blksize %lu) failed",
				__FUNCTION__, (long) sizeof(*blk));
	return (NULL);
}

void *
bfc_sortingpool_calloc(struct mempool *pool, int elements, size_t objsize,
		       const char *file, int line, const char *func)
{
	void *p;
	int size = elements * objsize;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	if (bfc_mempool_validate_pool(pool, file, line, __FUNCTION__) != 0) {
		L4SC_ERROR(logger, "%s: BAD POOL @%p!!!", __FUNCTION__, pool);	
		return (NULL);
	}

	if ((elements > 0) && (objsize > 0)) {
		p = bfc_sortingpool_malloc(pool, size, file, line, func);
		if (p != NULL) {
			memset(p, 0, size);
			return (p);
		}
	}
	return (NULL);
}

void *
bfc_sortingpool_realloc(struct mempool *pool, void *ptr, size_t size,
			const char *file, int line, const char *func)
{
	struct sortingpool *impl = (struct sortingpool *) pool;
	unsigned shift;
	unsigned j, k, n;
	struct smallitems *blk;
	void *p;
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
			return (bfc_sortingpool_malloc(pool, size,
							file, line, func));
		} else {
			return (NULL);
		}
	}

	locked = bfc_mutex_lock(impl->lock);
	for (shift = 0; shift < MAX_SHIFT; shift++) {
		BFC_LIST_FOREACH(blk, &impl->smallblks[shift], next) {
			if ((ptr >= (void*) blk->items) 
			 && (ptr <  (void*)&blk->items[MAX_ITEMS_PER_BLOCK])) {
				n = ((uint64_t*)ptr - &blk->items[0]) >> shift;
				k = n / 32;
				j = n & 31;
				if (size > (8 << shift)) {
					bfc_mutex_unlock(locked);
					p = bfc_sortingpool_malloc(pool, size,
								file,line,func);
					if (p != NULL) {
						memcpy(p, ptr, 8u << shift);
					}
					locked = bfc_mutex_lock(impl->lock);
					blk->full = 0;
					blk->map[k] &= ~(1u << j);
					bfc_mutex_unlock(locked);
					return (p);
				} else if (size > 0) {
					blk->map[k] |=  (1u << j);
					bfc_mutex_unlock(locked);
					return (ptr);
				} else {
					blk->full = 0;
					blk->map[k] &= ~(1u << j);
					bfc_mutex_unlock(locked);
					return (NULL);
				}
			}
		}
	}
	bfc_mutex_unlock(locked);

	/*
	 *  Item not matched so far ...
	 */

	return (bfc_chainedpool_realloc(pool, ptr, size, file, line, func));
}

void
bfc_sortingpool_free(struct mempool *pool, void *ptr,
	      const char *file, int line, const char *func)
{
	struct sortingpool *impl = (struct sortingpool *) pool;
	unsigned shift;
	unsigned j, k, n;
	struct smallitems *blk;
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
	for (shift = 0; shift < MAX_SHIFT; shift++) {
		BFC_LIST_FOREACH(blk, &impl->smallblks[shift], next) {
			if ((ptr >= (void*) blk->items) 
			 && (ptr <  (void*)&blk->items[MAX_ITEMS_PER_BLOCK])) {
				n = ((uint64_t*)ptr - &blk->items[0]) >> shift;
				k = n / 32;
				j = n & 31;
				blk->full = 0;
				blk->map[k] &= ~(1u << j);
				bfc_mutex_unlock(locked);
				return;
			}
		}
	}
	bfc_mutex_unlock(locked);

	bfc_chainedpool_free(pool, ptr, file, line, func);
}

