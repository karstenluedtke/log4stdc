
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

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

static int
test(const char *s1, int codepage, const char *s2, const char *expected)
{
	int rc;
	bfc_string_t s, rs;
	size_t origlen, rslen, newlen, size;
	char *buf;

	L4SC_TRACE(logger, "%s(\"%s\", codepage %d, \"%s\", expected \"%s\")",
		__FUNCTION__, s1, codepage, s2, expected);

	origlen = strlen(s1);
	rslen = strlen(s2);
	size = origlen + rslen + 80;
	buf = alloca(size);

	switch (codepage) {
	case 1:
		rc = bfc_init_buffered_iso8859_1_string(&s,sizeof(s),buf,size);
		break;
	case 2:
		rc = bfc_init_buffered_iso8859_2_string(&s,sizeof(s),buf,size);
		break;
	case 15:
		rc= bfc_init_buffered_iso8859_15_string(&s,sizeof(s),buf,size);
		break;
	case 1250:
		rc = bfc_init_buffered_cp1250_string(&s, sizeof(s), buf, size);
		break;
	case 1252:
		rc = bfc_init_buffered_cp1252_string(&s, sizeof(s), buf, size);
		break;
	default:
		/* UTF-8 */
		rc = bfc_init_string_buffer(&s, sizeof(s), pool, buf, size);
	}
	assert(rc >= 0);

	rc = bfc_string_assign_c_str(&s, s1);
	assert(rc >= 0);
	assert(bfc_string_data(&s) == buf);
	assert(bfc_string_capacity(&s) == size);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_init_shared_string_c_str(&rs, sizeof(rs), s2);
	assert(rc >= 0);

	rslen = bfc_string_length(&rs);
	assert(rslen == strlen(s2));
	
	rc = bfc_string_append(&s, &rs);
	newlen = bfc_string_length(&s);
	if (rc >= 0) {
		assert(newlen >= origlen);
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

	L4SC_TRACE(logger,
		"%s(\"%s\", codepage %d, \"%s\", expected \"%s\"): rc %d",
		"PASS", s1, codepage, s2, expected, rc);

	return (BFC_SUCCESS);
}

#define S1(x) x
#define S2(x) x
#define SE(x) x

static int
test1(int codepage)
{
	test(S1(""), codepage, S2(""), SE(""));
	test(S1(""), codepage, S2("12345"), SE("12345"));
	test(S1(""), codepage, S2("1234567890"), SE("1234567890"));
	test(S1(""), codepage, S2("12345678901234567890"), SE("12345678901234567890"));

	test(S1("12345"), codepage, S2(""), SE("12345"));
	test(S1("12345"), codepage, S2("12345"), SE("1234512345"));
	test(S1("12345"), codepage, S2("1234567890"), SE("123451234567890"));
	test(S1("12345"), codepage, S2("12345678901234567890"), SE("1234512345678901234567890"));

	test(S1("1234567890"), codepage, S2(""), SE("1234567890"));
	test(S1("1234567890"), codepage, S2("12345"), SE("123456789012345"));
	test(S1("1234567890"), codepage, S2("1234567890"), SE("12345678901234567890"));
	test(S1("1234567890"), codepage, S2("12345678901234567890"), SE("123456789012345678901234567890"));

	test(S1("12345678901234567890"), codepage, S2(""), SE("12345678901234567890"));
	test(S1("12345678901234567890"), codepage, S2("12345"), SE("1234567890123456789012345"));
	test(S1("12345678901234567890"), codepage, S2("1234567890"), SE("123456789012345678901234567890"));
	test(S1("12345678901234567890"), codepage, S2("12345678901234567890"), SE("1234567890123456789012345678901234567890"));

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);

	test1(0);
	test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"), 0, S2("12345"),
	     SE("\303\244bcdefghijklmn\303\266pqrst\303\27412345"));
	test(S1("12345"), 0, S2("\303\244"), SE("12345\303\244"));
	test(S1("12345"), 0, S2("klmn\303\266p"), SE("12345klmn\303\266p"));
	test(S1("abcd"),  0, S2("\342\202\254"), SE("abcd\342\202\254"));
	test(S1("q"),     0, S2("r\305\241t"), SE("qr\305\241t"));

	test1(1);
	test(S1("\344bcdefghijklmn\366pqrst\374"), 1, S2("12345"),
	     SE("\344bcdefghijklmn\366pqrst\37412345"));
	test(S1("12345"), 1, S2("\303\244"), SE("12345\344"));
	test(S1("12345"), 1, S2("klmn\303\266p"), SE("12345klmn\366p"));

	test1(2);
	test(S1("\344bcdefghijklmn\366pqrst\374"), 2, S2("12345"),
	     SE("\344bcdefghijklmn\366pqrst\37412345"));
	test(S1("12345"), 2, S2("\303\244"), SE("12345\344"));
	test(S1("12345"), 2, S2("klmn\303\266p"), SE("12345klmn\366p"));
	test(S1("q"),     2, S2("r\305\241t"), SE("qr\271t"));

	test1(15);
	test(S1("\344bcdefghijklmn\366pqrst\374"), 15, S2("12345"),
	     SE("\344bcdefghijklmn\366pqrst\37412345"));
	test(S1("12345"), 15, S2("\303\244"), SE("12345\344"));
	test(S1("12345"), 15, S2("klmn\303\266p"), SE("12345klmn\366p"));
	test(S1("abcd"),  15, S2("\342\202\254"), SE("abcd\244"));
	test(S1("q"),     15, S2("r\305\241t"), SE("qr\250t"));

	test1(1252);
	test(S1("\344bcdefghijklmn\366pqrst\374"), 1252, S2("12345"),
	     SE("\344bcdefghijklmn\366pqrst\37412345"));
	test(S1("12345"), 1252, S2("\303\244"), SE("12345\344"));
	test(S1("12345"), 1252, S2("klmn\303\266p"), SE("12345klmn\366p"));
	test(S1("abcd"),  1252, S2("\342\202\254"), SE("abcd\200"));
	test(S1("q"),     1252, S2("r\305\241t"), SE("qr\232t"));

	test1(1250);
	test(S1("\344bcdefghijklmn\366pqrst\374"), 1250, S2("12345"),
	     SE("\344bcdefghijklmn\366pqrst\37412345"));
	test(S1("12345"), 1250, S2("\303\244"), SE("12345\344"));
	test(S1("12345"), 1250, S2("klmn\303\266p"), SE("12345klmn\366p"));
	test(S1("abcd"),  1250, S2("\342\202\254"), SE("abcd\200"));
	test(S1("q"),     1250, S2("r\305\241t"), SE("qr\232t"));

	return (0);
}
