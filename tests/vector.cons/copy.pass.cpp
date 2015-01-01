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

// vector(const vector& v);

#include <vector>
#include <cassert>
#include "tests/support/test_allocator.h"
#include "tests/support/min_allocator.h"

template <class C>
void
test(const C& x)
{
    unsigned s = x.size();
    C c(x);
    assert(c.__invariants());
    assert(c.size() == s);
    assert(c == x);
}

int main()
{
    {
        int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
        int* an = a + sizeof(a)/sizeof(a[0]);
        test(barefootc::vector<int>(a, an));
    }
    {
        barefootc::vector<int, test_allocator<int> > v(3, 2, test_allocator<int>(5));
        barefootc::vector<int, test_allocator<int> > v2 = v;
        assert(v2 == v);
        assert(v2.get_allocator() == v.get_allocator());
    }
#if 0
#ifndef _LIBCPP_HAS_NO_ADVANCED_SFINAE
    {
        barefootc::vector<int, other_allocator<int> > v(3, 2, other_allocator<int>(5));
        barefootc::vector<int, other_allocator<int> > v2 = v;
        assert(v2 == v);
        assert(v2.get_allocator() == other_allocator<int>(-2));
    }
#endif  // _LIBCPP_HAS_NO_ADVANCED_SFINAE
#if __cplusplus >= 201103L
    {
        int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
        int* an = a + sizeof(a)/sizeof(a[0]);
        test(barefootc::vector<int, min_allocator<int>>(a, an));
    }
    {
        barefootc::vector<int, min_allocator<int> > v(3, 2, min_allocator<int>());
        barefootc::vector<int, min_allocator<int> > v2 = v;
        assert(v2 == v);
        assert(v2.get_allocator() == v.get_allocator());
    }
#endif
#endif
}
