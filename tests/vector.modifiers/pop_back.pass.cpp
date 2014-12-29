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

// void pop_back();

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <vector>
#include <cassert>
#include "tests/support/stack_allocator.h"
#include "tests/support/min_allocator.h"

#if _LIBCPP_DEBUG >= 1
#include <cstdlib>
#include <exception>

#endif

int main()
{
    {
        barefootc::vector<int> c;
        c.push_back(1);
        assert(c.size() == 1);
        c.pop_back();
        assert(c.size() == 0);
#if _LIBCPP_DEBUG >= 1
        c.pop_back();
        assert(false);
#endif        
    }
#if __cplusplus >= 201103L
    {
        barefootc::vector<int, min_allocator<int>> c;
        c.push_back(1);
        assert(c.size() == 1);
        c.pop_back();
        assert(c.size() == 0);
#if _LIBCPP_DEBUG >= 1
        c.pop_back();
        assert(false);
#endif        
    }
#endif        
}
