
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "compat.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#if defined(_MSC_VER)
#define L4SC_USE_WINDOWS_SYSTEMTIME 1
#endif
#define L4SC_USE_WINDOWS_THREADID 1
#else
#if defined(HAVE_PTHREAD_H)
#define L4SC_USE_PTHREAD_THREADID 1
#endif
#endif

#if defined(L4SC_USE_WINDOWS_SYSTEMTIME) || defined(L4SC_USE_WINDOWS_THREADID)
#include <windows.h>
#endif

#if defined(L4SC_USE_PTHREAD_THREADID)
#include <pthread.h>
#endif

#include "logobjs.h"

static int
init_logmessage(void *, size_t, bfc_mempool_t);
static size_t
get_logmessage_size(l4sc_logmessage_cptr_t obj);
static unsigned
get_logmessage_hashcode(l4sc_logmessage_cptr_t obj, int hashlen);
static int
compare_logmessage(l4sc_logmessage_cptr_t obj, l4sc_logmessage_cptr_t other);
static size_t
get_logmessage_length(l4sc_logmessage_cptr_t obj);
static int
logmessage_tostring(l4sc_logmessage_cptr_t obj, char *buf, size_t bufsize,
                    const char *fmt);

const struct l4sc_logmessage_class l4sc_logmessage_class = {
    /* .super        */ (l4sc_logmessage_class_ptr_t)&l4sc_object_class,
    /* .name         */ "logmessage",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_logmessage,
    /* .initrefc     */ NULL, /* inherit */
    /* .incrrefc     */ NULL, /* inherit */
    /* .decrrefc     */ NULL, /* inherit */
    /* .destroy      */ NULL, /* inherit */
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_logmessage_size,
    /* .compare      */ compare_logmessage,
    /* .hashcode     */ get_logmessage_hashcode,
    /* .length       */ get_logmessage_length,
    /* .tostring     */ logmessage_tostring,
    /* .dump         */ NULL /* inherit */
};

static int
init_logmessage(void *buf, size_t bufsize, bfc_mempool_t pool)
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
get_logmessage_hashcode(l4sc_logmessage_cptr_t obj, int hashlen)
{
    size_t k;
    unsigned x = 0;
    const unsigned char *cp;
    if (obj) {
        cp = (unsigned char *)obj->msg;
        for (k = 0; k < obj->msglen; cp++, k++) {
            x = (x << 7) ^ ((x >> (8 * sizeof(x) - 7)) & 0x7f) ^ *cp;
        }
    }
    return (l4sc_reduce_hashcode(x, 8 * sizeof(x), hashlen));
}

static int
compare_logmessage(l4sc_logmessage_cptr_t obj, l4sc_logmessage_cptr_t other)
{
    if (other == obj) {
        return (0);
    } else if ((BFC_CLASS(other) == BFC_CLASS(obj)) &&
               (other->msglen == obj->msglen)) {
        return (memcmp(other->msg, obj->msg, obj->msglen));
    }
    return (1);
}

static size_t
get_logmessage_length(l4sc_logmessage_cptr_t obj)
{
    return (obj ? obj->msglen : 0);
}

static int
logmessage_tostring(l4sc_logmessage_cptr_t obj, char *buf, size_t bufsize,
                    const char *fmt)
{
    if (obj && buf) {
        if (bufsize > obj->msglen) {
            memcpy(buf, obj->msg, obj->msglen);
            buf[obj->msglen] = 0;
            return ((int)obj->msglen);
        }
    }
    return (0);
}

int
l4sc_init_logmessage(void *buf, size_t bufsize, l4sc_logger_cptr_t logger,
                     int level, const char *msg, size_t msglen,
                     const char *file, int line, const char *func)
{
    l4sc_logmessage_ptr_t m = (l4sc_logmessage_ptr_t)buf;
    int rc;

    if ((rc = init_logmessage(buf, bufsize, NULL)) >= 0) {
#if defined(L4SC_USE_WINDOWS_SYSTEMTIME)
        FILETIME ft;
        UINT64 tmp, secs;
        INT32 day;
        GetSystemTimeAsFileTime(&ft);
        tmp = ft.dwHighDateTime;
        tmp <<= 32;
        tmp |= ft.dwLowDateTime;
        tmp /= 10;                    /* microseconds */
        tmp -= 11644473600000000Ui64; /* based on Jan 1st, 1970 */
        secs = tmp / 1000000;
        m->time.tv_day = day = (INT32)(secs / 86400Ui32);
        m->time.tv_sec = (UINT32)(secs - ((UINT64)day * 86400Ui32));
        m->time.tv_usec = (UINT32)(tmp - 1000000 * secs);
#elif defined(HAVE_STRUCT_TIMEVAL)
        struct timeval tv;
        gettimeofday(&tv, NULL);
        m->time.tv_day = tv.tv_sec / 86400uL;
        m->time.tv_sec = tv.tv_sec - (m->time.tv_day * 86400uL);
        m->time.tv_usec = tv.tv_usec;
#elif defined(HAVE_STRUCT_TIMESPEC)
        struct timespec tv;
        gettimeofday(&tv, NULL);
        m->time.tv_day = tv.tv_sec / 86400uL;
        m->time.tv_sec = tv.tv_sec - (m->time.tv_day * 86400uL);
        m->time.tv_usec = tv.tv_nsec / 1000;
#else
        time_t secs;
        time(&secs);
        m->time.tv_day = secs / 86400uL;
        m->time.tv_sec = secs - (m->time.tv_day * 86400uL);
        m->time.tv_usec = 0;
#endif
        m->logger = logger;
        m->level = level;
        m->msg = msg;
        m->msglen = msglen;
        m->func = func;
        m->file = file;
        m->line = line;
#if defined(L4SC_USE_WINDOWS_THREADID)
        l4sc_snprintf(m->threadid, sizeof(m->threadid), "%lu",
                      (unsigned long)GetCurrentThreadId());
#elif defined(L4SC_USE_PTHREAD_THREADID)
        l4sc_snprintf(m->threadid, sizeof(m->threadid), "%p",
                      (void *)pthread_self());
#else
        do {
            unsigned stack[4];
            l4sc_snprintf(m->threadid, sizeof(m->threadid), "%p",
                          (void *)stack);
        } while (0 /*just once*/);
#endif
    }
    return (rc);
}
