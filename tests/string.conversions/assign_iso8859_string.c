
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
test(const char *s1, const char *s2, const char *expected)
{
	int rc;
	bfc_string_t s, ws;
	size_t origlen, wslen, newlen;

	L4SC_TRACE(logger, "%s(\"%s\", \"%s\", expected \"%s\")",
		__FUNCTION__, s1, s2, expected);

	rc = bfc_init_basic_string_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_init_shared_iso8859_15_string_c_str(&ws, sizeof(ws), s2);
	assert(rc >= 0);

	wslen = bfc_string_length(&ws);
	assert(wslen == strlen(s2));
	
	rc = bfc_string_assign(&s, &ws);
	newlen = bfc_string_length(&s);
	if (rc >= 0) {
		assert(newlen >= wslen);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
		assert(bfc_string_compare_c_str(&ws, s2) == 0);
	} else {
		assert(newlen == origlen);
		assert(bfc_string_compare_c_str(&s, s1) == 0);
		assert(bfc_string_compare_c_str(&ws, s2) == 0);
		assert(rc >= 0); /* no error expected */
	}

	bfc_destroy(&s);
	bfc_destroy(&ws);

	L4SC_TRACE(logger, "%s(\"%s\", \"%s\", expected \"%s\"): rc %d",
			"PASS", s1, s2, expected, rc);

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);

#define S1(x) x
#define S2(x) x
#define SE(x) x
	test(S1(""), S2(""), SE(""));
	test(S1(""), S2("12345"), SE("12345"));
	test(S1(""), S2("1234567890"), SE("1234567890"));
	test(S1(""), S2("12345678901234567890"), SE("12345678901234567890"));

	test(S1("12345"), S2(""), SE(""));
	test(S1("12345"), S2("12345"), SE("12345"));
	test(S1("12345"), S2("1234567890"), SE("1234567890"));
	test(S1("12345"), S2("12345678901234567890"), SE("12345678901234567890"));

	test(S1("1234567890"), S2(""), SE(""));
	test(S1("1234567890"), S2("12345"), SE("12345"));
	test(S1("1234567890"), S2("1234567890"), SE("1234567890"));
	test(S1("1234567890"), S2("12345678901234567890"), SE("12345678901234567890"));

	test(S1("12345678901234567890"), S2(""), SE(""));
	test(S1("12345678901234567890"), S2("12345"), SE("12345"));
	test(S1("12345678901234567890"), S2("1234567890"), SE("1234567890"));
	test(S1("12345678901234567890"), S2("12345678901234567890"),
	     SE("12345678901234567890"));

	test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"), S2("12345"),
	     SE("12345"));

	test(S1("1234567890"), S2("\344bcdefghijklmn\366pqrst\374"),
	     SE("\303\244bcdefghijklmn\303\266pqrst\303\274"));

	return (0);
}
