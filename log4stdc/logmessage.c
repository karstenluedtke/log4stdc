
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if defined(_MSC_VER)
#include <stdint.h>
#define L4SC_USE_WINDOWS_SYSTEMTIME 1
#define L4SC_USE_WINDOWS_THREADID 1
#else
#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_USE_WINDOWS_THREADID 1
#else
#define L4SC_USE_PTHREAD_THREADID 1
#endif
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#endif

#if defined(L4SC_USE_WINDOWS_SYSTEMTIME) || defined(L4SC_USE_WINDOWS_THREADID)
#include <windows.h>
#endif

#if defined(L4SC_USE_PTHREAD_THREADID)
#include <pthread.h>
#endif

#include "logobjects.h"

static int init_logmessage(void *, size_t, struct mempool *);
static size_t get_logmessage_size(l4sc_logmessage_cptr_t obj);
static unsigned get_logmessage_hashcode(l4sc_logmessage_cptr_t obj);
static int  is_equal_logmessage(l4sc_logmessage_cptr_t obj, l4sc_logmessage_cptr_t other);
static int  get_logmessage_length(l4sc_logmessage_cptr_t obj);
static int  logmessage_tostring(l4sc_logmessage_cptr_t obj, char *buf, size_t bufsize);

const struct l4sc_logmessage_class l4sc_logmessage_class = {
	.super = NULL,
	.name = "logmessage",
	.init = init_logmessage,
	.clonesize = get_logmessage_size,
	.hashcode = get_logmessage_hashcode,
	.equals = is_equal_logmessage,
	.length = get_logmessage_length,
	.tostring = logmessage_tostring,
};

static int
init_logmessage(void *buf, size_t bufsize, struct mempool *pool)
{
	BFC_INIT_PROLOGUE(const struct l4sc_logmessage_class *,
			  l4sc_logmessage_ptr_t, logmessage, buf, bufsize, pool,
			  &l4sc_logmessage_class);

	return (BFC_SUCCESS);
}

static size_t
get_logmessage_size(l4sc_logmessage_cptr_t obj)
{
	return (sizeof(struct l4sc_logmessage));
}

static unsigned  
get_logmessage_hashcode(l4sc_logmessage_cptr_t obj)
{
	size_t k;
	unsigned x = 0;
	const unsigned char *cp;
	if (obj) {
		cp = (unsigned char *) obj->msg;
		for (k=0; k < obj->msglen; cp++, k++) {
			x = (x << 7) ^ ((x >> (8*sizeof(x)-7)) & 0x7f) ^ *cp;
		}
	}
	return (x);
}

static int
is_equal_logmessage(l4sc_logmessage_cptr_t obj, l4sc_logmessage_cptr_t other)
{
	if (other == obj) {
		return (1);
	} else if ((BFC_CLASS(other) == BFC_CLASS(obj))
		&& (other->msglen  == obj->msglen)) {
		return (memcmp(other->msg, obj->msg, obj->msglen) == 0);
	}
	return (0);
}

static int
get_logmessage_length(l4sc_logmessage_cptr_t obj)
{
	return (obj? ((int) obj->msglen): 0);
}

static int
logmessage_tostring(l4sc_logmessage_cptr_t obj, char *buf, size_t bufsize)
{
	if (obj && buf) {
		if (bufsize > obj->msglen) {
			memcpy(buf, obj->msg, obj->msglen);
			buf[obj->msglen] = 0;
			return((int) obj->msglen);
		}
	}
	return (0);
}


int
l4sc_init_logmessage(void *buf, size_t bufsize,
	l4sc_logger_cptr_t logger, int level, const char *msg, size_t msglen,
	const char *file, int line, const char *func)
{
	l4sc_logmessage_ptr_t m = (l4sc_logmessage_ptr_t) buf;
	int rc;
	
	if ((rc = init_logmessage(buf, bufsize, NULL)) >= 0) {
#if defined(L4SC_USE_WINDOWS_SYSTEMTIME)
		FILETIME ft;
		uint64_t tmp, secs;
		GetSystemTimeAsFileTime(&ft);
		tmp = ft.dwHighDateTime;
		tmp <<= 32;
		tmp |= ft.dwLowDateTime;
		tmp /= 10; /* microseconds */
		tmp -= 11644473600000000Ui64; /* based on Jan 1st, 1970 */
		m->time.tv_sec  = secs = tmp / 1000000;
		m->time.tv_usec = (uint32_t) (tmp - 1000000 * secs);
#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		m->time.tv_sec  = tv.tv_sec;
		m->time.tv_usec = tv.tv_usec;
#endif
		m->logger = logger;
		m->level = level;
		m->msg = msg;
		m->msglen = msglen;
		m->func = func;
		m->file = file;
		m->line = line;
#if defined(L4SC_USE_WINDOWS_THREADID)
		snprintf(m->threadid, sizeof(m->threadid), "%lu",
   			(unsigned long) GetCurrentThreadId());
#elif defined(L4SC_USE_PTHREAD_THREADID)
		snprintf(m->threadid, sizeof(m->threadid), "%p",
   			(void *) pthread_self());
#else
		do {
			unsigned stack[4];
			snprintf(m->threadid, sizeof(m->threadid),
						"%p", (void*)stack);
		} while (0 /*just once*/);
#endif
	}
	return (rc);
}

