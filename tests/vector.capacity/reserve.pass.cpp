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

// void reserve(size_type n);

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

int main()
{
    {
        barefootc::vector<int> v;
        v.reserve(10);
        assert(v.capacity() >= 10);
    }
    {
        barefootc::vector<int> v(100);
        assert(v.capacity() >= 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() >= 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() >= 150);
    }
    {
        barefootc::vector<int, stack_allocator<int, 250> > v(100);
        assert(v.capacity() >= 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() >= 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() >= 150);
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> v;
        v.reserve(10);
        assert(v.capacity() >= 10);
    }
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        assert(v.capacity() >= 100);
        v.reserve(50);
        assert(v.size() == 100);
        assert(v.capacity() >= 100);
        v.reserve(150);
        assert(v.size() == 100);
        assert(v.capacity() >= 150);
    }
#endif
}
