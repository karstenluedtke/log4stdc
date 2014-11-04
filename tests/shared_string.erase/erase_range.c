
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

static l4sc_logger_ptr_t logger;

static int
test(const char *s1, size_t pos1, size_t n1, const char *expected)
{
	int rc;
	bfc_string_t s, exp;
	bfc_iterator_t it1, it2, begin;
	size_t origlen;

	rc = bfc_init_shared_string_c_str(&s, sizeof(s), NULL, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_string_begin_iterator(&s, &begin, sizeof(begin));
	assert(rc >= 0);

	rc = bfc_init_iterator(&it1, sizeof(it1), (bfc_objptr_t) &s, pos1);
	assert(rc >= 0);
	assert(bfc_iterator_distance(&begin, &it1) == pos1);

	rc = bfc_init_iterator(&it2, sizeof(it2), (bfc_objptr_t) &s, pos1 + n1);
	assert(rc >= 0);
	assert(bfc_iterator_distance(&it1, &it2) == n1);
	assert(bfc_iterator_distance(&begin, &it2) == pos1 + n1);

	rc = bfc_string_erase_range(&s, &it1, &it2);
	if (rc >= 0) {
        	assert(pos1 + n1 <= origlen);
		bfc_init_shared_string_c_str(&exp, sizeof(exp), NULL, expected);
		assert(bfc_string_compare_bfstr(&s, &exp) == 0);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
		assert((pos1 == 0) || (pos1 + n1 == origlen) || (n1 == 0));
	} else {
		assert((pos1 != 0) && (pos1 + n1 != origlen) && (n1 != 0));
	}
	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
#define	S(x) x
	test(S(""), 0, 0, S(""));
	test(S("abcde"), 0, 0, S("abcde"));
	test(S("abcde"), 0, 1, S("bcde"));
	test(S("abcde"), 0, 2, S("cde"));
	test(S("abcde"), 0, 4, S("e"));
	test(S("abcde"), 0, 5, S(""));
	test(S("abcde"), 1, 0, S("abcde"));
	test(S("abcde"), 1, 1, S("acde"));
	test(S("abcde"), 1, 2, S("ade"));
	test(S("abcde"), 1, 3, S("ae"));
	test(S("abcde"), 1, 4, S("a"));
	test(S("abcde"), 2, 0, S("abcde"));
	test(S("abcde"), 2, 1, S("abde"));
	test(S("abcde"), 2, 2, S("abe"));
	test(S("abcde"), 2, 3, S("ab"));
	test(S("abcde"), 4, 0, S("abcde"));
	test(S("abcde"), 4, 1, S("abcd"));
	test(S("abcde"), 5, 0, S("abcde"));
	test(S("abcdefghij"), 0, 0, S("abcdefghij"));
	test(S("abcdefghij"), 0, 1, S("bcdefghij"));
	test(S("abcdefghij"), 0, 5, S("fghij"));
	test(S("abcdefghij"), 0, 9, S("j"));
	test(S("abcdefghij"), 0, 10, S(""));
	test(S("abcdefghij"), 1, 0, S("abcdefghij"));
	test(S("abcdefghij"), 1, 1, S("acdefghij"));
	test(S("abcdefghij"), 1, 4, S("afghij"));
	test(S("abcdefghij"), 1, 8, S("aj"));
	test(S("abcdefghij"), 1, 9, S("a"));
	test(S("abcdefghij"), 5, 0, S("abcdefghij"));
	test(S("abcdefghij"), 5, 1, S("abcdeghij"));
	test(S("abcdefghij"), 5, 2, S("abcdehij"));
	test(S("abcdefghij"), 5, 4, S("abcdej"));
	test(S("abcdefghij"), 5, 5, S("abcde"));
	test(S("abcdefghij"), 9, 0, S("abcdefghij"));
	test(S("abcdefghij"), 9, 1, S("abcdefghi"));
	test(S("abcdefghij"), 10, 0, S("abcdefghij"));
	test(S("abcdefghijklmnopqrst"), 0, 0, S("abcdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 0, 1, S("bcdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 0, 10, S("klmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 0, 19, S("t"));
	test(S("abcdefghijklmnopqrst"), 0, 20, S(""));
	test(S("abcdefghijklmnopqrst"), 1, 0, S("abcdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 1, 1, S("acdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 1, 9, S("aklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 1, 18, S("at"));
	test(S("abcdefghijklmnopqrst"), 1, 19, S("a"));
	test(S("abcdefghijklmnopqrst"), 10, 0, S("abcdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 10, 1, S("abcdefghijlmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 10, 5, S("abcdefghijpqrst"));
	test(S("abcdefghijklmnopqrst"), 10, 9, S("abcdefghijt"));
	test(S("abcdefghijklmnopqrst"), 10, 10, S("abcdefghij"));
	test(S("abcdefghijklmnopqrst"), 19, 0, S("abcdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 19, 1, S("abcdefghijklmnopqrs"));
	test(S("abcdefghijklmnopqrst"), 20, 0, S("abcdefghijklmnopqrst"));
	return (0);
}
