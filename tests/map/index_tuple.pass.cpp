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

// http://llvm.org/bugs/show_bug.cgi?id=16542

// #include <unordered_map>

#ifndef _LIBCPP_HAS_NO_VARIADICS

#include <tuple>

using namespace std;

struct my_hash
{
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
    size_t operator()(const tuple<int,int>&) const {return 0;}
};

#endif

int main()
{
#ifndef _LIBCPP_HAS_NO_VARIADICS
    unordered_map<tuple<int,int>, size_t, my_hash> m;
    m[make_tuple(2,3)]=7;
#endif
}
