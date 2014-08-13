
#include <wchar.h>

#include "barefootc/utf8.h"

size_t
bfc_wchar_from_utf8(wchar_t *buf, size_t max, const char *src, size_t len)
{
	const char *cp = src;
	const char *ep = src + len;
	const wchar_t *limit = buf + max;
	wchar_t *wp = buf;
	uint32_t codept;

	if (sizeof(*wp) == 2) {
		while (cp < ep) {
			BFC_GET_UTF8(codept, cp, ep);
			BFC_PUT_UTF16(wp, limit, codept);
		}
	} else {
		while (cp < ep) {
			BFC_GET_UTF8(codept, cp, ep);
			*(wp++) = codept;
		}
	}
	if (wp < limit) {
		*wp = '\0';
	} else if (buf && (max > 0)) {
		buf[max-1] = '\0';
	}
	return (wp - buf);
}

