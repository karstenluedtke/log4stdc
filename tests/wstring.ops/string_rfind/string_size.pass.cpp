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

// size_type rfind(const basic_string& str, size_type pos = npos) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const S& str, typename S::size_type pos, typename S::size_type x)
{
    assert(s.rfind(str, pos) == x);
    if (x != S::npos)
        assert(x <= pos && x + str.size() <= s.size());
}

template <class S>
void
test(const S& s, const S& str, typename S::size_type x)
{
    assert(s.rfind(str) == x);
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
    test(S(L""), S(L""), 1, 0);
    test(S(L""), S(L"abcde"), 1, S::npos);
    test(S(L""), S(L"abcdeabcde"), 1, S::npos);
    test(S(L""), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcde"), S(L""), 0, 0);
    test(S(L"abcde"), S(L"abcde"), 0, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 0, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 0, S::npos);
    test(S(L"abcde"), S(L""), 1, 1);
    test(S(L"abcde"), S(L"abcde"), 1, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 1, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcde"), S(L""), 2, 2);
    test(S(L"abcde"), S(L"abcde"), 2, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 2, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 2, S::npos);
    test(S(L"abcde"), S(L""), 4, 4);
    test(S(L"abcde"), S(L"abcde"), 4, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 4, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 4, S::npos);
    test(S(L"abcde"), S(L""), 5, 5);
    test(S(L"abcde"), S(L"abcde"), 5, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 5, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 5, S::npos);
    test(S(L"abcde"), S(L""), 6, 5);
    test(S(L"abcde"), S(L"abcde"), 6, 0);
    test(S(L"abcde"), S(L"abcdeabcde"), 6, S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), 6, S::npos);
    test(S(L"abcdeabcde"), S(L""), 0, 0);
    test(S(L"abcdeabcde"), S(L"abcde"), 0, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 0, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 0, S::npos);
    test(S(L"abcdeabcde"), S(L""), 1, 1);
    test(S(L"abcdeabcde"), S(L"abcde"), 1, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 1, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 1, S::npos);
    test(S(L"abcdeabcde"), S(L""), 5, 5);
    test(S(L"abcdeabcde"), S(L"abcde"), 5, 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 5, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 5, S::npos);
    test(S(L"abcdeabcde"), S(L""), 9, 9);
    test(S(L"abcdeabcde"), S(L"abcde"), 9, 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 9, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 9, S::npos);
    test(S(L"abcdeabcde"), S(L""), 10, 10);
    test(S(L"abcdeabcde"), S(L"abcde"), 10, 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 10, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 10, S::npos);
    test(S(L"abcdeabcde"), S(L""), 11, 10);
    test(S(L"abcdeabcde"), S(L"abcde"), 11, 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 11, 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 11, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 1, 1);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 1, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 1, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 1, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 10, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 19, 19);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 19, 15);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 19, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 19, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 20, 20);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 20, 15);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 20, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 20, 0);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 21, 20);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 21, 15);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 21, 10);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcdeabcdeabcde"), 21, 0);
}

template <class S>
void test1()
{
    test(S(L""), S(L""), 0);
    test(S(L""), S(L"abcde"), S::npos);
    test(S(L""), S(L"abcdeabcde"), S::npos);
    test(S(L""), S(L"abcdeabcdeabcdeabcde"), S::npos);
    test(S(L"abcde"), S(L""), 5);
    test(S(L"abcde"), S(L"abcde"), 0);
    test(S(L"abcde"), S(L"abcdeabcde"), S::npos);
    test(S(L"abcde"), S(L"abcdeabcdeabcdeabcde"), S::npos);
    test(S(L"abcdeabcde"), S(L""), 10);
    test(S(L"abcdeabcde"), S(L"abcde"), 5);
    test(S(L"abcdeabcde"), S(L"abcdeabcde"), 0);
    test(S(L"abcdeabcde"), S(L"abcdeabcdeabcdeabcde"), S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), S(L""), 20);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcde"), 15);
    test(S(L"abcdeabcdeabcdeabcde"), S(L"abcdeabcde"), 10);
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
