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

// iterator insert(const_iterator p, const value_type& x);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

// #include <unordered_map>
#include <cassert>

#include "tests/support/min_allocator.h"

#include <barefootc/pair.h>
#include <barefootc/string.h>

typedef barefootc::map<bfc_number_t, bfc_number_t> C;
typedef C::value_type P;

P P3(double k, int v)
{
	P p = { &bfc_real_int_pair_class,
		{ &bfc_real_number_class, 0 },
		BFC_SIGNED_NUMBER(v) };
	bfc_real_number_set(&p.first, k);
	return (p);
}

l4sc_logger_ptr_t logger;

int main()
{
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
    {
        typedef C::iterator R;
	P init[] = { P3(0.0, 0) }; // just for setting the pair type
        C c(init,init);            // begin == last, nothing inserted
        C::const_iterator e = c.end();
        R r = c.insert(e, P3(3.5, 3));
        assert(c.size() == 1);
	const P *rp = (P *) bfc_iterator_index(r.bfciter());
        assert(bfc_real_number_get(&rp->first) == 3.5);
        assert(bfc_object_getlong(&rp->second) == 3);

        r = c.insert(c.end(), P3(3.5, 4));
        assert(c.size() == 1);
	rp = (P *) bfc_iterator_index(r.bfciter());
        assert(bfc_real_number_get(&rp->first) == 3.5);
        assert(bfc_object_getlong(&rp->second) == 3);

        r = c.insert(c.end(), P3(4.5, 4));
        assert(c.size() == 2);
	rp = (P *) bfc_iterator_index(r.bfciter());
        assert(bfc_real_number_get(&rp->first) == 4.5);
        assert(bfc_object_getlong(&rp->second) == 4);

        r = c.insert(c.end(), P3(5.5, 4));
        assert(c.size() == 3);
	rp = (P *) bfc_iterator_index(r.bfciter());
        assert(bfc_real_number_get(&rp->first) == 5.5);
        assert(bfc_object_getlong(&rp->second) == 4);
    }
#if 0
#if __cplusplus >= 201103L
    {
        typedef std::unordered_map<double, int, std::hash<double>, std::equal_to<double>,
                            min_allocator<std::pair<const double, int>>> C;
        typedef C::iterator R;
        typedef C::value_type P;
        C c;
        C::const_iterator e = c.end();
        R r = c.insert(e, P(3.5, 3));
        assert(c.size() == 1);
        assert(r->first == 3.5);
        assert(r->second == 3);

        r = c.insert(c.end(), P(3.5, 4));
        assert(c.size() == 1);
        assert(r->first == 3.5);
        assert(r->second == 3);

        r = c.insert(c.end(), P(4.5, 4));
        assert(c.size() == 2);
        assert(r->first == 4.5);
        assert(r->second == 4);

        r = c.insert(c.end(), P(5.5, 4));
        assert(c.size() == 3);
        assert(r->first == 5.5);
        assert(r->second == 4);
    }
#endif
#if _LIBCPP_DEBUG >= 1
    {
        typedef std::unordered_map<double, int> C;
        typedef C::iterator R;
        typedef C::value_type P;
        C c;
        C c2;
        C::const_iterator e = c2.end();
        P v(3.5, 3);
        R r = c.insert(e, v);
        assert(false);
    }
#endif
#endif
}
