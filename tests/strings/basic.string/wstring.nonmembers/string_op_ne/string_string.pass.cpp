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
//   bool operator!=(const basic_string<charT,traits,Allocator>& lhs,
//                   const basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& lhs, const S& rhs, bool x)
{
    assert((lhs != rhs) == x);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), S(L""), false);
    test(S(L""), S(L"abcde"), true);
    test(S(L""), S(L"abcdefghij"), true);
    test(S(L""), S(L"abcdefghijklmnopqrst"), true);
    test(S(L"abcde"), S(L""), true);
    test(S(L"abcde"), S(L"abcde"), false);
    test(S(L"abcde"), S(L"abcdefghij"), true);
    test(S(L"abcde"), S(L"abcdefghijklmnopqrst"), true);
    test(S(L"abcdefghij"), S(L""), true);
    test(S(L"abcdefghij"), S(L"abcde"), true);
    test(S(L"abcdefghij"), S(L"abcdefghij"), false);
    test(S(L"abcdefghij"), S(L"abcdefghijklmnopqrst"), true);
    test(S(L"abcdefghijklmnopqrst"), S(L""), true);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcde"), true);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghij"), true);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghijklmnopqrst"), false);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), S(L""), false);
    test(S(L""), S(L"abcde"), true);
    test(S(L""), S(L"abcdefghij"), true);
    test(S(L""), S(L"abcdefghijklmnopqrst"), true);
    test(S(L"abcde"), S(L""), true);
    test(S(L"abcde"), S(L"abcde"), false);
    test(S(L"abcde"), S(L"abcdefghij"), true);
    test(S(L"abcde"), S(L"abcdefghijklmnopqrst"), true);
    test(S(L"abcdefghij"), S(L""), true);
    test(S(L"abcdefghij"), S(L"abcde"), true);
    test(S(L"abcdefghij"), S(L"abcdefghij"), false);
    test(S(L"abcdefghij"), S(L"abcdefghijklmnopqrst"), true);
    test(S(L"abcdefghijklmnopqrst"), S(L""), true);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcde"), true);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghij"), true);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghijklmnopqrst"), false);
    }
#endif
}
