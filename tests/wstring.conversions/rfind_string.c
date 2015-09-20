
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "barefootc/utf8.h"

static bfc_mempool_t pool;
static l4sc_logger_ptr_t logger;

static int
test(const wchar_t *s1, const char *s2, size_t pos1, size_t expected)
{
	int rc;
	bfc_string_t s, pat;
	size_t pos, wslen, bufsize;
	char *u8buf;

	wslen = wcslen(s1);
	bufsize = 2*wslen + 20;
	u8buf = alloca(bufsize);
	bfc_utf8_from_wchar(u8buf, bufsize, s1, wslen);

	L4SC_TRACE(logger, "%s(L\"%s\", \"%s\", %lu, expected %lu)",
		__FUNCTION__, u8buf, s2, (unsigned long) pos1,
		(unsigned long) expected);

	rc = bfc_init_basic_wstring_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	rc = bfc_init_shared_string_c_str(&pat, sizeof(pat), s2);
	assert(rc >= 0);

	pos = bfc_string_rfind_bfstr(&s, &pat, pos1);
	assert (pos == expected);

	L4SC_TRACE(logger, "%s(\"%s\", L\"%s\", %lu, expected %lu): pos %lu",
		"PASS", u8buf, s2, (unsigned long) pos1,
		(unsigned long)expected, (unsigned long)pos);

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);

#define S1(x) L##x
#define S2(x) x
	test(S1(""), S2(""), 0, 0);
	test(S1(""), S2("abcde"), 0, BFC_NPOS);
	test(S1(""), S2("abcdeabcde"), 0, BFC_NPOS);
	test(S1(""), S2("abcdeabcdeabcdeabcde"), 0, BFC_NPOS);
	test(S1(""), S2(""), 1, 0);
	test(S1(""), S2("abcde"), 1, BFC_NPOS);
	test(S1(""), S2("abcdeabcde"), 1, BFC_NPOS);
	test(S1(""), S2("abcdeabcdeabcdeabcde"), 1, BFC_NPOS);
	test(S1("abcde"), S2(""), 0, 0);
	test(S1("abcde"), S2("abcde"), 0, 0);
	test(S1("abcde"), S2("abcdeabcde"), 0, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), 0, BFC_NPOS);
	test(S1("abcde"), S2(""), 1, 1);
	test(S1("abcde"), S2("abcde"), 1, 0);
	test(S1("abcde"), S2("abcdeabcde"), 1, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), 1, BFC_NPOS);
	test(S1("abcde"), S2(""), 2, 2);
	test(S1("abcde"), S2("abcde"), 2, 0);
	test(S1("abcde"), S2("abcdeabcde"), 2, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), 2, BFC_NPOS);
	test(S1("abcde"), S2(""), 4, 4);
	test(S1("abcde"), S2("abcde"), 4, 0);
	test(S1("abcde"), S2("abcdeabcde"), 4, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), 4, BFC_NPOS);
	test(S1("abcde"), S2(""), 5, 5);
	test(S1("abcde"), S2("abcde"), 5, 0);
	test(S1("abcde"), S2("abcdeabcde"), 5, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), 5, BFC_NPOS);
	test(S1("abcde"), S2(""), 6, 5);
	test(S1("abcde"), S2("abcde"), 6, 0);
	test(S1("abcde"), S2("abcdeabcde"), 6, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), 6, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), 0, 0);
	test(S1("abcdeabcde"), S2("abcde"), 0, 0);
	test(S1("abcdeabcde"), S2("abcdeabcde"), 0, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), 0, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), 1, 1);
	test(S1("abcdeabcde"), S2("abcde"), 1, 0);
	test(S1("abcdeabcde"), S2("abcdeabcde"), 1, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), 1, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), 5, 5);
	test(S1("abcdeabcde"), S2("abcde"), 5, 5);
	test(S1("abcdeabcde"), S2("abcdeabcde"), 5, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), 5, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), 9, 9);
	test(S1("abcdeabcde"), S2("abcde"), 9, 5);
	test(S1("abcdeabcde"), S2("abcdeabcde"), 9, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), 9, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), 10, 10);
	test(S1("abcdeabcde"), S2("abcde"), 10, 5);
	test(S1("abcdeabcde"), S2("abcdeabcde"), 10, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), 10, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), 11, 10);
	test(S1("abcdeabcde"), S2("abcde"), 11, 5);
	test(S1("abcdeabcde"), S2("abcdeabcde"), 11, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), 11, BFC_NPOS);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), 0, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), 0, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), 0, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"), 0, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), 1, 1);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), 1, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), 1, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"), 1, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), 10, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), 10, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), 10, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"), 10, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), 19, 19);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), 19, 15);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), 19, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"), 19, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), 20, 20);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), 20, 15);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), 20, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"), 20, 0);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), 21, 20);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), 21, 15);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), 21, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"), 21, 0);

	test(S1(""), S2(""), BFC_NPOS, 0);
	test(S1(""), S2("abcde"), BFC_NPOS, BFC_NPOS);
	test(S1(""), S2("abcdeabcde"), BFC_NPOS, BFC_NPOS);
	test(S1(""), S2("abcdeabcdeabcdeabcde"), BFC_NPOS, BFC_NPOS);
	test(S1("abcde"), S2(""), BFC_NPOS, 5);
	test(S1("abcde"), S2("abcde"), BFC_NPOS, 0);
	test(S1("abcde"), S2("abcdeabcde"), BFC_NPOS, BFC_NPOS);
	test(S1("abcde"), S2("abcdeabcdeabcdeabcde"), BFC_NPOS, BFC_NPOS);
	test(S1("abcdeabcde"), S2(""), BFC_NPOS, 10);
	test(S1("abcdeabcde"), S2("abcde"), BFC_NPOS, 5);
	test(S1("abcdeabcde"), S2("abcdeabcde"), BFC_NPOS, 0);
	test(S1("abcdeabcde"), S2("abcdeabcdeabcdeabcde"), BFC_NPOS, BFC_NPOS);
	test(S1("abcdeabcdeabcdeabcde"), S2(""), BFC_NPOS, 20);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcde"), BFC_NPOS, 15);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcde"), BFC_NPOS, 10);
	test(S1("abcdeabcdeabcdeabcde"), S2("abcdeabcdeabcdeabcde"),BFC_NPOS,0);

	do {
	    wchar_t s1[] = { 'm', 'n', 0xF6, 'p', 'q', 'r', 0xF6, 0 };
	    test(s1, S2("\303\266"), BFC_NPOS, 6);
	    test(s1, S2("\303\266pq"), BFC_NPOS, 2);
	    test(s1, S2("\303\266Xq"), BFC_NPOS, BFC_NPOS);
	    test(s1, S2("\303\266pq"), 3, 2);
	    test(s1, S2("\303\266"), 3, 2);
	    test(s1, S2("mn\303\266pq"), BFC_NPOS, 0);
	} while (0 /* just once */);

	return (0);
}
