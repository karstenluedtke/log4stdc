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

// iterator insert(const_iterator position, size_type n, const value_type& x);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

int main()
{
    {
        barefootc::vector<int> v(100);
        barefootc::vector<int>::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        assert(v.size() == 105);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (; j < 15; ++j)
            assert(v[j] == 1);
        for (++j; j < 105; ++j)
            assert(v[j] == 0);
    }
    {
        barefootc::vector<int, stack_allocator<int, 300> > v(100);
        barefootc::vector<int, stack_allocator<int, 300> >::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        assert(v.size() == 105);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (; j < 15; ++j)
            assert(v[j] == 1);
        for (++j; j < 105; ++j)
            assert(v[j] == 0);
    }
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::vector<int> c1(100);
        barefootc::vector<int> c2;
        barefootc::vector<int>::iterator i = c1.insert(c2.cbegin() + 10, 5, 1);
        assert(false);
    }
#endif
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        barefootc::vector<int, min_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        assert(v.size() == 105);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (; j < 15; ++j)
            assert(v[j] == 1);
        for (++j; j < 105; ++j)
            assert(v[j] == 0);
    }
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        barefootc::vector<int, min_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        assert(v.size() == 105);
        assert(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            assert(v[j] == 0);
        for (; j < 15; ++j)
            assert(v[j] == 1);
        for (++j; j < 105; ++j)
            assert(v[j] == 0);
    }
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::vector<int, min_allocator<int>> c1(100);
        barefootc::vector<int, min_allocator<int>> c2;
        barefootc::vector<int, min_allocator<int>>::iterator i = c1.insert(c2.cbegin() + 10, 5, 1);
        assert(false);
    }
#endif
#endif
}