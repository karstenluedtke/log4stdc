
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "logobjects.h"

struct l4sc_object {
	BFC_OBJHDR(const struct l4sc_object_class *, struct l4sc_object *)
};

const struct l4sc_object_class l4sc_object_class = {
	.super = NULL,
	.name = "object",
	.init = l4sc_default_init_object,
	.destroy = l4sc_default_destroy_object,
	.clone = l4sc_default_clone_object,
	.clonesize = l4sc_default_get_object_size,
	.hashcode = l4sc_default_get_object_hashcode,
	.equals = l4sc_default_is_equal_object,
	.length = l4sc_default_get_object_length,
	.tostring = l4sc_default_object_tostring,

	.set_name = l4sc_default_set_object_name,
	.set_opt = l4sc_default_set_object_option,
	.get_opt = l4sc_default_get_object_option,
	.apply = l4sc_default_apply_object_options,
	.close = l4sc_default_close_object,
};

l4sc_objptr_t
l4sc_default_init_object(void *buf, size_t bufsize)
{
	BFC_INIT_PROLOGUE(const struct l4sc_object_class *,
			  l4sc_objptr_t, object, buf, bufsize,
			  &l4sc_object_class);
	return ((l4sc_objptr_t) object);
}

void
l4sc_default_destroy_object(l4sc_objptr_t obj)
{
	l4sc_class_ptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

l4sc_objptr_t
l4sc_default_clone_object(l4sc_objcptr_t obj, void *buf, size_t bufsize)
{
	l4sc_objptr_t object = (l4sc_objptr_t) buf;
	size_t size = VMETHCALL(obj, clonesize, (obj), sizeof(*object));
	if (bufsize < size) {
		return (NULL);
	}
	memcpy(object, obj, size);
	return ((l4sc_objptr_t) object);
}

size_t
l4sc_default_get_object_size(l4sc_objcptr_t obj)
{
	return (sizeof(struct l4sc_object));
}

unsigned  
l4sc_default_get_object_hashcode(l4sc_objcptr_t obj)
{
	size_t x = (size_t) obj;
	if (sizeof(x) > sizeof(unsigned)) {
		x ^= x >> (8*sizeof(unsigned));
	}
	return ((unsigned) x);
}

int
l4sc_default_is_equal_object(l4sc_objcptr_t obj, l4sc_objcptr_t other)
{
	return (obj == other);
}

int
l4sc_default_get_object_length(l4sc_objcptr_t obj)
{
	return (0);
}

int
l4sc_default_object_tostring(l4sc_objcptr_t obj, char *buf, size_t bufsize)
{
	if (obj && obj->name && buf) {
		size_t namelen = strlen(obj->name);
		if (bufsize > namelen) {
			memcpy(buf, obj->name, namelen);
			buf[namelen] = 0;
			return((int) namelen);
		}
	}
	return (0);
}


void
l4sc_default_set_object_name(l4sc_objptr_t obj, const char *name, int len)
{
}

int
l4sc_default_set_object_option(l4sc_objptr_t obj,
				const char *name, size_t namelen,
				const char *value, size_t vallen)
{
	return (0);
}

int
l4sc_default_get_object_option(l4sc_objcptr_t obj,
				const char *name, size_t namelen,
				char *valbuf, size_t bufsize)
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
l4sc_set_object_option(l4sc_objptr_t obj,
			const char *name, size_t namelen,
			const char *value, size_t vallen)
{
	int rc, nmlen = (namelen > 0)? namelen: strlen(name);

	if ((name == NULL) || (nmlen < 1)) {
		LOGERROR(("%s: no name", __FUNCTION__));
		return (-EFAULT);
	}

	rc = VMETHCALL(obj, set_opt, (obj, name, nmlen, value, vallen), 0);
	return (rc);
}
