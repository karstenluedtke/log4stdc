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
//   bool operator>=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& lhs, const typename S::value_type* rhs, bool x)
{
    assert((lhs >= rhs) == x);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), L"", true);
    test(S(L""), L"abcde", false);
    test(S(L""), L"abcdefghij", false);
    test(S(L""), L"abcdefghijklmnopqrst", false);
    test(S(L"abcde"), L"", true);
    test(S(L"abcde"), L"abcde", true);
    test(S(L"abcde"), L"abcdefghij", false);
    test(S(L"abcde"), L"abcdefghijklmnopqrst", false);
    test(S(L"abcdefghij"), L"", true);
    test(S(L"abcdefghij"), L"abcde", true);
    test(S(L"abcdefghij"), L"abcdefghij", true);
    test(S(L"abcdefghij"), L"abcdefghijklmnopqrst", false);
    test(S(L"abcdefghijklmnopqrst"), L"", true);
    test(S(L"abcdefghijklmnopqrst"), L"abcde", true);
    test(S(L"abcdefghijklmnopqrst"), L"abcdefghij", true);
    test(S(L"abcdefghijklmnopqrst"), L"abcdefghijklmnopqrst", true);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), L"", true);
    test(S(L""), L"abcde", false);
    test(S(L""), L"abcdefghij", false);
    test(S(L""), L"abcdefghijklmnopqrst", false);
    test(S(L"abcde"), L"", true);
    test(S(L"abcde"), L"abcde", true);
    test(S(L"abcde"), L"abcdefghij", false);
    test(S(L"abcde"), L"abcdefghijklmnopqrst", false);
    test(S(L"abcdefghij"), L"", true);
    test(S(L"abcdefghij"), L"abcde", true);
    test(S(L"abcdefghij"), L"abcdefghij", true);
    test(S(L"abcdefghij"), L"abcdefghijklmnopqrst", false);
    test(S(L"abcdefghijklmnopqrst"), L"", true);
    test(S(L"abcdefghijklmnopqrst"), L"abcde", true);
    test(S(L"abcdefghijklmnopqrst"), L"abcdefghij", true);
    test(S(L"abcdefghijklmnopqrst"), L"abcdefghijklmnopqrst", true);
    }
#endif
}
