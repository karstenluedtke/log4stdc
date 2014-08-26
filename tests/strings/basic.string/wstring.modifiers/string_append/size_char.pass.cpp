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

// basic_string<charT,traits,Allocator>&
//   append(size_type n, charT c);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type n, typename S::value_type c, S expected)
{
    s.append(n, c);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), 0, 'a', S());
    test(S(), 1, 'a', S(1, 'a'));
    test(S(), 10, 'a', S(10, 'a'));
    test(S(), 100, 'a', S(100, 'a'));

    test(S(L"12345"), 0, 'a', S(L"12345"));
    test(S(L"12345"), 1, 'a', S(L"12345a"));
    test(S(L"12345"), 10, 'a', S(L"12345aaaaaaaaaa"));

    test(S(L"12345678901234567890"), 0, 'a', S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), 1, 'a', S(L"12345678901234567890a"));
    test(S(L"12345678901234567890"), 10, 'a', S(L"12345678901234567890aaaaaaaaaa"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), 0, 'a', S());
    test(S(), 1, 'a', S(1, 'a'));
    test(S(), 10, 'a', S(10, 'a'));
    test(S(), 100, 'a', S(100, 'a'));

    test(S(L"12345"), 0, 'a', S(L"12345"));
    test(S(L"12345"), 1, 'a', S(L"12345a"));
    test(S(L"12345"), 10, 'a', S(L"12345aaaaaaaaaa"));

    test(S(L"12345678901234567890"), 0, 'a', S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), 1, 'a', S(L"12345678901234567890a"));
    test(S(L"12345678901234567890"), 10, 'a', S(L"12345678901234567890aaaaaaaaaa"));
    }
#endif
}