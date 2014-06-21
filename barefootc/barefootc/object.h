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

struct bfc_objhdr;
typedef struct bfc_objhdr bfc_object_t;
typedef struct bfc_objhdr *bfc_objptr_t;
typedef const struct bfc_objhdr *bfc_cobjptr_t;

struct bfc_classhdr;
typedef struct bfc_classhdr bfc_class_t;
typedef const struct bfc_classhdr *bfc_classptr_t;

extern const struct bfc_classhdr bfc_object_class;

struct mempool;
struct bfc_mutex;
struct l4sc_logger;

#define BFC_OBJHDR(classptrT,objptrT) \
	classptrT	vptr;	  /**< virtual methods */	\
	const char *	name;					\
	struct mempool *pool;					\
	struct bfc_mutex *lock;					\
	objptrT 	next;					\
	objptrT 	prev;					\
	unsigned	refc;

struct bfc_objhdr {
	BFC_OBJHDR(bfc_classptr_t,bfc_objptr_t)
};

#define BFC_CLASSHDR(classptrT,objptrT,cobjptrT) \
	classptrT	super;	  /**< possible super class */		     \
	const char *	name;	  /**< class name */			     \
	void *		spare2;						     \
	void *		spare3;						     \
	objptrT	      (*init)     (void *, size_t, struct mempool *);	     \
	void	      (*destroy)  (objptrT);				     \
	objptrT	      (*clone)    (cobjptrT, void *, size_t);		     \
	size_t	      (*clonesize)(cobjptrT);				     \
	unsigned      (*hashcode) (cobjptrT);				     \
	int	      (*equals)   (cobjptrT, cobjptrT);			     \
	int	      (*length)   (cobjptrT);				     \
	int	      (*tostring) (cobjptrT, char *, size_t);		     \
	void	      (*dump)     (cobjptrT, int, struct l4sc_logger *);     \
	void *		spare13;					     \
	void *		spare14;					     \
	void *		spare15;

struct bfc_classhdr {
	BFC_CLASSHDR(bfc_classptr_t,bfc_objptr_t,bfc_cobjptr_t)
};

#define BFC_CLASS(obj)	((obj)->vptr)
#define BFC_SUPER(obj)	(BFC_CLASS(obj)->super)

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

#define BFC_INIT_PROLOGUE(classptrT,objptrT,obj,buf,size,pool,cls)	\
	objptrT obj = (objptrT) (buf);					\
	if (size < sizeof(*obj)) {					\
		return (NULL);						\
	} else {							\
		memset(obj, 0, sizeof(*obj));				\
		if ((cls)->super) {					\
		    CMETHCALL((cls)->super, init,(obj,size,pool), obj);	\
		}							\
		obj->vptr = (cls);					\
		obj->name = #obj;					\
	}

#define BFC_DESTROY_EPILOGUE(obj,cls)					\
	if ((cls)->super && ((cls)->super != (cls))) {			\
		obj->vptr = (void *) (cls)->super;			\
		CMETHCALL((cls)->super, destroy,(obj), (void) 0);	\
	} else {							\
		obj->vptr = NULL;					\
	}


bfc_objptr_t bfc_new(bfc_classptr_t, struct mempool *);
bfc_objptr_t bfc_init_object(bfc_classptr_t, void *, size_t, struct mempool *);
bfc_objptr_t bfc_clone_object(const void *, void *, size_t);
void bfc_destroy(void *);
void bfc_delete(void *);

size_t bfc_object_size(const void *);
unsigned bfc_object_hashcode(const void *);
int  bfc_equal_object(const void *, const void *);
int  bfc_object_length(const void *);
int  bfc_object_tostring(const void *, char *, size_t);
void bfc_object_dump(const void *, int, struct l4sc_logger *);

size_t bfc_get_base_object_size(bfc_cobjptr_t);

bfc_objptr_t bfc_default_init_object(void *, size_t, struct mempool *);
bfc_objptr_t bfc_default_clone_object(bfc_cobjptr_t, void *, size_t);
void bfc_default_destroy_object(bfc_objptr_t);

unsigned bfc_default_get_object_hashcode(bfc_cobjptr_t);
int  bfc_default_is_equal_object(bfc_cobjptr_t, bfc_cobjptr_t);
int  bfc_default_get_object_length(bfc_cobjptr_t);
int  bfc_default_object_tostring(bfc_cobjptr_t, char *, size_t);
void bfc_default_dump_object(bfc_cobjptr_t, int, struct l4sc_logger *);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_OBJECT_H_ */
