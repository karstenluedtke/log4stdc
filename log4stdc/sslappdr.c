
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "compat.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_WINDOWS_LOCKS 1
#include <windows.h>
#else
#include <sys/types.h>
#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#endif
#endif

#if defined(HAVE_OPENSSL3) && (HAVE_OPENSSL3 - 1 >= 0)
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#endif

#include "logobjs.h"
#include "bareftc/mempool.h"
#include "bareftc/mutex.h"

static int
init_appender(void *, size_t, bfc_mempool_t);
static void
destroy_appender(l4sc_appender_ptr_t appender);
static size_t
get_appender_size(l4sc_appender_cptr_t obj);

static int
set_appender_option(l4sc_appender_ptr_t obj, const char *name, size_t namelen,
                    const char *value, size_t vallen);
static int
get_appender_option(l4sc_appender_cptr_t obj, const char *name, size_t namelen,
                    char *valbuf, size_t bufsize);
static void
apply_appender_options(l4sc_appender_ptr_t obj);
static void
append_to_output(l4sc_appender_ptr_t appender, l4sc_logmessage_cptr_t msg);
static void
open_appender(l4sc_appender_ptr_t appender);
static void
close_appender(l4sc_appender_ptr_t appender);
static int
init_layout(l4sc_appender_ptr_t appender, l4sc_layout_class_ptr_t cls);

extern const struct l4sc_appender_class l4sc_socket_appender_class;
extern const struct l4sc_layout_class l4sc_json_stream_layout_class;

const struct l4sc_appender_class l4sc_ssl_appender_class = {
    /* .super        */ &l4sc_socket_appender_class,
    /* .name         */ "SSLSocketAppender",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ init_appender,
    /* .initrefc     */ (void *)l4sc_default_init_refcount,
    /* .incrrefc     */ (void *)l4sc_default_incr_refcount,
    /* .decrrefc     */ (void *)l4sc_default_decr_refcount,
    /* .destroy      */ destroy_appender,
    /* .clone        */ (void *)l4sc_default_clone_object,
    /* .clonesize    */ get_appender_size,
    /* .compare      */ (void *)l4sc_default_compare_object,
    /* .hashcode     */ (void *)l4sc_default_get_object_hashcode,
    /* .length       */ (void *)l4sc_default_get_object_length,
    /* .tostring     */ (void *)l4sc_default_object_tostring,
    /* .dump         */ (void *)l4sc_default_dump_object,
    /* .set_name     */ NULL, /* inherit */
    /* .set_opt      */ NULL, /* inherit */
    /* .get_opt      */ NULL, /* inherit */
    /* .apply        */ NULL, /* inherit */
    /* .close        */ close_appender,
    /* .append       */ append_to_output,
    /* .set_layout   */ NULL, /* inherit */
    /* .ref_layout   */ NULL  /* inherit */
};

struct ssl_conn {
    int state;
#define SSL_CONNECTING   1
#define SSL_ESTABLISHED  3
#if defined(HAVE_OPENSSL3) && (HAVE_OPENSSL3 - 1 >= 0)
    SSL *ssl;
    SSL_CTX *ctx;
    BIO *bio;
#endif
};

#define sslconn(appender) ((struct ssl_conn *)((appender)->fu.fh))
#define is_open(appender) (sslconn(appender) && sslconn(appender)->bio)

static int
init_appender(void *buf, size_t bufsize, bfc_mempool_t pool)
{
#if defined(HAVE_OPENSSL3) && (HAVE_OPENSSL3 - 1 >= 0)
    BFC_INIT_PROLOGUE(l4sc_appender_class_ptr_t, l4sc_appender_ptr_t, appender,
                      buf, bufsize, pool, &l4sc_ssl_appender_class);

    appender->name = "ssl socket appender";
#if defined(L4SC_WINDOWS_LOCKS)
    l4sc_new_win32_mutex(&appender->lock, pool, __FILE__, __LINE__,
                         "sslappender::init_appender");
#elif defined(HAVE_PTHREAD_MUTEX_INIT) || defined(__STDC__)
    l4sc_new_posix_mutex(&appender->lock, pool, __FILE__, __LINE__,
                         "sslappender::init_appender");
#else
    LOGWARN(("sslappender::init_appender: no mutex for appender"));
#endif
    init_layout(appender, &l4sc_json_stream_layout_class);
    return (BFC_SUCCESS);
#else
    return ((*l4sc_socket_appender_class.init)(buf, bufsize, pool));
#endif
}

static void
destroy_appender(l4sc_appender_ptr_t appender)
{
    BFC_DESTROY_EPILOGUE(appender, &l4sc_socket_appender_class);
}

static size_t
get_appender_size(l4sc_appender_cptr_t obj)
{
    return (sizeof(struct l4sc_appender));
}

/* re-use properties of fileappender */
#define remotehost filename
#define remoteport maxbackupindex

static bfc_mutex_ptr_t
lock_appender(l4sc_appender_ptr_t appender)
{
    bfc_mutex_ptr_t lock = appender->lock;
    if (lock) {
        bfc_mutex_lock(lock);
        return (lock);
    }
    return (NULL);
}

