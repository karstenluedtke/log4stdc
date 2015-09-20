
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
test(const wchar_t *s1, const char *s2, int codepage, const wchar_t *expected)
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

	L4SC_TRACE(logger, "%s(L\"%s\", \"%s\", %d, expected L\"%s\")",
		__FUNCTION__, u8buf, s2, codepage, exp8buf);

	rc = bfc_init_basic_wstring_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == wcslen(s1));
	
	switch (codepage) {
	case 1:
		rc = bfc_init_shared_iso8859_1_string_c_str(&rs,sizeof(rs),s2);
		break;
	case 2:
		rc = bfc_init_shared_iso8859_2_string_c_str(&rs,sizeof(rs),s2);
		break;
	case 15:
		rc = bfc_init_shared_iso8859_15_string_c_str(&rs,sizeof(rs),s2);
		break;
	case 1250:
		rc = bfc_init_shared_cp1250_string_c_str(&rs, sizeof(rs), s2);
		break;
	case 1252:
		rc = bfc_init_shared_cp1252_string_c_str(&rs, sizeof(rs), s2);
		break;
	default:
		/* UTF-8 */
		rc = bfc_init_shared_string_c_str(&rs, sizeof(rs), s2);
	}
	assert(rc >= 0);
	
	rc = bfc_string_append(&s, &rs);
	newlen = bfc_string_length(&s);
	if (rc >= 0) {
		assert(newlen <= origlen + rslen);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
		assert(bfc_string_compare_c_str(&rs, s2) == 0);
	} else {
		assert(newlen == origlen);
		assert(bfc_string_compare_c_str(&s, s1) == 0);
		assert(bfc_string_compare_c_str(&rs, s2) == 0);
		assert(rc >= 0); /* no error expected */
	}

	bfc_destroy(&s);
	bfc_destroy(&rs);

	L4SC_TRACE(logger, "%s(L\"%s\", \"%s\", %d, expected L\"%s\"): rc %d",
			"PASS", u8buf, s2, codepage, exp8buf, rc);

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
	test(S1(""), S2(""), 0, SE(""));
	test(S1(""), S2("12345"), 0, SE("12345"));
	test(S1(""), S2("1234567890"), 0, SE("1234567890"));
	test(S1(""), S2("12345678901234567890"), 0, SE("12345678901234567890"));

	test(S1("12345"), S2(""), 0, SE("12345"));
	test(S1("12345"), S2("12345"), 0, SE("1234512345"));
	test(S1("12345"), S2("1234567890"), 0, SE("123451234567890"));
	test(S1("12345"), S2("12345678901234567890"), 0, SE("1234512345678901234567890"));

	test(S1("1234567890"), S2(""), 0, SE("1234567890"));
	test(S1("1234567890"), S2("12345"), 0, SE("123456789012345"));
	test(S1("1234567890"), S2("1234567890"), 0, SE("12345678901234567890"));
	test(S1("1234567890"), S2("12345678901234567890"), 0, SE("123456789012345678901234567890"));

	test(S1("12345678901234567890"), S2(""), 0, SE("12345678901234567890"));
	test(S1("12345678901234567890"), S2("12345"), 0, SE("1234567890123456789012345"));
	test(S1("12345678901234567890"), S2("1234567890"), 0, SE("123456789012345678901234567890"));
	test(S1("12345678901234567890"), S2("12345678901234567890"), 0,
	     SE("1234567890123456789012345678901234567890"));

	do {
		/* German Umlaute */
		const char s2[] = "\303\244bcdefghijklmn\303\266pqrst\303\274";
		const char s8859[] = "\344bcdefghijklmn\366pqrst\374";
		const wchar_t se1[] = { 0xe4, 'b', 'c', 'd', 'e', 'f', 'g', 'h',
					'i', 'j', 'k', 'l', 'm', 'n', 0xf6,
					'p', 'q', 'r', 's', 't', 0xfc, 0 };
		const wchar_t se2[] = { '1', '2', '3', '4', '5',
					0xe4, 'b', 'c', 'd', 'e', 'f', 'g', 'h',
					'i', 'j', 'k', 'l', 'm', 'n', 0xf6,
					'p', 'q', 'r', 's', 't', 0xfc, 0 };
		test(S1(""), s2, 0, se1);
		test(S1("12345"), s2, 0, se2);
		test(S1(""), s8859, 1, se1);
		test(S1("12345"), s8859, 1, se2);
		test(S1(""), s8859, 2, se1);
		test(S1("12345"), s8859, 2, se2);
		test(S1(""), s8859, 15, se1);
		test(S1("12345"), s8859, 15, se2);
		test(S1(""), s8859, 1250, se1);
		test(S1("12345"), s8859, 1250, se2);
		test(S1(""), s8859, 1252, se1);
		test(S1("12345"), s8859, 1252, se2);
	} while(0);

	do {
		/* Euro symbol */
		const char s2[] = "\342\202\254";
		const char s8859[] = "\244";
		const char s1252[] = "\200";
		const wchar_t se1[] = { 0x20AC, 0 };
		const wchar_t se2[] = { '1', '2', '3', '4', '5', 0x20AC, 0 };
		test(S1(""), s2, 0, se1);
		test(S1("12345"), s2, 0, se2);
		test(S1(""), s8859, 15, se1);
		test(S1("12345"), s8859, 15, se2);
		test(S1(""), s1252, 1250, se1);
		test(S1("12345"), s1252, 1250, se2);
		test(S1(""), s1252, 1252, se1);
		test(S1("12345"), s1252, 1252, se2);
	} while(0);

	if (sizeof(wchar_t) > 2) {
		/* Four aces (playcards) */
		const char s2[] = { 0xF0, 0x9F, 0x82, 0xA1,
				    0xF0, 0x9F, 0x82, 0xB1,
				    0xF0, 0x9F, 0x83, 0x81,
				    0xF0, 0x9F, 0x83, 0x91, 0 };
		const wchar_t se1[] = { 0x1F0A1, 0x1F0B1, 0x1F0C1, 0x1F0D1, 0 };
		const wchar_t se2[] = { '1', '2', '3', '4', '5',
					0x1F0A1, 0x1F0B1, 0x1F0C1, 0x1F0D1, 0 };
		test(S1(""), s2, 0, se1);
		test(S1("12345"), s2, 0, se2);
	}
	return (0);
}
