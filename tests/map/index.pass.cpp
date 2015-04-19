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

// mapped_type& operator[](const key_type& k);

// #include <unordered_map>
#include <string>
#include <cassert>

// #include "../../../MoveOnly.h"
// #include "min_allocator.h"

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
            P1(1, "one"),
            P1(2, "two"),
            P1(3, "three"),
            P1(4, "four"),
            P1(1, "four"),
            P1(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        assert(c.size() == 4);

        // c[1] = "ONE";
        // assert(c.at(1) == "ONE");
	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
	bfc_string_assign_c_str(&c[key1], "ONE");
        assert(bfc_string_compare_c_str(&c[key1], "ONE") == 0);

        // c[11] = "eleven";
        // assert(c.at(11) == "eleven");
	bfc_number_t key11 = BFC_SIGNED_NUMBER(11);
	bfc_string_assign_c_str(&c[key11], "eleven");
        assert(bfc_string_compare_c_str(&c[key11], "eleven") == 0);
        assert(c.size() == 5);
    }
#if 0
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        typedef std::unordered_map<MoveOnly, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        assert(c.size() == 4);
        c[1] = "ONE";
        assert(c.at(1) == "ONE");
        c[11] = "eleven";
        assert(c.size() == 5);
        assert(c.at(11) == "eleven");
    }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        assert(c.size() == 4);
        c[1] = "ONE";
        assert(c.at(1) == "ONE");
        c[11] = "eleven";
        assert(c.size() == 5);
        assert(c.at(11) == "eleven");
    }
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        typedef std::unordered_map<MoveOnly, std::string, std::hash<MoveOnly>, std::equal_to<MoveOnly>,
                            min_allocator<std::pair<const MoveOnly, std::string>>> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        assert(c.size() == 4);
        c[1] = "ONE";
        assert(c.at(1) == "ONE");
        c[11] = "eleven";
        assert(c.size() == 5);
        assert(c.at(11) == "eleven");
    }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#endif
#endif
}