static void
unlock_appender(l4sc_appender_ptr_t appender, bfc_mutex_ptr_t lock)
{
    if (lock) {
        bfc_mutex_unlock(lock);
    }
}

static void
append_to_output(l4sc_appender_ptr_t appender, l4sc_logmessage_cptr_t msg)
{
#if defined(HAVE_OPENSSL3) && (HAVE_OPENSSL3 - 1 >= 0)
    if (msg && (msg->msglen > 0)) {
        l4sc_layout_ptr_t layout = &appender->layout;
        bfc_mempool_t pool = appender->parent_pool;
        const size_t bufsize = l4sc_layout_estimate(layout, msg);
        char *poolmem = ((bufsize > 2000) && pool)
                            ? bfc_mempool_alloc(pool, bufsize)
                            : NULL;
        char *buf = poolmem ? poolmem : alloca(bufsize);
        int len;
        if (!is_open(appender)) {
            open_appender(appender);
        }
        if ((len = l4sc_formatmsg(layout, msg, buf, bufsize)) > 0) {
            int err, rc = 0;
            struct ssl_conn *conn = sslconn(appender);
            if (conn && (conn->state == SSL_ESTABLISHED)) {
                bfc_mutex_ptr_t lock = lock_appender(appender);
                if (lock && conn && (conn->state == SSL_ESTABLISHED)) {
                    BIO *bio = conn->bio;
                    const char *cp = buf;
                    int remain = len;

                    while ((remain > 0)
                        && ((rc = BIO_write(bio, cp, remain)) > 0)) {
                        cp += rc;
                        remain -= rc;
                    }

                    if ((rc < 0) && (remain > 0)) {
                        err = ERR_get_error();
                        if (BIO_should_retry(bio)) {
                            if ((rc = cp - buf) <= 0) {
                                rc = -EAGAIN;
                            }
                        } else {
                            rc = -EIO;
                        }
                    } else {
                        rc = cp - buf;
                    }

                    unlock_appender(appender, lock);
                }
                if ((rc < len) && (rc != -EAGAIN)) {
                    close_appender(appender);
                    LOGWARN(("%s: BIO_write returned %d, err %d.",
                             "sslappender::append_to_output", rc, err));
                }
            }
        }
        if (poolmem) {
            bfc_mempool_free(pool, poolmem);
            poolmem = NULL;
        }
    }
#endif
}

static void
open_appender(l4sc_appender_ptr_t appender)
{
#if 0
    int hdrlen = 0;
    char header[200];
    static const char thisfunction[] = "open_appender";

    if (appender->remoteport > 0) {
        int addrsize;
        union anyaddr addrbuf;
#if defined(L4SC_WINDOWS_SOCKETS)
        SOCKET sock;
        int err;
#define INVALSOCK INVALID_SOCKET
#define CLOSESOCK closesocket
#define SOCKERROR WSAGetLastError()
#else
        int sock, err;
#define INVALSOCK -1
#define CLOSESOCK close
#define SOCKERROR errno
#endif
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALSOCK) {
            err = SOCKERROR;
            LOGERROR(("%s: no socket, err %d %s", thisfunction, err,
                      strerror(err)));
            return;
        }

        memset(&addrbuf, 0, sizeof(addrbuf));
        addrbuf.sin.sin_family = AF_INET;
        if (bind(sock, &addrbuf.sa, sizeof(addrbuf.sin)) == -1) {
            err = SOCKERROR;
            CLOSESOCK(sock);
            LOGERROR(("%s: connect failed, err %d %s", thisfunction, err,
                      strerror(err)));
            return;
        }

        addrsize = set_address(&addrbuf, sizeof(addrbuf), AF_INET,
                               appender->remotehost, appender->remoteport);
        if (connect(sock, &addrbuf.sa, addrsize) == -1) {
            err = SOCKERROR;
            CLOSESOCK(sock);
            LOGERROR(("%s: connect failed, err %d %s", thisfunction, err,
                      strerror(err)));
            return;
        }
        hdrlen =
            format_header(appender, L4SC_FIRST_HEADER, header, sizeof(header));
#if defined(L4SC_WINDOWS_SOCKETS)
        if (hdrlen > 0) {
            send(sock, header, hdrlen, 0);
        }
        *get_socket_storage(appender) = sock;
#else
        if (hdrlen > 0) {
            send(sock, header, hdrlen, MSG_NOSIGNAL);
        }
        appender->fu.fd = sock;
#endif
        init_layout(appender, BFC_CLASS(&appender->layout));
    }
#endif
}

static void
close_appender(l4sc_appender_ptr_t appender)
{
#if 0
#if defined(L4SC_WINDOWS_SOCKETS)
    SOCKET sock = get_socket(appender);
#else
    int sock = get_socket(appender);
#endif
    appender->fu.fh = NULL;
    appender->fu.fd = 0;
    if (sock) {
        CLOSESOCK(sock);
    }
#endif
}

static int
init_layout(l4sc_appender_ptr_t appender, l4sc_layout_class_ptr_t cls)
{
    RETURN_CMETHCALL(
        l4sc_layout_class_ptr_t, cls, init,
        (&appender->layout, sizeof(appender->layout), appender->parent_pool),
        -ENOSYS);
}

