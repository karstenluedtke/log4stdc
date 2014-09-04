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

// void swap(basic_string& s);

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s1, S s2)
{
    S s1_ = s1;
    S s2_ = s2;
    s1.swap(s2);
    assert(s1.__invariants());
    assert(s2.__invariants());
    assert(s1 == s2_);
    assert(s2 == s1_);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), S(L""));
    test(S(L""), S(L"12345"));
    test(S(L""), S(L"1234567890"));
    test(S(L""), S(L"12345678901234567890"));
    test(S(L"abcde"), S(L""));
    test(S(L"abcde"), S(L"12345"));
    test(S(L"abcde"), S(L"1234567890"));
    test(S(L"abcde"), S(L"12345678901234567890"));
    test(S(L"abcdefghij"), S(L""));
    test(S(L"abcdefghij"), S(L"12345"));
    test(S(L"abcdefghij"), S(L"1234567890"));
    test(S(L"abcdefghij"), S(L"12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), S(L""));
    test(S(L"abcdefghijklmnopqrst"), S(L"12345"));
    test(S(L"abcdefghijklmnopqrst"), S(L"1234567890"));
    test(S(L"abcdefghijklmnopqrst"), S(L"12345678901234567890"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), S(L""));
    test(S(L""), S(L"12345"));
    test(S(L""), S(L"1234567890"));
    test(S(L""), S(L"12345678901234567890"));
    test(S(L"abcde"), S(L""));
    test(S(L"abcde"), S(L"12345"));
    test(S(L"abcde"), S(L"1234567890"));
    test(S(L"abcde"), S(L"12345678901234567890"));
    test(S(L"abcdefghij"), S(L""));
    test(S(L"abcdefghij"), S(L"12345"));
    test(S(L"abcdefghij"), S(L"1234567890"));
    test(S(L"abcdefghij"), S(L"12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), S(L""));
    test(S(L"abcdefghijklmnopqrst"), S(L"12345"));
    test(S(L"abcdefghijklmnopqrst"), S(L"1234567890"));
    test(S(L"abcdefghijklmnopqrst"), S(L"12345678901234567890"));
    }
#endif
}
