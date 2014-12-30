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

// void resize(size_type sz);

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
// #include "../../../MoveOnly.h"
#include "tests/support/min_allocator.h"

int main()
{
#if 0 // ndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        barefootc::vector<MoveOnly> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
    }
    {
        barefootc::vector<MoveOnly, stack_allocator<MoveOnly, 300> > v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() == 100);
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
    }
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
        barefootc::vector<int> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() >= 100);
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
    }
    {
        barefootc::vector<int, stack_allocator<int, 300> > v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() >= 100);
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
    }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#if __cplusplus >= 201103L
    {
        barefootc::vector<MoveOnly, min_allocator<MoveOnly>> v(100);
        v.resize(50);
        assert(v.size() == 50);
        assert(v.capacity() >= 100);
        v.resize(200);
        assert(v.size() == 200);
        assert(v.capacity() >= 200);
    }
#endif
}
