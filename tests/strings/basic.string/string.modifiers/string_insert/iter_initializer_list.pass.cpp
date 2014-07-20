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

// iterator insert(const_iterator p, initializer_list<charT> il);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

int main()
{
#ifndef _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS
    {
        barefootc::basic_string<char> s("123456");
        barefootc::basic_string<char>::iterator i = s.insert(s.begin() + 3, {'a', 'b', 'c'});
        assert(i - s.begin() == 3);
        assert(s == "123abc456");
    }
#if __cplusplus >= 201103L
    {
        typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
        S s("123456");
        S::iterator i = s.insert(s.begin() + 3, {'a', 'b', 'c'});
        assert(i - s.begin() == 3);
        assert(s == "123abc456");
    }
#endif
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::basic_string<char> s;
        barefootc::basic_string<char> s2;
        s.insert(s2.begin(), {'a', 'b', 'c'});
        assert(false);
    }
#endif
#endif  // _LIBCPP_HAS_NO_GENERALIZED_INITIALIZERS
}
