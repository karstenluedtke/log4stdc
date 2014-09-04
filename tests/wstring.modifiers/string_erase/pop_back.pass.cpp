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

// void pop_back();

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, S expected)
{
    s.pop_back();
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L"abcde"), S(L"abcd"));
    test(S(L"abcdefghij"), S(L"abcdefghi"));
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghijklmnopqrs"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L"abcde"), S(L"abcd"));
    test(S(L"abcdefghij"), S(L"abcdefghi"));
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghijklmnopqrs"));
    }
#endif
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::basic_string<wchar_t> s;
        s.pop_back();
        assert(false);
    }
#endif        
}
