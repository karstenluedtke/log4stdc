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

// size_type find(const basic_string& str, size_type pos = 0) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const S& str, typename S::size_type pos, typename S::size_type x)
{
    assert(s.find(str, pos) == x);
    if (x != S::npos)
        assert(pos <= x && x + str.size() <= s.size());
}

template <class S>
void
test(const S& s, const S& str, typename S::size_type x)
{
    assert(s.find(str) == x);
    if (x != S::npos)
        assert(0 <= x && x + str.size() <= s.size());
}

template <class S>
void test0()
{
    test(S(L""), S(L""), 0, 0);
    test(S(L""), S(L"abcde"), 0, S::npos);
    test(S(L""), S(L"abcdeabcde"), 0, S::npos);
    test(S(L""), S(L"abcdeabcdeabcdeabcde"), 0, S::npos);
    test(S(L""), S(L""), 1, S::npos);
    test(S(L""), S(L"abcde"), 1, S::npos);
    test(S(L""), S(L"abcdeabcde"), 1, S::npos);
    test(S(L""), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcde"), S(L""), 0, 0);
    test(S(L"abcde"), S(L"abcde"), 0, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 0, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 0, S::npos);
    test(S(L"abcde"), S(L""), 1, 1);
    test(S(L"abcde"), S(L"abcde"), 1, S::npos);
    test(S(L"abcde"), S(L"abcdeabcde"), 1, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcde"), S(L""), 2, 2);
    test(S(L"abcde"), S(L"abcde"), 2, S::npos);
    test(S(L"abcde"), S(L"abcdeabcde"), 2, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 2, S::npos);
    test(S(L"abcde"), S(L""), 4, 4);
    test(S(L"abcde"), S(L"abcde"), 4, S::npos);
    test(S(L"abcde"), S(L"abcdeabcde"), 4, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 4, S::npos);
    test(S(L"abcde"), S(L""), 5, 5);
    test(S(L"abcde"), S(L"abcde"), 5, S::npos);
    test(S(L"abcde"), S(L"abcdeabcde"), 5, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 5, S::npos);
    test(S(L"abcde"), S(L""), 6, S::npos);
    test(S(L"abcde"), S(L"abcde"), 6, S::npos);
    test(S(L"abcde"), S(L"abcdeabcde"), 6, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 6, S::npos);
    test(S(L"abcdeabcde"), S(L""), 0, 0);
    test(S(L"abcdeabcde"), S(L"abcde"), 0, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 0, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 0, S::npos);
    test(S(L"abcdeabcde"), S(L""), 1, 1);
    test(S(L"abcdeabcde"), S(L"abcde"), 1, 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 1, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcdeabcde"), S(L""), 5, 5);
    test(S(L"abcdeabcde"), S(L"abcde"), 5, 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 5, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 5, S::npos);
    test(S(L"abcdeabcde"), S(L""), 9, 9);
    test(S(L"abcdeabcde"), S(L"abcde"), 9, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 9, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 9, S::npos);
    test(S(L"abcdeabcde"), S(L""), 10, 10);
    test(S(L"abcdeabcde"), S(L"abcde"), 10, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 10, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 10, S::npos);
    test(S(L"abcdeabcde"), S(L""), 11, S::npos);
    test(S(L"abcdeabcde"), S(L"abcde"), 11, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 11, S::npos);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 11, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 1, 1);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 1, 5);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 1, 5);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 10, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 19, 19);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 19, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 19, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 19, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 20, 20);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 20, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 20, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 20, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 21, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 21, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 21, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 21, S::npos);
}

template <class S>
void test1()
{
    test(S(L""), S(L""), 0);
    test(S(L""), S(L"abcde"), S::npos);
    test(S(L""), S(L"abcdeabcde"), S::npos);
    test(S(L""), S(L"abcdeabcdeabcdeabcde"), S::npos);
    test(S(L"abcde"), S(L""), 0);
    test(S(L"abcde"), S(L"abcde"), 0);
    test(S(L"abcde"), S(L"abcdeabcde"), S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), S::npos);
    test(S(L"abcdeabcde"), S(L""), 0);
    test(S(L"abcdeabcde"), S(L"abcde"), 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 0);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0<S>();
    test1<S>();
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0<S>();
    test1<S>();
    }
#endif
}
