
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#endif

#include "log4stdc.h"
#include "log4stdc/logobjects.h"

#define  bfc_mutex  win32_mutex
#include "barefootc/synchronization.h"
#include "barefootc/object.h"
#include "barefootc/mempool.h"

struct win32_mutex {
	BFC_OBJHDR(bfc_mutex_class_ptr_t,bfc_mutex_ptr_t)
	const char *	file;
	int		line;
	const char *	func;
	union {
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
		struct {
			CRITICAL_SECTION critsection;
		} 	win32;
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

const struct bfc_mutex_class l4sc_win32_mutex_class = {
	.super = (bfc_mutex_class_ptr_t) &l4sc_object_class,
	.name = "win32 mutex",
	.init = init_mutex,
	.initrefc = (void *) l4sc_default_init_refcount,
	.incrrefc = (void *) l4sc_default_incr_refcount,
	.decrrefc = (void *) l4sc_default_decr_refcount,
	.destroy = destroy_mutex,
	.clone = clone_mutex,
	.clonesize = mutex_size,
	.dump = dump_mutex,
	.lock = mutex_lock,
	.unlock = mutex_unlock,
};

#if !defined(_WIN32) && !defined(__MINGW32__) && !defined(__MINGW64__)
#define	InitializeCriticalSection(s)	assert(!"CriticalSection")
#define	DeleteCriticalSection(s)
#define	EnterCriticalSection(s)
#define	LeaveCriticalSection(s)
#endif

static int
init_mutex(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(bfc_mutex_class_ptr_t,
			  struct win32_mutex *, object, buf, bufsize, pool,
			  &l4sc_win32_mutex_class);
	if (object) {
		InitializeCriticalSection(&object->u.win32.critsection);
	}
	return (BFC_SUCCESS);
}

static void
destroy_mutex(bfc_mutex_ptr_t obj)
{
	bfc_mutex_class_ptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) == &l4sc_win32_mutex_class)) {
		DeleteCriticalSection(&obj->u.win32.critsection);
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
	return (sizeof(struct win32_mutex));
}

static bfc_mutex_ptr_t
mutex_lock(bfc_mutex_ptr_t obj, const char *file, int line, const char *func)
{
	EnterCriticalSection(&obj->u.win32.critsection);
	obj->file = file;
	obj->line = line;
	obj->func = func;
	return (obj);
}

static void
mutex_unlock(bfc_mutex_ptr_t obj, const char *file, int line, const char *func)
{
	LeaveCriticalSection(&obj->u.win32.critsection);
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

#undef bfc_mutex

int
l4sc_new_win32_mutex(struct bfc_mutex **objpp, bfc_mempool_t pool,
		    const char *file, int line, const char *func)
{
	int rc = -ENOMEM;
	size_t size = mutex_size(NULL);
	struct win32_mutex *object = mempool_alloc(pool, size);

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

