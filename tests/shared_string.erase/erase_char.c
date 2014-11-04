
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

static l4sc_logger_ptr_t logger;

static int
test(const char *s1, size_t pos1, const char *expected)
{
	int rc;
	bfc_string_t s, exp;
	bfc_iterator_t it, begin;
	size_t origlen;

	rc = bfc_init_shared_string_c_str(&s, sizeof(s), NULL, s1);
	assert(rc >= 0);

	origlen = bfc_string_length(&s);
	assert(origlen == strlen(s1));
	
	rc = bfc_string_begin_iterator(&s, &begin, sizeof(begin));
	assert(rc >= 0);

	rc = bfc_init_iterator(&it, sizeof(it), (bfc_objptr_t) &s, pos1);
	assert(rc >= 0);
	assert(bfc_iterator_distance(&begin, &it) == pos1);

	rc = bfc_string_erase_char(&s, &it);
	if (rc >= 0) {
        	assert(pos1 <= origlen);
		bfc_init_shared_string_c_str(&exp, sizeof(exp), NULL, expected);
		assert(bfc_string_compare_bfstr(&s, &exp) == 0);
		assert(bfc_string_compare_c_str(&s, expected) == 0);
		assert((pos1 == 0) || (pos1 == origlen-1));
	} else {
		assert((pos1 != 0) && (pos1 != origlen-1));
	}
	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
#define	S(x) x
	test(S("abcde"), 0, S("bcde"));
	test(S("abcde"), 1, S("acde"));
	test(S("abcde"), 2, S("abde"));
	test(S("abcde"), 4, S("abcd"));
	test(S("abcdefghij"), 0, S("bcdefghij"));
	test(S("abcdefghij"), 1, S("acdefghij"));
	test(S("abcdefghij"), 5, S("abcdeghij"));
	test(S("abcdefghij"), 9, S("abcdefghi"));
	test(S("abcdefghijklmnopqrst"), 0, S("bcdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 1, S("acdefghijklmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 10, S("abcdefghijlmnopqrst"));
	test(S("abcdefghijklmnopqrst"), 19, S("abcdefghijklmnopqrs"));

	return (0);
}
