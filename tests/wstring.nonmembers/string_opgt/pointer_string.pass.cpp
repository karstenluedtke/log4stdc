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

// template<class charT, class traits, class Allocator>
//   bool operator>(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const typename S::value_type* lhs, const S& rhs, bool x)
{
    assert((lhs > rhs) == x);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(L"", S(L""), false);
    test(L"", S(L"abcde"), false);
    test(L"", S(L"abcdefghij"), false);
    test(L"", S(L"abcdefghijklmnopqrst"), false);
    test(L"abcde", S(L""), true);
    test(L"abcde", S(L"abcde"), false);
    test(L"abcde", S(L"abcdefghij"), false);
    test(L"abcde", S(L"abcdefghijklmnopqrst"), false);
    test(L"abcdefghij", S(L""), true);
    test(L"abcdefghij", S(L"abcde"), true);
    test(L"abcdefghij", S(L"abcdefghij"), false);
    test(L"abcdefghij", S(L"abcdefghijklmnopqrst"), false);
    test(L"abcdefghijklmnopqrst", S(L""), true);
    test(L"abcdefghijklmnopqrst", S(L"abcde"), true);
    test(L"abcdefghijklmnopqrst", S(L"abcdefghij"), true);
    test(L"abcdefghijklmnopqrst", S(L"abcdefghijklmnopqrst"), false);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(L"", S(L""), false);
    test(L"", S(L"abcde"), false);
    test(L"", S(L"abcdefghij"), false);
    test(L"", S(L"abcdefghijklmnopqrst"), false);
    test(L"abcde", S(L""), true);
    test(L"abcde", S(L"abcde"), false);
    test(L"abcde", S(L"abcdefghij"), false);
    test(L"abcde", S(L"abcdefghijklmnopqrst"), false);
    test(L"abcdefghij", S(L""), true);
    test(L"abcdefghij", S(L"abcde"), true);
    test(L"abcdefghij", S(L"abcdefghij"), false);
    test(L"abcdefghij", S(L"abcdefghijklmnopqrst"), false);
    test(L"abcdefghijklmnopqrst", S(L""), true);
    test(L"abcdefghijklmnopqrst", S(L"abcde"), true);
    test(L"abcdefghijklmnopqrst", S(L"abcdefghij"), true);
    test(L"abcdefghijklmnopqrst", S(L"abcdefghijklmnopqrst"), false);
    }
#endif
}
