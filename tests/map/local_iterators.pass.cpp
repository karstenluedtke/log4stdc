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

// local_iterator       begin (size_type n);
// local_iterator       end   (size_type n);
// const_local_iterator begin (size_type n) const;
// const_local_iterator end   (size_type n) const;
// const_local_iterator cbegin(size_type n) const;
// const_local_iterator cend  (size_type n) const;

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
        typedef C::local_iterator I;
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
        assert(c.bucket_count() >= 5);

	bfc_number_t key0 = BFC_SIGNED_NUMBER(0);
        C::size_type b = c.bucket(key0);
        I i = c.begin(b);
        I j = c.end(b);
        assert(i.distance(j) == 0);

	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
        b = c.bucket(key1);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	P *p = (P *)bfc_iterator_index(i.bfciter());
	bfc_cnumptr_t num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	bfc_cstrptr_t str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 1);
        assert(bfc_string_compare_c_str(str, "one") == 0);

	bfc_number_t key2 = BFC_SIGNED_NUMBER(2);
        b = c.bucket(key2);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 2);
        assert(bfc_string_compare_c_str(str, "two") == 0);

	bfc_number_t key3 = BFC_SIGNED_NUMBER(3);
        b = c.bucket(key3);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 3);
        assert(bfc_string_compare_c_str(str, "three") == 0);

	bfc_number_t key4 = BFC_SIGNED_NUMBER(4);
        b = c.bucket(key4);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 4);
        assert(bfc_string_compare_c_str(str, "four") == 0);
    }
    {
	typedef barefootc::map<bfc_number_t, bfc_string_t> C;
	typedef C::value_type P;
        typedef C::const_local_iterator I;
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
        assert(c.bucket_count() >= 5);

	bfc_number_t key0 = BFC_SIGNED_NUMBER(0);
        C::size_type b = c.bucket(key0);
        I i = c.begin(b);
        I j = c.end(b);
        assert(i.distance(j) == 0);

	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
        b = c.bucket(key1);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	P *p = (P *)bfc_iterator_index(i.bfciter());
	bfc_cnumptr_t num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	bfc_cstrptr_t str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 1);
        assert(bfc_string_compare_c_str(str, "one") == 0);

	bfc_number_t key2 = BFC_SIGNED_NUMBER(2);
        b = c.bucket(key2);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 2);
        assert(bfc_string_compare_c_str(str, "two") == 0);

	bfc_number_t key3 = BFC_SIGNED_NUMBER(3);
        b = c.bucket(key3);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 3);
        assert(bfc_string_compare_c_str(str, "three") == 0);

	bfc_number_t key4 = BFC_SIGNED_NUMBER(4);
        b = c.bucket(key4);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 4);
        assert(bfc_string_compare_c_str(str, "four") == 0);
    }
    {
	typedef barefootc::map<bfc_number_t, bfc_string_t> C;
	typedef C::value_type P;
        typedef C::const_local_iterator I;
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
        assert(c.bucket_count() >= 5);

	bfc_number_t key0 = BFC_SIGNED_NUMBER(0);
        C::size_type b = c.bucket(key0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        assert(i.distance(j) == 0);

	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
        b = c.bucket(key1);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	P *p = (P *)bfc_iterator_index(i.bfciter());
	bfc_cnumptr_t num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	bfc_cstrptr_t str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 1);
        assert(bfc_string_compare_c_str(str, "one") == 0);

	bfc_number_t key2 = BFC_SIGNED_NUMBER(2);
        b = c.bucket(key2);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 2);
        assert(bfc_string_compare_c_str(str, "two") == 0);

	bfc_number_t key3 = BFC_SIGNED_NUMBER(3);
        b = c.bucket(key3);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 3);
        assert(bfc_string_compare_c_str(str, "three") == 0);

	bfc_number_t key4 = BFC_SIGNED_NUMBER(4);
        b = c.bucket(key4);
        i = c.begin(b);
        j = c.end(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 4);
        assert(bfc_string_compare_c_str(str, "four") == 0);
    }
    {
	typedef barefootc::map<bfc_number_t, bfc_string_t> C;
	typedef C::value_type P;
        typedef C::const_local_iterator I;
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
        assert(c.bucket_count() >= 5);

	bfc_number_t key0 = BFC_SIGNED_NUMBER(0);
        C::size_type b = c.bucket(key0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        assert(i.distance(j) == 0);

	bfc_number_t key1 = BFC_SIGNED_NUMBER(1);
        b = c.bucket(key1);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(i.distance(j) == 1);
	P *p = (P *)bfc_iterator_index(i.bfciter());
	bfc_cnumptr_t num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	bfc_cstrptr_t str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 1);
        assert(bfc_string_compare_c_str(str, "one") == 0);

	bfc_number_t key2 = BFC_SIGNED_NUMBER(2);
        b = c.bucket(key2);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 2);
        assert(bfc_string_compare_c_str(str, "two") == 0);

	bfc_number_t key3 = BFC_SIGNED_NUMBER(3);
        b = c.bucket(key3);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 3);
        assert(bfc_string_compare_c_str(str, "three") == 0);

	bfc_number_t key4 = BFC_SIGNED_NUMBER(4);
        b = c.bucket(key4);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(i.distance(j) == 1);
	p = (P *)bfc_iterator_index(i.bfciter());
	num = (bfc_cnumptr_t)bfc_container_first((bfc_contptr_t)p);
	str = (bfc_cstrptr_t)bfc_container_index((bfc_contptr_t)p,1);
        assert(bfc_object_getlong(num) == 4);
        assert(bfc_string_compare_c_str(str, "four") == 0);
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::local_iterator I;
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
        assert(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        assert(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 1);
        assert(i->second == "one");

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 2);
        assert(i->second == "two");

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 3);
        assert(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 4);
        assert(i->second == "four");
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
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
        assert(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.begin(b);
        I j = c.end(b);
        assert(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 1);
        assert(i->second == "one");

        b = c.bucket(2);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 2);
        assert(i->second == "two");

        b = c.bucket(3);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 3);
        assert(i->second == "three");

        b = c.bucket(4);
        i = c.begin(b);
        j = c.end(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 4);
        assert(i->second == "four");
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
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
        assert(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        assert(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 1);
        assert(i->second == "one");

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 2);
        assert(i->second == "two");

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 3);
        assert(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 4);
        assert(i->second == "four");
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        typedef C::const_local_iterator I;
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
        assert(c.bucket_count() >= 5);
        C::size_type b = c.bucket(0);
        I i = c.cbegin(b);
        I j = c.cend(b);
        assert(std::distance(i, j) == 0);

        b = c.bucket(1);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 1);
        assert(i->second == "one");

        b = c.bucket(2);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 2);
        assert(i->second == "two");

        b = c.bucket(3);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 3);
        assert(i->second == "three");

        b = c.bucket(4);
        i = c.cbegin(b);
        j = c.cend(b);
        assert(std::distance(i, j) == 1);
        assert(i->first == 4);
        assert(i->second == "four");
    }
#endif
#endif
}
