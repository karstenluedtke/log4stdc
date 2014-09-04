#include <wchar.h>
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

// Call erase(const_iterator first, const_iterator last); with first iterator from another container

#if _LIBCPP_DEBUG >= 1

#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))

#include <string>
#include <cassert>
#include <exception>
#include <cstdlib>

#include "tests/support/min_allocator.h"

int main()
{
    {
    barefootc::basic_string<wchar_t> l1(L"123");
    barefootc::basic_string<wchar_t> l2(L"123");
    barefootc::basic_string<wchar_t>::iterator i = l1.erase(l2.cbegin(), l1.cbegin()+1);
    assert(false);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    S l1(L"123");
    S l2(L"123");
    S::iterator i = l1.erase(l2.cbegin(), l1.cbegin()+1);
    assert(false);
    }
#endif
}

#else

int main()
{
}

#endif
