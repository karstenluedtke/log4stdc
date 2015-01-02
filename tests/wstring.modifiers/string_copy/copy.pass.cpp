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

// size_type copy(charT* s, size_type n, size_type pos = 0) const;

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S str, typename S::value_type* s, typename S::size_type n,
     typename S::size_type pos)
{
    try
    {
        const S& cs = str;
        typename S::size_type r = cs.copy(s, n, pos);
        assert(pos <= cs.size());
        typename S::size_type rlen = cs.size() - pos;
	if (rlen > n) {
		rlen = n;
	}
        assert(r == rlen);
        for (r = 0; r < rlen; ++r)
            assert(S::traits_type::eq(cs[pos+r], s[r]));
    }
    catch (std::out_of_range&)
    {
        assert(pos > str.size());
    }
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    wchar_t s[50];
    test(S(L""), s, 0, 0);
    test(S(L""), s, 0, 1);
    test(S(L""), s, 1, 0);
    test(S(L"abcde"), s, 0, 0);
    test(S(L"abcde"), s, 0, 1);
    test(S(L"abcde"), s, 0, 2);
    test(S(L"abcde"), s, 0, 4);
    test(S(L"abcde"), s, 0, 5);
    test(S(L"abcde"), s, 0, 6);
    test(S(L"abcde"), s, 1, 0);
    test(S(L"abcde"), s, 1, 1);
    test(S(L"abcde"), s, 1, 2);
    test(S(L"abcde"), s, 1, 4);
    test(S(L"abcde"), s, 1, 5);
    test(S(L"abcde"), s, 2, 0);
    test(S(L"abcde"), s, 2, 1);
    test(S(L"abcde"), s, 2, 2);
    test(S(L"abcde"), s, 2, 4);
    test(S(L"abcde"), s, 4, 0);
    test(S(L"abcde"), s, 4, 1);
    test(S(L"abcde"), s, 4, 2);
    test(S(L"abcde"), s, 5, 0);
    test(S(L"abcde"), s, 5, 1);
    test(S(L"abcde"), s, 6, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 10);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 19);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 20);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 21);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 9);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 18);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 19);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 20);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 9);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 17);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 18);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 19);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 5);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 9);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 10);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 11);
    test(S(L"abcdefghijklmnopqrst"), s, 19, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 19, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 19, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 20, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 20, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 21, 0);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    char s[50];
    test(S(L""), s, 0, 0);
    test(S(L""), s, 0, 1);
    test(S(L""), s, 1, 0);
    test(S(L"abcde"), s, 0, 0);
    test(S(L"abcde"), s, 0, 1);
    test(S(L"abcde"), s, 0, 2);
    test(S(L"abcde"), s, 0, 4);
    test(S(L"abcde"), s, 0, 5);
    test(S(L"abcde"), s, 0, 6);
    test(S(L"abcde"), s, 1, 0);
    test(S(L"abcde"), s, 1, 1);
    test(S(L"abcde"), s, 1, 2);
    test(S(L"abcde"), s, 1, 4);
    test(S(L"abcde"), s, 1, 5);
    test(S(L"abcde"), s, 2, 0);
    test(S(L"abcde"), s, 2, 1);
    test(S(L"abcde"), s, 2, 2);
    test(S(L"abcde"), s, 2, 4);
    test(S(L"abcde"), s, 4, 0);
    test(S(L"abcde"), s, 4, 1);
    test(S(L"abcde"), s, 4, 2);
    test(S(L"abcde"), s, 5, 0);
    test(S(L"abcde"), s, 5, 1);
    test(S(L"abcde"), s, 6, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 10);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 19);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 20);
    test(S(L"abcdefghijklmnopqrst"), s, 0, 21);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 9);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 18);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 19);
    test(S(L"abcdefghijklmnopqrst"), s, 1, 20);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 9);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 17);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 18);
    test(S(L"abcdefghijklmnopqrst"), s, 2, 19);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 5);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 9);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 10);
    test(S(L"abcdefghijklmnopqrst"), s, 10, 11);
    test(S(L"abcdefghijklmnopqrst"), s, 19, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 19, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 19, 2);
    test(S(L"abcdefghijklmnopqrst"), s, 20, 0);
    test(S(L"abcdefghijklmnopqrst"), s, 20, 1);
    test(S(L"abcdefghijklmnopqrst"), s, 21, 0);
    }
#endif
}
