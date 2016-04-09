#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

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

	VOID_CMETHCALL(l4sc_configurator_class_ptr_t, configurator,
			init, (&obj, sizeof(obj), pool));

	RETVAR_CMETHCALL(rc, l4sc_configurator_class_ptr_t, configurator,
			configure_from_file, (&obj, pathbuf), -ENOSYS);

	VOID_CMETHCALL(l4sc_configurator_class_ptr_t, configurator,
			destroy, (&obj));

	l4sc_set_configured(1);

	return (rc);
}

