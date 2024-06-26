
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "compat.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_WINDOWS_SOCKETS 1
#define L4SC_WINDOWS_LOCKS 1
#include <winsock2.h>
#include <windows.h>
#elif defined(HAVE_SYS_SOCKET_H)
#define L4SC_BSD_SOCKETS 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

#if defined(HAVE_SIGNAL_H)
#include <signal.h>
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

#define is_open(appender) ((appender)->fu.fh != NULL)

extern const struct l4sc_appender_class l4sc_sysout_appender_class;
extern const struct l4sc_layout_class l4sc_log4j_stream_layout_class;

const struct l4sc_appender_class l4sc_socket_appender_class = {
    /* .super        */ &l4sc_sysout_appender_class,
    /* .name         */ "SocketAppender",
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
#if defined(L4SC_WINDOWS_SOCKETS) || defined(L4SC_BSD_SOCKETS)
    /* .set_opt      */ set_appender_option,
    /* .get_opt      */ get_appender_option,
    /* .apply        */ apply_appender_options,
    /* .close        */ close_appender,
    /* .append       */ append_to_output,
    /* .set_layout   */ NULL, /* inherit */
    /* .ref_layout   */ NULL  /* inherit */
#endif
};

static int
init_appender(void *buf, size_t bufsize, bfc_mempool_t pool)
{
#if defined(L4SC_WINDOWS_SOCKETS) || defined(L4SC_BSD_SOCKETS)
    BFC_INIT_PROLOGUE(l4sc_appender_class_ptr_t, l4sc_appender_ptr_t, appender,
                      buf, bufsize, pool, &l4sc_socket_appender_class);

    appender->name = "socket appender";
    init_layout(appender, &l4sc_log4j_stream_layout_class);
#if defined(L4SC_WINDOWS_SOCKETS)
    do {
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSAStartup(wVersionRequested, &wsaData);
    } while (0 /*just once*/);
#endif
#if !defined(MSG_NOSIGNAL) && defined(SIGPIPE) && defined(SIG_IGN)
    signal(SIGPIPE, SIG_IGN);
#define MSG_NOSIGNAL 0
#endif
    return (BFC_SUCCESS);
#else
    return ((*l4sc_sysout_appender_class.init)(buf, bufsize, pool));
#endif
}

static void
destroy_appender(l4sc_appender_ptr_t appender)
{
    bfc_mutex_ptr_t lock = appender->lock;
#if defined(L4SC_WINDOWS_SOCKETS) || defined(L4SC_BSD_SOCKETS)
    close_appender(appender);
#endif
    if (lock) {
        bfc_mempool_t pool = lock->parent_pool;
        appender->lock = NULL;
        VOID_METHCALL(bfc_mutex_class_ptr_t, lock, destroy, (lock));
        if (pool != NULL) {
            mempool_free(pool, lock);
        }
    }
#if defined(L4SC_WINDOWS_SOCKETS)
    WSACleanup();
#endif
    BFC_DESTROY_EPILOGUE(appender, &l4sc_file_appender_class);
}

static size_t
get_appender_size(l4sc_appender_cptr_t obj)
{
    return (sizeof(struct l4sc_appender));
}

#if defined(L4SC_WINDOWS_SOCKETS) || defined(L4SC_BSD_SOCKETS)

/* re-use properties of fileappender */
#define remotehost filename
#define remoteport maxbackupindex

#if defined(L4SC_WINDOWS_SOCKETS)
static SOCKET *
get_socket_storage(l4sc_appender_ptr_t appender)
{
    return (SOCKET *)&appender->fu;
}
#define get_socket(a) *get_socket_storage(a)
#else
#define get_socket(a) (a)->fu.fd
#endif

