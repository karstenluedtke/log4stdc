
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
test(const char *s1, size_t pos1, size_t n1, const wchar_t *s2,
						const char *expected)
{
	int rc;
	bfc_string_t s, ws;
	size_t origlen, xlen, wslen, newlen, bufsize;
	char *u8buf;

	wslen = wcslen(s2);
	bufsize = 2*wslen + 20;
	u8buf = alloca(bufsize);
	bfc_utf8_from_wchar(u8buf, bufsize, s2, wslen);

	L4SC_TRACE(logger,
		"%s(\"%s\", pos %ld, n %ld, L\"%s\", expected \"%s\")",
		__FUNCTION__, s1, (long) pos1, (long)n1, u8buf, expected);

	rc = bfc_init_basic_string_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));

	xlen = n1;
	if ((pos1 == BFC_NPOS) || (pos1 >= origlen)) {
		xlen = 0;
	} else if ((n1 == BFC_NPOS) || (n1 >= origlen - pos1)) {
		xlen = origlen - pos1;
	}
	assert(xlen <= n1);
	assert(xlen <= origlen);
	
	rc = bfc_init_shared_wstring_c_str(&ws, sizeof(ws), s2);
	assert(rc >= 0);

	wslen = bfc_string_length(&ws);
	assert(wslen == wcslen(s2));
	
	rc = bfc_string_replace(&s, pos1, n1, &ws);
	newlen = bfc_string_length(&s);
	if (rc >= 0) {
        	assert(pos1 <= origlen);
		assert(newlen == origlen - xlen + wslen);
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
	test(S1(""), 0, 0, S2(""), SE(""));
	test(S1(""), 0, 0, S2("12345"), SE("12345"));
	test(S1(""), 0, 0, S2("1234567890"), SE("1234567890"));
	test(S1(""), 0, 0, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1(""), 0, 1, S2(""), SE(""));
	test(S1(""), 0, 1, S2("12345"), SE("12345"));
	test(S1(""), 0, 1, S2("1234567890"), SE("1234567890"));
	test(S1(""), 0, 1, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1(""), 1, 0, S2(""), SE("can't happen"));
	test(S1(""), 1, 0, S2("12345"), SE("can't happen"));
	test(S1(""), 1, 0, S2("1234567890"), SE("can't happen"));
	test(S1(""), 1, 0, S2("12345678901234567890"), SE("can't happen"));
	test(S1("abcde"), 0, 0, S2(""), SE("abcde"));
	test(S1("abcde"), 0, 0, S2("12345"), SE("12345abcde"));
	test(S1("abcde"), 0, 0, S2("1234567890"), SE("1234567890abcde"));
	test(S1("abcde"), 0, 0, S2("12345678901234567890"), SE("12345678901234567890abcde"));
	test(S1("abcde"), 0, 1, S2(""), SE("bcde"));
	test(S1("abcde"), 0, 1, S2("12345"), SE("12345bcde"));
	test(S1("abcde"), 0, 1, S2("1234567890"), SE("1234567890bcde"));
	test(S1("abcde"), 0, 1, S2("12345678901234567890"), SE("12345678901234567890bcde"));
	test(S1("abcde"), 0, 2, S2(""), SE("cde"));
	test(S1("abcde"), 0, 2, S2("12345"), SE("12345cde"));
	test(S1("abcde"), 0, 2, S2("1234567890"), SE("1234567890cde"));
	test(S1("abcde"), 0, 2, S2("12345678901234567890"), SE("12345678901234567890cde"));
	test(S1("abcde"), 0, 4, S2(""), SE("e"));
	test(S1("abcde"), 0, 4, S2("12345"), SE("12345e"));
	test(S1("abcde"), 0, 4, S2("1234567890"), SE("1234567890e"));
	test(S1("abcde"), 0, 4, S2("12345678901234567890"), SE("12345678901234567890e"));
	test(S1("abcde"), 0, 5, S2(""), SE(""));
	test(S1("abcde"), 0, 5, S2("12345"), SE("12345"));
	test(S1("abcde"), 0, 5, S2("1234567890"), SE("1234567890"));
	test(S1("abcde"), 0, 5, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1("abcde"), 0, 6, S2(""), SE(""));
	test(S1("abcde"), 0, 6, S2("12345"), SE("12345"));
	test(S1("abcde"), 0, 6, S2("1234567890"), SE("1234567890"));
	test(S1("abcde"), 0, 6, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1("abcde"), 1, 0, S2(""), SE("abcde"));
	test(S1("abcde"), 1, 0, S2("12345"), SE("a12345bcde"));
	test(S1("abcde"), 1, 0, S2("1234567890"), SE("a1234567890bcde"));
	test(S1("abcde"), 1, 0, S2("12345678901234567890"), SE("a12345678901234567890bcde"));
	test(S1("abcde"), 1, 1, S2(""), SE("acde"));
	test(S1("abcde"), 1, 1, S2("12345"), SE("a12345cde"));
	test(S1("abcde"), 1, 1, S2("1234567890"), SE("a1234567890cde"));
	test(S1("abcde"), 1, 1, S2("12345678901234567890"), SE("a12345678901234567890cde"));
	test(S1("abcde"), 1, 2, S2(""), SE("ade"));
	test(S1("abcde"), 1, 2, S2("12345"), SE("a12345de"));
	test(S1("abcde"), 1, 2, S2("1234567890"), SE("a1234567890de"));
	test(S1("abcde"), 1, 2, S2("12345678901234567890"), SE("a12345678901234567890de"));
	test(S1("abcde"), 1, 3, S2(""), SE("ae"));
	test(S1("abcde"), 1, 3, S2("12345"), SE("a12345e"));
	test(S1("abcde"), 1, 3, S2("1234567890"), SE("a1234567890e"));
	test(S1("abcde"), 1, 3, S2("12345678901234567890"), SE("a12345678901234567890e"));
	test(S1("abcde"), 1, 4, S2(""), SE("a"));
	test(S1("abcde"), 1, 4, S2("12345"), SE("a12345"));
	test(S1("abcde"), 1, 4, S2("1234567890"), SE("a1234567890"));
	test(S1("abcde"), 1, 4, S2("12345678901234567890"), SE("a12345678901234567890"));
	test(S1("abcde"), 1, 5, S2(""), SE("a"));
	test(S1("abcde"), 1, 5, S2("12345"), SE("a12345"));
	test(S1("abcde"), 1, 5, S2("1234567890"), SE("a1234567890"));
	test(S1("abcde"), 1, 5, S2("12345678901234567890"), SE("a12345678901234567890"));
	test(S1("abcde"), 2, 0, S2(""), SE("abcde"));
	test(S1("abcde"), 2, 0, S2("12345"), SE("ab12345cde"));
	test(S1("abcde"), 2, 0, S2("1234567890"), SE("ab1234567890cde"));
	test(S1("abcde"), 2, 0, S2("12345678901234567890"), SE("ab12345678901234567890cde"));
	test(S1("abcde"), 2, 1, S2(""), SE("abde"));
	test(S1("abcde"), 2, 1, S2("12345"), SE("ab12345de"));
	test(S1("abcde"), 2, 1, S2("1234567890"), SE("ab1234567890de"));
	test(S1("abcde"), 2, 1, S2("12345678901234567890"), SE("ab12345678901234567890de"));
	test(S1("abcde"), 2, 2, S2(""), SE("abe"));
	test(S1("abcde"), 2, 2, S2("12345"), SE("ab12345e"));
	test(S1("abcde"), 2, 2, S2("1234567890"), SE("ab1234567890e"));
	test(S1("abcde"), 2, 2, S2("12345678901234567890"), SE("ab12345678901234567890e"));
	test(S1("abcde"), 2, 3, S2(""), SE("ab"));
	test(S1("abcde"), 2, 3, S2("12345"), SE("ab12345"));
	test(S1("abcde"), 2, 3, S2("1234567890"), SE("ab1234567890"));
	test(S1("abcde"), 2, 3, S2("12345678901234567890"), SE("ab12345678901234567890"));
	test(S1("abcde"), 2, 4, S2(""), SE("ab"));
	test(S1("abcde"), 2, 4, S2("12345"), SE("ab12345"));
	test(S1("abcde"), 2, 4, S2("1234567890"), SE("ab1234567890"));
	test(S1("abcde"), 2, 4, S2("12345678901234567890"), SE("ab12345678901234567890"));
	test(S1("abcde"), 4, 0, S2(""), SE("abcde"));
	test(S1("abcde"), 4, 0, S2("12345"), SE("abcd12345e"));
	test(S1("abcde"), 4, 0, S2("1234567890"), SE("abcd1234567890e"));
	test(S1("abcde"), 4, 0, S2("12345678901234567890"), SE("abcd12345678901234567890e"));
	test(S1("abcde"), 4, 1, S2(""), SE("abcd"));
	test(S1("abcde"), 4, 1, S2("12345"), SE("abcd12345"));
	test(S1("abcde"), 4, 1, S2("1234567890"), SE("abcd1234567890"));
	test(S1("abcde"), 4, 1, S2("12345678901234567890"), SE("abcd12345678901234567890"));
	test(S1("abcde"), 4, 2, S2(""), SE("abcd"));
	test(S1("abcde"), 4, 2, S2("12345"), SE("abcd12345"));
	test(S1("abcde"), 4, 2, S2("1234567890"), SE("abcd1234567890"));
	test(S1("abcde"), 4, 2, S2("12345678901234567890"), SE("abcd12345678901234567890"));
	test(S1("abcde"), 5, 0, S2(""), SE("abcde"));
	test(S1("abcde"), 5, 0, S2("12345"), SE("abcde12345"));
	test(S1("abcde"), 5, 0, S2("1234567890"), SE("abcde1234567890"));
	test(S1("abcde"), 5, 0, S2("12345678901234567890"), SE("abcde12345678901234567890"));
	test(S1("abcde"), 5, 1, S2(""), SE("abcde"));
	test(S1("abcde"), 5, 1, S2("12345"), SE("abcde12345"));
	test(S1("abcde"), 5, 1, S2("1234567890"), SE("abcde1234567890"));
	test(S1("abcde"), 5, 1, S2("12345678901234567890"), SE("abcde12345678901234567890"));

	test(S1("abcde"), 6, 0, S2(""), SE("can't happen"));
	test(S1("abcde"), 6, 0, S2("12345"), SE("can't happen"));
	test(S1("abcde"), 6, 0, S2("1234567890"), SE("can't happen"));
	test(S1("abcde"), 6, 0, S2("12345678901234567890"), SE("can't happen"));
	test(S1("abcdefghij"), 0, 0, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 0, 0, S2("12345"), SE("12345abcdefghij"));
	test(S1("abcdefghij"), 0, 0, S2("1234567890"), SE("1234567890abcdefghij"));
	test(S1("abcdefghij"), 0, 0, S2("12345678901234567890"), SE("12345678901234567890abcdefghij"));
	test(S1("abcdefghij"), 0, 1, S2(""), SE("bcdefghij"));
	test(S1("abcdefghij"), 0, 1, S2("12345"), SE("12345bcdefghij"));
	test(S1("abcdefghij"), 0, 1, S2("1234567890"), SE("1234567890bcdefghij"));
	test(S1("abcdefghij"), 0, 1, S2("12345678901234567890"), SE("12345678901234567890bcdefghij"));
	test(S1("abcdefghij"), 0, 5, S2(""), SE("fghij"));
	test(S1("abcdefghij"), 0, 5, S2("12345"), SE("12345fghij"));
	test(S1("abcdefghij"), 0, 5, S2("1234567890"), SE("1234567890fghij"));
	test(S1("abcdefghij"), 0, 5, S2("12345678901234567890"), SE("12345678901234567890fghij"));
	test(S1("abcdefghij"), 0, 9, S2(""), SE("j"));
	test(S1("abcdefghij"), 0, 9, S2("12345"), SE("12345j"));
	test(S1("abcdefghij"), 0, 9, S2("1234567890"), SE("1234567890j"));
	test(S1("abcdefghij"), 0, 9, S2("12345678901234567890"), SE("12345678901234567890j"));
	test(S1("abcdefghij"), 0, 10, S2(""), SE(""));
	test(S1("abcdefghij"), 0, 10, S2("12345"), SE("12345"));
	test(S1("abcdefghij"), 0, 10, S2("1234567890"), SE("1234567890"));
	test(S1("abcdefghij"), 0, 10, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1("abcdefghij"), 0, 11, S2(""), SE(""));
	test(S1("abcdefghij"), 0, 11, S2("12345"), SE("12345"));
	test(S1("abcdefghij"), 0, 11, S2("1234567890"), SE("1234567890"));
	test(S1("abcdefghij"), 0, 11, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1("abcdefghij"), 1, 0, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 1, 0, S2("12345"), SE("a12345bcdefghij"));
	test(S1("abcdefghij"), 1, 0, S2("1234567890"), SE("a1234567890bcdefghij"));
	test(S1("abcdefghij"), 1, 0, S2("12345678901234567890"), SE("a12345678901234567890bcdefghij"));
	test(S1("abcdefghij"), 1, 1, S2(""), SE("acdefghij"));
	test(S1("abcdefghij"), 1, 1, S2("12345"), SE("a12345cdefghij"));
	test(S1("abcdefghij"), 1, 1, S2("1234567890"), SE("a1234567890cdefghij"));
	test(S1("abcdefghij"), 1, 1, S2("12345678901234567890"), SE("a12345678901234567890cdefghij"));
	test(S1("abcdefghij"), 1, 4, S2(""), SE("afghij"));
	test(S1("abcdefghij"), 1, 4, S2("12345"), SE("a12345fghij"));
	test(S1("abcdefghij"), 1, 4, S2("1234567890"), SE("a1234567890fghij"));
	test(S1("abcdefghij"), 1, 4, S2("12345678901234567890"), SE("a12345678901234567890fghij"));
	test(S1("abcdefghij"), 1, 8, S2(""), SE("aj"));
	test(S1("abcdefghij"), 1, 8, S2("12345"), SE("a12345j"));
	test(S1("abcdefghij"), 1, 8, S2("1234567890"), SE("a1234567890j"));
	test(S1("abcdefghij"), 1, 8, S2("12345678901234567890"), SE("a12345678901234567890j"));
	test(S1("abcdefghij"), 1, 9, S2(""), SE("a"));
	test(S1("abcdefghij"), 1, 9, S2("12345"), SE("a12345"));
	test(S1("abcdefghij"), 1, 9, S2("1234567890"), SE("a1234567890"));
	test(S1("abcdefghij"), 1, 9, S2("12345678901234567890"), SE("a12345678901234567890"));
	test(S1("abcdefghij"), 1, 10, S2(""), SE("a"));
	test(S1("abcdefghij"), 1, 10, S2("12345"), SE("a12345"));
	test(S1("abcdefghij"), 1, 10, S2("1234567890"), SE("a1234567890"));
	test(S1("abcdefghij"), 1, 10, S2("12345678901234567890"), SE("a12345678901234567890"));
	test(S1("abcdefghij"), 5, 0, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 5, 0, S2("12345"), SE("abcde12345fghij"));
	test(S1("abcdefghij"), 5, 0, S2("1234567890"), SE("abcde1234567890fghij"));
	test(S1("abcdefghij"), 5, 0, S2("12345678901234567890"), SE("abcde12345678901234567890fghij"));
	test(S1("abcdefghij"), 5, 1, S2(""), SE("abcdeghij"));
	test(S1("abcdefghij"), 5, 1, S2("12345"), SE("abcde12345ghij"));
	test(S1("abcdefghij"), 5, 1, S2("1234567890"), SE("abcde1234567890ghij"));
	test(S1("abcdefghij"), 5, 1, S2("12345678901234567890"), SE("abcde12345678901234567890ghij"));
	test(S1("abcdefghij"), 5, 2, S2(""), SE("abcdehij"));
	test(S1("abcdefghij"), 5, 2, S2("12345"), SE("abcde12345hij"));
	test(S1("abcdefghij"), 5, 2, S2("1234567890"), SE("abcde1234567890hij"));
	test(S1("abcdefghij"), 5, 2, S2("12345678901234567890"), SE("abcde12345678901234567890hij"));
	test(S1("abcdefghij"), 5, 4, S2(""), SE("abcdej"));
	test(S1("abcdefghij"), 5, 4, S2("12345"), SE("abcde12345j"));
	test(S1("abcdefghij"), 5, 4, S2("1234567890"), SE("abcde1234567890j"));
	test(S1("abcdefghij"), 5, 4, S2("12345678901234567890"), SE("abcde12345678901234567890j"));
	test(S1("abcdefghij"), 5, 5, S2(""), SE("abcde"));
	test(S1("abcdefghij"), 5, 5, S2("12345"), SE("abcde12345"));
	test(S1("abcdefghij"), 5, 5, S2("1234567890"), SE("abcde1234567890"));
	test(S1("abcdefghij"), 5, 5, S2("12345678901234567890"), SE("abcde12345678901234567890"));
	test(S1("abcdefghij"), 5, 6, S2(""), SE("abcde"));
	test(S1("abcdefghij"), 5, 6, S2("12345"), SE("abcde12345"));
	test(S1("abcdefghij"), 5, 6, S2("1234567890"), SE("abcde1234567890"));
	test(S1("abcdefghij"), 5, 6, S2("12345678901234567890"), SE("abcde12345678901234567890"));
	test(S1("abcdefghij"), 9, 0, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 9, 0, S2("12345"), SE("abcdefghi12345j"));
	test(S1("abcdefghij"), 9, 0, S2("1234567890"), SE("abcdefghi1234567890j"));
	test(S1("abcdefghij"), 9, 0, S2("12345678901234567890"), SE("abcdefghi12345678901234567890j"));
	test(S1("abcdefghij"), 9, 1, S2(""), SE("abcdefghi"));
	test(S1("abcdefghij"), 9, 1, S2("12345"), SE("abcdefghi12345"));
	test(S1("abcdefghij"), 9, 1, S2("1234567890"), SE("abcdefghi1234567890"));
	test(S1("abcdefghij"), 9, 1, S2("12345678901234567890"), SE("abcdefghi12345678901234567890"));
	test(S1("abcdefghij"), 9, 2, S2(""), SE("abcdefghi"));
	test(S1("abcdefghij"), 9, 2, S2("12345"), SE("abcdefghi12345"));
	test(S1("abcdefghij"), 9, 2, S2("1234567890"), SE("abcdefghi1234567890"));
	test(S1("abcdefghij"), 9, 2, S2("12345678901234567890"), SE("abcdefghi12345678901234567890"));
	test(S1("abcdefghij"), 10, 0, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 10, 0, S2("12345"), SE("abcdefghij12345"));
	test(S1("abcdefghij"), 10, 0, S2("1234567890"), SE("abcdefghij1234567890"));
	test(S1("abcdefghij"), 10, 0, S2("12345678901234567890"), SE("abcdefghij12345678901234567890"));
	test(S1("abcdefghij"), 10, 1, S2(""), SE("abcdefghij"));
	test(S1("abcdefghij"), 10, 1, S2("12345"), SE("abcdefghij12345"));
	test(S1("abcdefghij"), 10, 1, S2("1234567890"), SE("abcdefghij1234567890"));
	test(S1("abcdefghij"), 10, 1, S2("12345678901234567890"), SE("abcdefghij12345678901234567890"));
	test(S1("abcdefghij"), 11, 0, S2(""), SE("can't happen"));
	test(S1("abcdefghij"), 11, 0, S2("12345"), SE("can't happen"));
	test(S1("abcdefghij"), 11, 0, S2("1234567890"), SE("can't happen"));
	test(S1("abcdefghij"), 11, 0, S2("12345678901234567890"), SE("can't happen"));

	test(S1("abcdefghijklmnopqrst"), 0, 0, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 0, S2("12345"), SE("12345abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 0, S2("1234567890"), SE("1234567890abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 0, S2("12345678901234567890"), SE("12345678901234567890abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 1, S2(""), SE("bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 1, S2("12345"), SE("12345bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 1, S2("1234567890"), SE("1234567890bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 1, S2("12345678901234567890"), SE("12345678901234567890bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 10, S2(""), SE("klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 10, S2("12345"), SE("12345klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 10, S2("1234567890"), SE("1234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 10, S2("12345678901234567890"), SE("12345678901234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 0, 19, S2(""), SE("t"));
	test(S1("abcdefghijklmnopqrst"), 0, 19, S2("12345"), SE("12345t"));
	test(S1("abcdefghijklmnopqrst"), 0, 19, S2("1234567890"), SE("1234567890t"));
	test(S1("abcdefghijklmnopqrst"), 0, 19, S2("12345678901234567890"), SE("12345678901234567890t"));
	test(S1("abcdefghijklmnopqrst"), 0, 20, S2(""), SE(""));
	test(S1("abcdefghijklmnopqrst"), 0, 20, S2("12345"), SE("12345"));
	test(S1("abcdefghijklmnopqrst"), 0, 20, S2("1234567890"), SE("1234567890"));
	test(S1("abcdefghijklmnopqrst"), 0, 20, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 0, 21, S2(""), SE(""));
	test(S1("abcdefghijklmnopqrst"), 0, 21, S2("12345"), SE("12345"));
	test(S1("abcdefghijklmnopqrst"), 0, 21, S2("1234567890"), SE("1234567890"));
	test(S1("abcdefghijklmnopqrst"), 0, 21, S2("12345678901234567890"), SE("12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 1, 0, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 0, S2("12345"), SE("a12345bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 0, S2("1234567890"), SE("a1234567890bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 0, S2("12345678901234567890"), SE("a12345678901234567890bcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 1, S2(""), SE("acdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 1, S2("12345"), SE("a12345cdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 1, S2("1234567890"), SE("a1234567890cdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 1, S2("12345678901234567890"), SE("a12345678901234567890cdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 9, S2(""), SE("aklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 9, S2("12345"), SE("a12345klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 9, S2("1234567890"), SE("a1234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 9, S2("12345678901234567890"), SE("a12345678901234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 1, 18, S2(""), SE("at"));
	test(S1("abcdefghijklmnopqrst"), 1, 18, S2("12345"), SE("a12345t"));
	test(S1("abcdefghijklmnopqrst"), 1, 18, S2("1234567890"), SE("a1234567890t"));
	test(S1("abcdefghijklmnopqrst"), 1, 18, S2("12345678901234567890"), SE("a12345678901234567890t"));
	test(S1("abcdefghijklmnopqrst"), 1, 19, S2(""), SE("a"));
	test(S1("abcdefghijklmnopqrst"), 1, 19, S2("12345"), SE("a12345"));
	test(S1("abcdefghijklmnopqrst"), 1, 19, S2("1234567890"), SE("a1234567890"));
	test(S1("abcdefghijklmnopqrst"), 1, 19, S2("12345678901234567890"), SE("a12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 1, 20, S2(""), SE("a"));
	test(S1("abcdefghijklmnopqrst"), 1, 20, S2("12345"), SE("a12345"));
	test(S1("abcdefghijklmnopqrst"), 1, 20, S2("1234567890"), SE("a1234567890"));
	test(S1("abcdefghijklmnopqrst"), 1, 20, S2("12345678901234567890"), SE("a12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 10, 0, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 0, S2("12345"), SE("abcdefghij12345klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 0, S2("1234567890"), SE("abcdefghij1234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 0, S2("12345678901234567890"), SE("abcdefghij12345678901234567890klmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 1, S2(""), SE("abcdefghijlmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 1, S2("12345"), SE("abcdefghij12345lmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 1, S2("1234567890"), SE("abcdefghij1234567890lmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 1, S2("12345678901234567890"), SE("abcdefghij12345678901234567890lmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 5, S2(""), SE("abcdefghijpqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 5, S2("12345"), SE("abcdefghij12345pqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 5, S2("1234567890"), SE("abcdefghij1234567890pqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 5, S2("12345678901234567890"), SE("abcdefghij12345678901234567890pqrst"));
	test(S1("abcdefghijklmnopqrst"), 10, 9, S2(""), SE("abcdefghijt"));
	test(S1("abcdefghijklmnopqrst"), 10, 9, S2("12345"), SE("abcdefghij12345t"));
	test(S1("abcdefghijklmnopqrst"), 10, 9, S2("1234567890"), SE("abcdefghij1234567890t"));
	test(S1("abcdefghijklmnopqrst"), 10, 9, S2("12345678901234567890"), SE("abcdefghij12345678901234567890t"));
	test(S1("abcdefghijklmnopqrst"), 10, 10, S2(""), SE("abcdefghij"));
	test(S1("abcdefghijklmnopqrst"), 10, 10, S2("12345"), SE("abcdefghij12345"));
	test(S1("abcdefghijklmnopqrst"), 10, 10, S2("1234567890"), SE("abcdefghij1234567890"));
	test(S1("abcdefghijklmnopqrst"), 10, 10, S2("12345678901234567890"), SE("abcdefghij12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 10, 11, S2(""), SE("abcdefghij"));
	test(S1("abcdefghijklmnopqrst"), 10, 11, S2("12345"), SE("abcdefghij12345"));
	test(S1("abcdefghijklmnopqrst"), 10, 11, S2("1234567890"), SE("abcdefghij1234567890"));
	test(S1("abcdefghijklmnopqrst"), 10, 11, S2("12345678901234567890"), SE("abcdefghij12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 19, 0, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 19, 0, S2("12345"), SE("abcdefghijklmnopqrs12345t"));
	test(S1("abcdefghijklmnopqrst"), 19, 0, S2("1234567890"), SE("abcdefghijklmnopqrs1234567890t"));
	test(S1("abcdefghijklmnopqrst"), 19, 0, S2("12345678901234567890"), SE("abcdefghijklmnopqrs12345678901234567890t"));
	test(S1("abcdefghijklmnopqrst"), 19, 1, S2(""), SE("abcdefghijklmnopqrs"));
	test(S1("abcdefghijklmnopqrst"), 19, 1, S2("12345"), SE("abcdefghijklmnopqrs12345"));
	test(S1("abcdefghijklmnopqrst"), 19, 1, S2("1234567890"), SE("abcdefghijklmnopqrs1234567890"));
	test(S1("abcdefghijklmnopqrst"), 19, 1, S2("12345678901234567890"), SE("abcdefghijklmnopqrs12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 19, 2, S2(""), SE("abcdefghijklmnopqrs"));
	test(S1("abcdefghijklmnopqrst"), 19, 2, S2("12345"), SE("abcdefghijklmnopqrs12345"));
	test(S1("abcdefghijklmnopqrst"), 19, 2, S2("1234567890"), SE("abcdefghijklmnopqrs1234567890"));
	test(S1("abcdefghijklmnopqrst"), 19, 2, S2("12345678901234567890"), SE("abcdefghijklmnopqrs12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 20, 0, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 20, 0, S2("12345"), SE("abcdefghijklmnopqrst12345"));
	test(S1("abcdefghijklmnopqrst"), 20, 0, S2("1234567890"), SE("abcdefghijklmnopqrst1234567890"));
	test(S1("abcdefghijklmnopqrst"), 20, 0, S2("12345678901234567890"), SE("abcdefghijklmnopqrst12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 20, 1, S2(""), SE("abcdefghijklmnopqrst"));
	test(S1("abcdefghijklmnopqrst"), 20, 1, S2("12345"), SE("abcdefghijklmnopqrst12345"));
	test(S1("abcdefghijklmnopqrst"), 20, 1, S2("1234567890"), SE("abcdefghijklmnopqrst1234567890"));
	test(S1("abcdefghijklmnopqrst"), 20, 1, S2("12345678901234567890"), SE("abcdefghijklmnopqrst12345678901234567890"));
	test(S1("abcdefghijklmnopqrst"), 21, 0, S2(""), SE("can't happen"));
	test(S1("abcdefghijklmnopqrst"), 21, 0, S2("12345"), SE("can't happen"));
	test(S1("abcdefghijklmnopqrst"), 21, 0, S2("1234567890"), SE("can't happen"));

	/* pos and n count bytes, not utf-8 characters: 2 positions each umlaut */
	test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"), 10, 10, S2("12345"), SE("\303\244bcdefghi12345st\303\274"));
	return (0);
}
