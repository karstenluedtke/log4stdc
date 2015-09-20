#include "tests/map/cxxmap.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// iterator find(const key_type& k);

// #include <unordered_map>
#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

#include <barefootc/pair.h>
#include <barefootc/string.h>
#define P1(k,v)	{ &bfc_int_string_pair_class, BFC_SIGNED_NUMBER(k), BFCSTR(v) }

l4sc_logger_ptr_t logger;

int main()
{
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
    {
	typedef barefootc::map<bfc_number_t, bfc_string_t> C;
	typedef C::value_type P;
        P a[] =
        {
            P1(10, "ten"),
            P1(20, "twenty"),
            P1(30, "thirty"),
            P1(40, "fourty"),
            P1(50, "fifty"),
            P1(60, "sixty"),
            P1(70, "seventy"),
            P1(80, "eighty"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
	bfc_number_t key30 = BFC_SIGNED_NUMBER(30);
        C::iterator i = c.find(key30);
	P *p = (P *)bfc_iterator_index(i.bfciter());
	bfc_cnumptr_t num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	bfc_cstrptr_t str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 30);
        assert(bfc_string_compare_c_str(str, "thirty") == 0);

	bfc_number_t key5 = BFC_SIGNED_NUMBER(5);
        i = c.find(key5);
        assert(i == c.end());
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "fourty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c(std::begin(a), std::end(a));
        C::iterator i = c.find(30);
        assert(i->first == 30);
        assert(i->second == "thirty");
        i = c.find(5);
        assert(i == c.end());
    }
#endif
#endif
}
