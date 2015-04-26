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

// pair<iterator, bool> insert(const value_type& x);

// #include <unordered_map>
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
        typedef std::unordered_map<double, int> C;
        typedef std::pair<C::iterator, bool> R;
        typedef C::value_type P;
        C c;
        R r = c.insert(P(3.5, 3));
        assert(r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3.5);
        assert(r.first->second == 3);

        r = c.insert(P(3.5, 4));
        assert(!r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3.5);
        assert(r.first->second == 3);

        r = c.insert(P(4.5, 4));
        assert(r.second);
        assert(c.size() == 2);
        assert(r.first->first == 4.5);
        assert(r.first->second == 4);

        r = c.insert(P(5.5, 4));
        assert(r.second);
        assert(c.size() == 3);
        assert(r.first->first == 5.5);
        assert(r.first->second == 4);
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<double, int, std::hash<double>, std::equal_to<double>,
                            min_allocator<std::pair<const double, int>>> C;
        typedef std::pair<C::iterator, bool> R;
        typedef C::value_type P;
        C c;
        R r = c.insert(P(3.5, 3));
        assert(r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3.5);
        assert(r.first->second == 3);

        r = c.insert(P(3.5, 4));
        assert(!r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3.5);
        assert(r.first->second == 3);

        r = c.insert(P(4.5, 4));
        assert(r.second);
        assert(c.size() == 2);
        assert(r.first->first == 4.5);
        assert(r.first->second == 4);

        r = c.insert(P(5.5, 4));
        assert(r.second);
        assert(c.size() == 3);
        assert(r.first->first == 5.5);
        assert(r.first->second == 4);
    }
#endif
#endif
}