static int
set_appender_option(l4sc_appender_ptr_t obj, const char *name, size_t namelen,
                    const char *value, size_t vallen)
{
    static const char thisfunction[] = "set_appender_option";

    LOGINFO(("%s: %.*s=\"%.*s\"", thisfunction, (int)namelen, name,
             (int)vallen, value));

    if ((namelen == 10) && (strncasecmp(name, "RemoteHost", 10) == 0)) {
        const int n = vallen;
        if ((n > 0) && (n < sizeof(obj->pathbuf))) {
            memcpy(obj->pathbuf, value, n);
            obj->pathbuf[n] = '\0';
            obj->remotehost = obj->pathbuf;
            LOGINFO(("%s: Host set to \"%s\"", thisfunction, obj->remotehost));
        } else {
            bfc_mempool_t pool =
                obj->parent_pool ? obj->parent_pool : get_default_mempool();
            char *p = mempool_alloc(pool, n + 20);
            if (p != NULL) {
                memcpy(p, value, n);
                p[n] = '\0';
                obj->remotehost = p;
                LOGINFO(("%s: Host set to \"%s\" (malloc)", thisfunction,
                         obj->remotehost));
            } else {
                LOGERROR(("%s: no memory for Host \"%.*s\"", thisfunction,
                          (int)vallen, value));
            }
        }
    } else if ((namelen == 4) && (strncasecmp(name, "Port", 4) == 0)) {
        obj->remoteport = strtoul(value, NULL, 10);
        LOGINFO(("%s: Port set to %u", thisfunction, obj->remoteport));
    }

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
            if (is_open(appender)) {
#if defined(L4SC_WINDOWS_SOCKETS)
                SOCKET sock = get_socket(appender);
                if (send(sock, buf, len, 0) == -1) {
                    close_appender(appender);
                }
#else
                int sock = get_socket(appender);
                if (send(sock, buf, len, MSG_NOSIGNAL) == -1) {
                    close_appender(appender);
                }
#endif
            }
        }
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

union anyaddr {
    struct sockaddr sa;
    struct sockaddr_in sin;
    unsigned long space[8];
} addrbuf;

static int
set_address(union anyaddr *addr, size_t bufsize, int family, const char *host,
            unsigned port)
{
    static const char thisfunction[] = "set_address";

    memset(addr, 0, bufsize);
    addr->sin.sin_family = AF_INET;
    ((unsigned char *)&addr->sin.sin_addr)[0] = 127;
    ((unsigned char *)&addr->sin.sin_addr)[3] = 1;

    if ((host != NULL) && (strlen(host) > 0) &&
        (strcmp(host, "localhost") != 0) && (strcmp(host, "127.0.0.1") != 0)) {
        LOGERROR(("%s: TODO: address lookup!", thisfunction));
    }

    ((unsigned char *)&addr->sin.sin_port)[0] = (unsigned char)(port >> 8);
    ((unsigned char *)&addr->sin.sin_port)[1] = (unsigned char)(port);

    return (sizeof(struct sockaddr_in));
}

static int
format_header(l4sc_appender_ptr_t appender, int kind, char *buf,
              size_t bufsize)
{
    l4sc_layout_ptr_t layout = &appender->layout;
    RETURN_METHCALL(l4sc_layout_class_ptr_t, layout, header,
                    (layout, kind, buf, bufsize), 0);
}

static void
open_appender(l4sc_appender_ptr_t appender)
{
    int hdrlen = 0;
    char header[200];
    static const char thisfunction[] = "open_appender";

    if (appender->remoteport > 0) {
        int addrsize;
        union anyaddr addrbuf;
#if defined(L4SC_WINDOWS_SOCKETS)
        SOCKET sock;
        int err;
        DWORD flag;
#define INVALSOCK INVALID_SOCKET
#define CLOSESOCK closesocket
#define SOCKERROR WSAGetLastError()
#else
        int sock, err, flag;
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
            LOGERROR(("%s: bind failed, err %d %s", thisfunction, err,
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

        flag = 1;
        if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag,
                       sizeof(flag)) < 0) {
            err = SOCKERROR;
            LOGERROR(("%s: TCP_NODELAY failed, err %d %s", thisfunction, err,
                      strerror(err)));
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
}

static void
close_appender(l4sc_appender_ptr_t appender)
{
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
}

static int
init_layout(l4sc_appender_ptr_t appender, l4sc_layout_class_ptr_t cls)
{
    RETURN_CMETHCALL(
        l4sc_layout_class_ptr_t, cls, init,
        (&appender->layout, sizeof(appender->layout), appender->parent_pool),
        -ENOSYS);
}

#endif /* defined(L4SC_WINDOWS_SOCKETS) || defined(L4SC_BSD_SOCKETS) */
