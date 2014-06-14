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

struct mempool;

#define BFC_OBJHDR(classptrT,objptrT) \
	classptrT	vptr;	  /**< virtual methods */	\
	const char *	name;					\
	struct mempool *pool;					\
	void *		lock;					\
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
	objptrT	      (*init)     (void *buf, size_t bufsize);		     \
	void	      (*destroy)  (objptrT obj);			     \
	objptrT	      (*clone)    (cobjptrT obj, void *buf, size_t bufsize); \
	size_t	      (*clonesize)(cobjptrT obj);			     \
	unsigned      (*hashcode) (cobjptrT obj);			     \
	int	      (*equals)   (cobjptrT obj, cobjptrT other);	     \
	int	      (*length)   (cobjptrT obj);			     \
	int	      (*tostring) (cobjptrT obj, char *buf, size_t bufsize); \
	void *		spare12;					     \
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

#define BFC_INIT_PROLOGUE(classptrT,objptrT,obj,buf,size,cls)		\
	objptrT obj = (objptrT) (buf);					\
	if (size < sizeof(*obj)) {					\
		return (NULL);						\
	} else {							\
		memset(obj, 0, sizeof(*obj));				\
		if ((cls)->super) {					\
			CMETHCALL((cls)->super, init,(obj,size), obj);	\
		}							\
		obj->vptr = (cls);					\
		obj->name = #obj;					\
		obj->refc = 1;						\
	}

#define BFC_DESTROY_EPILOGUE(obj,cls)					\
	if ((cls)->super && ((cls)->super != (cls))) {			\
		obj->vptr = (void *) (cls)->super;			\
		CMETHCALL((cls)->super, destroy,(obj), (void) 0);	\
	}

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /* _BFC_OBJECT_H_ */
