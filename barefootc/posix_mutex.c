
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "barefootc/object.h"
#include "log4stdc.h"

#define  bfc_mutex  posix_mutex
#include "barefootc/synchronization.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_PTHREAD_H
#define __USE_UNIX98 1
#include <pthread.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

struct posix_mutex {
	BFC_OBJHDR(bfc_mutex_class_ptr_t,bfc_mutex_ptr_t)
	const char *	file;
	int		line;
	const char *	func;
	union {
#ifdef HAVE_PTHREAD_H
		struct {
			pthread_mutex_t	mutex;
			pthread_t	owner;
		} 	pthread;
#endif
		void *	dummy[8];
	} u;
};

static int  init_mutex(void *, size_t, bfc_mempool_t );
static int  clone_mutex(bfc_mutex_ptr_t, void *, size_t, bfc_mempool_t );
static void destroy_mutex(bfc_mutex_ptr_t);
static size_t mutex_size(bfc_mutex_ptr_t);
static void dump_mutex(bfc_mutex_ptr_t, int, struct l4sc_logger *);

static bfc_mutex_ptr_t mutex_lock(bfc_mutex_ptr_t,const char*,int,const char*);
static void mutex_unlock(bfc_mutex_ptr_t, const char *, int, const char *);

const struct bfc_mutex_class bfc_posix_mutex_class = {
	.super = (bfc_mutex_class_ptr_t) &bfc_object_class,
	.name = "posix mutex",
	.init = init_mutex,
	.initrefc = (void *) bfc_default_init_refcount,
	.incrrefc = (void *) bfc_default_incr_refcount,
	.decrrefc = (void *) bfc_default_decr_refcount,
	.destroy = destroy_mutex,
	.clone = clone_mutex,
	.clonesize = mutex_size,
	.dump = dump_mutex,
	.lock = mutex_lock,
	.unlock = mutex_unlock,
};

#ifndef HAVE_PTHREAD_MUTEX_INIT
#define pthread_mutex_init(m,a)		assert(!"pthread")
#define pthread_mutex_destroy(m)
#define pthread_mutex_lock(m)
#define pthread_mutex_unlock(m)
#define pthread_mutexattr_init(a)
#define pthread_mutexattr_settype(a,v)
#define pthread_mutexattr_t long
#endif

static int
init_mutex(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(const struct bfc_object_class *,
			  struct posix_mutex *, object, buf, bufsize, pool,
			  &bfc_posix_mutex_class);
	if (object) {
		static pthread_mutexattr_t attr;
		pthread_mutexattr_init (&attr);
		pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&object->u.pthread.mutex, &attr);
	}
	return (BFC_SUCCESS);
}

static void
destroy_mutex(bfc_mutex_ptr_t obj)
{
	bfc_mutex_class_ptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) == &bfc_posix_mutex_class)) {
		pthread_mutex_destroy(&obj->u.pthread.mutex);
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
	obj->vptr = 0;
}

static int
clone_mutex(bfc_mutex_ptr_t obj,
	    void *buf, size_t bufsize, bfc_mempool_t pool)
{
	return (init_mutex(buf, bufsize, pool? pool: obj->parent_pool));
}

static size_t
mutex_size(bfc_mutex_ptr_t obj)
{
	return (sizeof(struct posix_mutex));
}

static bfc_mutex_ptr_t
mutex_lock(bfc_mutex_ptr_t obj, const char *file, int line, const char *func)
{
	pthread_mutex_lock(&obj->u.pthread.mutex);
	obj->file = file;
	obj->line = line;
	obj->func = func;
	return (obj);
}

static void
mutex_unlock(bfc_mutex_ptr_t obj, const char *file, int line, const char *func)
{
	pthread_mutex_unlock(&obj->u.pthread.mutex);
}

static void
dump_mutex(bfc_mutex_ptr_t obj, int depth, struct l4sc_logger *log)
{
	if (obj && obj->name && BFC_CLASS(obj)) {
		L4SC_DEBUG(log, "object \"%s\" @%p", obj->name, obj);
		L4SC_DEBUG(log, " class \"%s\", pool %p, lock %p, refc %d",
			BFC_CLASS(obj)->name, obj->parent_pool,
			obj->lock, obj->refc);
		L4SC_DEBUG(log, " last access from %s in %s:%d",
			obj->func, obj->file, obj->line);
	}
}

int
bfc_new_posix_mutex(struct bfc_mutex **objpp, bfc_mempool_t pool,
		    const char *file, int line, const char *func)
{
	struct posix_mutex *object = NULL;
	int rc;

	rc = bfc_new((bfc_objptr_t *) &object,
		     (bfc_classptr_t) &bfc_posix_mutex_class, pool);
	if ((rc >= 0) && object) {
		object->file = file;
		object->line = line;
		object->func = func;
		*objpp = (struct bfc_mutex *) object;
	}
	return (rc);
}

