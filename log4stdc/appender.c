
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "logobjects.h"
#include "barefootc/object.h"
#include "barefootc/mempool.h"
#include "barefootc/linkedlist.h"

#if defined(__ANDROID__)
#include <android/log.h>
#define ANDROID_LOG_PRIO(level) \
	( IS_AT_LEAST_FATAL_LEVEL(level)? ANDROID_LOG_FATAL: \
	  IS_AT_LEAST_ERROR_LEVEL(level)? ANDROID_LOG_ERROR: \
	  IS_AT_LEAST_WARN_LEVEL(level)?  ANDROID_LOG_WARN:  \
	  IS_AT_LEAST_INFO_LEVEL(level)?  ANDROID_LOG_INFO:  \
	  IS_AT_LEAST_DEBUG_LEVEL(level)? ANDROID_LOG_DEBUG: \
					  ANDROID_LOG_VERBOSE )
#endif

static int init_appender(void *, size_t, struct mempool *);
static void destroy_appender(l4sc_appender_ptr_t appender);
static size_t get_appender_size(l4sc_appender_cptr_t obj);

static void set_appender_name(l4sc_appender_ptr_t obj,const char *name,int len);
static int  set_appender_option(l4sc_appender_ptr_t obj,
				const char *name,size_t namelen,
				const char *value, size_t vallen);
static int  get_appender_option(l4sc_appender_cptr_t obj,
				const char *name,size_t namelen,
				char *valbuf, size_t bufsize);
static void apply_appender_options(l4sc_appender_ptr_t obj);
static void append_to_output(l4sc_appender_ptr_t appender,
			     l4sc_logmessage_cptr_t msg);
static void close_appender(l4sc_appender_ptr_t obj);

static int  set_appender_layout(l4sc_appender_ptr_t appender,
				l4sc_layout_ptr_t layout);
static l4sc_layout_ptr_t ref_appender_layout(l4sc_appender_ptr_t appender);

const struct l4sc_appender_class l4sc_sysout_appender_class = {
	.super = (l4sc_appender_class_ptr_t) &l4sc_object_class,
	.name = "appender",
	.init = init_appender,
	.destroy = destroy_appender,
	.clonesize = get_appender_size,

	.set_name = set_appender_name,
	.set_opt = set_appender_option,
	.get_opt = get_appender_option,
	.apply = apply_appender_options,
	.close = close_appender,

	.append = append_to_output,
	.set_layout = set_appender_layout,
	.ref_layout = ref_appender_layout,
};

struct appenderpool {
	BFC_LIST_HEAD(l4sc_appender_ptr_t)
};

struct appenderpool l4sc_appenders = { NULL, NULL };


static int
init_appender(void *buf, size_t bufsize, struct mempool *pool)
{
	BFC_INIT_PROLOGUE(l4sc_appender_class_ptr_t,
			  l4sc_appender_ptr_t, appender, buf, bufsize, pool,
			  &l4sc_sysout_appender_class);

	appender->name = "sysout appender";
	bfc_init_object((bfc_classptr_t) &l4sc_patternlayout_class,
			&appender->layout, sizeof(appender->layout), pool);
	BFC_SLIST_INSERT_FIRST(&l4sc_appenders, appender, next);
	return (BFC_SUCCESS);
}

static void
destroy_appender(l4sc_appender_ptr_t appender)
{
	int i;
	l4sc_layout_ptr_t layout = &appender->layout;

	BFC_SLIST_REMOVE(&l4sc_appenders, appender, l4sc_appender_ptr_t, next);
	VOID_METHCALL(l4sc_layout_class_ptr_t, layout, destroy, (layout));
	BFC_DESTROY_EPILOGUE(appender, &l4sc_sysout_appender_class);
}

static size_t
get_appender_size(l4sc_appender_cptr_t obj)
{
	return (sizeof(struct l4sc_appender));
}


static void
set_appender_name(l4sc_appender_ptr_t obj, const char *name, int len)
{
	if (obj && name) {
		if (len >= sizeof(obj->namebuf)) {
			memcpy(obj->namebuf, name, sizeof(obj->namebuf));
			obj->namebuf[sizeof(obj->namebuf)-1] = 0;
			obj->name = obj->namebuf;
		} else if (len > 0) {
			memcpy(obj->namebuf, name, len);
			obj->namebuf[len] = 0;
			obj->name = obj->namebuf;
		}
	}
}

