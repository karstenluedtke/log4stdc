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

// template <class Key, class T, class Hash, class Pred, class Alloc>
// bool
// operator==(const unordered_map<Key, T, Hash, Pred, Alloc>& x,
//            const unordered_map<Key, T, Hash, Pred, Alloc>& y);
//
// template <class Key, class T, class Hash, class Pred, class Alloc>
// bool
// operator!=(const unordered_map<Key, T, Hash, Pred, Alloc>& x,
//            const unordered_map<Key, T, Hash, Pred, Alloc>& y);

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
        const C c1(a, a + sizeof(a)/sizeof(a[0]));
        const C c2;
        assert(!(c1 == c2));
        assert( (c1 != c2));
    }
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
        const C c1(a, a + sizeof(a)/sizeof(a[0]));
        const C c2 = c1;
        assert( (c1 == c2));
        assert(!(c1 != c2));
    }
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
        C c1(a, a + sizeof(a)/sizeof(a[0]));
        C c2 = c1;
        c2.rehash(30);
        assert( (c1 == c2));
        assert(!(c1 != c2));
	P p90 = P1(90, "ninety");
        c2.insert(p90);
        assert(!(c1 == c2));
        assert( (c1 != c2));
        c1.insert(p90);
        assert( (c1 == c2));
        assert(!(c1 != c2));
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
        const C c1(std::begin(a), std::end(a));
        const C c2;
        assert(!(c1 == c2));
        assert( (c1 != c2));
    }
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
        const C c1(std::begin(a), std::end(a));
        const C c2 = c1;
        assert( (c1 == c2));
        assert(!(c1 != c2));
    }
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
        C c1(std::begin(a), std::end(a));
        C c2 = c1;
        c2.rehash(30);
        assert( (c1 == c2));
        assert(!(c1 != c2));
        c2.insert(P(90, "ninety"));
        assert(!(c1 == c2));
        assert( (c1 != c2));
        c1.insert(P(90, "ninety"));
        assert( (c1 == c2));
        assert(!(c1 != c2));
    }
#endif
#endif
}
