#include "tests/vector/cxxvector.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// iterator erase(const_iterator first, const_iterator last);

#include <vector>
#include <cassert>

#include "tests/support/min_allocator.h"

static l4sc_logger_ptr_t logger;

int main()
{
    int a1[] = {1, 2, 3};
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger("barefootc.container", 0);
    {
        barefootc::vector<int> l1(a1, a1+3);
        barefootc::vector<int>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        assert(l1.size() == 3);
        assert(distance(l1.cbegin(), l1.cend()) == 3);
        assert(i == l1.begin());
    }
    {
        barefootc::vector<int> l1(a1, a1+3);
        barefootc::vector<int>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin()));
        assert(l1.size() == 2);
        assert(distance(l1.cbegin(), l1.cend()) == 2);
        assert(i == l1.begin());
        assert(l1 == barefootc::vector<int>(a1+1, a1+3));
    }
    {
        barefootc::vector<int> l1(a1, a1+3);
        barefootc::vector<int>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 2));
        assert(l1.size() == 1);
        assert(distance(l1.cbegin(), l1.cend()) == 1);
        assert(i == l1.begin());
        assert(l1 == barefootc::vector<int>(a1+2, a1+3));
    }
    {
        barefootc::vector<int> l1(a1, a1+3);
        barefootc::vector<int>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 3));
        assert(l1.size() == 0);
        assert(distance(l1.cbegin(), l1.cend()) == 0);
        assert(i == l1.begin());
    }
    {
	L4SC_DEBUG(logger, "%s: vector-of-vector test", __FUNCTION__);
        barefootc::vector<barefootc::vector<int> > outer(2, barefootc::vector<int>(1));
        outer.erase(outer.begin(), outer.begin());
        assert(outer.size() == 2);
        assert(outer[0].size() == 1);
        assert(outer[1].size() == 1);
	L4SC_DEBUG(logger, "%s: vector-of-vector test done.", __FUNCTION__);
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> l1(a1, a1+3);
        barefootc::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        assert(l1.size() == 3);
        assert(distance(l1.cbegin(), l1.cend()) == 3);
        assert(i == l1.begin());
    }
    {
        barefootc::vector<int, min_allocator<int>> l1(a1, a1+3);
        barefootc::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin()));
        assert(l1.size() == 2);
        assert(distance(l1.cbegin(), l1.cend()) == 2);
        assert(i == l1.begin());
        assert((l1 == barefootc::vector<int, min_allocator<int>>(a1+1, a1+3)));
    }
    {
        barefootc::vector<int, min_allocator<int>> l1(a1, a1+3);
        barefootc::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 2));
        assert(l1.size() == 1);
        assert(distance(l1.cbegin(), l1.cend()) == 1);
        assert(i == l1.begin());
        assert((l1 == barefootc::vector<int, min_allocator<int>>(a1+2, a1+3)));
    }
    {
        barefootc::vector<int, min_allocator<int>> l1(a1, a1+3);
        barefootc::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 3));
        assert(l1.size() == 0);
        assert(distance(l1.cbegin(), l1.cend()) == 0);
        assert(i == l1.begin());
    }
    {
        barefootc::vector<barefootc::vector<int, min_allocator<int>>, min_allocator<barefootc::vector<int, min_allocator<int>>>> outer(2, barefootc::vector<int, min_allocator<int>>(1));
        outer.erase(outer.begin(), outer.begin());
        assert(outer.size() == 2);
        assert(outer[0].size() == 1);
        assert(outer[1].size() == 1);
    }
#endif
}
