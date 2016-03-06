
#include <stddef.h>
#include <stdlib.h>

#include "logobjs.h"

void
l4sc_log(l4sc_logger_ptr_t logger, int level, const char *msg,
	 size_t msglen, const char *file, int line, const char *func)
{
	RETURN_METHCALL(l4sc_logger_class_ptr_t, logger,
			log, (logger, level, msg, msglen, file, line, func),
			(void) 0);
}

