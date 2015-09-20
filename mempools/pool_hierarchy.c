
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "barefootc/object.h"
#include "barefootc/mempool.h"
#include "barefootc/linkedlist.h"
#include "barefootc/synchronization.h"

#include "log4stdc.h"

#define get_pool_root()	bfc_get_stdc_mempool(__FILE__,__LINE__,__FUNCTION__)

static int		/* called with pool_root mutext locked! */
mempool_validate_recursive(const struct mempool *pool,
			   const struct mempool *parent);

int
bfc_mempool_add_pool(struct mempool *pool, struct mempool *parent)
{
	int rc = -EFAULT;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	L4SC_TRACE(logger,"%s(pool %p, parent %p)", __FUNCTION__, pool, parent);

	if (pool && parent) {
		if (parent->lock == NULL) {
			bfc_new_mutex(&parent->lock, get_stdc_mempool());
			if (parent->lock == NULL) {
				L4SC_ERROR(logger, "%s(pool %p, parent %p):"
					" cannot initialize parent lock",
					 __FUNCTION__, pool, parent);
				return (-ENOMEM);
			}
		}
		locked = bfc_mutex_lock(parent->lock);

		BFC_DLIST_INSERT_LAST(&parent->sub_pools, pool, next, prev);
		rc = BFC_SUCCESS;

		bfc_mutex_unlock(locked);
	}
	L4SC_TRACE(logger, "%s(pool %p, parent %p): rc %d",
				__FUNCTION__, pool, parent, rc);
	return (rc);
}

int
bfc_mempool_remove_pool(struct mempool *pool, struct mempool *parent)
{
	int rc = -EFAULT;
	bfc_mutex_ptr_t locked;
	l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);

	L4SC_TRACE(logger,"%s(pool %p, parent %p)", __FUNCTION__, pool, parent);

	if (pool && parent && parent->lock) {
		locked = bfc_mutex_lock(parent->lock);

		BFC_DLIST_REMOVE(&parent->sub_pools, pool, next, prev);
		rc = BFC_SUCCESS;

		bfc_mutex_unlock(locked);
	}
	return (rc);
}

int
bfc_mempool_validate_pool(const struct mempool *pool,
			  const char *file, int line, const char *func)
{
	int rc = -EFAULT;
	struct mempool *p;
	bfc_mutex_ptr_t locked;

	if (pool) {
		p = get_pool_root();
		if (p == pool) {
			return (BFC_SUCCESS);
		}
		if (p->lock && (locked = bfc_mutex_lock(p->lock))) {
			rc = mempool_validate_recursive(pool, p);
			bfc_mutex_unlock(locked);
		}
	}
	if (rc != BFC_SUCCESS) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(MEMPOOL_LOGGER);
		L4SC_WARN(logger,"%s/%s: error %d validating pool @%p in %s:%d",
				 func, __FUNCTION__, rc, pool, file, line);
	}
	return (rc);
}

/**
 * To be called by mempool_validate_pool ONLY!
 */
static int
mempool_validate_recursive(const struct mempool *pool,
			   const struct mempool *parent)
			  /* called with parent mutext locked! */
{
	int rc;
	const struct mempool *p;
	bfc_mutex_ptr_t locked;

	BFC_LIST_FOREACH(p, &parent->sub_pools, next) {
		if (p == pool) {
			return (BFC_SUCCESS);
		}
	}
	BFC_LIST_FOREACH(p, &parent->sub_pools, next) {
		if (p == pool) {
			return (BFC_SUCCESS);
		}
		if (p->lock && (locked = bfc_mutex_lock(p->lock))) {
			rc = mempool_validate_recursive(pool, p);
			bfc_mutex_unlock(locked);
			if (rc != -ENOENT) {
				return (rc);
			}
		}
	}
	return (-ENOENT);
}

void
bfc_mempool_dump_all(int depth, struct l4sc_logger *log)
{
	struct mempool *p;

	p = get_stdc_mempool();
	bfc_incr_refcount((bfc_objptr_t)p);
	bfc_mempool_dump_subpools(p, depth, log);
	bfc_decr_refcount((bfc_objptr_t)p);
}

void
bfc_mempool_dump_subpools(const struct mempool *pool, int depth,
			  struct l4sc_logger *log)
{
	int i=0, n=0;
	struct mempool *p, **pp;
	bfc_mutex_ptr_t locked = NULL;
	
	if (pool->lock) {
		locked = bfc_mutex_lock(pool->lock);
	}

	BFC_LIST_FOREACH(p, &pool->sub_pools, next) {
		n++;
	}
	pp = alloca((n+1) * sizeof(pp[0]));
	BFC_LIST_FOREACH(p, &pool->sub_pools, next) {
		if (i < n) {
			bfc_incr_refcount((bfc_objptr_t)p);
			pp[i++] = p;
		}
	}
	/*
	 * Now we have references to all subpools and have incremented
	 * their refcounts, we can safely release the parent's lock.
	 */
	if (locked) {
		bfc_mutex_unlock(locked);
		locked = NULL;
	}

	if (n == 0) {
		return;
	}

	L4SC_INFO(log,
		"%s: dumping %d subpools of pool @%p created in %s:%d %s",
		__FUNCTION__, n, pool, pool->file, pool->line, pool->func);

	for (i=0; i < n; i++) {
		p = pp[i];
		if (p->parent_pool != pool) {
			L4SC_ERROR(log, "%s: CORRUPT POOL %p: parent %p != %p",
				__FUNCTION__, p, p->parent_pool, pool);
			continue;
		}
		if ((i > 0) && (p->prev != pp[i-1])) {
			L4SC_ERROR(log, "%s: CORRUPT POOL %p: prev %p != %p",
				__FUNCTION__, p, p->prev, pp[i-1]);
			continue;
		}
		bfc_object_dump((bfc_cobjptr_t)p, depth, log);
	}

	for (i=0; i < n; i++) {
		p = pp[i];
		pp[i] = NULL;
		bfc_decr_refcount((bfc_objptr_t)p);
	}
}

