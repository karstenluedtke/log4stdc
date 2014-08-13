
#include "barefootc/utf8.h"

size_t
bfc_utf16_from_utf8(uint16_t *buf, size_t max, const char *src, size_t len)
{
	const char *cp = src;
	const char *ep = src + len;
	const uint16_t *limit = buf + max;
	uint16_t *wp = buf;
	uint32_t codept;

	while (cp < ep) {
		BFC_GET_UTF8(codept, cp, ep);
		BFC_PUT_UTF16(wp, limit, codept);
	}
	if (wp < limit) {
		*wp = '\0';
	} else if (buf && (max > 0)) {
		buf[max-1] = '\0';
	}
	return (wp - buf);
}

