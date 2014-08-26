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

// void push_back(charT c)

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::value_type c, S expected)
{
    s.push_back(c);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), 'a', S(1, 'a'));
    test(S(L"12345"), 'a', S(L"12345a"));
    test(S(L"12345678901234567890"), 'a', S(L"12345678901234567890a"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), 'a', S(1, 'a'));
    test(S(L"12345"), 'a', S(L"12345a"));
    test(S(L"12345678901234567890"), 'a', S(L"12345678901234567890a"));
    }
#endif
}
