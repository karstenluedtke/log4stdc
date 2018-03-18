
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "compat.h"
#include "logobjs.h"
#include "bareftc/atomic.h"
#include "bareftc/mempool.h"

static void
l4sc_destroy_base_object(l4sc_objptr_t obj);

struct l4sc_object {
    BFC_OBJHDR(const struct l4sc_object_class *, struct l4sc_object *)
};

const struct l4sc_object_class l4sc_object_class = {
    /* intentionally not using selective initialization for base class: */
    /* I want the compiler to complain if something is missing.         */
    /* .super        */ NULL,
    /* .name         */ "object",
    /* .spare2       */ NULL,
    /* .spare3       */ NULL,
    /* .init         */ l4sc_default_init_object,
    /* .initrefc     */ l4sc_default_init_refcount,
    /* .incrrefc     */ l4sc_default_incr_refcount,
    /* .decrrefc     */ l4sc_default_decr_refcount,
    /* .destroy      */ l4sc_destroy_base_object,
    /* .clone        */ l4sc_default_clone_object,
    /* .clonesize    */ l4sc_default_get_object_size,
    /* .compare      */ l4sc_default_compare_object,
    /* .hashcode     */ l4sc_default_get_object_hashcode,
    /* .length       */ l4sc_default_get_object_length,
    /* .tostring     */ l4sc_default_object_tostring,
    /* .dump         */ l4sc_default_dump_object,
    /* .set_name     */ l4sc_default_set_object_name,
    /* .set_opt      */ l4sc_default_set_object_option,
    /* .get_opt      */ l4sc_default_get_object_option,
    /* .apply        */ l4sc_default_apply_object_options,
    /* .close        */ l4sc_default_close_object};

int
l4sc_default_init_object(void *buf, size_t bufsize, bfc_mempool_t pool)
{
    BFC_INIT_PROLOGUE(const struct l4sc_object_class *, l4sc_objptr_t, object,
                      buf, bufsize, pool, &l4sc_object_class);
    return (BFC_SUCCESS);
}

int
l4sc_default_init_refcount(l4sc_objptr_t obj, int n)
{
    bfc_init_atomic_counter(obj->refc, n);
    return (n);
}

int
l4sc_default_incr_refcount(l4sc_objptr_t obj)
{
    int incremented_refcount = bfc_incr_atomic_counter(obj->refc);
    return (incremented_refcount);
}

int
l4sc_default_decr_refcount(l4sc_objptr_t obj)
{
    int decremented_refcount = bfc_decr_atomic_counter(obj->refc);
    if (decremented_refcount == 0) {
        struct mempool *pool = obj->parent_pool;
        VOID_METHCALL(const struct l4sc_object_class *, obj, destroy, (obj));
        if (pool != NULL) {
            mempool_free(pool, obj);
        }
    }
    return (decremented_refcount);
}

void
l4sc_default_destroy_object(l4sc_objptr_t obj)
{
    l4sc_class_ptr_t cls;

    if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
        BFC_DESTROY_EPILOGUE(obj, cls);
    }
}

static void
l4sc_destroy_base_object(l4sc_objptr_t obj)
{
    obj->vptr = 0;
}

int
l4sc_default_clone_object(l4sc_objcptr_t obj, void *buf, size_t bufsize,
                          bfc_mempool_t pool)
{
    l4sc_objptr_t object = (l4sc_objptr_t)buf;
    size_t size;
    RETVAR_METHCALL(size, bfc_classptr_t, obj, clonesize, ((bfc_cobjptr_t)obj),
                    10000);
    if (bufsize < size) {
        return (-ENOSPC);
    }
    memcpy(object, obj, size);
    return (BFC_SUCCESS);
}

size_t
l4sc_default_get_object_size(l4sc_objcptr_t obj)
{
    return (sizeof(struct l4sc_object));
}

unsigned
l4sc_reduce_hashcode(size_t origval, int origbits, int hashlen)
{
    size_t x = origval;
    unsigned mask = (1u << hashlen) - 1;
    unsigned code = (unsigned)x & mask;
    unsigned over;
    int bits = origbits;
    while (bits > hashlen) {
        x >>= hashlen;
        bits -= hashlen;
        code += (unsigned)x & mask;
    }
    /* add overflow bits as least significant bits (like TCP/IP) */
    while ((over = (code >> hashlen)) > 0) {
        code = (code & mask) + over;
    }
    return (code);
}

unsigned
l4sc_default_get_object_hashcode(l4sc_objcptr_t obj, int hashlen)
{
    return (l4sc_reduce_hashcode((size_t)obj, 8 * sizeof(obj), hashlen));
}

int
l4sc_default_compare_object(l4sc_objcptr_t obj, l4sc_objcptr_t other)
{
    return (obj != other);
}

size_t
l4sc_default_get_object_length(l4sc_objcptr_t obj)
{
    return (0);
}

int
l4sc_default_object_tostring(l4sc_objcptr_t obj, char *buf, size_t bufsize,
                             const char *fmt)
{
    if (obj && obj->name && buf) {
        size_t namelen = strlen(obj->name);
        if (bufsize > namelen) {
            memcpy(buf, obj->name, namelen);
            buf[namelen] = 0;
            return ((int)namelen);
        }
    }
    return (0);
}

void
l4sc_default_dump_object(l4sc_objcptr_t obj, int depth,
                         struct l4sc_logger *log)
{
    size_t size;
    const unsigned char *p, *lim;

    if (obj && obj->name && BFC_CLASS(obj)) {
        LOGDEBUG(("object \"%s\" @%p", obj->name, obj));
        LOGDEBUG((" class \"%s\", pool %p, lock %p, refc %d",
                  BFC_CLASS(obj)->name, obj->parent_pool, obj->lock,
                  obj->refc));
        RETVAR_METHCALL(size, bfc_classptr_t, obj, clonesize,
                        ((bfc_cobjptr_t)obj), 10000);
        lim = ((const unsigned char *)obj) + size;
        for (p = (const unsigned char *)obj; p < lim; p += 8) {
            LOGDEBUG((" %p: %02x %02x %02x %02x %02x %02x %02x %02x", p, p[0],
                      p[1], p[2], p[3], p[4], p[5], p[6], p[7]));
        }
    }
}

void
l4sc_default_set_object_name(l4sc_objptr_t obj, const char *name, int len)
{
}

int
l4sc_default_set_object_option(l4sc_objptr_t obj, const char *name,
                               size_t namelen, const char *value,
                               size_t vallen)
{
    return (0);
}

int
l4sc_default_get_object_option(l4sc_objcptr_t obj, const char *name,
                               size_t namelen, char *valbuf, size_t bufsize)
{
    return (0);
}

void
l4sc_default_apply_object_options(l4sc_objptr_t obj)
{
}

void
l4sc_default_close_object(l4sc_objptr_t obj)
{
}

int
l4sc_set_object_option(l4sc_objptr_t obj, const char *name, size_t namelen,
                       const char *value, size_t vallen)
{
    int nmlen = (namelen > 0) ? namelen : strlen(name);

    if ((name == NULL) || (nmlen < 1)) {
        LOGERROR(("%s: no name", __FUNCTION__));
        return (-EFAULT);
    }

    RETURN_METHCALL(l4sc_class_ptr_t, obj, set_opt,
                    (obj, name, nmlen, value, vallen), 0);
}
