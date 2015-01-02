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
//   replace(size_type pos, size_type n1, size_type n2, charT c);

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type pos, typename S::size_type n1,
     typename S::size_type n2, typename S::value_type c,
     S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.replace(pos, n1, n2, c);
        assert(s.__invariants());
        assert(pos <= old_size);
        assert(s == expected);
        typename S::size_type xlen = old_size - pos;
	if (xlen > n1) {
		xlen = n1;
	}
        typename S::size_type rlen = n2;
        assert(s.size() == old_size - xlen + rlen);
    }
    catch (std::out_of_range&)
    {
        assert(pos > old_size);
        assert(s == s0);
    }
}

template <class S>
void test0()
{
    test(S(L""), 0, 0, 0, '2', S(L""));
    test(S(L""), 0, 0, 5, '2', S(L"22222"));
    test(S(L""), 0, 0, 10, '2', S(L"2222222222"));
    test(S(L""), 0, 0, 20, '2', S(L"22222222222222222222"));
    test(S(L""), 0, 1, 0, '2', S(L""));
    test(S(L""), 0, 1, 5, '2', S(L"22222"));
    test(S(L""), 0, 1, 10, '2', S(L"2222222222"));
    test(S(L""), 0, 1, 20, '2', S(L"22222222222222222222"));
    test(S(L""), 1, 0, 0, '2', S(L"can't happen"));
    test(S(L""), 1, 0, 5, '2', S(L"can't happen"));
    test(S(L""), 1, 0, 10, '2', S(L"can't happen"));
    test(S(L""), 1, 0, 20, '2', S(L"can't happen"));
    test(S(L"abcde"), 0, 0, 0, '2', S(L"abcde"));
    test(S(L"abcde"), 0, 0, 5, '2', S(L"22222abcde"));
    test(S(L"abcde"), 0, 0, 10, '2', S(L"2222222222abcde"));
    test(S(L"abcde"), 0, 0, 20, '2', S(L"22222222222222222222abcde"));
    test(S(L"abcde"), 0, 1, 0, '2', S(L"bcde"));
    test(S(L"abcde"), 0, 1, 5, '2', S(L"22222bcde"));
    test(S(L"abcde"), 0, 1, 10, '2', S(L"2222222222bcde"));
    test(S(L"abcde"), 0, 1, 20, '2', S(L"22222222222222222222bcde"));
    test(S(L"abcde"), 0, 2, 0, '2', S(L"cde"));
    test(S(L"abcde"), 0, 2, 5, '2', S(L"22222cde"));
    test(S(L"abcde"), 0, 2, 10, '2', S(L"2222222222cde"));
    test(S(L"abcde"), 0, 2, 20, '2', S(L"22222222222222222222cde"));
    test(S(L"abcde"), 0, 4, 0, '2', S(L"e"));
    test(S(L"abcde"), 0, 4, 5, '2', S(L"22222e"));
    test(S(L"abcde"), 0, 4, 10, '2', S(L"2222222222e"));
    test(S(L"abcde"), 0, 4, 20, '2', S(L"22222222222222222222e"));
    test(S(L"abcde"), 0, 5, 0, '2', S(L""));
    test(S(L"abcde"), 0, 5, 5, '2', S(L"22222"));
    test(S(L"abcde"), 0, 5, 10, '2', S(L"2222222222"));
    test(S(L"abcde"), 0, 5, 20, '2', S(L"22222222222222222222"));
    test(S(L"abcde"), 0, 6, 0, '2', S(L""));
    test(S(L"abcde"), 0, 6, 5, '2', S(L"22222"));
    test(S(L"abcde"), 0, 6, 10, '2', S(L"2222222222"));
    test(S(L"abcde"), 0, 6, 20, '2', S(L"22222222222222222222"));
    test(S(L"abcde"), 1, 0, 0, '2', S(L"abcde"));
    test(S(L"abcde"), 1, 0, 5, '2', S(L"a22222bcde"));
    test(S(L"abcde"), 1, 0, 10, '2', S(L"a2222222222bcde"));
    test(S(L"abcde"), 1, 0, 20, '2', S(L"a22222222222222222222bcde"));
    test(S(L"abcde"), 1, 1, 0, '2', S(L"acde"));
    test(S(L"abcde"), 1, 1, 5, '2', S(L"a22222cde"));
    test(S(L"abcde"), 1, 1, 10, '2', S(L"a2222222222cde"));
    test(S(L"abcde"), 1, 1, 20, '2', S(L"a22222222222222222222cde"));
    test(S(L"abcde"), 1, 2, 0, '2', S(L"ade"));
    test(S(L"abcde"), 1, 2, 5, '2', S(L"a22222de"));
    test(S(L"abcde"), 1, 2, 10, '2', S(L"a2222222222de"));
    test(S(L"abcde"), 1, 2, 20, '2', S(L"a22222222222222222222de"));
    test(S(L"abcde"), 1, 3, 0, '2', S(L"ae"));
    test(S(L"abcde"), 1, 3, 5, '2', S(L"a22222e"));
    test(S(L"abcde"), 1, 3, 10, '2', S(L"a2222222222e"));
    test(S(L"abcde"), 1, 3, 20, '2', S(L"a22222222222222222222e"));
    test(S(L"abcde"), 1, 4, 0, '2', S(L"a"));
    test(S(L"abcde"), 1, 4, 5, '2', S(L"a22222"));
    test(S(L"abcde"), 1, 4, 10, '2', S(L"a2222222222"));
    test(S(L"abcde"), 1, 4, 20, '2', S(L"a22222222222222222222"));
    test(S(L"abcde"), 1, 5, 0, '2', S(L"a"));
    test(S(L"abcde"), 1, 5, 5, '2', S(L"a22222"));
    test(S(L"abcde"), 1, 5, 10, '2', S(L"a2222222222"));
    test(S(L"abcde"), 1, 5, 20, '2', S(L"a22222222222222222222"));
    test(S(L"abcde"), 2, 0, 0, '2', S(L"abcde"));
    test(S(L"abcde"), 2, 0, 5, '2', S(L"ab22222cde"));
    test(S(L"abcde"), 2, 0, 10, '2', S(L"ab2222222222cde"));
    test(S(L"abcde"), 2, 0, 20, '2', S(L"ab22222222222222222222cde"));
    test(S(L"abcde"), 2, 1, 0, '2', S(L"abde"));
    test(S(L"abcde"), 2, 1, 5, '2', S(L"ab22222de"));
    test(S(L"abcde"), 2, 1, 10, '2', S(L"ab2222222222de"));
    test(S(L"abcde"), 2, 1, 20, '2', S(L"ab22222222222222222222de"));
    test(S(L"abcde"), 2, 2, 0, '2', S(L"abe"));
    test(S(L"abcde"), 2, 2, 5, '2', S(L"ab22222e"));
    test(S(L"abcde"), 2, 2, 10, '2', S(L"ab2222222222e"));
    test(S(L"abcde"), 2, 2, 20, '2', S(L"ab22222222222222222222e"));
    test(S(L"abcde"), 2, 3, 0, '2', S(L"ab"));
    test(S(L"abcde"), 2, 3, 5, '2', S(L"ab22222"));
    test(S(L"abcde"), 2, 3, 10, '2', S(L"ab2222222222"));
    test(S(L"abcde"), 2, 3, 20, '2', S(L"ab22222222222222222222"));
    test(S(L"abcde"), 2, 4, 0, '2', S(L"ab"));
    test(S(L"abcde"), 2, 4, 5, '2', S(L"ab22222"));
    test(S(L"abcde"), 2, 4, 10, '2', S(L"ab2222222222"));
    test(S(L"abcde"), 2, 4, 20, '2', S(L"ab22222222222222222222"));
    test(S(L"abcde"), 4, 0, 0, '2', S(L"abcde"));
    test(S(L"abcde"), 4, 0, 5, '2', S(L"abcd22222e"));
    test(S(L"abcde"), 4, 0, 10, '2', S(L"abcd2222222222e"));
    test(S(L"abcde"), 4, 0, 20, '2', S(L"abcd22222222222222222222e"));
    test(S(L"abcde"), 4, 1, 0, '2', S(L"abcd"));
    test(S(L"abcde"), 4, 1, 5, '2', S(L"abcd22222"));
    test(S(L"abcde"), 4, 1, 10, '2', S(L"abcd2222222222"));
    test(S(L"abcde"), 4, 1, 20, '2', S(L"abcd22222222222222222222"));
    test(S(L"abcde"), 4, 2, 0, '2', S(L"abcd"));
    test(S(L"abcde"), 4, 2, 5, '2', S(L"abcd22222"));
    test(S(L"abcde"), 4, 2, 10, '2', S(L"abcd2222222222"));
    test(S(L"abcde"), 4, 2, 20, '2', S(L"abcd22222222222222222222"));
    test(S(L"abcde"), 5, 0, 0, '2', S(L"abcde"));
    test(S(L"abcde"), 5, 0, 5, '2', S(L"abcde22222"));
    test(S(L"abcde"), 5, 0, 10, '2', S(L"abcde2222222222"));
    test(S(L"abcde"), 5, 0, 20, '2', S(L"abcde22222222222222222222"));
    test(S(L"abcde"), 5, 1, 0, '2', S(L"abcde"));
    test(S(L"abcde"), 5, 1, 5, '2', S(L"abcde22222"));
    test(S(L"abcde"), 5, 1, 10, '2', S(L"abcde2222222222"));
    test(S(L"abcde"), 5, 1, 20, '2', S(L"abcde22222222222222222222"));
}

