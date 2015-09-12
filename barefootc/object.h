/**
 * @file      object.h
 *
 * @brief     Virtual methods associated with barefoot "C" objects.
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-05-10
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_OBJECT_H_
#define _BFC_OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <errno.h>

struct bfc_objhdr;
struct bfc_basic_object;
typedef struct bfc_basic_object bfc_object_t;
typedef struct bfc_basic_object *bfc_objptr_t;
typedef const struct bfc_basic_object *bfc_cobjptr_t;

struct bfc_classhdr;
typedef struct bfc_classhdr bfc_class_t;
typedef const struct bfc_classhdr *bfc_classptr_t;

extern const struct bfc_classhdr bfc_object_class;

struct mempool;
struct bfc_mutex;
struct bfc_iterator;
struct l4sc_logger;

#define BFC_OBJHDR(classptrT,objptrT) \
	classptrT	vptr;		/**< virtual methods	*/	\
	const char *	name;		/**< object name	*/	\
	volatile int	refc;		/**< reference count	*/	\
	struct bfc_mutex *lock;		/**< for locking the object */	\
	struct mempool *parent_pool;	/**< for freeing the object */

#define BFC_CONTAINER_HDR(classptrT,objptrT) \
	BFC_OBJHDR(classptrT,objptrT) \
	struct mempool *pool;		/**< for allocating children */

struct bfc_objhdr {
	BFC_OBJHDR(bfc_classptr_t,bfc_objptr_t)
};

struct bfc_basic_object {
	BFC_OBJHDR(bfc_classptr_t,bfc_objptr_t)
	size_t	length;
	size_t	private_6;
	size_t	private_7;
};

#define BFC_STATIC_OBJHDR_INITIALIZERS(cls,name) \
	(bfc_classptr_t) &cls, name, 30000,      \
	(struct bfc_mutex *)0, (struct mempool *)0

#define BFC_CONTAINER_CLASSHDR(classptrT,objptrT,cobjptrT,elemT) \
	classptrT	super;	  /**< possible super class */		     \
	const char *	name;	  /**< class name */			     \
	void *		spare2;						     \
	void *		spare3;						     \
	int	      (*init)     (void *, size_t, struct mempool *);	     \
	int	      (*initrefc) (objptrT, int);			     \
	int	      (*incrrefc) (objptrT);				     \
	int	      (*decrrefc) (objptrT);				     \
	void	      (*destroy)  (objptrT);				     \
	int	      (*clone)    (cobjptrT, void*, size_t, struct mempool*);\
	size_t	      (*clonesize)(cobjptrT);				     \
	unsigned      (*hashcode) (cobjptrT, int);			     \
	int	      (*equals)   (cobjptrT, cobjptrT);			     \
	size_t	      (*length)   (cobjptrT);				     \
	int	      (*tostring) (cobjptrT, char *, size_t, const char *);  \
	void	      (*dump)     (cobjptrT, int, struct l4sc_logger *);     \
	const elemT * (*first)    (cobjptrT);				     \
	elemT *	      (*index)    (objptrT, size_t);			     \
	long	      (*getl)     (cobjptrT, size_t);			     \
	int	      (*setl)     (objptrT, size_t, long);		     \
	elemT *	      (*place)    (objptrT, size_t, elemT*, struct mempool*);\
	int	      (*ibegin)   (cobjptrT, struct bfc_iterator *, size_t); \
	int	      (*ilimit)   (cobjptrT, struct bfc_iterator *, size_t); \
	int	      (*rbegin)   (cobjptrT, struct bfc_iterator *, size_t); \
	int	      (*rlimit)   (cobjptrT, struct bfc_iterator *, size_t); \
	size_t	      (*max_size) (cobjptrT c);				     \
	size_t	      (*element_size)(cobjptrT c);			     \
	size_t	      (*capacity) (cobjptrT c);				     \
	int	      (*reserve)  (objptrT c, size_t n);		     \
	void *		spare29;					     \
	void *		spare30;					     \
	void *		spare31;

