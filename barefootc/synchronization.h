
#ifndef _BAREFOOTC_SYNCHRONIZATION_H_
#define _BAREFOOTC_SYNCHRONIZATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"

struct bfc_mutex;
typedef struct bfc_mutex *bfc_mutex_ptr_t;

struct bfc_mutex_class;
typedef const struct bfc_mutex_class *bfc_mutex_class_ptr_t;

#ifndef bfc_mutex
struct bfc_mutex {
	BFC_OBJHDR(bfc_mutex_class_ptr_t,bfc_mutex_ptr_t)
	const char *	file;
	int		line;
	const char *	func;
	union {
		void *	dummy[8];
	} u;
};
#endif

struct mempool;
struct l4sc_logger;

struct bfc_mutex_class {
	BFC_OBJECT_CLASSHDR(bfc_mutex_class_ptr_t,
			bfc_mutex_ptr_t, bfc_mutex_ptr_t)

	bfc_mutex_ptr_t	(*lock)   (bfc_mutex_ptr_t,
				   const char *, int, const char *);

	void 		(*unlock) (bfc_mutex_ptr_t,
				   const char *, int, const char *);

	void *		spare[10];
};

extern const struct bfc_mutex_class bfc_posix_mutex_class;
extern const struct bfc_mutex_class bfc_win32_mutex_class;

#define bfc_mutex_lock(mutex)	\
	(*BFC_CLASS(mutex)->lock)(mutex,__FILE__,__LINE__,__FUNCTION__)

#define bfc_mutex_unlock(mutex)	\
	(*BFC_CLASS(mutex)->unlock)(mutex,__FILE__,__LINE__,__FUNCTION__)

int bfc_new_posix_mutex(struct bfc_mutex **, bfc_mempool_t ,
			const char *, int, const char *);
int bfc_new_win32_mutex(struct bfc_mutex **, bfc_mempool_t ,
			const char *, int, const char *);
#ifndef bfc_new_mutex
#if defined(_WIN32) || defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#define bfc_new_mutex(mpp,pool)\
	bfc_new_win32_mutex(mpp,pool,__FILE__,__LINE__,__FUNCTION__)
#else
#define bfc_new_mutex(mpp,pool)\
	bfc_new_posix_mutex(mpp,pool,__FILE__,__LINE__,__FUNCTION__)
#endif
#endif

#define bfc_mutex_init_refcount(mutex,n) \
	(*BFC_CLASS(mutex)->incrrefc)(mutex,n)

#define bfc_mutex_incr_refcount(mutex) \
	(*BFC_CLASS(mutex)->incrrefc)(mutex)

#define bfc_mutex_decr_refcount(mutex) \
	(*BFC_CLASS(mutex)->decrrefc)(mutex)

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /*_BAREFOOTC_SYNCHRONIZATION_H_*/
