
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

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

static int
test(const char *s1, size_t pos1, const wchar_t *s2, const char *expected)
{
	int rc;
	bfc_string_t s, ws;
	size_t origlen, wslen, newlen, bufsize;
	char *u8buf;

	wslen = wcslen(s2);
	bufsize = 2*wslen + 20;
	u8buf = alloca(bufsize);
	bfc_utf8_from_wchar(u8buf, bufsize, s2, wslen);

	L4SC_TRACE(logger, "%s(\"%s\", pos %ld, L\"%s\", expected \"%s\")",
		__FUNCTION__, s1, (long) pos1, u8buf, expected);

	rc = bfc_init_basic_string_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_init_shared_wstring_c_str(&ws, sizeof(ws), s2);
	assert(rc >= 0);

	wslen = bfc_string_length(&ws);
	assert(wslen == wcslen(s2));
	
	rc = bfc_string_insert(&s, pos1, &ws);
	newlen = bfc_string_length(&s);
	if (rc >= 0) {
        	assert(pos1 <= origlen);
		assert(newlen == origlen + wslen);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
		assert(bfc_string_compare_c_str(&ws, s2) == 0);
	} else {
		assert(pos1 > origlen);
		assert(newlen == origlen);
		assert(bfc_string_compare_c_str(&s, s1) == 0);
		assert(bfc_string_compare_c_str(&ws, s2) == 0);
	}

	bfc_destroy(&s);
	bfc_destroy(&ws);

	L4SC_TRACE(logger,
		"%s(\"%s\", pos %ld, L\"%s\", expected \"%s\"): rc %d",
		"PASS", s1, (long) pos1, u8buf, expected, rc);

	return (BFC_SUCCESS);
}


int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);

