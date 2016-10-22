
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "compat.h"
#include "log4stdc.h"
#include "logobjs.h"

#define  bfc_mutex  posix_mutex
#include "bareftc/mutex.h"
#include "bareftc/object.h"
#include "bareftc/mempool.h"

#ifdef HAVE_PTHREAD_H
#define __USE_UNIX98 1
#include <pthread.h>
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

const struct bfc_mutex_class l4sc_posix_mutex_class = {
	/* .super 	*/ (bfc_mutex_class_ptr_t) &l4sc_object_class,
	/* .name 	*/ "posix mutex",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_mutex,
	/* .initrefc 	*/ (void *) l4sc_default_init_refcount,
	/* .incrrefc 	*/ (void *) l4sc_default_incr_refcount,
	/* .decrrefc 	*/ (void *) l4sc_default_decr_refcount,
	/* .destroy 	*/ destroy_mutex,
	/* .clone 	*/ clone_mutex,
	/* .clonesize 	*/ mutex_size,
	/* .compare 	*/ (void *) l4sc_default_compare_object,
	/* .hashcode 	*/ (void *) l4sc_default_get_object_hashcode,
	/* .length 	*/ (void *) l4sc_default_get_object_length,
	/* .tostring 	*/ (void *) l4sc_default_object_tostring,
	/* .dump 	*/ dump_mutex,
	/* .lock	*/ mutex_lock,
	/* .unlock	*/ mutex_unlock
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
	BFC_INIT_PROLOGUE(const struct bfc_mutex_class *,
			  struct posix_mutex *, object, buf, bufsize, pool,
			  &l4sc_posix_mutex_class);
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

	if (obj && ((cls = BFC_CLASS(obj)) == &l4sc_posix_mutex_class)) {
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
	const char thisfunction[] = "dump_mutex";

	if (obj && obj->name && BFC_CLASS(obj)) {
		l4sc_logprintf(log, INFO_LEVEL,__FILE__,__LINE__,thisfunction,
			"object \"%s\" @%p", obj->name, obj);
		l4sc_logprintf(log, INFO_LEVEL,__FILE__,__LINE__,thisfunction,
			" class \"%s\", pool %p, lock %p, refc %d",
			BFC_CLASS(obj)->name, obj->parent_pool,
			obj->lock, obj->refc);
		l4sc_logprintf(log, INFO_LEVEL,__FILE__,__LINE__,thisfunction,
			" last access from %s in %s:%d",
			obj->func, obj->file, obj->line);
	}
}

#undef bfc_mutex

int
l4sc_new_posix_mutex(struct bfc_mutex **objpp, bfc_mempool_t pool,
		     const char *file, int line, const char *func)
{
	int rc = -ENOMEM;
	size_t size = mutex_size(NULL);
	struct posix_mutex *object = mempool_alloc(pool, size);

	if (object) {
		rc = init_mutex(object, size, pool);
		if (rc >= 0) {
			object->parent_pool = pool;
			VOID_METHCALL(bfc_mutex_class_ptr_t, object,
				initrefc, (object, 1));
			object->file = file;
			object->line = line;
			object->func = func;
			*objpp = (struct bfc_mutex *) object;
		}
	}
	return (rc);
}

