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
l4sc_configure_from_string(const struct l4sc_configurator_class *configurator,
			   const char *buf, size_t len, ...)
{
	int rc;
	struct l4sc_configurator obj;
	l4sc_class_ptr_t cls;
	struct mempool *pool;
	va_list ap;
	static const char thisfunction[] = "l4sc_configure_from_string";

	pool = l4sc_get_default_mempool(__FILE__, __LINE__, thisfunction);

	va_start(ap, len);
	cls = va_arg(ap, l4sc_class_ptr_t);
	while (cls) {
		l4sc_register_extra_class(cls);
		cls = va_arg(ap, l4sc_class_ptr_t);
	}
	va_end(ap);

	l4sc_close_appenders();

	VOID_CMETHCALL(l4sc_configurator_class_ptr_t, configurator,
			init, (&obj, sizeof(obj), pool));

	RETVAR_CMETHCALL(rc, l4sc_configurator_class_ptr_t, configurator,
			configure, (&obj, buf, len), -ENOSYS);

	VOID_CMETHCALL(l4sc_configurator_class_ptr_t, configurator,
			destroy, (&obj));

	l4sc_set_configured(1);

	return (rc);
}

