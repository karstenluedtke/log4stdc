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

// void swap(vector& x);

#include <vector>
#include <cassert>

#include "tests/support/min_allocator.h"

int main()
{
    {
        barefootc::vector<int> v1(100);
        barefootc::vector<int> v2(200);
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() == 200);
        assert(v2.size() == 100);
        assert(v2.capacity() == 100);
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> v1(100);
        barefootc::vector<int, min_allocator<int>> v2(200);
        v1.swap(v2);
        assert(v1.size() == 200);
        assert(v1.capacity() == 200);
        assert(v2.size() == 100);
        assert(v2.capacity() == 100);
    }
#endif
}
