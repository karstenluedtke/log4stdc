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

// Increment iterator past end.

#if _LIBCPP_DEBUG >= 1

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))

#include <string>
#include <cassert>
#include <iterator>
#include <exception>
#include <cstdlib>

#include "tests/support/min_allocator.h"

int main()
{
    {
    typedef barefootc::basic_string<char> C;
    C c(1, '\0');
    C::iterator i = c.begin();
    ++i;
    assert(i == c.end());
    ++i;
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> C;
    C c(1, '\0');
    C::iterator i = c.begin();
    ++i;
    assert(i == c.end());
    ++i;
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