#define BFC_CLASSHDR(classptrT,objptrT,cobjptrT) \
	BFC_CONTAINER_CLASSHDR(classptrT,objptrT,cobjptrT,void)

struct bfc_classhdr {
	BFC_CLASSHDR(bfc_classptr_t,bfc_objptr_t,bfc_cobjptr_t)
};

#define BFC_CLASS(obj)	((obj)->vptr)
#define BFC_SUPER(obj)	(BFC_CLASS(obj)->super)

#define BFC_SUCCESS	0

#define CMETHOD(cls,vmeth)  \
	(((cls)->vmeth)?			  \
	 ((cls)->vmeth):			  \
	 ((cls)->super &&			  \
	  (cls)->super->vmeth)?			  \
	 ((cls)->super->vmeth):			  \
	 ((cls)->super &&			  \
	  (cls)->super->super &&		  \
	  (cls)->super->super->vmeth)?		  \
	 ((cls)->super->super->vmeth):		  \
	 ((cls)->super &&			  \
	  (cls)->super->super &&		  \
	  (cls)->super->super->super &&		  \
	  (cls)->super->super->super->vmeth)?	  \
	 ((cls)->super->super->super->vmeth):	  \
	 (NULL))

#define VMETHOD(obj,vmeth) \
	(BFC_CLASS(obj)? CMETHOD(BFC_CLASS(obj)): NULL)

/*
 * args must be enclosed in (...)
 */
#define CMETHCALL(cls,vmeth,args,dflt)	\
	(((cls)->vmeth)?				\
	 (*(cls)->vmeth) args:				\
	 ((cls)->super &&				\
	  (cls)->super->vmeth)?				\
	 (*(cls)->super->vmeth) args:			\
	 ((cls)->super &&				\
	  (cls)->super->super &&			\
	  (cls)->super->super->vmeth)?			\
	 (*(cls)->super->super->vmeth) args:		\
	 ((cls)->super &&				\
	  (cls)->super->super &&			\
	  (cls)->super->super->super &&			\
	  (cls)->super->super->super->vmeth)?	 	\
	 (*(cls)->super->super->super->vmeth) args:	\
	 (dflt))

#define VMETHCALL(obj,vmeth,args,dflt) \
	(BFC_CLASS(obj)? CMETHCALL(BFC_CLASS(obj),vmeth,args,dflt): (dflt))

#define RETURN_CMETHCALL(classT,cls,vmeth,args,dflt)			\
	if (cls) {							\
		classT __cls = (classT) cls;				\
		do {							\
			if (__cls->vmeth) {				\
				return (*__cls->vmeth) args;		\
			}						\
		} while ((__cls = __cls->super) != NULL);		\
	}								\
	return (dflt)

#define RETURN_METHCALL(classT,obj,vmeth,args,dflt)			\
	RETURN_CMETHCALL(classT,BFC_CLASS(obj),vmeth,args,dflt)

#define RETVAR_CMETHCALL(var,classT,cls,vmeth,args,dflt)		\
	if (cls) {							\
		classT __cls = (classT) cls;				\
		do {							\
			if (__cls->vmeth) {				\
				var = (*__cls->vmeth) args;		\
				break;					\
			}						\
		} while ((__cls = __cls->super) != NULL);		\
		if (__cls == NULL) {					\
			var = (dflt);					\
		}							\
	} else {							\
		var = (dflt);						\
	}

#define RETVAR_METHCALL(var,classT,obj,vmeth,args,dflt)			\
	RETVAR_CMETHCALL(var,classT,BFC_CLASS(obj),vmeth,args,dflt)

#define VOID_CMETHCALL(classT,cls,vmeth,args)				\
	if (cls) {							\
		classT __cls = (classT) cls;				\
		do {							\
			if (__cls->vmeth) {				\
				(*__cls->vmeth) args;			\
				break;					\
			}						\
		} while ((__cls = __cls->super) != NULL);		\
	}

