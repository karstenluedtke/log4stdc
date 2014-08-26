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

// basic_string<charT,traits,Allocator>&
//   replace(const_iterator i1, const_iterator i2, size_type n, charT c);

#include <stdio.h>

#include <string>
#include <algorithm>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type pos1, typename S::size_type n1, typename S::size_type n2,
     typename S::value_type c, S expected)
{
    typename S::size_type old_size = s.size();
    typename S::const_iterator first = s.begin() + pos1;
    typename S::const_iterator last = s.begin() + pos1 + n1;
    typename S::size_type xlen = last - first;
    s.replace(first, last, n2, c);
    assert(s.__invariants());
    assert(s == expected);
    typename S::size_type rlen = n2;
    assert(s.size() == old_size - xlen + rlen);
}

template <class S>
void test0()
{
    test(S(L""), 0, 0, 0, '3', S(L""));
    test(S(L""), 0, 0, 5, '3', S(L"33333"));
    test(S(L""), 0, 0, 10, '3', S(L"3333333333"));
    test(S(L""), 0, 0, 20, '3', S(L"33333333333333333333"));
    test(S(L"abcde"), 0, 0, 0, '3', S(L"abcde"));
    test(S(L"abcde"), 0, 0, 5, '3', S(L"33333abcde"));
    test(S(L"abcde"), 0, 0, 10, '3', S(L"3333333333abcde"));
    test(S(L"abcde"), 0, 0, 20, '3', S(L"33333333333333333333abcde"));
    test(S(L"abcde"), 0, 1, 0, '3', S(L"bcde"));
    test(S(L"abcde"), 0, 1, 5, '3', S(L"33333bcde"));
    test(S(L"abcde"), 0, 1, 10, '3', S(L"3333333333bcde"));
    test(S(L"abcde"), 0, 1, 20, '3', S(L"33333333333333333333bcde"));
    test(S(L"abcde"), 0, 2, 0, '3', S(L"cde"));
    test(S(L"abcde"), 0, 2, 5, '3', S(L"33333cde"));
    test(S(L"abcde"), 0, 2, 10, '3', S(L"3333333333cde"));
    test(S(L"abcde"), 0, 2, 20, '3', S(L"33333333333333333333cde"));
    test(S(L"abcde"), 0, 4, 0, '3', S(L"e"));
    test(S(L"abcde"), 0, 4, 5, '3', S(L"33333e"));
    test(S(L"abcde"), 0, 4, 10, '3', S(L"3333333333e"));
    test(S(L"abcde"), 0, 4, 20, '3', S(L"33333333333333333333e"));
    test(S(L"abcde"), 0, 5, 0, '3', S(L""));
    test(S(L"abcde"), 0, 5, 5, '3', S(L"33333"));
    test(S(L"abcde"), 0, 5, 10, '3', S(L"3333333333"));
    test(S(L"abcde"), 0, 5, 20, '3', S(L"33333333333333333333"));
    test(S(L"abcde"), 1, 0, 0, '3', S(L"abcde"));
    test(S(L"abcde"), 1, 0, 5, '3', S(L"a33333bcde"));
    test(S(L"abcde"), 1, 0, 10, '3', S(L"a3333333333bcde"));
    test(S(L"abcde"), 1, 0, 20, '3', S(L"a33333333333333333333bcde"));
    test(S(L"abcde"), 1, 1, 0, '3', S(L"acde"));
    test(S(L"abcde"), 1, 1, 5, '3', S(L"a33333cde"));
    test(S(L"abcde"), 1, 1, 10, '3', S(L"a3333333333cde"));
    test(S(L"abcde"), 1, 1, 20, '3', S(L"a33333333333333333333cde"));
    test(S(L"abcde"), 1, 2, 0, '3', S(L"ade"));
    test(S(L"abcde"), 1, 2, 5, '3', S(L"a33333de"));
    test(S(L"abcde"), 1, 2, 10, '3', S(L"a3333333333de"));
    test(S(L"abcde"), 1, 2, 20, '3', S(L"a33333333333333333333de"));
    test(S(L"abcde"), 1, 3, 0, '3', S(L"ae"));
    test(S(L"abcde"), 1, 3, 5, '3', S(L"a33333e"));
    test(S(L"abcde"), 1, 3, 10, '3', S(L"a3333333333e"));
    test(S(L"abcde"), 1, 3, 20, '3', S(L"a33333333333333333333e"));
    test(S(L"abcde"), 1, 4, 0, '3', S(L"a"));
    test(S(L"abcde"), 1, 4, 5, '3', S(L"a33333"));
    test(S(L"abcde"), 1, 4, 10, '3', S(L"a3333333333"));
    test(S(L"abcde"), 1, 4, 20, '3', S(L"a33333333333333333333"));
    test(S(L"abcde"), 2, 0, 0, '3', S(L"abcde"));
    test(S(L"abcde"), 2, 0, 5, '3', S(L"ab33333cde"));
    test(S(L"abcde"), 2, 0, 10, '3', S(L"ab3333333333cde"));
    test(S(L"abcde"), 2, 0, 20, '3', S(L"ab33333333333333333333cde"));
    test(S(L"abcde"), 2, 1, 0, '3', S(L"abde"));
    test(S(L"abcde"), 2, 1, 5, '3', S(L"ab33333de"));
    test(S(L"abcde"), 2, 1, 10, '3', S(L"ab3333333333de"));
    test(S(L"abcde"), 2, 1, 20, '3', S(L"ab33333333333333333333de"));
    test(S(L"abcde"), 2, 2, 0, '3', S(L"abe"));
    test(S(L"abcde"), 2, 2, 5, '3', S(L"ab33333e"));
    test(S(L"abcde"), 2, 2, 10, '3', S(L"ab3333333333e"));
    test(S(L"abcde"), 2, 2, 20, '3', S(L"ab33333333333333333333e"));
    test(S(L"abcde"), 2, 3, 0, '3', S(L"ab"));
    test(S(L"abcde"), 2, 3, 5, '3', S(L"ab33333"));
    test(S(L"abcde"), 2, 3, 10, '3', S(L"ab3333333333"));
    test(S(L"abcde"), 2, 3, 20, '3', S(L"ab33333333333333333333"));
    test(S(L"abcde"), 4, 0, 0, '3', S(L"abcde"));
    test(S(L"abcde"), 4, 0, 5, '3', S(L"abcd33333e"));
    test(S(L"abcde"), 4, 0, 10, '3', S(L"abcd3333333333e"));
    test(S(L"abcde"), 4, 0, 20, '3', S(L"abcd33333333333333333333e"));
    test(S(L"abcde"), 4, 1, 0, '3', S(L"abcd"));
    test(S(L"abcde"), 4, 1, 5, '3', S(L"abcd33333"));
    test(S(L"abcde"), 4, 1, 10, '3', S(L"abcd3333333333"));
    test(S(L"abcde"), 4, 1, 20, '3', S(L"abcd33333333333333333333"));
    test(S(L"abcde"), 5, 0, 0, '3', S(L"abcde"));
    test(S(L"abcde"), 5, 0, 5, '3', S(L"abcde33333"));
    test(S(L"abcde"), 5, 0, 10, '3', S(L"abcde3333333333"));
    test(S(L"abcde"), 5, 0, 20, '3', S(L"abcde33333333333333333333"));
    test(S(L"abcdefghij"), 0, 0, 0, '3', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, 5, '3', S(L"33333abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, 10, '3', S(L"3333333333abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, 20, '3', S(L"33333333333333333333abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 0, '3', S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 5, '3', S(L"33333bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 10, '3', S(L"3333333333bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 20, '3', S(L"33333333333333333333bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, 0, '3', S(L"fghij"));
    test(S(L"abcdefghij"), 0, 5, 5, '3', S(L"33333fghij"));
    test(S(L"abcdefghij"), 0, 5, 10, '3', S(L"3333333333fghij"));
    test(S(L"abcdefghij"), 0, 5, 20, '3', S(L"33333333333333333333fghij"));
    test(S(L"abcdefghij"), 0, 9, 0, '3', S(L"j"));
    test(S(L"abcdefghij"), 0, 9, 5, '3', S(L"33333j"));
    test(S(L"abcdefghij"), 0, 9, 10, '3', S(L"3333333333j"));
    test(S(L"abcdefghij"), 0, 9, 20, '3', S(L"33333333333333333333j"));
    test(S(L"abcdefghij"), 0, 10, 0, '3', S(L""));
    test(S(L"abcdefghij"), 0, 10, 5, '3', S(L"33333"));
    test(S(L"abcdefghij"), 0, 10, 10, '3', S(L"3333333333"));
    test(S(L"abcdefghij"), 0, 10, 20, '3', S(L"33333333333333333333"));
    test(S(L"abcdefghij"), 1, 0, 0, '3', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 0, 5, '3', S(L"a33333bcdefghij"));
    test(S(L"abcdefghij"), 1, 0, 10, '3', S(L"a3333333333bcdefghij"));
    test(S(L"abcdefghij"), 1, 0, 20, '3', S(L"a33333333333333333333bcdefghij"));
    test(S(L"abcdefghij"), 1, 1, 0, '3', S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 1, 5, '3', S(L"a33333cdefghij"));
    test(S(L"abcdefghij"), 1, 1, 10, '3', S(L"a3333333333cdefghij"));
    test(S(L"abcdefghij"), 1, 1, 20, '3', S(L"a33333333333333333333cdefghij"));
}

template <class S>
void test1()
{
    test(S(L"abcdefghij"), 1, 4, 0, '3', S(L"afghij"));
    test(S(L"abcdefghij"), 1, 4, 5, '3', S(L"a33333fghij"));
    test(S(L"abcdefghij"), 1, 4, 10, '3', S(L"a3333333333fghij"));
    test(S(L"abcdefghij"), 1, 4, 20, '3', S(L"a33333333333333333333fghij"));
    test(S(L"abcdefghij"), 1, 8, 0, '3', S(L"aj"));
    test(S(L"abcdefghij"), 1, 8, 5, '3', S(L"a33333j"));
    test(S(L"abcdefghij"), 1, 8, 10, '3', S(L"a3333333333j"));
    test(S(L"abcdefghij"), 1, 8, 20, '3', S(L"a33333333333333333333j"));
    test(S(L"abcdefghij"), 1, 9, 0, '3', S(L"a"));
    test(S(L"abcdefghij"), 1, 9, 5, '3', S(L"a33333"));
    test(S(L"abcdefghij"), 1, 9, 10, '3', S(L"a3333333333"));
    test(S(L"abcdefghij"), 1, 9, 20, '3', S(L"a33333333333333333333"));
    test(S(L"abcdefghij"), 5, 0, 0, '3', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 0, 5, '3', S(L"abcde33333fghij"));
    test(S(L"abcdefghij"), 5, 0, 10, '3', S(L"abcde3333333333fghij"));
    test(S(L"abcdefghij"), 5, 0, 20, '3', S(L"abcde33333333333333333333fghij"));
    test(S(L"abcdefghij"), 5, 1, 0, '3', S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 1, 5, '3', S(L"abcde33333ghij"));
    test(S(L"abcdefghij"), 5, 1, 10, '3', S(L"abcde3333333333ghij"));
    test(S(L"abcdefghij"), 5, 1, 20, '3', S(L"abcde33333333333333333333ghij"));
    test(S(L"abcdefghij"), 5, 2, 0, '3', S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 2, 5, '3', S(L"abcde33333hij"));
    test(S(L"abcdefghij"), 5, 2, 10, '3', S(L"abcde3333333333hij"));
    test(S(L"abcdefghij"), 5, 2, 20, '3', S(L"abcde33333333333333333333hij"));
    test(S(L"abcdefghij"), 5, 4, 0, '3', S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 4, 5, '3', S(L"abcde33333j"));
    test(S(L"abcdefghij"), 5, 4, 10, '3', S(L"abcde3333333333j"));
    test(S(L"abcdefghij"), 5, 4, 20, '3', S(L"abcde33333333333333333333j"));
    test(S(L"abcdefghij"), 5, 5, 0, '3', S(L"abcde"));
    test(S(L"abcdefghij"), 5, 5, 5, '3', S(L"abcde33333"));
    test(S(L"abcdefghij"), 5, 5, 10, '3', S(L"abcde3333333333"));
    test(S(L"abcdefghij"), 5, 5, 20, '3', S(L"abcde33333333333333333333"));
    test(S(L"abcdefghij"), 9, 0, 0, '3', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 0, 5, '3', S(L"abcdefghi33333j"));
    test(S(L"abcdefghij"), 9, 0, 10, '3', S(L"abcdefghi3333333333j"));
    test(S(L"abcdefghij"), 9, 0, 20, '3', S(L"abcdefghi33333333333333333333j"));
    test(S(L"abcdefghij"), 9, 1, 0, '3', S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 1, 5, '3', S(L"abcdefghi33333"));
    test(S(L"abcdefghij"), 9, 1, 10, '3', S(L"abcdefghi3333333333"));
    test(S(L"abcdefghij"), 9, 1, 20, '3', S(L"abcdefghi33333333333333333333"));
    test(S(L"abcdefghij"), 10, 0, 0, '3', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 0, 5, '3', S(L"abcdefghij33333"));
    test(S(L"abcdefghij"), 10, 0, 10, '3', S(L"abcdefghij3333333333"));
    test(S(L"abcdefghij"), 10, 0, 20, '3', S(L"abcdefghij33333333333333333333"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 0, '3', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 5, '3', S(L"33333abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 10, '3', S(L"3333333333abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 20, '3', S(L"33333333333333333333abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 0, '3', S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 5, '3', S(L"33333bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 10, '3', S(L"3333333333bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 20, '3', S(L"33333333333333333333bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 0, '3', S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 5, '3', S(L"33333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 10, '3', S(L"3333333333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 20, '3', S(L"33333333333333333333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 0, '3', S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 5, '3', S(L"33333t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 10, '3', S(L"3333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 20, '3', S(L"33333333333333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 0, '3', S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 5, '3', S(L"33333"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 10, '3', S(L"3333333333"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 20, '3', S(L"33333333333333333333"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 0, '3', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 5, '3', S(L"a33333bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 10, '3', S(L"a3333333333bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 20, '3', S(L"a33333333333333333333bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 0, '3', S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 5, '3', S(L"a33333cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 10, '3', S(L"a3333333333cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 20, '3', S(L"a33333333333333333333cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 0, '3', S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 5, '3', S(L"a33333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 10, '3', S(L"a3333333333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 20, '3', S(L"a33333333333333333333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 0, '3', S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 5, '3', S(L"a33333t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 10, '3', S(L"a3333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 20, '3', S(L"a33333333333333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 0, '3', S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 5, '3', S(L"a33333"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 10, '3', S(L"a3333333333"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 20, '3', S(L"a33333333333333333333"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 0, '3', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 5, '3', S(L"abcdefghij33333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 10, '3', S(L"abcdefghij3333333333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 20, '3', S(L"abcdefghij33333333333333333333klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 0, '3', S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 5, '3', S(L"abcdefghij33333lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 10, '3', S(L"abcdefghij3333333333lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 20, '3', S(L"abcdefghij33333333333333333333lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 0, '3', S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 5, '3', S(L"abcdefghij33333pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 10, '3', S(L"abcdefghij3333333333pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 20, '3', S(L"abcdefghij33333333333333333333pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 0, '3', S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 5, '3', S(L"abcdefghij33333t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 10, '3', S(L"abcdefghij3333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 20, '3', S(L"abcdefghij33333333333333333333t"));
}

template <class S>
void test2()
{
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 0, '3', S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 5, '3', S(L"abcdefghij33333"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 10, '3', S(L"abcdefghij3333333333"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 20, '3', S(L"abcdefghij33333333333333333333"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 0, '3', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 5, '3', S(L"abcdefghijklmnopqrs33333t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 10, '3', S(L"abcdefghijklmnopqrs3333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 20, '3', S(L"abcdefghijklmnopqrs33333333333333333333t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 0, '3', S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 5, '3', S(L"abcdefghijklmnopqrs33333"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 10, '3', S(L"abcdefghijklmnopqrs3333333333"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 20, '3', S(L"abcdefghijklmnopqrs33333333333333333333"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 0, '3', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 5, '3', S(L"abcdefghijklmnopqrst33333"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 10, '3', S(L"abcdefghijklmnopqrst3333333333"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 20, '3', S(L"abcdefghijklmnopqrst33333333333333333333"));
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