template <class S>
void test1()
{
    test(S(L"abcde"), 6, 0, 0, '2', S(L"can't happen"));
    test(S(L"abcde"), 6, 0, 5, '2', S(L"can't happen"));
    test(S(L"abcde"), 6, 0, 10, '2', S(L"can't happen"));
    test(S(L"abcde"), 6, 0, 20, '2', S(L"can't happen"));
    test(S(L"abcdefghij"), 0, 0, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, 5, '2', S(L"22222abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, 10, '2', S(L"2222222222abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, 20, '2', S(L"22222222222222222222abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 0, '2', S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 5, '2', S(L"22222bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 10, '2', S(L"2222222222bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, 20, '2', S(L"22222222222222222222bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, 0, '2', S(L"fghij"));
    test(S(L"abcdefghij"), 0, 5, 5, '2', S(L"22222fghij"));
    test(S(L"abcdefghij"), 0, 5, 10, '2', S(L"2222222222fghij"));
    test(S(L"abcdefghij"), 0, 5, 20, '2', S(L"22222222222222222222fghij"));
    test(S(L"abcdefghij"), 0, 9, 0, '2', S(L"j"));
    test(S(L"abcdefghij"), 0, 9, 5, '2', S(L"22222j"));
    test(S(L"abcdefghij"), 0, 9, 10, '2', S(L"2222222222j"));
    test(S(L"abcdefghij"), 0, 9, 20, '2', S(L"22222222222222222222j"));
    test(S(L"abcdefghij"), 0, 10, 0, '2', S(L""));
    test(S(L"abcdefghij"), 0, 10, 5, '2', S(L"22222"));
    test(S(L"abcdefghij"), 0, 10, 10, '2', S(L"2222222222"));
    test(S(L"abcdefghij"), 0, 10, 20, '2', S(L"22222222222222222222"));
    test(S(L"abcdefghij"), 0, 11, 0, '2', S(L""));
    test(S(L"abcdefghij"), 0, 11, 5, '2', S(L"22222"));
    test(S(L"abcdefghij"), 0, 11, 10, '2', S(L"2222222222"));
    test(S(L"abcdefghij"), 0, 11, 20, '2', S(L"22222222222222222222"));
    test(S(L"abcdefghij"), 1, 0, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 0, 5, '2', S(L"a22222bcdefghij"));
    test(S(L"abcdefghij"), 1, 0, 10, '2', S(L"a2222222222bcdefghij"));
    test(S(L"abcdefghij"), 1, 0, 20, '2', S(L"a22222222222222222222bcdefghij"));
    test(S(L"abcdefghij"), 1, 1, 0, '2', S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 1, 5, '2', S(L"a22222cdefghij"));
    test(S(L"abcdefghij"), 1, 1, 10, '2', S(L"a2222222222cdefghij"));
    test(S(L"abcdefghij"), 1, 1, 20, '2', S(L"a22222222222222222222cdefghij"));
    test(S(L"abcdefghij"), 1, 4, 0, '2', S(L"afghij"));
    test(S(L"abcdefghij"), 1, 4, 5, '2', S(L"a22222fghij"));
    test(S(L"abcdefghij"), 1, 4, 10, '2', S(L"a2222222222fghij"));
    test(S(L"abcdefghij"), 1, 4, 20, '2', S(L"a22222222222222222222fghij"));
    test(S(L"abcdefghij"), 1, 8, 0, '2', S(L"aj"));
    test(S(L"abcdefghij"), 1, 8, 5, '2', S(L"a22222j"));
    test(S(L"abcdefghij"), 1, 8, 10, '2', S(L"a2222222222j"));
    test(S(L"abcdefghij"), 1, 8, 20, '2', S(L"a22222222222222222222j"));
    test(S(L"abcdefghij"), 1, 9, 0, '2', S(L"a"));
    test(S(L"abcdefghij"), 1, 9, 5, '2', S(L"a22222"));
    test(S(L"abcdefghij"), 1, 9, 10, '2', S(L"a2222222222"));
    test(S(L"abcdefghij"), 1, 9, 20, '2', S(L"a22222222222222222222"));
    test(S(L"abcdefghij"), 1, 10, 0, '2', S(L"a"));
    test(S(L"abcdefghij"), 1, 10, 5, '2', S(L"a22222"));
    test(S(L"abcdefghij"), 1, 10, 10, '2', S(L"a2222222222"));
    test(S(L"abcdefghij"), 1, 10, 20, '2', S(L"a22222222222222222222"));
    test(S(L"abcdefghij"), 5, 0, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 0, 5, '2', S(L"abcde22222fghij"));
    test(S(L"abcdefghij"), 5, 0, 10, '2', S(L"abcde2222222222fghij"));
    test(S(L"abcdefghij"), 5, 0, 20, '2', S(L"abcde22222222222222222222fghij"));
    test(S(L"abcdefghij"), 5, 1, 0, '2', S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 1, 5, '2', S(L"abcde22222ghij"));
    test(S(L"abcdefghij"), 5, 1, 10, '2', S(L"abcde2222222222ghij"));
    test(S(L"abcdefghij"), 5, 1, 20, '2', S(L"abcde22222222222222222222ghij"));
    test(S(L"abcdefghij"), 5, 2, 0, '2', S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 2, 5, '2', S(L"abcde22222hij"));
    test(S(L"abcdefghij"), 5, 2, 10, '2', S(L"abcde2222222222hij"));
    test(S(L"abcdefghij"), 5, 2, 20, '2', S(L"abcde22222222222222222222hij"));
    test(S(L"abcdefghij"), 5, 4, 0, '2', S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 4, 5, '2', S(L"abcde22222j"));
    test(S(L"abcdefghij"), 5, 4, 10, '2', S(L"abcde2222222222j"));
    test(S(L"abcdefghij"), 5, 4, 20, '2', S(L"abcde22222222222222222222j"));
    test(S(L"abcdefghij"), 5, 5, 0, '2', S(L"abcde"));
    test(S(L"abcdefghij"), 5, 5, 5, '2', S(L"abcde22222"));
    test(S(L"abcdefghij"), 5, 5, 10, '2', S(L"abcde2222222222"));
    test(S(L"abcdefghij"), 5, 5, 20, '2', S(L"abcde22222222222222222222"));
    test(S(L"abcdefghij"), 5, 6, 0, '2', S(L"abcde"));
    test(S(L"abcdefghij"), 5, 6, 5, '2', S(L"abcde22222"));
    test(S(L"abcdefghij"), 5, 6, 10, '2', S(L"abcde2222222222"));
    test(S(L"abcdefghij"), 5, 6, 20, '2', S(L"abcde22222222222222222222"));
    test(S(L"abcdefghij"), 9, 0, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 0, 5, '2', S(L"abcdefghi22222j"));
    test(S(L"abcdefghij"), 9, 0, 10, '2', S(L"abcdefghi2222222222j"));
    test(S(L"abcdefghij"), 9, 0, 20, '2', S(L"abcdefghi22222222222222222222j"));
    test(S(L"abcdefghij"), 9, 1, 0, '2', S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 1, 5, '2', S(L"abcdefghi22222"));
    test(S(L"abcdefghij"), 9, 1, 10, '2', S(L"abcdefghi2222222222"));
    test(S(L"abcdefghij"), 9, 1, 20, '2', S(L"abcdefghi22222222222222222222"));
    test(S(L"abcdefghij"), 9, 2, 0, '2', S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 2, 5, '2', S(L"abcdefghi22222"));
    test(S(L"abcdefghij"), 9, 2, 10, '2', S(L"abcdefghi2222222222"));
    test(S(L"abcdefghij"), 9, 2, 20, '2', S(L"abcdefghi22222222222222222222"));
    test(S(L"abcdefghij"), 10, 0, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 0, 5, '2', S(L"abcdefghij22222"));
    test(S(L"abcdefghij"), 10, 0, 10, '2', S(L"abcdefghij2222222222"));
    test(S(L"abcdefghij"), 10, 0, 20, '2', S(L"abcdefghij22222222222222222222"));
    test(S(L"abcdefghij"), 10, 1, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 1, 5, '2', S(L"abcdefghij22222"));
    test(S(L"abcdefghij"), 10, 1, 10, '2', S(L"abcdefghij2222222222"));
    test(S(L"abcdefghij"), 10, 1, 20, '2', S(L"abcdefghij22222222222222222222"));
    test(S(L"abcdefghij"), 11, 0, 0, '2', S(L"can't happen"));
    test(S(L"abcdefghij"), 11, 0, 5, '2', S(L"can't happen"));
    test(S(L"abcdefghij"), 11, 0, 10, '2', S(L"can't happen"));
    test(S(L"abcdefghij"), 11, 0, 20, '2', S(L"can't happen"));
}

template <class S>
void test2()
{
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 0, '2', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 5, '2', S(L"22222abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 10, '2', S(L"2222222222abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, 20, '2', S(L"22222222222222222222abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 0, '2', S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 5, '2', S(L"22222bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 10, '2', S(L"2222222222bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, 20, '2', S(L"22222222222222222222bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 0, '2', S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 5, '2', S(L"22222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 10, '2', S(L"2222222222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, 20, '2', S(L"22222222222222222222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 0, '2', S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 5, '2', S(L"22222t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 10, '2', S(L"2222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, 20, '2', S(L"22222222222222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 0, '2', S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 5, '2', S(L"22222"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 10, '2', S(L"2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, 20, '2', S(L"22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, 0, '2', S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, 5, '2', S(L"22222"));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, 10, '2', S(L"2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, 20, '2', S(L"22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 0, '2', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 5, '2', S(L"a22222bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 10, '2', S(L"a2222222222bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, 20, '2', S(L"a22222222222222222222bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 0, '2', S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 5, '2', S(L"a22222cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 10, '2', S(L"a2222222222cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, 20, '2', S(L"a22222222222222222222cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 0, '2', S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 5, '2', S(L"a22222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 10, '2', S(L"a2222222222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, 20, '2', S(L"a22222222222222222222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 0, '2', S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 5, '2', S(L"a22222t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 10, '2', S(L"a2222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, 20, '2', S(L"a22222222222222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 0, '2', S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 5, '2', S(L"a22222"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 10, '2', S(L"a2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, 20, '2', S(L"a22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, 0, '2', S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, 5, '2', S(L"a22222"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, 10, '2', S(L"a2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, 20, '2', S(L"a22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 0, '2', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 5, '2', S(L"abcdefghij22222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 10, '2', S(L"abcdefghij2222222222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, 20, '2', S(L"abcdefghij22222222222222222222klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 0, '2', S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 5, '2', S(L"abcdefghij22222lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 10, '2', S(L"abcdefghij2222222222lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, 20, '2', S(L"abcdefghij22222222222222222222lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 0, '2', S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 5, '2', S(L"abcdefghij22222pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 10, '2', S(L"abcdefghij2222222222pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, 20, '2', S(L"abcdefghij22222222222222222222pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 0, '2', S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 5, '2', S(L"abcdefghij22222t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 10, '2', S(L"abcdefghij2222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, 20, '2', S(L"abcdefghij22222222222222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 5, '2', S(L"abcdefghij22222"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 10, '2', S(L"abcdefghij2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, 20, '2', S(L"abcdefghij22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, 0, '2', S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, 5, '2', S(L"abcdefghij22222"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, 10, '2', S(L"abcdefghij2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, 20, '2', S(L"abcdefghij22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 0, '2', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 5, '2', S(L"abcdefghijklmnopqrs22222t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 10, '2', S(L"abcdefghijklmnopqrs2222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, 20, '2', S(L"abcdefghijklmnopqrs22222222222222222222t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 0, '2', S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 5, '2', S(L"abcdefghijklmnopqrs22222"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 10, '2', S(L"abcdefghijklmnopqrs2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, 20, '2', S(L"abcdefghijklmnopqrs22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, 0, '2', S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, 5, '2', S(L"abcdefghijklmnopqrs22222"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, 10, '2', S(L"abcdefghijklmnopqrs2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, 20, '2', S(L"abcdefghijklmnopqrs22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 0, '2', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 5, '2', S(L"abcdefghijklmnopqrst22222"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 10, '2', S(L"abcdefghijklmnopqrst2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, 20, '2', S(L"abcdefghijklmnopqrst22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, 0, '2', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, 5, '2', S(L"abcdefghijklmnopqrst22222"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, 10, '2', S(L"abcdefghijklmnopqrst2222222222"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, 20, '2', S(L"abcdefghijklmnopqrst22222222222222222222"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, 0, '2', S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, 5, '2', S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, 10, '2', S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, 20, '2', S(L"can't happen"));
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
