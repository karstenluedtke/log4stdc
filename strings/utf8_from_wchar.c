
#include <wchar.h>

#include "barefootc/utf8.h"

size_t
bfc_utf8_from_wchar(char *buf, size_t max, const wchar_t *src, size_t len)
{
	const wchar_t *wp = src;
	const wchar_t *ep = src + len;
	const char *limit = buf + max;
	char *cp = buf;
	uint32_t codept;

	if (sizeof(*wp) == 2) {
		while (wp < ep) {
			BFC_GET_UTF16(codept, wp, ep);
			BFC_PUT_UTF8(cp, limit, codept);
		}
	} else {
		while (wp < ep) {
			codept = *(wp++);
			BFC_PUT_UTF8(cp, limit, codept);
		}
	}
	if (cp < limit) {
		*cp = '\0';
	} else if (buf && (max > 0)) {
		buf[max-1] = '\0';
	}
	return (cp - buf);
}

