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

// template <class... Args>
//     pair<iterator, bool> emplace(Args&&... args);

// #include <unordered_map>
#include <cassert>

#include "../../../Emplaceable.h"
#include "tests/support/min_allocator.h"

#include <barefootc/pair.h>
#include <barefootc/string.h>
#define P1(k,v)	{ &bfc_int_string_pair_class, BFC_SIGNED_NUMBER(k), BFCSTR(v) }

l4sc_logger_ptr_t logger;

int main()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
        typedef std::unordered_map<int, Emplaceable> C;
        typedef std::pair<C::iterator, bool> R;
        C c;
        R r = c.emplace(std::piecewise_construct, std::forward_as_tuple(3),
                                                  std::forward_as_tuple());
        assert(r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3);
        assert(r.first->second == Emplaceable());

        r = c.emplace(std::pair<const int, Emplaceable>(4, Emplaceable(5, 6)));
        assert(r.second);
        assert(c.size() == 2);
        assert(r.first->first == 4);
        assert(r.first->second == Emplaceable(5, 6));

        r = c.emplace(std::piecewise_construct, std::forward_as_tuple(5),
                                               std::forward_as_tuple(6, 7));
        assert(r.second);
        assert(c.size() == 3);
        assert(r.first->first == 5);
        assert(r.first->second == Emplaceable(6, 7));
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<int, Emplaceable, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, Emplaceable>>> C;
        typedef std::pair<C::iterator, bool> R;
        C c;
        R r = c.emplace(std::piecewise_construct, std::forward_as_tuple(3),
                                                  std::forward_as_tuple());
        assert(r.second);
        assert(c.size() == 1);
        assert(r.first->first == 3);
        assert(r.first->second == Emplaceable());

        r = c.emplace(std::pair<const int, Emplaceable>(4, Emplaceable(5, 6)));
        assert(r.second);
        assert(c.size() == 2);
        assert(r.first->first == 4);
        assert(r.first->second == Emplaceable(5, 6));

        r = c.emplace(std::piecewise_construct, std::forward_as_tuple(5),
                                               std::forward_as_tuple(6, 7));
        assert(r.second);
        assert(c.size() == 3);
        assert(r.first->first == 5);
        assert(r.first->second == Emplaceable(6, 7));
    }
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}
#endif
