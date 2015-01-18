
#ifndef _BAREFOOTC_MEMPOOL_H_
#define _BAREFOOTC_MEMPOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "barefootc/object.h"

struct bfc_mempool_class;
typedef const struct bfc_mempool_class *bfc_mempool_class_ptr_t;

struct mempool;
struct mempool_info;

struct l4sc_logger;

struct bfc_mempool_class {
	BFC_CLASSHDR(bfc_mempool_class_ptr_t,
			struct mempool *, const struct mempool *)

	void *		(*alloc)  (struct mempool *, size_t,
				   const char *, int, const char *);

	void *		(*calloc) (struct mempool *, int, size_t,
				   const char *, int, const char *);

	void *		(*realloc)(struct mempool *, void *, size_t,
				   const char *, int, const char *);

	void 		(*free)   (struct mempool *, void *,
				   const char *, int, const char *);

	void		(*reset)  (struct mempool *,
				   const char *, int, const char *);

	void		(*release)(struct mempool *,
				   const char *, int, const char *);

	struct mempool *(*newpool)(struct mempool *, struct mempool *,
				   const char *, int, const char *);

	int 		(*info)   (struct mempool *pool, int depth,
		  		   struct mempool_info *buf, int maxentries);

	void *		spare[10];
};

extern const struct bfc_mempool_class bfc_stdc_mempool_class;
extern const struct bfc_mempool_class bfc_linear_mempool_class;
extern const struct bfc_mempool_class bfc_chained_mempool_class;
extern const struct bfc_mempool_class bfc_sorting_mempool_class;

#define BFC_MEMPOOLHDR(classptrT,objptrT) \
	BFC_OBJHDR(classptrT, objptrT)	\
	int		line;					\
	const char *	file;					\
	const char *	func;					\
	struct {						\
		objptrT	first;					\
		objptrT	last;					\
	}		sub_pools;

struct mempool {
	BFC_MEMPOOLHDR(bfc_mempool_class_ptr_t, struct mempool *)
};


#define bfc_mempool_alloc(pool,size)	\
	(*BFC_CLASS(pool)->alloc)(pool,size,__FILE__,__LINE__,__FUNCTION__)
#ifndef mempool_alloc
#define mempool_alloc bfc_mempool_alloc
#endif

#define bfc_mempool_calloc(pool,n,size)	\
	(*BFC_CLASS(pool)->calloc)(pool,n,size,__FILE__,__LINE__,__FUNCTION__)
#ifndef mempool_calloc
#define mempool_calloc bfc_mempool_calloc
#endif

#define bfc_mempool_realloc(pool,ptr,size)	\
	(*BFC_CLASS(pool)->realloc)(pool,ptr,size,__FILE__,__LINE__,__FUNCTION__)
#ifndef mempool_realloc
#define mempool_realloc bfc_mempool_realloc
#endif

#define bfc_mempool_free(pool,ptr)		\
	if (pool) (*BFC_CLASS(pool)->free)(pool,ptr,__FILE__,__LINE__,__FUNCTION__)
#ifndef mempool_free
#define mempool_free bfc_mempool_free
#endif

int bfc_mempool_incr_refcount_x(struct mempool*, const char*,int, const char*);
int bfc_mempool_decr_refcount_x(struct mempool*, const char*,int, const char*);

#define bfc_mempool_release(pool)		\
	bfc_mempool_decr_refcount_x(pool,__FILE__,__LINE__,__FUNCTION__)

#define bfc_mempool_decr_refcount(pool)		\
	bfc_mempool_decr_refcount_x(pool,__FILE__,__LINE__,__FUNCTION__)

#define bfc_mempool_incr_refcount(pool)		\
	bfc_mempool_incr_refcount_x(pool,__FILE__,__LINE__,__FUNCTION__)

int mempool_validate_pool_x(struct mempool *, const char *, int, const char *);

#define mempool_validate_pool(pool)		\
	mempool_validate_pool_x(pool,__FILE__,__LINE__,__FUNCTION__)

#define mempool_newpool(pool,parent)	\
	(*BFC_CLASS(pool)->newpool)(pool,parent,__FILE__,__LINE__,__FUNCTION__)

#define mempool_info(pool,depth,buf,maxentries)	\
	(*BFC_CLASS(pool)->info)(pool,depth,buf,maxentries)

#define mempool_dump(pool,depth)	\
	(*BFC_CLASS(pool)->dump)(pool,depth)

void mempool_dump_all(int depth);
void mempool_dump_subpools(struct mempool *pool, int depth);
void mempool_fatal(const char *msg, const struct mempool *pool);

struct mempool *bfc_get_default_mempool(void);
#ifndef get_default_mempool
#define get_default_mempool	bfc_get_default_mempool
#endif

struct mempool *bfc_get_stdc_mempool(const char *, int, const char *);
#ifndef get_stdc_mempool
#define get_stdc_mempool() bfc_get_stdc_mempool(__FILE__,__LINE__,__FUNCTION__)
#endif

struct mempool *bfc_get_win32_mempool(struct mempool *, struct mempool *,
		const char *, int, const char *);
struct mempool *bfc_new_chunked_mempool(struct mempool *, struct mempool *,
		const char *, int, const char *);
struct mempool *bfc_new_chained_mempool(struct mempool *, struct mempool *,
		const char *, int, const char *);

struct mempool_info {
	void *		ptr;
	const char *	file;
	const char *	func;
	unsigned	line;
	unsigned	item_size;
	unsigned	item_usage;
	unsigned	item_cap;
	unsigned long	total_size;
};

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /*_BAREFOOTC_MEMPOOL_H_*/
