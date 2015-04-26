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

// void rehash(size_type n);

// #include <unordered_map>
#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

#include <barefootc/pair.h>
#include <barefootc/string.h>
#define P1(k,v)	{ &bfc_int_string_pair_class, BFC_SIGNED_NUMBER(k), BFCSTR(v) }

l4sc_logger_ptr_t logger;

template <class C>
void test(const C& c)
{
    assert(c.size() == 4);
    bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
    assert(bfc_string_compare_c_str(&c.at(key1), "one") == 0);
    bfc_number_t key2 = BFC_SIGNED_NUMBER(2);
    assert(bfc_string_compare_c_str(&c.at(key2), "two") == 0);
    bfc_number_t key3 = BFC_SIGNED_NUMBER(3);
    assert(bfc_string_compare_c_str(&c.at(key3), "three") == 0);
    bfc_number_t key4 = BFC_SIGNED_NUMBER(4);
    assert(bfc_string_compare_c_str(&c.at(key4), "four") == 0);
}

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
        test(c);
        assert(c.bucket_count() >= 5);
        c.rehash(3);
        assert(c.bucket_count() == 5);
        test(c);
        c.max_load_factor(2);
        c.rehash(3);
      //  assert(c.bucket_count() == 3);
        test(c);
        c.rehash(31);
      //  assert(c.bucket_count() == 31);
        test(c);
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
        test(c);
        assert(c.bucket_count() >= 5);
        c.rehash(3);
        assert(c.bucket_count() == 5);
        test(c);
        c.max_load_factor(2);
        c.rehash(3);
        assert(c.bucket_count() == 3);
        test(c);
        c.rehash(31);
        assert(c.bucket_count() == 31);
        test(c);
    }
#endif
#endif
}
