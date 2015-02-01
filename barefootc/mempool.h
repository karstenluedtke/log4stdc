
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
#define MEMPOOL_LOGGER	"barefootc.memory",16

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

	int 		(*info)   (const struct mempool *pool,
		  		   struct mempool_info *buf, int maxentries);

	void *		spare[10];
};

extern const struct bfc_mempool_class bfc_stdc_mempool_class;
extern const struct bfc_mempool_class bfc_linear_mempool_class;
extern const struct bfc_mempool_class bfc_chained_mempool_class;
extern const struct bfc_mempool_class bfc_sorting_mempool_class;

typedef const struct bfc_mempool_class *bfc_mempool_classptr_t;

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

#define BFC_CHAINEDPOOLHDR(classptrT,objptrT)			\
	BFC_MEMPOOLHDR(classptrT,objptrT)			\
	struct mplarge {					\
		struct largeitem *first;			\
		struct largeitem *last;				\
	}		largeblks;

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

#define mempool_validate_pool(pool)		\
	bfc_mempool_validate_pool(pool,__FILE__,__LINE__,__FUNCTION__)

#define mempool_newpool(pool,parent)	\
	(*BFC_CLASS(pool)->newpool)(pool,parent,__FILE__,__LINE__,__FUNCTION__)

#define mempool_info(pool,buf,maxentries)	\
	(*BFC_CLASS(pool)->info)(pool,buf,maxentries)

#define mempool_dump(pool,depth)	\
	(*BFC_CLASS(pool)->dump)(pool,depth)

struct mempool *bfc_get_default_mempool(void);
#ifndef get_default_mempool
#define get_default_mempool	bfc_get_default_mempool
#endif

struct mempool *bfc_get_stdc_mempool(const char *, int, const char *);
#ifndef get_stdc_mempool
#define get_stdc_mempool() bfc_get_stdc_mempool(__FILE__,__LINE__,__FUNCTION__)
#endif

struct mempool *bfc_new_chained_mempool(struct mempool *parent,
		const char *file, int line, const char *func);
struct mempool *bfc_new_sorting_mempool(struct mempool *parent,
		const char *file, int line, const char *func);

int  bfc_mempool_add_pool(struct mempool *pool, struct mempool *parent);
int  bfc_mempool_remove_pool(struct mempool *pool, struct mempool *parent);
int  bfc_mempool_validate_pool(const struct mempool *,
				const char*, int, const char*);

void bfc_mempool_dump_all(int depth, struct l4sc_logger *log);
void bfc_mempool_dump_subpools(const struct mempool *pool, int depth,
			       struct l4sc_logger *log);


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

void *bfc_chainedpool_malloc(struct mempool *pool, size_t size,
			     const char *file, int line, const char *func);
void *bfc_chainedpool_calloc(struct mempool *pool,int elements,size_t objsize,
			     const char *file, int line, const char *func);
void *bfc_chainedpool_realloc(struct mempool *pool, void *ptr, size_t size,
			     const char *file, int line, const char *func);
void bfc_chainedpool_free(struct mempool *pool, void *ptr,
			     const char *file, int line, const char *func);
void bfc_chainedpool_dump(const struct mempool *pool, int depth,
			     struct l4sc_logger *log);
int  bfc_chainedpool_info (const struct mempool *pool,
			     struct mempool_info *buf, int maxentries);
unsigned bfc_chainedpool_overhead_per_block(const struct mempool *pool);

void *bfc_sortingpool_malloc(struct mempool *pool, size_t size,
			     const char *file, int line, const char *func);
void *bfc_sortingpool_calloc(struct mempool *pool,int elements,size_t objsize,
			     const char *file, int line, const char *func);
void *bfc_sortingpool_realloc(struct mempool *pool, void *ptr, size_t size,
			     const char *file, int line, const char *func);
void bfc_sortingpool_free(struct mempool *pool, void *ptr,
			     const char *file, int line, const char *func);
void bfc_sortingpool_dump(const struct mempool *pool, int depth,
			     struct l4sc_logger *log);
int  bfc_sortingpool_info (const struct mempool *pool,
			     struct mempool_info *buf, int maxentries);

#ifdef __cplusplus
}	/* C++ */
#endif
#endif /*_BAREFOOTC_MEMPOOL_H_*/
