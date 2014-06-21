
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
	BFC_CLASSHDR(bfc_mutex_class_ptr_t,
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
	(*BFC_CLASS(mutex)->lock)(mutex,__FILE__,__LINE__,__FUNCTION__)

bfc_mutex_ptr_t bfc_new_posix_mutex(struct mempool *,
				    const char *, int, const char *);
bfc_mutex_ptr_t bfc_new_win32_mutex(struct mempool *,
				    const char *, int, const char *);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /*_BAREFOOTC_SYNCHRONIZATION_H_*/