
#include <stddef.h>
#include <stdlib.h>

#include "logobjs.h"

int
l4sc_formatmsg(l4sc_layout_ptr_t layout, l4sc_logmessage_cptr_t msg, char *buf,
               size_t bufsize)
{
    RETURN_METHCALL(l4sc_layout_class_ptr_t, layout, format,
                    (layout, msg, buf, bufsize), 0);
}
