#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "compat.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#define L4SC_WINDOWS_FILES 1
#define L4SC_WINDOWS_LOCKS 1
#include <windows.h>
#endif

#include "log4stdc.h"
#include "logobjs.h"

#include "bareftc/object.h"
#include "bareftc/mempool.h"

int
l4sc_configure_from_file(const struct l4sc_configurator_class *configurator,
                         const char *path, ...)
{
    int rc;
    const size_t pathlen = strlen(path);
    const size_t pathbufsize = 256 + pathlen;
    char *pathbuf;
    struct l4sc_configurator obj;
    l4sc_class_ptr_t cls;
    struct mempool *pool;
    va_list ap;
    static const char thisfunction[] = "l4sc_configure_from_file";

    pool = l4sc_get_default_mempool(__FILE__, __LINE__, thisfunction);

    va_start(ap, path);
    cls = va_arg(ap, l4sc_class_ptr_t);
    while (cls) {
        l4sc_register_extra_class(cls);
        cls = va_arg(ap, l4sc_class_ptr_t);
    }
    va_end(ap);

    pathbuf = alloca(pathbufsize);
    l4sc_merge_base_directory_path(pathbuf, pathbufsize, path, pathlen);

    l4sc_close_appenders();

    VOID_CMETHCALL(l4sc_configurator_class_ptr_t, configurator, init,
                   (&obj, sizeof(obj), pool));

    RETVAR_CMETHCALL(rc, l4sc_configurator_class_ptr_t, configurator,
                     configure_from_file, (&obj, pathbuf), -ENOSYS);

    VOID_CMETHCALL(l4sc_configurator_class_ptr_t, configurator, destroy,
                   (&obj));

    l4sc_set_configured(1);

    return (rc);
}

static int
l4sc_read_cfgfile_win(const char *path, struct mempool *pool,
                      /*out*/ char **bufp, /*out*/ size_t *sizep)
{
#if defined(L4SC_WINDOWS_FILES)
    HANDLE fh;
    wchar_t *wbuf;
    char *buff;
    size_t wlen, clen = strlen(path);
    static const char thisfunction[] = "l4sc_read_cfgfile_win";

    wbuf = (wchar_t *)alloca((clen + 1) * sizeof(wchar_t));
    wlen = MultiByteToWideChar(CP_UTF8, 0, path, clen, wbuf, clen + 1);
    wbuf[(wlen < clen) ? wlen : clen] = 0;

    fh = CreateFileW(wbuf, FILE_READ_DATA, FILE_SHARE_READ, NULL,
                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fh != INVALID_HANDLE_VALUE) {
        DWORD sizehigh = 0;
        DWORD size = GetFileSize(fh, &sizehigh);
        DWORD dwBytesRead = 0;
        LOGINFO(("%s: opening file \"%s\" succeded, size %ld", thisfunction,
                 path, (long)size));
        if (size != INVALID_FILE_SIZE) {
            buff = mempool_alloc(pool, size + 1);
            if (buff) {
                if (ReadFile(fh, buff, size, &dwBytesRead, NULL)) {
                    LOGINFO(("%s: reading file \"%s\" succeded, size %ld",
                             thisfunction, path, (long)dwBytesRead));
                } else {
                    dwBytesRead = 0;
                }
                CloseHandle(fh);
                if ((dwBytesRead >= 0) && (dwBytesRead <= size)) {
                    buff[dwBytesRead] = '\0';
                    *bufp = buff;
                    *sizep = (size_t)dwBytesRead;
                    return (int)dwBytesRead;
                } else {
                    mempool_free(pool, buff);
                }
            }
        }
    }
    return -ENOENT;
#else // !defined(L4SC_WINDOWS_FILES)
    return -ENOSYS;
#endif
}

int
l4sc_read_cfgfile(const char *path, struct mempool *pool,
                  /*out*/ char **bufp, /*out*/ size_t *sizep)
{
    FILE *fp;
    char *buff;
    int rc = 0, rc2 = 0;
    size_t bufsize = 8000, length = 0;
    static const char thisfunction[] = "l4sc_read_cfgfile";

    fp = fopen(path, "r");
    if (fp == NULL) {
        rc = -errno;
        if ((rc2 = l4sc_read_cfgfile_win(path, pool, bufp, sizep)) > 0) {
            return rc2;
        }
        LOGERROR(("%s: opening file \"%s\" failed, error %d: %s", thisfunction,
                  path, rc, strerror(-rc)));
        return (rc);
    }

    buff = mempool_alloc(pool, bufsize);
    while ((rc = fread(buff + length, 1, bufsize - length, fp)) > 0) {
        length += rc;
        if (length >= bufsize) {
            bufsize = (3 * bufsize / 2) + 4000;
            LOGDEBUG(("%s: have %ld, realloc(%ld)...", thisfunction,
                      (long)length, (long)bufsize));
            buff = mempool_realloc(pool, buff, bufsize);
            if (buff == NULL) {
                rc = -ENOMEM;
                LOGERROR(
                    ("%s: realloc(%ld) failed.", thisfunction, (long)bufsize));
                fclose(fp);
                return (rc);
            }
        }
    }
    fclose(fp);

    *bufp = buff;
    *sizep = length;

    return (int)length;
}
