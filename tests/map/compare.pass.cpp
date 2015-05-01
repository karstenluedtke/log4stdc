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

// http://llvm.org/bugs/show_bug.cgi?id=16538
// http://llvm.org/bugs/show_bug.cgi?id=16549

// #include <unordered_map>

struct Key {
  template <typename T> Key(const T&) {}
  bool operator== (const Key&) const { return true; }
};

namespace std
{
    template <>
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
    struct hash<Key>
    {
        size_t operator()(Key const &) const {return 0;}
    };
}

int
main()
{
    std::unordered_map<Key, int>::iterator it =
        std::unordered_map<Key, int>().find(Key(0));
    std::pair<std::unordered_map<Key, int>::iterator, bool> result =
        std::unordered_map<Key, int>().insert(std::make_pair(Key(0), 0));
}
#if 0
#endif