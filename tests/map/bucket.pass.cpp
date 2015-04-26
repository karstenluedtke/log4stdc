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

// size_type bucket(const key_type& __k) const;

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

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
            P1(1, "one"),
            P1(2, "two"),
            P1(3, "three"),
            P1(4, "four"),
            P1(1, "four"),
            P1(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        size_t bc = c.bucket_count();
        assert(bc >= 5);
	bfc_number_t key = BFC_SIGNED_NUMBER(0);
        for (size_t i = 0; i < 13; ++i) {
	    key.u.n = i;
	    L4SC_DEBUG(logger, "%s: key %d, bucket %d/%d",
		__FUNCTION__, (int) i, (int) c.bucket(key), (int) bc);
            assert(c.bucket(key) == i % bc);
	}
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
        const C c(std::begin(a), std::end(a));
        size_t bc = c.bucket_count();
        assert(bc >= 5);
        for (size_t i = 0; i < 13; ++i)
            assert(c.bucket(i) == i % bc);
    }
#endif
#if _LIBCPP_DEBUG_LEVEL >= 1
    {
        typedef std::unordered_map<int, std::string> C;
        C c;
        C::size_type i = c.bucket(3);
        assert(false);
    }
#endif
#endif
}
