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

// int compare(size_type pos1, size_type n1, const basic_string& str) const;

#include <string>
#include <stdexcept>
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
test(const S& s, typename S::size_type pos1, typename S::size_type n1,
     const S& str, int x)
{
    try
    {
        assert(sign(s.compare(pos1, n1, str)) == sign(x));
        assert(pos1 <= s.size());
    }
    catch (std::out_of_range&)
    {
        assert(pos1 > s.size());
    }
}

template <class S>
void test0()
{
    test(S(L""), 0, 0, S(L""), 0);
    test(S(L""), 0, 0, S(L"abcde"), -5);
    test(S(L""), 0, 0, S(L"abcdefghij"), -10);
    test(S(L""), 0, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L""), 0, 1, S(L""), 0);
    test(S(L""), 0, 1, S(L"abcde"), -5);
    test(S(L""), 0, 1, S(L"abcdefghij"), -10);
    test(S(L""), 0, 1, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L""), 1, 0, S(L""), 0);
    test(S(L""), 1, 0, S(L"abcde"), 0);
    test(S(L""), 1, 0, S(L"abcdefghij"), 0);
    test(S(L""), 1, 0, S(L"abcdefghijklmnopqrst"), 0);
    test(S(L"abcde"), 0, 0, S(L""), 0);
    test(S(L"abcde"), 0, 0, S(L"abcde"), -5);
    test(S(L"abcde"), 0, 0, S(L"abcdefghij"), -10);
    test(S(L"abcde"), 0, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), 0, 1, S(L""), 1);
    test(S(L"abcde"), 0, 1, S(L"abcde"), -4);
    test(S(L"abcde"), 0, 1, S(L"abcdefghij"), -9);
    test(S(L"abcde"), 0, 1, S(L"abcdefghijklmnopqrst"), -19);
    test(S(L"abcde"), 0, 2, S(L""), 2);
    test(S(L"abcde"), 0, 2, S(L"abcde"), -3);
    test(S(L"abcde"), 0, 2, S(L"abcdefghij"), -8);
    test(S(L"abcde"), 0, 2, S(L"abcdefghijklmnopqrst"), -18);
    test(S(L"abcde"), 0, 4, S(L""), 4);
    test(S(L"abcde"), 0, 4, S(L"abcde"), -1);
    test(S(L"abcde"), 0, 4, S(L"abcdefghij"), -6);
    test(S(L"abcde"), 0, 4, S(L"abcdefghijklmnopqrst"), -16);
    test(S(L"abcde"), 0, 5, S(L""), 5);
    test(S(L"abcde"), 0, 5, S(L"abcde"), 0);
    test(S(L"abcde"), 0, 5, S(L"abcdefghij"), -5);
    test(S(L"abcde"), 0, 5, S(L"abcdefghijklmnopqrst"), -15);
    test(S(L"abcde"), 0, 6, S(L""), 5);
    test(S(L"abcde"), 0, 6, S(L"abcde"), 0);
    test(S(L"abcde"), 0, 6, S(L"abcdefghij"), -5);
    test(S(L"abcde"), 0, 6, S(L"abcdefghijklmnopqrst"), -15);
    test(S(L"abcde"), 1, 0, S(L""), 0);
    test(S(L"abcde"), 1, 0, S(L"abcde"), -5);
    test(S(L"abcde"), 1, 0, S(L"abcdefghij"), -10);
    test(S(L"abcde"), 1, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), 1, 1, S(L""), 1);
    test(S(L"abcde"), 1, 1, S(L"abcde"), 1);
    test(S(L"abcde"), 1, 1, S(L"abcdefghij"), 1);
    test(S(L"abcde"), 1, 1, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcde"), 1, 2, S(L""), 2);
    test(S(L"abcde"), 1, 2, S(L"abcde"), 1);
    test(S(L"abcde"), 1, 2, S(L"abcdefghij"), 1);
    test(S(L"abcde"), 1, 2, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcde"), 1, 3, S(L""), 3);
    test(S(L"abcde"), 1, 3, S(L"abcde"), 1);
    test(S(L"abcde"), 1, 3, S(L"abcdefghij"), 1);
    test(S(L"abcde"), 1, 3, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcde"), 1, 4, S(L""), 4);
    test(S(L"abcde"), 1, 4, S(L"abcde"), 1);
    test(S(L"abcde"), 1, 4, S(L"abcdefghij"), 1);
    test(S(L"abcde"), 1, 4, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcde"), 1, 5, S(L""), 4);
    test(S(L"abcde"), 1, 5, S(L"abcde"), 1);
    test(S(L"abcde"), 1, 5, S(L"abcdefghij"), 1);
    test(S(L"abcde"), 1, 5, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcde"), 2, 0, S(L""), 0);
    test(S(L"abcde"), 2, 0, S(L"abcde"), -5);
    test(S(L"abcde"), 2, 0, S(L"abcdefghij"), -10);
    test(S(L"abcde"), 2, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), 2, 1, S(L""), 1);
    test(S(L"abcde"), 2, 1, S(L"abcde"), 2);
    test(S(L"abcde"), 2, 1, S(L"abcdefghij"), 2);
    test(S(L"abcde"), 2, 1, S(L"abcdefghijklmnopqrst"), 2);
    test(S(L"abcde"), 2, 2, S(L""), 2);
    test(S(L"abcde"), 2, 2, S(L"abcde"), 2);
    test(S(L"abcde"), 2, 2, S(L"abcdefghij"), 2);
    test(S(L"abcde"), 2, 2, S(L"abcdefghijklmnopqrst"), 2);
    test(S(L"abcde"), 2, 3, S(L""), 3);
    test(S(L"abcde"), 2, 3, S(L"abcde"), 2);
    test(S(L"abcde"), 2, 3, S(L"abcdefghij"), 2);
    test(S(L"abcde"), 2, 3, S(L"abcdefghijklmnopqrst"), 2);
    test(S(L"abcde"), 2, 4, S(L""), 3);
    test(S(L"abcde"), 2, 4, S(L"abcde"), 2);
    test(S(L"abcde"), 2, 4, S(L"abcdefghij"), 2);
    test(S(L"abcde"), 2, 4, S(L"abcdefghijklmnopqrst"), 2);
    test(S(L"abcde"), 4, 0, S(L""), 0);
    test(S(L"abcde"), 4, 0, S(L"abcde"), -5);
    test(S(L"abcde"), 4, 0, S(L"abcdefghij"), -10);
    test(S(L"abcde"), 4, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), 4, 1, S(L""), 1);
    test(S(L"abcde"), 4, 1, S(L"abcde"), 4);
    test(S(L"abcde"), 4, 1, S(L"abcdefghij"), 4);
    test(S(L"abcde"), 4, 1, S(L"abcdefghijklmnopqrst"), 4);
    test(S(L"abcde"), 4, 2, S(L""), 1);
    test(S(L"abcde"), 4, 2, S(L"abcde"), 4);
    test(S(L"abcde"), 4, 2, S(L"abcdefghij"), 4);
    test(S(L"abcde"), 4, 2, S(L"abcdefghijklmnopqrst"), 4);
    test(S(L"abcde"), 5, 0, S(L""), 0);
    test(S(L"abcde"), 5, 0, S(L"abcde"), -5);
    test(S(L"abcde"), 5, 0, S(L"abcdefghij"), -10);
    test(S(L"abcde"), 5, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcde"), 5, 1, S(L""), 0);
    test(S(L"abcde"), 5, 1, S(L"abcde"), -5);
    test(S(L"abcde"), 5, 1, S(L"abcdefghij"), -10);
    test(S(L"abcde"), 5, 1, S(L"abcdefghijklmnopqrst"), -20);
}

template <class S>
void test1()
{
    test(S(L"abcde"), 6, 0, S(L""), 0);
    test(S(L"abcde"), 6, 0, S(L"abcde"), 0);
    test(S(L"abcde"), 6, 0, S(L"abcdefghij"), 0);
    test(S(L"abcde"), 6, 0, S(L"abcdefghijklmnopqrst"), 0);
    test(S(L"abcdefghij"), 0, 0, S(L""), 0);
    test(S(L"abcdefghij"), 0, 0, S(L"abcde"), -5);
    test(S(L"abcdefghij"), 0, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghij"), 0, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghij"), 0, 1, S(L""), 1);
    test(S(L"abcdefghij"), 0, 1, S(L"abcde"), -4);
    test(S(L"abcdefghij"), 0, 1, S(L"abcdefghij"), -9);
    test(S(L"abcdefghij"), 0, 1, S(L"abcdefghijklmnopqrst"), -19);
    test(S(L"abcdefghij"), 0, 5, S(L""), 5);
    test(S(L"abcdefghij"), 0, 5, S(L"abcde"), 0);
    test(S(L"abcdefghij"), 0, 5, S(L"abcdefghij"), -5);
    test(S(L"abcdefghij"), 0, 5, S(L"abcdefghijklmnopqrst"), -15);
    test(S(L"abcdefghij"), 0, 9, S(L""), 9);
    test(S(L"abcdefghij"), 0, 9, S(L"abcde"), 4);
    test(S(L"abcdefghij"), 0, 9, S(L"abcdefghij"), -1);
    test(S(L"abcdefghij"), 0, 9, S(L"abcdefghijklmnopqrst"), -11);
    test(S(L"abcdefghij"), 0, 10, S(L""), 10);
    test(S(L"abcdefghij"), 0, 10, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 0, 10, S(L"abcdefghij"), 0);
    test(S(L"abcdefghij"), 0, 10, S(L"abcdefghijklmnopqrst"), -10);
    test(S(L"abcdefghij"), 0, 11, S(L""), 10);
    test(S(L"abcdefghij"), 0, 11, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 0, 11, S(L"abcdefghij"), 0);
    test(S(L"abcdefghij"), 0, 11, S(L"abcdefghijklmnopqrst"), -10);
    test(S(L"abcdefghij"), 1, 0, S(L""), 0);
    test(S(L"abcdefghij"), 1, 0, S(L"abcde"), -5);
    test(S(L"abcdefghij"), 1, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghij"), 1, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghij"), 1, 1, S(L""), 1);
    test(S(L"abcdefghij"), 1, 1, S(L"abcde"), 1);
    test(S(L"abcdefghij"), 1, 1, S(L"abcdefghij"), 1);
    test(S(L"abcdefghij"), 1, 1, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghij"), 1, 4, S(L""), 4);
    test(S(L"abcdefghij"), 1, 4, S(L"abcde"), 1);
    test(S(L"abcdefghij"), 1, 4, S(L"abcdefghij"), 1);
    test(S(L"abcdefghij"), 1, 4, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghij"), 1, 8, S(L""), 8);
    test(S(L"abcdefghij"), 1, 8, S(L"abcde"), 1);
    test(S(L"abcdefghij"), 1, 8, S(L"abcdefghij"), 1);
    test(S(L"abcdefghij"), 1, 8, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghij"), 1, 9, S(L""), 9);
    test(S(L"abcdefghij"), 1, 9, S(L"abcde"), 1);
    test(S(L"abcdefghij"), 1, 9, S(L"abcdefghij"), 1);
    test(S(L"abcdefghij"), 1, 9, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghij"), 1, 10, S(L""), 9);
    test(S(L"abcdefghij"), 1, 10, S(L"abcde"), 1);
    test(S(L"abcdefghij"), 1, 10, S(L"abcdefghij"), 1);
    test(S(L"abcdefghij"), 1, 10, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghij"), 5, 0, S(L""), 0);
    test(S(L"abcdefghij"), 5, 0, S(L"abcde"), -5);
    test(S(L"abcdefghij"), 5, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghij"), 5, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghij"), 5, 1, S(L""), 1);
    test(S(L"abcdefghij"), 5, 1, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 5, 1, S(L"abcdefghij"), 5);
    test(S(L"abcdefghij"), 5, 1, S(L"abcdefghijklmnopqrst"), 5);
    test(S(L"abcdefghij"), 5, 2, S(L""), 2);
    test(S(L"abcdefghij"), 5, 2, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 5, 2, S(L"abcdefghij"), 5);
    test(S(L"abcdefghij"), 5, 2, S(L"abcdefghijklmnopqrst"), 5);
    test(S(L"abcdefghij"), 5, 4, S(L""), 4);
    test(S(L"abcdefghij"), 5, 4, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 5, 4, S(L"abcdefghij"), 5);
    test(S(L"abcdefghij"), 5, 4, S(L"abcdefghijklmnopqrst"), 5);
    test(S(L"abcdefghij"), 5, 5, S(L""), 5);
    test(S(L"abcdefghij"), 5, 5, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 5, 5, S(L"abcdefghij"), 5);
    test(S(L"abcdefghij"), 5, 5, S(L"abcdefghijklmnopqrst"), 5);
    test(S(L"abcdefghij"), 5, 6, S(L""), 5);
    test(S(L"abcdefghij"), 5, 6, S(L"abcde"), 5);
    test(S(L"abcdefghij"), 5, 6, S(L"abcdefghij"), 5);
    test(S(L"abcdefghij"), 5, 6, S(L"abcdefghijklmnopqrst"), 5);
    test(S(L"abcdefghij"), 9, 0, S(L""), 0);
    test(S(L"abcdefghij"), 9, 0, S(L"abcde"), -5);
    test(S(L"abcdefghij"), 9, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghij"), 9, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghij"), 9, 1, S(L""), 1);
    test(S(L"abcdefghij"), 9, 1, S(L"abcde"), 9);
    test(S(L"abcdefghij"), 9, 1, S(L"abcdefghij"), 9);
    test(S(L"abcdefghij"), 9, 1, S(L"abcdefghijklmnopqrst"), 9);
    test(S(L"abcdefghij"), 9, 2, S(L""), 1);
    test(S(L"abcdefghij"), 9, 2, S(L"abcde"), 9);
    test(S(L"abcdefghij"), 9, 2, S(L"abcdefghij"), 9);
    test(S(L"abcdefghij"), 9, 2, S(L"abcdefghijklmnopqrst"), 9);
    test(S(L"abcdefghij"), 10, 0, S(L""), 0);
    test(S(L"abcdefghij"), 10, 0, S(L"abcde"), -5);
    test(S(L"abcdefghij"), 10, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghij"), 10, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghij"), 10, 1, S(L""), 0);
    test(S(L"abcdefghij"), 10, 1, S(L"abcde"), -5);
    test(S(L"abcdefghij"), 10, 1, S(L"abcdefghij"), -10);
    test(S(L"abcdefghij"), 10, 1, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghij"), 11, 0, S(L""), 0);
    test(S(L"abcdefghij"), 11, 0, S(L"abcde"), 0);
    test(S(L"abcdefghij"), 11, 0, S(L"abcdefghij"), 0);
    test(S(L"abcdefghij"), 11, 0, S(L"abcdefghijklmnopqrst"), 0);
}

template <class S>
void test2()
{
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcde"), -5);
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L""), 1);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"abcde"), -4);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"abcdefghij"), -9);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"abcdefghijklmnopqrst"), -19);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L""), 10);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"abcde"), 5);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"abcdefghij"), 0);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"abcdefghijklmnopqrst"), -10);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L""), 19);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"abcde"), 14);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"abcdefghij"), 9);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"abcdefghijklmnopqrst"), -1);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L""), 20);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L"abcde"), 15);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L"abcdefghijklmnopqrst"), 0);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, S(L""), 20);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, S(L"abcde"), 15);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, S(L"abcdefghijklmnopqrst"), 0);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcde"), -5);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L""), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"abcde"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"abcdefghij"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L""), 9);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"abcde"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"abcdefghij"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L""), 18);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"abcde"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"abcdefghij"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L""), 19);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"abcde"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"abcdefghij"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, S(L""), 19);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, S(L"abcde"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, S(L"abcdefghij"), 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, S(L"abcdefghijklmnopqrst"), 1);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcde"), -5);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L""), 1);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcde"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcdefghijklmnopqrst"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L""), 5);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcde"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcdefghijklmnopqrst"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L""), 9);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcde"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcdefghijklmnopqrst"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L""), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcde"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcdefghijklmnopqrst"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, S(L""), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, S(L"abcde"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, S(L"abcdefghij"), 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, S(L"abcdefghijklmnopqrst"), 10);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcde"), -5);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L""), 1);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcde"), 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcdefghij"), 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcdefghijklmnopqrst"), 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, S(L""), 1);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, S(L"abcde"), 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, S(L"abcdefghij"), 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, S(L"abcdefghijklmnopqrst"), 19);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcde"), -5);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcdefghij"), -10);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, S(L"abcde"), -5);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, S(L"abcdefghij"), -10);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, S(L"abcdefghijklmnopqrst"), -20);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, S(L""), 0);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, S(L"abcde"), 0);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, S(L"abcdefghij"), 0);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, S(L"abcdefghijklmnopqrst"), 0);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0<S>();
    test1<S>();
    test2<S>();
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0<S>();
    test1<S>();
    test2<S>();
    }
#endif
}
