
#include <stddef.h>
#include <stdlib.h>

#include "logobjects.h"

void
l4sc_log(l4sc_logger_ptr_t logger, int level, const char *msg,
	 size_t msglen, const char *file, int line, const char *func)
{
	VMETHCALL(logger,log, (logger, level, msg, msglen, file, line, func),
		  (void) 0);
}
