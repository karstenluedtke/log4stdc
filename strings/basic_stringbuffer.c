
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "log4stdc.h"

struct stringbuffer {
	bfc_classptr_t	cls;
	struct mempool *pool;
	size_t		size;
	char		buf[1 /* or longer */];
};

static const struct {
	bfc_classptr_t super;
	const char *name;
} stringbuffer_dummy_class = {
	/* super */ NULL,
	/* name  */ "stringbuffer"
};

void *
bfc_alloc_stringbuf(struct mempool *pool, size_t nbytes)
{
	struct stringbuffer *b;

	if ((b = bfc_mempool_alloc(pool, sizeof(*b) + nbytes)) == NULL) {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s(%p, %ld): no memory",
					__FUNCTION__, pool, (long) nbytes);
		return (NULL);
	}
	b->cls  = (bfc_classptr_t) &stringbuffer_dummy_class;
	b->pool = pool;
	b->size = nbytes;
	return ((void *) b->buf);
}

void *
bfc_realloc_stringbuf(void *charbuf, size_t nbytes)
{
	struct mempool *pool;
	l4sc_logger_ptr_t logger;
	struct stringbuffer *b = (struct stringbuffer *)
			((char*)charbuf - offsetof(struct stringbuffer, buf));

	if ((charbuf != NULL) && (b != NULL)
	 && (b->cls == (bfc_classptr_t) &stringbuffer_dummy_class)
	 && ((pool = b->pool) != NULL)) {
		if ((b = bfc_mempool_realloc(pool,b,sizeof(*b)+nbytes))==NULL) {
			logger = l4sc_get_logger(BFC_STRING_LOGGER);
			L4SC_ERROR(logger, "%s(%p, %ld): no memory",
				__FUNCTION__, charbuf, (long) nbytes);
			return (NULL);
		}
	} else {
		logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s(%p, %ld): invalid stringbuffer %p",
				__FUNCTION__, charbuf, (long) nbytes, b);
		return(NULL);
	}
	b->size = nbytes;
	return ((void *) b->buf);
}

void
bfc_free_stringbuf(void *charbuf)
{
	struct mempool *pool;
	struct stringbuffer *b = (struct stringbuffer *)
			((char*)charbuf - offsetof(struct stringbuffer, buf));

	if ((charbuf != NULL) && (b != NULL)
	 && (b->cls == (bfc_classptr_t) &stringbuffer_dummy_class)
	 && ((pool = b->pool) != NULL)) {
		b->cls = NULL;
		bfc_mempool_free(pool, b);
	} else {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s(%p): invalid stringbuffer %p",
						__FUNCTION__, charbuf, b);
	}
}

#define GET_STRBUF(s)		((s)->name)

bfc_mempool_t
bfc_basic_string_pool(bfc_cstrptr_t s)
{
	struct mempool *pool;
	const char *charbuf = GET_STRBUF(s);
	const struct stringbuffer *b = (struct stringbuffer *)
			((char*)charbuf - offsetof(struct stringbuffer, buf));

	if ((charbuf != NULL) && (b != NULL)
	 && (b->cls == (bfc_classptr_t) &stringbuffer_dummy_class)
	 && ((pool = b->pool) != NULL)) {
		return (pool);
	} else {
		l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
		L4SC_ERROR(logger, "%s(%p): invalid stringbuffer %p",
						__FUNCTION__, charbuf, b);
	}
	return (NULL);
}
