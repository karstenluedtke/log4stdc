
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
	VMETHCALL(layout, destroy, (layout), (void) 0);
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
	l4sc_layout_cptr_t layout = &appender->layout;
	size_t len = 0;
	int rc, written = 0;
	if (msg && ((len = msg->msglen) > 0)) {
		int fd = IS_AT_LEAST_WARN_LEVEL(msg->level)? 2: 1;
		size_t bufsize = len + 100;
		char *buf = alloca(bufsize);
		len = VMETHCALL(layout,format,(layout,msg,buf,bufsize), 0);
		while (len > written) {
			if ((rc = write(fd, buf, len)) > 0) {
				written += rc;
			} else {
				break;
			}
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
	VMETHCALL(appender,append, (appender, msg), (void) 0);
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
	l4sc_layout_ptr_t layout;

	layout = VMETHCALL(appender, ref_layout, (appender), &appender->layout);
	return (layout);
}

