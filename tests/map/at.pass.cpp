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

// mapped_type&       at(const key_type& k);
// const mapped_type& at(const key_type& k) const;

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
	typedef barefootc::map<bfc_number_t, bfc_number_t> C;
	typedef C::value_type P;
	static const P a[] = {
		{ &bfc_int_pair_class,
		  BFC_SIGNED_NUMBER(1), BFC_SIGNED_NUMBER(11) },
		{ &bfc_int_pair_class,
		  BFC_SIGNED_NUMBER(2), BFC_SIGNED_NUMBER(22) },
	};
        C c(a, a + sizeof(a)/sizeof(a[0]));
    }
    {
	typedef barefootc::map<bfc_number_t, bfc_string_t> C;
	typedef C::value_type P;
        const P a[] =
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
	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
	bfc_string_assign_c_str(&c.at(key1), "ONE");
        assert(bfc_string_compare_c_str(&c.at(key1), "ONE") == 0);
        try
        {
	    bfc_number_t key11 = BFC_SIGNED_NUMBER(11);
	    bfc_string_assign_c_str(&c.at(key11), "eleven");
            assert(false);
        }
        catch (std::out_of_range&)
        {
        }
        assert(c.size() == 4);
    }
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
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        assert(c.size() == 4);
	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
        assert(bfc_string_compare_c_str(&c.at(key1), "one") == 0);
        try
        {
	    bfc_number_t key11 = BFC_SIGNED_NUMBER(11);
            c.at(key11);
            assert(false);
        }
        catch (std::out_of_range&)
        {
        }
        assert(c.size() == 4);
    }
#if 0
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
        c.at(1) = "ONE";
        assert(c.at(1) == "ONE");
        try
        {
            c.at(11) = "eleven";
            assert(false);
        }
        catch (std::out_of_range&)
        {
        }
        assert(c.size() == 4);
    }
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
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        assert(c.size() == 4);
        assert(c.at(1) == "one");
        try
        {
            c.at(11);
            assert(false);
        }
        catch (std::out_of_range&)
        {
        }
        assert(c.size() == 4);
    }
#endif
#endif
}
