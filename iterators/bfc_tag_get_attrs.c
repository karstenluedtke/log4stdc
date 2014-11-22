
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/xmltag.h"

int
bfc_tag_get_attrs(bfc_ctagptr_t tag, bfc_string_t *buf, size_t bufsize)
{
	RETURN_METHCALL(bfc_xmltag_classptr_t, tag,
			get_attrs, (tag, buf, bufsize),
			-ENOSYS);
}

