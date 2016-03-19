
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "compat.h"
#include "logobjs.h"
#include "bareftc/object.h"
#include "bareftc/mempool.h"
#include "bareftc/lnkdlist.h"

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

static int init_appender(void *, size_t, bfc_mempool_t );
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
	/* .super 	*/ (l4sc_appender_class_ptr_t) &l4sc_object_class,
	/* .name 	*/ "appender",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_appender,
	/* .initrefc 	*/ (void *) l4sc_default_init_refcount,
	/* .incrrefc 	*/ (void *) l4sc_default_incr_refcount,
	/* .decrrefc 	*/ (void *) l4sc_default_decr_refcount,
	/* .destroy 	*/ destroy_appender,
	/* .clone 	*/ (void *) l4sc_default_clone_object,
	/* .clonesize 	*/ get_appender_size,
	/* .hashcode 	*/ (void *) l4sc_default_get_object_hashcode,
	/* .equals 	*/ (void *) l4sc_default_is_equal_object,
	/* .length 	*/ (void *) l4sc_default_get_object_length,
	/* .tostring 	*/ (void *) l4sc_default_object_tostring,
	/* .dump 	*/ (void *) l4sc_default_dump_object,
	/* .set_name	*/ set_appender_name,
	/* .set_opt	*/ set_appender_option,
	/* .get_opt	*/ get_appender_option,
	/* .apply	*/ apply_appender_options,
	/* .close	*/ close_appender,
	/* .append	*/ append_to_output,
	/* .set_layout	*/ set_appender_layout,
	/* .ref_layout	*/ ref_appender_layout
};

struct appenderpool {
	BFC_LIST_HEAD(l4sc_appender_ptr_t)
};

struct appenderpool l4sc_appenders = { NULL, NULL };

const char obsolete_appender_name[] = "###obsolete-appender###";

static int
init_appender(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(l4sc_appender_class_ptr_t,
			  l4sc_appender_ptr_t, appender, buf, bufsize, pool,
			  &l4sc_sysout_appender_class);

	appender->name = "sysout appender";
	appender->layout.vptr = &l4sc_patternlayout_class;
	VOID_METHCALL(l4sc_layout_class_ptr_t, &appender->layout,
		init, (&appender->layout, sizeof(appender->layout), pool));
	BFC_SLIST_INSERT_FIRST(&l4sc_appenders, appender, next);
	return (BFC_SUCCESS);
}

static void
destroy_appender(l4sc_appender_ptr_t appender)
{
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
#if defined(__ANDROID__)
		android_LogPriority prio = ANDROID_LOG_PRIO(level);
		const char *tag = msg->logger?	msg->logger->name:
						appender->name;
		if (msg->msg[msg->msglen] == '\0') {
			/* formatting is done by android log lib */
			__android_log_write(prio, tag, msg->msg);
			return;
		}
#endif

		bfc_mempool_t pool = appender->parent_pool;
		const size_t bufsize = msg->msglen + 100;
		char *poolmem = ((bufsize > 2000) && pool)?
				bfc_mempool_alloc(pool, bufsize):
				NULL;
		char *buf = poolmem? poolmem: alloca(bufsize);

#if defined(__ANDROID__)
		/* formatting is done by android log lib */
		memcpy(buf, msg->msg, msg->msglen);
		buf[msg->msglen] = '\0';
		__android_log_write(prio, tag, buf);
#else
		l4sc_layout_cptr_t layout = &appender->layout;
		const int len = l4sc_formatmsg(layout, msg, buf, bufsize);
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
			bfc_mempool_free(pool, poolmem);
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
	int rc;
	l4sc_appender_ptr_t appender = NULL;
	l4sc_appender_class_ptr_t cl, clazz = NULL;
	bfc_mempool_t pool = get_default_mempool();
	size_t size;

	if (kind && (klen >= 5)) {
		clazz = &l4sc_sysout_appender_class;
		if ((klen >= 12)
		 && (strncasecmp(kind+klen-12, "FileAppender", 12) == 0)) {
			clazz = &l4sc_file_appender_class;
		}
		BFC_LIST_FOREACH(appender, &l4sc_appenders, next) {
			cl = BFC_CLASS(appender);
			if (cl && cl->name
			 && (strncasecmp(cl->name, kind, klen) == 0)
			 && (cl->name[klen] == '\0')) {
				clazz = cl;
			}
		}
	} else {
		BFC_LIST_FOREACH(appender, &l4sc_appenders, next) {
			if ((strncasecmp(appender->name, name, nlen) == 0)
			 && (appender->name[nlen] == '\0')) {
				return (appender);
			}
		}
		LOGINFO(("%s: appender %.*s not found, no class",
					__FUNCTION__, nlen, name));
		return (NULL);
	}

	BFC_LIST_FOREACH(appender, &l4sc_appenders, next) {
		if ((strncasecmp(appender->name, name, nlen) == 0)
		 && (appender->name[nlen] == '\0')) {
			cl = BFC_CLASS(appender);
			if (cl == clazz) {
				return (appender);
			} else {
				LOGINFO(("%s: appender %s class %s != %s",
					__FUNCTION__, appender->name,
					cl? cl->name: "(null)", clazz->name));
				appender->name = obsolete_appender_name;
				VOID_METHCALL(l4sc_appender_class_ptr_t,
					appender, close, (appender));
				break;
			}
		}
	}

	appender = NULL;

	LOGINFO(("%s: appender %.*s not found, creating %.*s ...",
			__FUNCTION__, nlen, name, klen, kind));
	RETVAR_CMETHCALL(size, l4sc_appender_class_ptr_t, clazz,
			 clonesize, (NULL), get_appender_size(NULL));
	appender = mempool_alloc(pool, size);
	if (appender != NULL) {
		RETVAR_CMETHCALL(rc, l4sc_appender_class_ptr_t, clazz,
				 init, (appender, size, pool), -1);
		if (rc >= 0) {
			appender->parent_pool = pool;
			VOID_METHCALL(l4sc_appender_class_ptr_t, appender,
				initrefc, (appender, 1));
			VOID_METHCALL(l4sc_appender_class_ptr_t, appender,
			      	set_name, (appender, name, nlen));
			LOGINFO(("%s: created %s (class %s).",
				__FUNCTION__, appender->name, clazz->name));
		} else {
			LOGERROR(("%s: error %d creating %s (class %s).",
				__FUNCTION__, rc, appender->name, clazz->name));
			appender = NULL;
		}
	}

	return (appender);
}

void
l4sc_close_appenders(void)
{
	l4sc_appender_ptr_t appender = NULL;

	BFC_LIST_FOREACH(appender, &l4sc_appenders, next) {
		VOID_METHCALL(l4sc_appender_class_ptr_t, appender,
			      close, (appender));
	}
}


l4sc_layout_ptr_t l4sc_get_appender_layout(l4sc_appender_ptr_t appender)
{
	RETURN_METHCALL(l4sc_appender_class_ptr_t, appender,
			ref_layout, (appender), &appender->layout);
}

