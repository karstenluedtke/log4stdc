
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/string.h"

static l4sc_logger_ptr_t logger;

static int
test1(const char *s1, size_t pos1, size_t n1, const char *expected)
{
	int rc;
	bfc_string_t s, exp;
	size_t origlen;

	rc = bfc_init_shared_string_c_str(&s, sizeof(s), s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_string_erase_seq(&s, pos1, n1);
	if (rc >= 0) {
        	assert(pos1 <= origlen);
		bfc_init_shared_string_c_str(&exp, sizeof(exp), expected);
		assert(bfc_string_compare_bfstr(&s, &exp) == 0);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
	} else {
		assert((pos1 != 0) && (n1 != BFC_NPOS) && (pos1+n1 != origlen));
	}
	return (BFC_SUCCESS);
}

static int
test2(const char *s1, size_t pos1, const char *expected)
{
	int rc;
	bfc_string_t s, exp;
	size_t origlen;

	rc = bfc_init_shared_string_c_str(&s, sizeof(s), s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_string_erase_tail(&s, pos1);
	if (rc >= 0) {
        	assert(pos1 <= origlen);
		bfc_init_shared_string_c_str(&exp, sizeof(exp), expected);
		assert(bfc_string_compare_bfstr(&s, &exp) == 0);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
	} else {
		assert((pos1 != 0) && (pos1 + 1 != origlen));
	}
	return (BFC_SUCCESS);
}

static int
test3(const char *s1, const char *expected)
{
	int rc;
	bfc_string_t s, exp;
	size_t origlen;

	rc = bfc_init_shared_string_c_str(&s, sizeof(s), s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_string_erase_tail(&s, 0);
	assert(rc >= 0);
	bfc_init_shared_string_c_str(&exp, sizeof(exp), expected);
	assert(bfc_string_compare_bfstr(&s, &exp) == 0);
	assert(bfc_string_compare_c_str(&s, expected) == 0);
	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
#define	S(x) x
	test1(S(""), 0, 0, S(""));
	test1(S(""), 0, 1, S(""));
	test1(S(""), 1, 0, S("can't happen"));
	test1(S("abcde"), 0, 0, S("abcde"));
	test1(S("abcde"), 0, 1, S("bcde"));
	test1(S("abcde"), 0, 2, S("cde"));
	test1(S("abcde"), 0, 4, S("e"));
	test1(S("abcde"), 0, 5, S(""));
	test1(S("abcde"), 0, 6, S(""));
	test1(S("abcde"), 1, 0, S("abcde"));
	test1(S("abcde"), 1, 1, S("acde"));
	test1(S("abcde"), 1, 2, S("ade"));
	test1(S("abcde"), 1, 3, S("ae"));
	test1(S("abcde"), 1, 4, S("a"));
	test1(S("abcde"), 1, 5, S("a"));
	test1(S("abcde"), 2, 0, S("abcde"));
	test1(S("abcde"), 2, 1, S("abde"));
	test1(S("abcde"), 2, 2, S("abe"));
	test1(S("abcde"), 2, 3, S("ab"));
	test1(S("abcde"), 2, 4, S("ab"));
	test1(S("abcde"), 4, 0, S("abcde"));
	test1(S("abcde"), 4, 1, S("abcd"));
	test1(S("abcde"), 4, 2, S("abcd"));
	test1(S("abcde"), 5, 0, S("abcde"));
	test1(S("abcde"), 5, 1, S("abcde"));
	test1(S("abcde"), 6, 0, S("can't happen"));
	test1(S("abcdefghij"), 0, 0, S("abcdefghij"));
	test1(S("abcdefghij"), 0, 1, S("bcdefghij"));
	test1(S("abcdefghij"), 0, 5, S("fghij"));
	test1(S("abcdefghij"), 0, 9, S("j"));
	test1(S("abcdefghij"), 0, 10, S(""));
	test1(S("abcdefghij"), 0, 11, S(""));
	test1(S("abcdefghij"), 1, 0, S("abcdefghij"));
	test1(S("abcdefghij"), 1, 1, S("acdefghij"));
	test1(S("abcdefghij"), 1, 4, S("afghij"));
	test1(S("abcdefghij"), 1, 8, S("aj"));
	test1(S("abcdefghij"), 1, 9, S("a"));
	test1(S("abcdefghij"), 1, 10, S("a"));
	test1(S("abcdefghij"), 5, 0, S("abcdefghij"));
	test1(S("abcdefghij"), 5, 1, S("abcdeghij"));
	test1(S("abcdefghij"), 5, 2, S("abcdehij"));
	test1(S("abcdefghij"), 5, 4, S("abcdej"));
	test1(S("abcdefghij"), 5, 5, S("abcde"));
	test1(S("abcdefghij"), 5, 6, S("abcde"));
	test1(S("abcdefghij"), 9, 0, S("abcdefghij"));
	test1(S("abcdefghij"), 9, 1, S("abcdefghi"));
	test1(S("abcdefghij"), 9, 2, S("abcdefghi"));
	test1(S("abcdefghij"), 10, 0, S("abcdefghij"));
	test1(S("abcdefghij"), 10, 1, S("abcdefghij"));
	test1(S("abcdefghij"), 11, 0, S("can't happen"));
	test1(S("abcdefghijklmnopqrst"), 0, 0, S("abcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 0, 1, S("bcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 0, 10, S("klmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 0, 19, S("t"));
	test1(S("abcdefghijklmnopqrst"), 0, 20, S(""));
	test1(S("abcdefghijklmnopqrst"), 0, 21, S(""));
	test1(S("abcdefghijklmnopqrst"), 1, 0, S("abcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 1, 1, S("acdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 1, 9, S("aklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 1, 18, S("at"));
	test1(S("abcdefghijklmnopqrst"), 1, 19, S("a"));
	test1(S("abcdefghijklmnopqrst"), 1, 20, S("a"));
	test1(S("abcdefghijklmnopqrst"), 10, 0, S("abcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 10, 1, S("abcdefghijlmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 10, 5, S("abcdefghijpqrst"));
	test1(S("abcdefghijklmnopqrst"), 10, 9, S("abcdefghijt"));
	test1(S("abcdefghijklmnopqrst"), 10, 10, S("abcdefghij"));
	test1(S("abcdefghijklmnopqrst"), 10, 11, S("abcdefghij"));
	test1(S("abcdefghijklmnopqrst"), 19, 0, S("abcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 19, 1, S("abcdefghijklmnopqrs"));
	test1(S("abcdefghijklmnopqrst"), 19, 2, S("abcdefghijklmnopqrs"));
	test1(S("abcdefghijklmnopqrst"), 20, 0, S("abcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 20, 1, S("abcdefghijklmnopqrst"));
	test1(S("abcdefghijklmnopqrst"), 21, 0, S("can't happen"));

	test2(S(""), 0, S(""));
	test2(S(""), 1, S("can't happen"));
	test2(S("abcde"), 0, S(""));
	test2(S("abcde"), 1, S("a"));
	test2(S("abcde"), 2, S("ab"));
	test2(S("abcde"), 4, S("abcd"));
	test2(S("abcde"), 5, S("abcde"));
	test2(S("abcde"), 6, S("can't happen"));
	test2(S("abcdefghij"), 0, S(""));
	test2(S("abcdefghij"), 1, S("a"));
	test2(S("abcdefghij"), 5, S("abcde"));
	test2(S("abcdefghij"), 9, S("abcdefghi"));
	test2(S("abcdefghij"), 10, S("abcdefghij"));
	test2(S("abcdefghij"), 11, S("can't happen"));
	test2(S("abcdefghijklmnopqrst"), 0, S(""));
	test2(S("abcdefghijklmnopqrst"), 1, S("a"));
	test2(S("abcdefghijklmnopqrst"), 10, S("abcdefghij"));
	test2(S("abcdefghijklmnopqrst"), 19, S("abcdefghijklmnopqrs"));
	test2(S("abcdefghijklmnopqrst"), 20, S("abcdefghijklmnopqrst"));
	test2(S("abcdefghijklmnopqrst"), 21, S("can't happen"));

	test3(S(""), S(""));
	test3(S("abcde"), S(""));
	test3(S("abcdefghij"), S(""));
	test3(S("abcdefghijklmnopqrst"), S(""));

	return (0);
}
