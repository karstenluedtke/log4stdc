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

// pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

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
        typedef C::const_iterator I;
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
        const C c(a, a + sizeof(a)/sizeof(a[0]));
	bfc_number_t key30 = BFC_SIGNED_NUMBER(30);
        std::pair<I, I> r = c.equal_range(key30);
        assert(r.first.distance(r.second) == 1);
	P *p = (P *)bfc_container_first(r.first.bfciter());
	bfc_cnumptr_t num = (bfc_cnumptr_t)bfc_container_first(p);
	bfc_cstrptr_t str = (bfc_cstrptr_t)bfc_container_index(p,1);
        assert(num->u.n == 30);
        assert(bfc_string_compare_c_str(str, "thirty") == 0);

	bfc_number_t key5 = BFC_SIGNED_NUMBER(5);
        r = c.equal_range(key5);
        assert(r.first.distance(r.second) == 0);
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef C::const_iterator I;
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
        const C c(std::begin(a), std::end(a));
        std::pair<I, I> r = c.equal_range(30);
        assert(std::distance(r.first, r.second) == 1);
        assert(r.first->first == 30);
        assert(r.first->second == "thirty");
        r = c.equal_range(5);
        assert(std::distance(r.first, r.second) == 0);
    }
#endif
#endif
}
