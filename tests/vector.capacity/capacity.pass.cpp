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

// size_type capacity() const;

#include <vector>
#include <cassert>

#include "tests/support/min_allocator.h"

int main()
{
    {
        barefootc::vector<int> v;
        assert(v.capacity() >= 0);
    }
    {
        barefootc::vector<int> v(100);
        assert(v.capacity() >= 100);
        v.push_back(0);
        assert(v.capacity() > 101);
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> v;
        assert(v.capacity() >= 0);
    }
    {
        barefootc::vector<int, min_allocator<int>> v(100);
        assert(v.capacity() >= 100);
        v.push_back(0);
        assert(v.capacity() > 101);
    }
#endif
}
