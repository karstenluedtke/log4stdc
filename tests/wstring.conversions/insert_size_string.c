
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
test(const wchar_t *s1, size_t pos1, const char *s2, const wchar_t *expected)
{
	int rc;
	bfc_string_t s, rs;
	size_t origlen, rslen, newlen, bufsize;
	char *u8buf, *exp8buf;

	rslen = wcslen(s1);
	bufsize = 2*rslen + 20;
	u8buf = alloca(bufsize);
	bfc_utf8_from_wchar(u8buf, bufsize, s1, rslen);

	rslen = wcslen(expected);
	bufsize = 2*rslen + 20;
	exp8buf = alloca(bufsize);
	bfc_utf8_from_wchar(exp8buf, bufsize, expected, rslen);

	L4SC_TRACE(logger, "%s(L\"%s\", pos %ld, \"%s\", expected L\"%s\")",
		__FUNCTION__, u8buf, (long) pos1, s2, exp8buf);

	rc = bfc_init_basic_wstring_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == wcslen(s1));
	
	rc = bfc_init_shared_string_c_str(&rs, sizeof(rs), s2);
	assert(rc >= 0);

	rslen = bfc_string_length(&rs);
	assert(rslen == strlen(s2));
	
	rc = bfc_string_insert(&s, pos1, &rs);
	newlen = bfc_string_length(&s);
	if (rc >= 0) {
        	assert(pos1 <= origlen);
		assert(newlen <= origlen + rslen);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
		assert(bfc_string_compare_c_str(&rs, s2) == 0);
	} else {
		assert(pos1 > origlen);
		assert(newlen == origlen);
		assert(bfc_string_compare_c_str(&s, s1) == 0);
		assert(bfc_string_compare_c_str(&rs, s2) == 0);
	}

	bfc_destroy(&s);
	bfc_destroy(&rs);

	L4SC_TRACE(logger,
		"%s(L\"%s\", pos %ld, \"%s\", expected L\"%s\"): rc %d",
		"PASS", u8buf, (long) pos1, s2, exp8buf, rc);

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
#define SE(x) L##x
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

	do {
		/* German Umlaute */
		const char s2[] = "\303\244bcdefghijklmn\303\266pqrst\303\274";
		const wchar_t se1[] = { 0xe4, 'b', 'c', 'd', 'e', 'f', 'g', 'h',
					'i', 'j', 'k', 'l', 'm', 'n', 0xf6,
					'p', 'q', 'r', 's', 't', 0xfc, 0 };
		const wchar_t se2[] = { 0xe4, 'b', 'c', 'd', 'e', 'f', 'g', 'h',
					'i', 'j', 'k', 'l', 'm', 'n', 0xf6,
					'p', 'q', 'r', 's', 't', 0xfc,
					'1', '2', '3', '4', '5', 0 };
		const wchar_t se3[] = { '1', '2', '3', '4', '5',
					0xe4, 'b', 'c', 'd', 'e', 'f', 'g', 'h',
					'i', 'j', 'k', 'l', 'm', 'n', 0xf6,
					'p', 'q', 'r', 's', 't', 0xfc, 0 };
		const wchar_t se4[] = { '1', '2', '3', '4', '5',
					0xe4, 'b', 'c', 'd', 'e', 'f', 'g', 'h',
					'i', 'j', 'k', 'l', 'm', 'n', 0xf6,
					'p', 'q', 'r', 's', 't', 0xfc,
					'6', '7', '8', '9', '0', 0 };
		test(S1(""), 0, s2, se1);
		test(S1("12345"), 0, s2, se2);
		test(S1("12345"), 5, s2, se3);
		test(S1("1234567890"), 5, s2, se4);
	} while(0);

	if (sizeof(wchar_t) > 2) {
		/* Four aces (playcards) */
		const char s2[] = { 0xF0, 0x9F, 0x82, 0xA1,
				    0xF0, 0x9F, 0x82, 0xB1,
				    0xF0, 0x9F, 0x83, 0x81,
				    0xF0, 0x9F, 0x83, 0x91, 0 };
		const wchar_t se1[] = { 0x1F0A1, 0x1F0B1, 0x1F0C1, 0x1F0D1, 0 };
		const wchar_t se2[] = { 0x1F0A1, 0x1F0B1, 0x1F0C1, 0x1F0D1,
					'1', '2', '3', '4', '5', 0 };
		const wchar_t se3[] = { '1', '2', '3', '4', '5',
					0x1F0A1, 0x1F0B1, 0x1F0C1, 0x1F0D1, 0 };
		const wchar_t se4[] = { '1', '2', '3', '4', '5',
					0x1F0A1, 0x1F0B1, 0x1F0C1, 0x1F0D1,
					'6', '7', '8', '9', '0', 0 };
		test(S1(""), 0, s2, se1);
		test(S1("12345"), 0, s2, se2);
		test(S1("12345"), 5, s2, se3);
		test(S1("1234567890"), 5, s2, se4);
	}
	return (0);
}
