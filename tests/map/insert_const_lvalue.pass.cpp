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

typedef barefootc::map<bfc_number_t, bfc_number_t> C;
typedef C::value_type P;
typedef std::pair<C::iterator, bool> R;

P P3(double k, int v)
{
	P p = { &bfc_real_int_pair_class,
		{ &bfc_real_number_class, 0 },
		BFC_SIGNED_NUMBER(v) };
	p.first.u.f = k;
	return (p);
}
		

l4sc_logger_ptr_t logger;

int main()
{
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
    {
        C c;
        R r = c.insert(P3(3.5, 3));
        assert(r.second);
        assert(c.size() == 1);
	const P *rp = (P *) bfc_iterator_index(r.first.bfciter());
        assert(rp->first.u.f == 3.5);
        assert(rp->second.u.n == 3);

        r = c.insert(P3(3.5, 4));
        assert(!r.second);
        assert(c.size() == 1);
	rp = (P *) bfc_iterator_index(r.first.bfciter());
        assert(rp->first.u.f == 3.5);
        assert(rp->second.u.n == 3);

        r = c.insert(P3(4.5, 4));
        assert(r.second);
        assert(c.size() == 2);
	rp = (P *) bfc_iterator_index(r.first.bfciter());
        assert(rp->first.u.f == 4.5);
        assert(rp->second.u.n == 4);

        r = c.insert(P3(5.5, 4));
        assert(r.second);
        assert(c.size() == 3);
	rp = (P *) bfc_iterator_index(r.first.bfciter());
        assert(rp->first.u.f == 5.5);
        assert(rp->second.u.n == 4);
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