static int
set_appender_option(l4sc_appender_ptr_t obj, const char *name, size_t namelen,
				     const char *value, size_t vallen)
{
	LOGINFO(("%s: %.*s=\"%.*s\"",__FUNCTION__,
		(int) namelen, name, (int) vallen, value));
	return (0);
}

static int
get_appender_option(l4sc_appender_cptr_t obj, const char *name, size_t namelen,
				     char *valbuf, size_t bufsize)
{
	return (0);
}

static void
append_to_output(l4sc_appender_ptr_t appender, l4sc_logmessage_cptr_t msg)
{
	if (msg && (msg->msglen > 0)) {
		const int level = msg->level;
		l4sc_layout_cptr_t layout = &appender->layout;
		const size_t bufsize = msg->msglen + 100;
		char *poolmem = ((bufsize > 2000) && appender->pool)?
				bfc_mempool_alloc(appender->pool, bufsize):
				NULL;
		char *buf = poolmem? poolmem: alloca(bufsize);
		const int len = l4sc_formatmsg(layout, msg, buf, bufsize);
#if defined(__ANDROID__)
		if (len > 0) {
			android_LogPriority prio = ANDROID_LOG_PRIO(level);
			const char *tag = msg->logger?	msg->logger->name:
							appender->name;
			buf[(len < bufsize)? len: bufsize-1] = '\0';
			__android_log_write(prio, tag, buf);
			written = len;
		}
#else
		int fd = IS_AT_LEAST_WARN_LEVEL(level)? 2: 1;
		int rc, written = 0;
		while (len > written) {
			if ((rc = write(fd, buf, len)) > 0) {
				written += rc;
			} else {
				break;
			}
		}
#endif
		if (poolmem) {
			bfc_mempool_free(appender->pool, poolmem);
			poolmem = NULL;
		}
	}
}

static void
apply_appender_options(l4sc_appender_ptr_t obj)
{
}

static void
close_appender(l4sc_appender_ptr_t obj)
{
}

static int
set_appender_layout(l4sc_appender_ptr_t appender,
				l4sc_layout_ptr_t layout)
{
	appender->layout = *layout; /* struct copy */
	return (0);
}

static l4sc_layout_ptr_t
ref_appender_layout(l4sc_appender_ptr_t appender)
{
	return (&appender->layout);
}

void
l4sc_append(l4sc_appender_ptr_t appender, l4sc_logmessage_cptr_t msg)
{
	VOID_METHCALL(l4sc_appender_class_ptr_t,
			appender, append, (appender, msg));
}

l4sc_appender_ptr_t
l4sc_get_appender(const char *name, int nlen, const char *kind, int klen)
{
	int i, rc;
	l4sc_appender_ptr_t appender = NULL;
	l4sc_appender_class_ptr_t clazz = &l4sc_sysout_appender_class;
	struct mempool *pool = get_default_mempool();

	BFC_LIST_FOREACH(appender, &l4sc_appenders, next) {
		if ((strncasecmp(appender->name, name, nlen) == 0)
		 && (appender->name[nlen] == '\0')) {
			return (appender);
		}
	}

	if (!kind || (klen < 5)) {
		LOGINFO(("%s: appender %.*s not found, no class",
					__FUNCTION__, nlen, name));
		return (NULL);
	} else if ((klen >= 12)
		&& (strncasecmp(kind+klen-12, "FileAppender", 12) == 0)) {
		clazz = &l4sc_file_appender_class;
	}
	BFC_LIST_FOREACH(appender, &l4sc_appenders, next) {
		if (BFC_CLASS(appender) && BFC_CLASS(appender)->name
		 && (strncasecmp(BFC_CLASS(appender)->name, kind, klen) == 0)
		 && (BFC_CLASS(appender)->name[klen] == '\0')) {
			clazz = BFC_CLASS(appender);
		}
	}

	LOGINFO(("%s: appender %.*s not found, creating %.*s ...",
				__FUNCTION__, nlen, name, klen, kind));
	appender = NULL;
	rc = bfc_new((void **) &appender, (bfc_classptr_t) clazz, pool);
	if ((rc >= 0) && appender) {
		CMETHCALL(clazz, set_name, (appender, name, nlen), (void)0);
		appender->pool = pool;
		LOGINFO(("%s: created %s (class %s).",
				__FUNCTION__, appender->name, clazz->name));
	}
	return (appender);
}

l4sc_layout_ptr_t l4sc_get_appender_layout(l4sc_appender_ptr_t appender)
{
	RETURN_METHCALL(l4sc_appender_class_ptr_t, appender,
			ref_layout, (appender), &appender->layout);
}

