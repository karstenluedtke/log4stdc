
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#endif

#include "barefootc/object.h"
#include "log4stdc.h"

#define  bfc_mutex  win32_mutex
#include "barefootc/synchronization.h"

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
	
static bfc_mutex_ptr_t init_mutex(void *, size_t, struct mempool *);
static bfc_mutex_ptr_t clone_mutex(bfc_mutex_ptr_t, void *, size_t);
static void destroy_mutex(bfc_mutex_ptr_t);
static size_t mutex_size(bfc_mutex_ptr_t);
static void dump_mutex(bfc_mutex_ptr_t, int, struct l4sc_logger *);

static bfc_mutex_ptr_t mutex_lock(bfc_mutex_ptr_t,const char*,int,const char*);
static void mutex_unlock(bfc_mutex_ptr_t, const char *, int, const char *);

const struct bfc_mutex_class bfc_win32_mutex_class = {
	.super = (bfc_mutex_class_ptr_t) &bfc_object_class,
	.name = "win32 mutex",
	.init = init_mutex,
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

static bfc_mutex_ptr_t
init_mutex(void *buf, size_t bufsize, struct mempool *pool)
{
	BFC_INIT_PROLOGUE(const struct bfc_object_class *,
			  struct win32_mutex *, object, buf, bufsize, pool,
			  &bfc_win32_mutex_class);
	if (object) {
		InitializeCriticalSection(&object->u.win32.critsection);
	}
	return ((bfc_mutex_ptr_t) object);
}

static void
destroy_mutex(bfc_mutex_ptr_t obj)
{
	bfc_mutex_class_ptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) == &bfc_win32_mutex_class)) {
		DeleteCriticalSection(&obj->u.win32.critsection);
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
	obj->vptr = 0;
}

static bfc_mutex_ptr_t
clone_mutex(bfc_mutex_ptr_t obj, void *buf, size_t bufsize)
{
	return (init_mutex(buf, bufsize, obj->pool));
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
	size_t size;
	const unsigned char *p, *lim;

	if (obj && obj->name && BFC_CLASS(obj)) {
		L4SC_DEBUG(log, "object \"%s\" @%p", obj->name, obj);
		L4SC_DEBUG(log, " class \"%s\", pool %p, lock %p, refc %d",
			BFC_CLASS(obj)->name, obj->pool, obj->lock, obj->refc);
		L4SC_DEBUG(log, " last access from %s in %s:%d",
			obj->func, obj->file, obj->line);
	}
}

bfc_mutex_ptr_t
bfc_new_win32_mutex(struct mempool *pool,
		    const char *file, int line, const char *func)
{
	struct win32_mutex *object;

	object = (struct win32_mutex *)
		 bfc_new((bfc_classptr_t) &bfc_win32_mutex_class, pool);
	if (object) {
		object->file = file;
		object->line = line;
		object->func = func;
	}
	return ((bfc_mutex_ptr_t) object);
}

