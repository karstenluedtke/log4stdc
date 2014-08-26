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

// int compare(const basic_string& str) const

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

int sign(int x)
{
    if (x == 0)
        return 0;
    if (x < 0)
        return -1;
    return 1;
}

template <class S>
void
test(const S& s, const S& str, int x)
{
    assert(sign(s.compare(str)) == sign(x));
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), S(L""), 0);
    test(S(L""), S(L"abcde"), -5);
    test(S(L""), S(L"abcdefghij"), -10);
    test(S(L""), S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), S(L""), 5);
    test(S(L"abcde"), S(L"abcde"), 0);
    test(S(L"abcde"), S(L"abcdefghij"), -5);
    test(S(L"abcde"), S(L"abcdefghijklmnopqrst"), -15);
    test(S(L"abcdefghij"), S(L""), 10);
    test(S(L"abcdefghij"), S(L"abcde"), 5);
    test(S(L"abcdefghij"), S(L"abcdefghij"), 0);
    test(S(L"abcdefghij"), S(L"abcdefghijklmnopqrst"), -10);
    test(S(L"abcdefghijklmnopqrst"), S(L""), 20);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcde"), 15);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghijklmnopqrst"), 0);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), S(L""), 0);
    test(S(L""), S(L"abcde"), -5);
    test(S(L""), S(L"abcdefghij"), -10);
    test(S(L""), S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), S(L""), 5);
    test(S(L"abcde"), S(L"abcde"), 0);
    test(S(L"abcde"), S(L"abcdefghij"), -5);
    test(S(L"abcde"), S(L"abcdefghijklmnopqrst"), -15);
    test(S(L"abcdefghij"), S(L""), 10);
    test(S(L"abcdefghij"), S(L"abcde"), 5);
    test(S(L"abcdefghij"), S(L"abcdefghij"), 0);
    test(S(L"abcdefghij"), S(L"abcdefghijklmnopqrst"), -10);
    test(S(L"abcdefghijklmnopqrst"), S(L""), 20);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcde"), 15);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), S(L"abcdefghijklmnopqrst"), 0);
    }
#endif
}
