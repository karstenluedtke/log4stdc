
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "compat.h"
#include "logobjs.h"
#include "bareftc/mempool.h"

int
l4sc_vlog(l4sc_logger_ptr_t logger, int level, size_t maxbytes, int partial,
          const char *file, int line, const char *func, const char *fmt,
          va_list ap)
{
    int rc;
    size_t len = 0;

    if (l4sc_logger_enabled(logger, level)) {
        bfc_mempool_t pool = logger->parent_pool;
        char *poolmem = ((maxbytes > 2000) && pool)
                            ? bfc_mempool_alloc(pool, maxbytes + 1)
                            : NULL;
        char *msg = poolmem ? poolmem : alloca(maxbytes + 1);
        rc = vsnprintf(msg, maxbytes + 1, fmt, ap);
        if ((rc < 0) || ((len = (size_t)rc) > maxbytes)) {
            if (partial) {
                len = maxbytes;
            } else {
                return (rc);
            }
        }
        l4sc_log(logger, level, msg, len, file, line, func);
        if (poolmem) {
            bfc_mempool_free(pool, poolmem);
            poolmem = NULL;
        }
    }
    return ((int)len);
}
