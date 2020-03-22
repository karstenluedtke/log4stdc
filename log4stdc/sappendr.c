
#include <stddef.h>

#include "log4stdc.h"
#include "logobjs.h"

int
l4sc_set_logger_appender(l4sc_logger_ptr_t logger,
                         l4sc_appender_ptr_t appender)
{
    RETURN_METHCALL(l4sc_logger_class_ptr_t, logger, set_appender,
                    (logger, appender), 0);
}
