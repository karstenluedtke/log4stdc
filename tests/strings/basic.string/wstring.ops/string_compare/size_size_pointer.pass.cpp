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

// int compare(size_type pos, size_type n1, const charT *s) const;

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
     const typename S::value_type* str, int x)
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
    test(S(L""), 0, 0, L"", 0);
    test(S(L""), 0, 0, L"abcde", -5);
    test(S(L""), 0, 0, L"abcdefghij", -10);
    test(S(L""), 0, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L""), 0, 1, L"", 0);
    test(S(L""), 0, 1, L"abcde", -5);
    test(S(L""), 0, 1, L"abcdefghij", -10);
    test(S(L""), 0, 1, L"abcdefghijklmnopqrst", -20);
    test(S(L""), 1, 0, L"", 0);
    test(S(L""), 1, 0, L"abcde", 0);
    test(S(L""), 1, 0, L"abcdefghij", 0);
    test(S(L""), 1, 0, L"abcdefghijklmnopqrst", 0);
    test(S(L"abcde"), 0, 0, L"", 0);
    test(S(L"abcde"), 0, 0, L"abcde", -5);
    test(S(L"abcde"), 0, 0, L"abcdefghij", -10);
    test(S(L"abcde"), 0, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcde"), 0, 1, L"", 1);
    test(S(L"abcde"), 0, 1, L"abcde", -4);
    test(S(L"abcde"), 0, 1, L"abcdefghij", -9);
    test(S(L"abcde"), 0, 1, L"abcdefghijklmnopqrst", -19);
    test(S(L"abcde"), 0, 2, L"", 2);
    test(S(L"abcde"), 0, 2, L"abcde", -3);
    test(S(L"abcde"), 0, 2, L"abcdefghij", -8);
    test(S(L"abcde"), 0, 2, L"abcdefghijklmnopqrst", -18);
    test(S(L"abcde"), 0, 4, L"", 4);
    test(S(L"abcde"), 0, 4, L"abcde", -1);
    test(S(L"abcde"), 0, 4, L"abcdefghij", -6);
    test(S(L"abcde"), 0, 4, L"abcdefghijklmnopqrst", -16);
    test(S(L"abcde"), 0, 5, L"", 5);
    test(S(L"abcde"), 0, 5, L"abcde", 0);
    test(S(L"abcde"), 0, 5, L"abcdefghij", -5);
    test(S(L"abcde"), 0, 5, L"abcdefghijklmnopqrst", -15);
    test(S(L"abcde"), 0, 6, L"", 5);
    test(S(L"abcde"), 0, 6, L"abcde", 0);
    test(S(L"abcde"), 0, 6, L"abcdefghij", -5);
    test(S(L"abcde"), 0, 6, L"abcdefghijklmnopqrst", -15);
    test(S(L"abcde"), 1, 0, L"", 0);
    test(S(L"abcde"), 1, 0, L"abcde", -5);
    test(S(L"abcde"), 1, 0, L"abcdefghij", -10);
    test(S(L"abcde"), 1, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcde"), 1, 1, L"", 1);
    test(S(L"abcde"), 1, 1, L"abcde", 1);
    test(S(L"abcde"), 1, 1, L"abcdefghij", 1);
    test(S(L"abcde"), 1, 1, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcde"), 1, 2, L"", 2);
    test(S(L"abcde"), 1, 2, L"abcde", 1);
    test(S(L"abcde"), 1, 2, L"abcdefghij", 1);
    test(S(L"abcde"), 1, 2, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcde"), 1, 3, L"", 3);
    test(S(L"abcde"), 1, 3, L"abcde", 1);
    test(S(L"abcde"), 1, 3, L"abcdefghij", 1);
    test(S(L"abcde"), 1, 3, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcde"), 1, 4, L"", 4);
    test(S(L"abcde"), 1, 4, L"abcde", 1);
    test(S(L"abcde"), 1, 4, L"abcdefghij", 1);
    test(S(L"abcde"), 1, 4, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcde"), 1, 5, L"", 4);
    test(S(L"abcde"), 1, 5, L"abcde", 1);
    test(S(L"abcde"), 1, 5, L"abcdefghij", 1);
    test(S(L"abcde"), 1, 5, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcde"), 2, 0, L"", 0);
    test(S(L"abcde"), 2, 0, L"abcde", -5);
    test(S(L"abcde"), 2, 0, L"abcdefghij", -10);
    test(S(L"abcde"), 2, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcde"), 2, 1, L"", 1);
    test(S(L"abcde"), 2, 1, L"abcde", 2);
    test(S(L"abcde"), 2, 1, L"abcdefghij", 2);
    test(S(L"abcde"), 2, 1, L"abcdefghijklmnopqrst", 2);
    test(S(L"abcde"), 2, 2, L"", 2);
    test(S(L"abcde"), 2, 2, L"abcde", 2);
    test(S(L"abcde"), 2, 2, L"abcdefghij", 2);
    test(S(L"abcde"), 2, 2, L"abcdefghijklmnopqrst", 2);
    test(S(L"abcde"), 2, 3, L"", 3);
    test(S(L"abcde"), 2, 3, L"abcde", 2);
    test(S(L"abcde"), 2, 3, L"abcdefghij", 2);
    test(S(L"abcde"), 2, 3, L"abcdefghijklmnopqrst", 2);
    test(S(L"abcde"), 2, 4, L"", 3);
    test(S(L"abcde"), 2, 4, L"abcde", 2);
    test(S(L"abcde"), 2, 4, L"abcdefghij", 2);
    test(S(L"abcde"), 2, 4, L"abcdefghijklmnopqrst", 2);
    test(S(L"abcde"), 4, 0, L"", 0);
    test(S(L"abcde"), 4, 0, L"abcde", -5);
    test(S(L"abcde"), 4, 0, L"abcdefghij", -10);
    test(S(L"abcde"), 4, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcde"), 4, 1, L"", 1);
    test(S(L"abcde"), 4, 1, L"abcde", 4);
    test(S(L"abcde"), 4, 1, L"abcdefghij", 4);
    test(S(L"abcde"), 4, 1, L"abcdefghijklmnopqrst", 4);
    test(S(L"abcde"), 4, 2, L"", 1);
    test(S(L"abcde"), 4, 2, L"abcde", 4);
    test(S(L"abcde"), 4, 2, L"abcdefghij", 4);
    test(S(L"abcde"), 4, 2, L"abcdefghijklmnopqrst", 4);
    test(S(L"abcde"), 5, 0, L"", 0);
    test(S(L"abcde"), 5, 0, L"abcde", -5);
    test(S(L"abcde"), 5, 0, L"abcdefghij", -10);
    test(S(L"abcde"), 5, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcde"), 5, 1, L"", 0);
    test(S(L"abcde"), 5, 1, L"abcde", -5);
    test(S(L"abcde"), 5, 1, L"abcdefghij", -10);
    test(S(L"abcde"), 5, 1, L"abcdefghijklmnopqrst", -20);
}

template <class S>
void test1()
{
    test(S(L"abcde"), 6, 0, L"", 0);
    test(S(L"abcde"), 6, 0, L"abcde", 0);
    test(S(L"abcde"), 6, 0, L"abcdefghij", 0);
    test(S(L"abcde"), 6, 0, L"abcdefghijklmnopqrst", 0);
    test(S(L"abcdefghij"), 0, 0, L"", 0);
    test(S(L"abcdefghij"), 0, 0, L"abcde", -5);
    test(S(L"abcdefghij"), 0, 0, L"abcdefghij", -10);
    test(S(L"abcdefghij"), 0, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghij"), 0, 1, L"", 1);
    test(S(L"abcdefghij"), 0, 1, L"abcde", -4);
    test(S(L"abcdefghij"), 0, 1, L"abcdefghij", -9);
    test(S(L"abcdefghij"), 0, 1, L"abcdefghijklmnopqrst", -19);
    test(S(L"abcdefghij"), 0, 5, L"", 5);
    test(S(L"abcdefghij"), 0, 5, L"abcde", 0);
    test(S(L"abcdefghij"), 0, 5, L"abcdefghij", -5);
    test(S(L"abcdefghij"), 0, 5, L"abcdefghijklmnopqrst", -15);
    test(S(L"abcdefghij"), 0, 9, L"", 9);
    test(S(L"abcdefghij"), 0, 9, L"abcde", 4);
    test(S(L"abcdefghij"), 0, 9, L"abcdefghij", -1);
    test(S(L"abcdefghij"), 0, 9, L"abcdefghijklmnopqrst", -11);
    test(S(L"abcdefghij"), 0, 10, L"", 10);
    test(S(L"abcdefghij"), 0, 10, L"abcde", 5);
    test(S(L"abcdefghij"), 0, 10, L"abcdefghij", 0);
    test(S(L"abcdefghij"), 0, 10, L"abcdefghijklmnopqrst", -10);
    test(S(L"abcdefghij"), 0, 11, L"", 10);
    test(S(L"abcdefghij"), 0, 11, L"abcde", 5);
    test(S(L"abcdefghij"), 0, 11, L"abcdefghij", 0);
    test(S(L"abcdefghij"), 0, 11, L"abcdefghijklmnopqrst", -10);
    test(S(L"abcdefghij"), 1, 0, L"", 0);
    test(S(L"abcdefghij"), 1, 0, L"abcde", -5);
    test(S(L"abcdefghij"), 1, 0, L"abcdefghij", -10);
    test(S(L"abcdefghij"), 1, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghij"), 1, 1, L"", 1);
    test(S(L"abcdefghij"), 1, 1, L"abcde", 1);
    test(S(L"abcdefghij"), 1, 1, L"abcdefghij", 1);
    test(S(L"abcdefghij"), 1, 1, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghij"), 1, 4, L"", 4);
    test(S(L"abcdefghij"), 1, 4, L"abcde", 1);
    test(S(L"abcdefghij"), 1, 4, L"abcdefghij", 1);
    test(S(L"abcdefghij"), 1, 4, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghij"), 1, 8, L"", 8);
    test(S(L"abcdefghij"), 1, 8, L"abcde", 1);
    test(S(L"abcdefghij"), 1, 8, L"abcdefghij", 1);
    test(S(L"abcdefghij"), 1, 8, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghij"), 1, 9, L"", 9);
    test(S(L"abcdefghij"), 1, 9, L"abcde", 1);
    test(S(L"abcdefghij"), 1, 9, L"abcdefghij", 1);
    test(S(L"abcdefghij"), 1, 9, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghij"), 1, 10, L"", 9);
    test(S(L"abcdefghij"), 1, 10, L"abcde", 1);
    test(S(L"abcdefghij"), 1, 10, L"abcdefghij", 1);
    test(S(L"abcdefghij"), 1, 10, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghij"), 5, 0, L"", 0);
    test(S(L"abcdefghij"), 5, 0, L"abcde", -5);
    test(S(L"abcdefghij"), 5, 0, L"abcdefghij", -10);
    test(S(L"abcdefghij"), 5, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghij"), 5, 1, L"", 1);
    test(S(L"abcdefghij"), 5, 1, L"abcde", 5);
    test(S(L"abcdefghij"), 5, 1, L"abcdefghij", 5);
    test(S(L"abcdefghij"), 5, 1, L"abcdefghijklmnopqrst", 5);
    test(S(L"abcdefghij"), 5, 2, L"", 2);
    test(S(L"abcdefghij"), 5, 2, L"abcde", 5);
    test(S(L"abcdefghij"), 5, 2, L"abcdefghij", 5);
    test(S(L"abcdefghij"), 5, 2, L"abcdefghijklmnopqrst", 5);
    test(S(L"abcdefghij"), 5, 4, L"", 4);
    test(S(L"abcdefghij"), 5, 4, L"abcde", 5);
    test(S(L"abcdefghij"), 5, 4, L"abcdefghij", 5);
    test(S(L"abcdefghij"), 5, 4, L"abcdefghijklmnopqrst", 5);
    test(S(L"abcdefghij"), 5, 5, L"", 5);
    test(S(L"abcdefghij"), 5, 5, L"abcde", 5);
    test(S(L"abcdefghij"), 5, 5, L"abcdefghij", 5);
    test(S(L"abcdefghij"), 5, 5, L"abcdefghijklmnopqrst", 5);
    test(S(L"abcdefghij"), 5, 6, L"", 5);
    test(S(L"abcdefghij"), 5, 6, L"abcde", 5);
    test(S(L"abcdefghij"), 5, 6, L"abcdefghij", 5);
    test(S(L"abcdefghij"), 5, 6, L"abcdefghijklmnopqrst", 5);
    test(S(L"abcdefghij"), 9, 0, L"", 0);
    test(S(L"abcdefghij"), 9, 0, L"abcde", -5);
    test(S(L"abcdefghij"), 9, 0, L"abcdefghij", -10);
    test(S(L"abcdefghij"), 9, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghij"), 9, 1, L"", 1);
    test(S(L"abcdefghij"), 9, 1, L"abcde", 9);
    test(S(L"abcdefghij"), 9, 1, L"abcdefghij", 9);
    test(S(L"abcdefghij"), 9, 1, L"abcdefghijklmnopqrst", 9);
    test(S(L"abcdefghij"), 9, 2, L"", 1);
    test(S(L"abcdefghij"), 9, 2, L"abcde", 9);
    test(S(L"abcdefghij"), 9, 2, L"abcdefghij", 9);
    test(S(L"abcdefghij"), 9, 2, L"abcdefghijklmnopqrst", 9);
    test(S(L"abcdefghij"), 10, 0, L"", 0);
    test(S(L"abcdefghij"), 10, 0, L"abcde", -5);
    test(S(L"abcdefghij"), 10, 0, L"abcdefghij", -10);
    test(S(L"abcdefghij"), 10, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghij"), 10, 1, L"", 0);
    test(S(L"abcdefghij"), 10, 1, L"abcde", -5);
    test(S(L"abcdefghij"), 10, 1, L"abcdefghij", -10);
    test(S(L"abcdefghij"), 10, 1, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghij"), 11, 0, L"", 0);
    test(S(L"abcdefghij"), 11, 0, L"abcde", 0);
    test(S(L"abcdefghij"), 11, 0, L"abcdefghij", 0);
    test(S(L"abcdefghij"), 11, 0, L"abcdefghijklmnopqrst", 0);
}

template <class S>
void test2()
{
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"abcde", -5);
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"abcdefghij", -10);
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"", 1);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"abcde", -4);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"abcdefghij", -9);
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"abcdefghijklmnopqrst", -19);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"", 10);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"abcde", 5);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"abcdefghij", 0);
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"abcdefghijklmnopqrst", -10);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"", 19);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"abcde", 14);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"abcdefghij", 9);
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"abcdefghijklmnopqrst", -1);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"", 20);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"abcde", 15);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"abcdefghijklmnopqrst", 0);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"", 20);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"abcde", 15);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"abcdefghijklmnopqrst", 0);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"abcde", -5);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"abcdefghij", -10);
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"abcde", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"abcdefghij", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"", 9);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"abcde", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"abcdefghij", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"", 18);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"abcde", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"abcdefghij", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"", 19);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"abcde", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"abcdefghij", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"", 19);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"abcde", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"abcdefghij", 1);
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"abcdefghijklmnopqrst", 1);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"abcde", -5);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"abcdefghij", -10);
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"", 1);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"abcde", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"abcdefghijklmnopqrst", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"", 5);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"abcde", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"abcdefghijklmnopqrst", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"", 9);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"abcde", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"abcdefghijklmnopqrst", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"abcde", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"abcdefghijklmnopqrst", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"abcde", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"abcdefghij", 10);
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"abcdefghijklmnopqrst", 10);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"abcde", -5);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"abcdefghij", -10);
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"", 1);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"abcde", 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"abcdefghij", 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"abcdefghijklmnopqrst", 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"", 1);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"abcde", 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"abcdefghij", 19);
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"abcdefghijklmnopqrst", 19);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"abcde", -5);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"abcdefghij", -10);
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"abcde", -5);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"abcdefghij", -10);
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"abcdefghijklmnopqrst", -20);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"", 0);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"abcde", 0);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"abcdefghij", 0);
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"abcdefghijklmnopqrst", 0);
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
