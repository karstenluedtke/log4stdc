
#include <stddef.h>

#include "log4stdc.h"
#include "logobjs.h"

int
l4sc_set_logger_parent(l4sc_logger_ptr_t logger, l4sc_logger_ptr_t parent)
{
    RETURN_METHCALL(l4sc_logger_class_ptr_t, logger, set_parent,
                    (logger, parent), 0);
}