#define VOID_METHCALL(classT,obj,vmeth,args)				\
	VOID_CMETHCALL(classT,BFC_CLASS(obj),vmeth,args)

#define BFC_INIT_PROLOGUE(classptrT,objptrT,obj,buf,size,pool,cls)	\
	bfc_classptr_t super = (bfc_classptr_t) (cls)->super;		\
	objptrT obj = (objptrT) (buf);					\
	if (size < sizeof(*obj)) {					\
		return (-ENOSPC);					\
	} else {							\
		memset(obj, 0, sizeof(*obj));				\
		if (super) {						\
			bfc_init_object(super, obj, size, pool);	\
		}							\
		obj->vptr = (void *) (cls);				\
		obj->name = #obj;					\
		bfc_init_refcount(obj, 1);				\
	}

#define BFC_DESTROY_EPILOGUE(obj,cls)					\
	if ((cls)->super && ((cls)->super != (cls))) {			\
		obj->vptr = (void *) (cls)->super;			\
		bfc_destroy((bfc_objptr_t) obj);			\
	} else {							\
		obj->vptr = NULL;					\
	}


int  bfc_new(void **, bfc_classptr_t, struct mempool *);
int  bfc_init_object(bfc_classptr_t, void *, size_t, struct mempool *);
int  bfc_clone_object(const void *, void *, size_t, struct mempool *);
int  bfc_clone_new(const void *, void **, struct mempool *);
int  bfc_swap_objects(void *, void *);
int  bfc_init_refcount(void *, int);
int  bfc_incr_refcount(void *);
int  bfc_decr_refcount(void *);
void bfc_destroy(void *);
void bfc_delete(void *);
int  bfc_instance_of_class(const void *, bfc_classptr_t);
int  bfc_instance_of_classname(const void *, const char *);
bfc_classptr_t bfc_baseclass_by_name(const void *, const char *);

size_t bfc_object_size(const void *);
unsigned bfc_object_hashcode(const void *, int hashlen);
unsigned bfc_reduce_hashcode(size_t origval, int origbits, int hashlen);
int  bfc_equal_object(const void *, const void *);
int  bfc_object_equals(const void *, const void *);
int  bfc_object_length(const void *);
int  bfc_object_tostring(const void *, char *, size_t, const char *);
void bfc_object_dump(const void *, int, struct l4sc_logger *);

const void *bfc_container_first(const void *);
const void *bfc_container_cindex(const void *, size_t);
void *bfc_container_index(void *, size_t);
long bfc_container_getlong(const void *, size_t);
int  bfc_container_setlong(void *, size_t, long);
void *bfc_container_place(void *, size_t, void *, struct mempool *);
int  bfc_container_begin_iterator(const void *obj,
				struct bfc_iterator *it, size_t bufsize);
int  bfc_container_end_iterator(const void *obj, 
				struct bfc_iterator *it, size_t bufsize);

size_t bfc_get_base_object_size(bfc_cobjptr_t);

int  bfc_default_init_object(void *, size_t, struct mempool *);
int  bfc_default_init_refcount(bfc_objptr_t, int);
int  bfc_default_incr_refcount(bfc_objptr_t);
int  bfc_default_decr_refcount(bfc_objptr_t);
int  bfc_default_move_object(bfc_objptr_t, void *, size_t);
int  bfc_default_clone_object(bfc_cobjptr_t, void *, size_t, struct mempool *);
void bfc_default_destroy_object(bfc_objptr_t);

unsigned bfc_default_get_object_hashcode(bfc_cobjptr_t, int);
int  bfc_default_is_equal_object(bfc_cobjptr_t, bfc_cobjptr_t);
size_t bfc_default_get_object_length(bfc_cobjptr_t);
int  bfc_default_object_tostring(bfc_cobjptr_t, char *, size_t, const char *);
void bfc_default_dump_object(bfc_cobjptr_t, int, struct l4sc_logger *);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_OBJECT_H_ */
