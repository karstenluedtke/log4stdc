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

// unordered_map(size_type n, const hasher& hf, const key_equal& eql);

// #include <unordered_map>
#include <cassert>

#include "../../../NotConstructible.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
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
        typedef std::unordered_map<NotConstructible, NotConstructible,
                                   test_hash<std::hash<NotConstructible> >,
                                   test_compare<std::equal_to<NotConstructible> >,
                                   test_allocator<std::pair<const NotConstructible,
                                                                  NotConstructible> >
                                   > C;
        C c(7,
            test_hash<std::hash<NotConstructible> >(8),
            test_compare<std::equal_to<NotConstructible> >(9)
           );
        assert(c.bucket_count() == 7);
        assert(c.hash_function() == test_hash<std::hash<NotConstructible> >(8));
        assert(c.key_eq() == test_compare<std::equal_to<NotConstructible> >(9));
        assert(c.get_allocator() ==
               (test_allocator<std::pair<const NotConstructible, NotConstructible> >()));
        assert(c.size() == 0);
        assert(c.empty());
        assert(std::distance(c.begin(), c.end()) == 0);
        assert(c.load_factor() == 0);
        assert(c.max_load_factor() == 1);
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<NotConstructible, NotConstructible,
                                   test_hash<std::hash<NotConstructible> >,
                                   test_compare<std::equal_to<NotConstructible> >,
                                   min_allocator<std::pair<const NotConstructible,
                                                                 NotConstructible> >
                                   > C;
        C c(7,
            test_hash<std::hash<NotConstructible> >(8),
            test_compare<std::equal_to<NotConstructible> >(9)
           );
        assert(c.bucket_count() == 7);
        assert(c.hash_function() == test_hash<std::hash<NotConstructible> >(8));
        assert(c.key_eq() == test_compare<std::equal_to<NotConstructible> >(9));
        assert(c.get_allocator() ==
               (min_allocator<std::pair<const NotConstructible, NotConstructible> >()));
        assert(c.size() == 0);
        assert(c.empty());
        assert(std::distance(c.begin(), c.end()) == 0);
        assert(c.load_factor() == 0);
        assert(c.max_load_factor() == 1);
    }
#endif
#endif
}
