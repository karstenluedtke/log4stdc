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

// vector(vector&& c);

#include <vector>
#include <cassert>
#include "../../../MoveOnly.h"
#include "test_allocator.h"
#include "tests/support/min_allocator.h"

int main()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        barefootc::vector<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        barefootc::vector<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        barefootc::vector<MoveOnly, test_allocator<MoveOnly> > l2 = std::move(l);
        assert(l2 == lo);
        assert(l.empty());
        assert(l2.get_allocator() == lo.get_allocator());
    }
    {
        barefootc::vector<MoveOnly, other_allocator<MoveOnly> > l(other_allocator<MoveOnly>(5));
        barefootc::vector<MoveOnly, other_allocator<MoveOnly> > lo(other_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        barefootc::vector<MoveOnly, other_allocator<MoveOnly> > l2 = std::move(l);
        assert(l2 == lo);
        assert(l.empty());
        assert(l2.get_allocator() == lo.get_allocator());
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        barefootc::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        barefootc::vector<int>::const_iterator i = c1.begin();
        barefootc::vector<int> c2 = std::move(c1);
        barefootc::vector<int>::iterator j = c2.erase(i);
        assert(*j == 3);
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<MoveOnly, min_allocator<MoveOnly> > l(min_allocator<MoveOnly>{});
        barefootc::vector<MoveOnly, min_allocator<MoveOnly> > lo(min_allocator<MoveOnly>{});
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        barefootc::vector<MoveOnly, min_allocator<MoveOnly> > l2 = std::move(l);
        assert(l2 == lo);
        assert(l.empty());
        assert(l2.get_allocator() == lo.get_allocator());
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        barefootc::vector<int, min_allocator<int>> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        barefootc::vector<int, min_allocator<int>>::const_iterator i = c1.begin();
        barefootc::vector<int, min_allocator<int>> c2 = std::move(c1);
        barefootc::vector<int, min_allocator<int>>::iterator j = c2.erase(i);
        assert(*j == 3);
    }
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}
