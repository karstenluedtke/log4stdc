
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "compat.h"
#include "logobjs.h"

#if defined(__STDC__) || defined(HAVE_INTTYPES_H)
#include <inttypes.h>
#elif defined(HAVE_STDINT_H)
#include <stdint.h>
#endif

static int init_json_stream_layout(void *, size_t, bfc_mempool_t );
static size_t get_layout_size(l4sc_layout_cptr_t obj);

static void set_layout_name(l4sc_layout_ptr_t obj, const char *name, int len);
static int  set_layout_option(l4sc_layout_ptr_t obj,
				const char *name,size_t namelen,
				const char *value, size_t vallen);
static int  get_layout_option(l4sc_layout_cptr_t obj,
				const char *name,size_t namelen,
				char *valbuf, size_t bufsize);
static void apply_layout_options(l4sc_layout_ptr_t obj);

static size_t format_json_message(l4sc_layout_ptr_t layout,
				l4sc_logmessage_cptr_t msg,
				char *buf, size_t bufsize);

const struct l4sc_layout_class l4sc_json_stream_layout_class = {
	/* .super 	*/ (l4sc_layout_class_ptr_t) &l4sc_object_class,
	/* .name 	*/ "json_stream_layout",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_json_stream_layout,
	/* .initrefc 	*/ (void *) l4sc_default_init_refcount,
	/* .incrrefc 	*/ (void *) l4sc_default_incr_refcount,
	/* .decrrefc 	*/ (void *) l4sc_default_decr_refcount,
	/* .destroy 	*/ NULL, /* inherit */
	/* .clone 	*/ (void *) l4sc_default_clone_object,
	/* .clonesize 	*/ get_layout_size,
	/* .hashcode 	*/ NULL, /* inherit */
	/* .equals 	*/ NULL, /* inherit */
	/* .length 	*/ NULL, /* inherit */
	/* .tostring 	*/ NULL, /* inherit */
	/* .dump 	*/ NULL, /* inherit */
	/* .set_name	*/ set_layout_name,
	/* .set_opt	*/ set_layout_option,
	/* .get_opt	*/ get_layout_option,
	/* .apply	*/ apply_layout_options,
	/* .close	*/ NULL,
	/* .format	*/ format_json_message
};

static int
init_json_stream_layout(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(l4sc_layout_class_ptr_t,
			  l4sc_layout_ptr_t, layout, buf, bufsize, pool,
			  &l4sc_json_stream_layout_class);
	layout->name = "json stream layout";
	layout->u.jstrm.object_handle = 0x7E0000;
	return (BFC_SUCCESS);
}

static size_t
get_layout_size(l4sc_layout_cptr_t obj)
{
	return (sizeof(struct l4sc_layout));
}


static void
set_layout_name(l4sc_layout_ptr_t obj, const char *name, int len)
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
set_layout_option(l4sc_layout_ptr_t obj, const char *name, size_t namelen,
				     const char *value, size_t vallen)
{
	LOGINFO(("%s: %.*s=\"%.*s\"",__FUNCTION__,
		(int) namelen, name, (int) vallen, value));

	return (0);
}

static int
get_layout_option(l4sc_layout_cptr_t obj, const char *name, size_t namelen,
				     char *valbuf, size_t bufsize)
{
	return (0);
}

static void
apply_layout_options(l4sc_layout_ptr_t obj)
{
}

		
static size_t
format_json_message(l4sc_layout_ptr_t layout,
		  l4sc_logmessage_cptr_t msg,
		  char *buf, size_t bufsize)
{
	char *dp = buf;
	const char *limit = buf + bufsize;
	int rc;
#if defined(__STDC__) || defined(HAVE_INTTYPES_H) || defined(HAVE_STDINT_H)
	uint64_t timestamp;
#else
	unsigned long timestamp;
#endif

	timestamp = msg->time.tv_day;
	timestamp = timestamp * 86400uL + msg->time.tv_sec;
	timestamp = timestamp *  1000   + msg->time.tv_usec/1000;

	return (dp - buf);
}

