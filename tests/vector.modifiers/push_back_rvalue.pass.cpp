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

// void push_back(value_type&& x);

#include <vector>
#include <cassert>
#include "../../../MoveOnly.h"
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

int main()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        barefootc::vector<MoveOnly> c;
        c.push_back(MoveOnly(0));
        assert(c.size() == 1);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        assert(c.size() == 2);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        assert(c.size() == 3);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        assert(c.size() == 4);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        assert(c.size() == 5);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
    }
    {
        barefootc::vector<MoveOnly, stack_allocator<MoveOnly, 15> > c;
        c.push_back(MoveOnly(0));
        assert(c.size() == 1);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        assert(c.size() == 2);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        assert(c.size() == 3);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        assert(c.size() == 4);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        assert(c.size() == 5);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<MoveOnly, min_allocator<MoveOnly>> c;
        c.push_back(MoveOnly(0));
        assert(c.size() == 1);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        assert(c.size() == 2);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        assert(c.size() == 3);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        assert(c.size() == 4);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        assert(c.size() == 5);
        for (int j = 0; j < c.size(); ++j)
            assert(c[j] == MoveOnly(j));
    }
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}