#define S1(x) x
#define S2(x) L##x
#define SE(x) x
	test(S1(""), 0, S2(""), SE(""));
	test(S1(""), 0, S2("12345"), SE("12345"));
	test(S1(""), 0, S2("1234567890"), SE("1234567890"));
	test(S1(""), 0, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1(""), 1, S2(""), SE("can't happen"));
	test(S1(""), 1, S2("12345"), SE("can't happen"));
	test(S1(""), 1, S2("1234567890"), SE("can't happen"));
	test(S1(""), 1, S2("12345678901234567890"), SE("can't happen"));
	test(S1("abcde"), 0, S2(""), SE("abcde"));
	test(S1("abcde"), 0, S2("12345"), SE("12345abcde"));
	test(S1("abcde"), 0, S2("1234567890"), SE("1234567890abcde"));
	test(S1("abcde"), 0, S2("12345678901234567890"), SE("12345678901234567890abcde"));
	test(S1("abcde"), 1, S2(""), SE("abcde"));
	test(S1("abcde"), 1, S2("12345"), SE("a12345bcde"));
	test(S1("abcde"), 1, S2("1234567890"), SE("a1234567890bcde"));
	test(S1("abcde"), 1, S2("12345678901234567890"), SE("a12345678901234567890bcde"));
	test(S1("abcde"), 2, S2(""), SE("abcde"));
	test(S1("abcde"), 2, S2("12345"), SE("ab12345cde"));
	test(S1("abcde"), 2, S2("1234567890"), SE("ab1234567890cde"));
	test(S1("abcde"), 2, S2("12345678901234567890"), SE("ab12345678901234567890cde"));
	test(S1("abcde"), 4, S2(""), SE("abcde"));
	test(S1("abcde"), 4, S2("12345"), SE("abcd12345e"));
	test(S1("abcde"), 4, S2("1234567890"), SE("abcd1234567890e"));
	test(S1("abcde"), 4, S2("12345678901234567890"), SE("abcd12345678901234567890e"));
	test(S1("abcde"), 5, S2(""), SE("abcde"));
	test(S1("abcde"), 5, S2("12345"), SE("abcde12345"));
	test(S1("abcde"), 5, S2("1234567890"), SE("abcde1234567890"));
	test(S1("abcde"), 5, S2("12345678901234567890"), SE("abcde12345678901234567890"));
	test(S1("abcde"), 6, S2(""), SE("can't happen"));
	test(S1("abcde"), 6, S2("12345"), SE("can't happen"));
	test(S1("abcde"), 6, S2("1234567890"), SE("can't happen"));
	test(S1("abcde"), 6, S2("12345678901234567890"), SE("can't happen"));
	test(S1("abcdefghij"), 0, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 0, S2("12345"), SE("12345abcdefghij"));
	test(S1("abcdefghij"), 0, S2("1234567890"), SE("1234567890abcdefghij"));
	test(S1("abcdefghij"), 0, S2("12345678901234567890"), SE("12345678901234567890abcdefghij"));
	test(S1("abcdefghij"), 1, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 1, S2("12345"), SE("a12345bcdefghij"));
	test(S1("abcdefghij"), 1, S2("1234567890"), SE("a1234567890bcdefghij"));
	test(S1("abcdefghij"), 1, S2("12345678901234567890"), SE("a12345678901234567890bcdefghij"));
	test(S1("abcdefghij"), 5, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 5, S2("12345"), SE("abcde12345fghij"));
	test(S1("abcdefghij"), 5, S2("1234567890"), SE("abcde1234567890fghij"));
	test(S1("abcdefghij"), 5, S2("12345678901234567890"), SE("abcde12345678901234567890fghij"));
	test(S1("abcdefghij"), 9, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 9, S2("12345"), SE("abcdefghi12345j"));
	test(S1("abcdefghij"), 9, S2("1234567890"), SE("abcdefghi1234567890j"));
	test(S1("abcdefghij"), 9, S2("12345678901234567890"), SE("abcdefghi12345678901234567890j"));
	test(S1("abcdefghij"), 10, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 10, S2("12345"), SE("abcdefghij12345"));
	test(S1("abcdefghij"), 10, S2("1234567890"), SE("abcdefghij1234567890"));
	test(S1("abcdefghij"), 10, S2("12345678901234567890"), SE("abcdefghij12345678901234567890"));
	test(S1("abcdefghij"), 11, S2(""), SE("can't happen"));
	test(S1("abcdefghij"), 11, S2("12345"), SE("can't happen"));
	test(S1("abcdefghij"), 11, S2("1234567890"), SE("can't happen"));
	test(S1("abcdefghij"), 11, S2("12345678901234567890"), SE("can't happen"));
	test(S1("abcdefghijklmnopqrst"), 0, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, S2("12345"), SE("12345abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, S2("1234567890"), SE("1234567890abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, S2("12345678901234567890"), SE("12345678901234567890abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, S2("12345"), SE("a12345bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, S2("1234567890"), SE("a1234567890bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, S2("12345678901234567890"), SE("a12345678901234567890bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, S2("12345"), SE("abcdefghij12345klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, S2("1234567890"), SE("abcdefghij1234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, S2("12345678901234567890"), SE("abcdefghij12345678901234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 19, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 19, S2("12345"), SE("abcdefghijklmnopqrs12345t"));
	test(S1("abcdefghijklmnopqrst"), 19, S2("1234567890"), SE("abcdefghijklmnopqrs1234567890t"));
	test(S1("abcdefghijklmnopqrst"), 19, S2("12345678901234567890"), SE("abcdefghijklmnopqrs12345678901234567890t"));
	test(S1("abcdefghijklmnopqrst"), 20, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 20, S2("12345"), SE("abcdefghijklmnopqrst12345"));
	test(S1("abcdefghijklmnopqrst"), 20, S2("1234567890"), SE("abcdefghijklmnopqrst1234567890"));
	test(S1("abcdefghijklmnopqrst"), 20, S2("12345678901234567890"), SE("abcdefghijklmnopqrst12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 21, S2(""), SE("can't happen"));
	test(S1("abcdefghijklmnopqrst"), 21, S2("12345"), SE("can't happen"));
	test(S1("abcdefghijklmnopqrst"), 21, S2("1234567890"), SE("can't happen"));
	test(S1("abcdefghijklmnopqrst"), 21, S2("12345678901234567890"), SE("can't happen"));

	/* pos counts bytes, not utf-8 characters: 2 positions each umlaut */
	test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"), 10, S2("12345"), SE("\303\244bcdefghi12345jklmn\303\266pqrst\303\274"));
	return (0);
}
