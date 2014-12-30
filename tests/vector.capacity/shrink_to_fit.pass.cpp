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

// void shrink_to_fit();

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

int main()
{
    {
        barefootc::vector<int> v(100);
        v.push_back(1);
        v.shrink_to_fit();
        assert(v.capacity() >= 101);
        assert(v.size() == 101);
    }
    {
        barefootc::vector<int, stack_allocator<int, 401> > v(100);
        v.push_back(1);
        v.shrink_to_fit();
        assert(v.capacity() >= 101);
        assert(v.size() == 101);
    }
#ifndef _LIBCPP_NO_EXCEPTIONS
    {
        barefootc::vector<int, stack_allocator<int, 400> > v(100);
        v.push_back(1);
        v.shrink_to_fit();
        assert(v.capacity() >= 200);
        assert(v.size() == 101);
    }
#endif
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        v.push_back(1);
        v.shrink_to_fit();
        assert(v.capacity() >= 101);
        assert(v.size() == 101);
    }
#endif
}
