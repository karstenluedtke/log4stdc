#include "tests/strings/cxxwrapper.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// Call erase(const_iterator position) with end()

#if _LIBCPP_DEBUG >= 1

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))

#include <string>
#include <cassert>
#include <cstdlib>
#include <exception>

#include "tests/support/min_allocator.h"

int main()
{
    {
    barefootc::basic_string<char> l1("123");
    barefootc::basic_string<char>::const_iterator i = l1.end();
    l1.erase(i);
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    S l1("123");
    S::const_iterator i = l1.end();
    l1.erase(i);
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
