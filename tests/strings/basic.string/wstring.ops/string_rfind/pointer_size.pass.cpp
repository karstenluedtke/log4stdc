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

// size_type rfind(const charT* s, size_type pos = npos) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.rfind(str, pos) == x);
    if (x != S::npos)
    {
        typename S::size_type n = S::traits_type::length(str);
        assert(x <= pos && x + n <= s.size());
    }
}

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type x)
{
    assert(s.rfind(str) == x);
    if (x != S::npos)
    {
        typename S::size_type pos = s.size();
        typename S::size_type n = S::traits_type::length(str);
        assert(x <= pos && x + n <= s.size());
    }
}

template <class S>
void test0()
{
    test(S(L""), L"", 0, 0);
    test(S(L""), L"abcde", 0, S::npos);
    test(S(L""), L"abcdeabcde", 0, S::npos);
    test(S(L""), L"abcdeabcdeabcdeabcde", 0, S::npos);
    test(S(L""), L"", 1, 0);
    test(S(L""), L"abcde", 1, S::npos);
    test(S(L""), L"abcdeabcde", 1, S::npos);
    test(S(L""), L"abcdeabcdeabcdeabcde", 1, S::npos);
    test(S(L"abcde"), L"", 0, 0);
    test(S(L"abcde"), L"abcde", 0, 0);
    test(S(L"abcde"), L"abcdeabcde", 0, S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", 0, S::npos);
    test(S(L"abcde"), L"", 1, 1);
    test(S(L"abcde"), L"abcde", 1, 0);
    test(S(L"abcde"), L"abcdeabcde", 1, S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", 1, S::npos);
    test(S(L"abcde"), L"", 2, 2);
    test(S(L"abcde"), L"abcde", 2, 0);
    test(S(L"abcde"), L"abcdeabcde", 2, S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", 2, S::npos);
    test(S(L"abcde"), L"", 4, 4);
    test(S(L"abcde"), L"abcde", 4, 0);
    test(S(L"abcde"), L"abcdeabcde", 4, S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", 4, S::npos);
    test(S(L"abcde"), L"", 5, 5);
    test(S(L"abcde"), L"abcde", 5, 0);
    test(S(L"abcde"), L"abcdeabcde", 5, S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", 5, S::npos);
    test(S(L"abcde"), L"", 6, 5);
    test(S(L"abcde"), L"abcde", 6, 0);
    test(S(L"abcde"), L"abcdeabcde", 6, S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", 6, S::npos);
    test(S(L"abcdeabcde"), L"", 0, 0);
    test(S(L"abcdeabcde"), L"abcde", 0, 0);
    test(S(L"abcdeabcde"), L"abcdeabcde", 0, 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", 0, S::npos);
    test(S(L"abcdeabcde"), L"", 1, 1);
    test(S(L"abcdeabcde"), L"abcde", 1, 0);
    test(S(L"abcdeabcde"), L"abcdeabcde", 1, 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", 1, S::npos);
    test(S(L"abcdeabcde"), L"", 5, 5);
    test(S(L"abcdeabcde"), L"abcde", 5, 5);
    test(S(L"abcdeabcde"), L"abcdeabcde", 5, 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", 5, S::npos);
    test(S(L"abcdeabcde"), L"", 9, 9);
    test(S(L"abcdeabcde"), L"abcde", 9, 5);
    test(S(L"abcdeabcde"), L"abcdeabcde", 9, 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", 9, S::npos);
    test(S(L"abcdeabcde"), L"", 10, 10);
    test(S(L"abcdeabcde"), L"abcde", 10, 5);
    test(S(L"abcdeabcde"), L"abcdeabcde", 10, 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", 10, S::npos);
    test(S(L"abcdeabcde"), L"", 11, 10);
    test(S(L"abcdeabcde"), L"abcde", 11, 5);
    test(S(L"abcdeabcde"), L"abcdeabcde", 11, 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", 11, S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 0, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 1, 1);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 1, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 1, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 1, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 10, 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 10, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 19, 19);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 19, 15);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 19, 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 19, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 20, 20);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 20, 15);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 20, 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 20, 0);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 21, 20);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 21, 15);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 21, 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 21, 0);
}

template <class S>
void test1()
{
    test(S(L""), L"", 0);
    test(S(L""), L"abcde", S::npos);
    test(S(L""), L"abcdeabcde", S::npos);
    test(S(L""), L"abcdeabcdeabcdeabcde", S::npos);
    test(S(L"abcde"), L"", 5);
    test(S(L"abcde"), L"abcde", 0);
    test(S(L"abcde"), L"abcdeabcde", S::npos);
    test(S(L"abcde"), L"abcdeabcdeabcdeabcde", S::npos);
    test(S(L"abcdeabcde"), L"", 10);
    test(S(L"abcdeabcde"), L"abcde", 5);
    test(S(L"abcdeabcde"), L"abcdeabcde", 0);
    test(S(L"abcdeabcde"), L"abcdeabcdeabcdeabcde", S::npos);
    test(S(L"abcdeabcdeabcdeabcde"), L"", 20);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcde", 15);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcde", 10);
    test(S(L"abcdeabcdeabcdeabcde"), L"abcdeabcdeabcdeabcde", 0);
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
