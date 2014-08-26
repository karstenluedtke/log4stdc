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
//   replace(size_type pos, size_type n1, const charT* s);

#include <stdio.h>

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type pos, typename S::size_type n1,
     const typename S::value_type* str, S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.replace(pos, n1, str);
        assert(s.__invariants());
        assert(pos <= old_size);
        assert(s == expected);
        typename S::size_type xlen = std::min(n1, old_size - pos);
        typename S::size_type rlen = S::traits_type::length(str);
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
    test(S(L""), 0, 0, L"", S(L""));
    test(S(L""), 0, 0, L"12345", S(L"12345"));
    test(S(L""), 0, 0, L"1234567890", S(L"1234567890"));
    test(S(L""), 0, 0, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L""), 0, 1, L"", S(L""));
    test(S(L""), 0, 1, L"12345", S(L"12345"));
    test(S(L""), 0, 1, L"1234567890", S(L"1234567890"));
    test(S(L""), 0, 1, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L""), 1, 0, L"", S(L"can't happen"));
    test(S(L""), 1, 0, L"12345", S(L"can't happen"));
    test(S(L""), 1, 0, L"1234567890", S(L"can't happen"));
    test(S(L""), 1, 0, L"12345678901234567890", S(L"can't happen"));
    test(S(L"abcde"), 0, 0, L"", S(L"abcde"));
    test(S(L"abcde"), 0, 0, L"12345", S(L"12345abcde"));
    test(S(L"abcde"), 0, 0, L"1234567890", S(L"1234567890abcde"));
    test(S(L"abcde"), 0, 0, L"12345678901234567890", S(L"12345678901234567890abcde"));
    test(S(L"abcde"), 0, 1, L"", S(L"bcde"));
    test(S(L"abcde"), 0, 1, L"12345", S(L"12345bcde"));
    test(S(L"abcde"), 0, 1, L"1234567890", S(L"1234567890bcde"));
    test(S(L"abcde"), 0, 1, L"12345678901234567890", S(L"12345678901234567890bcde"));
    test(S(L"abcde"), 0, 2, L"", S(L"cde"));
    test(S(L"abcde"), 0, 2, L"12345", S(L"12345cde"));
    test(S(L"abcde"), 0, 2, L"1234567890", S(L"1234567890cde"));
    test(S(L"abcde"), 0, 2, L"12345678901234567890", S(L"12345678901234567890cde"));
    test(S(L"abcde"), 0, 4, L"", S(L"e"));
    test(S(L"abcde"), 0, 4, L"12345", S(L"12345e"));
    test(S(L"abcde"), 0, 4, L"1234567890", S(L"1234567890e"));
    test(S(L"abcde"), 0, 4, L"12345678901234567890", S(L"12345678901234567890e"));
    test(S(L"abcde"), 0, 5, L"", S(L""));
    test(S(L"abcde"), 0, 5, L"12345", S(L"12345"));
    test(S(L"abcde"), 0, 5, L"1234567890", S(L"1234567890"));
    test(S(L"abcde"), 0, 5, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L"abcde"), 0, 6, L"", S(L""));
    test(S(L"abcde"), 0, 6, L"12345", S(L"12345"));
    test(S(L"abcde"), 0, 6, L"1234567890", S(L"1234567890"));
    test(S(L"abcde"), 0, 6, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L"abcde"), 1, 0, L"", S(L"abcde"));
    test(S(L"abcde"), 1, 0, L"12345", S(L"a12345bcde"));
    test(S(L"abcde"), 1, 0, L"1234567890", S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, 0, L"12345678901234567890", S(L"a12345678901234567890bcde"));
    test(S(L"abcde"), 1, 1, L"", S(L"acde"));
    test(S(L"abcde"), 1, 1, L"12345", S(L"a12345cde"));
    test(S(L"abcde"), 1, 1, L"1234567890", S(L"a1234567890cde"));
    test(S(L"abcde"), 1, 1, L"12345678901234567890", S(L"a12345678901234567890cde"));
    test(S(L"abcde"), 1, 2, L"", S(L"ade"));
    test(S(L"abcde"), 1, 2, L"12345", S(L"a12345de"));
    test(S(L"abcde"), 1, 2, L"1234567890", S(L"a1234567890de"));
    test(S(L"abcde"), 1, 2, L"12345678901234567890", S(L"a12345678901234567890de"));
    test(S(L"abcde"), 1, 3, L"", S(L"ae"));
    test(S(L"abcde"), 1, 3, L"12345", S(L"a12345e"));
    test(S(L"abcde"), 1, 3, L"1234567890", S(L"a1234567890e"));
    test(S(L"abcde"), 1, 3, L"12345678901234567890", S(L"a12345678901234567890e"));
    test(S(L"abcde"), 1, 4, L"", S(L"a"));
    test(S(L"abcde"), 1, 4, L"12345", S(L"a12345"));
    test(S(L"abcde"), 1, 4, L"1234567890", S(L"a1234567890"));
    test(S(L"abcde"), 1, 4, L"12345678901234567890", S(L"a12345678901234567890"));
    test(S(L"abcde"), 1, 5, L"", S(L"a"));
    test(S(L"abcde"), 1, 5, L"12345", S(L"a12345"));
    test(S(L"abcde"), 1, 5, L"1234567890", S(L"a1234567890"));
    test(S(L"abcde"), 1, 5, L"12345678901234567890", S(L"a12345678901234567890"));
    test(S(L"abcde"), 2, 0, L"", S(L"abcde"));
    test(S(L"abcde"), 2, 0, L"12345", S(L"ab12345cde"));
    test(S(L"abcde"), 2, 0, L"1234567890", S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, 0, L"12345678901234567890", S(L"ab12345678901234567890cde"));
    test(S(L"abcde"), 2, 1, L"", S(L"abde"));
    test(S(L"abcde"), 2, 1, L"12345", S(L"ab12345de"));
    test(S(L"abcde"), 2, 1, L"1234567890", S(L"ab1234567890de"));
    test(S(L"abcde"), 2, 1, L"12345678901234567890", S(L"ab12345678901234567890de"));
    test(S(L"abcde"), 2, 2, L"", S(L"abe"));
    test(S(L"abcde"), 2, 2, L"12345", S(L"ab12345e"));
    test(S(L"abcde"), 2, 2, L"1234567890", S(L"ab1234567890e"));
    test(S(L"abcde"), 2, 2, L"12345678901234567890", S(L"ab12345678901234567890e"));
    test(S(L"abcde"), 2, 3, L"", S(L"ab"));
    test(S(L"abcde"), 2, 3, L"12345", S(L"ab12345"));
    test(S(L"abcde"), 2, 3, L"1234567890", S(L"ab1234567890"));
    test(S(L"abcde"), 2, 3, L"12345678901234567890", S(L"ab12345678901234567890"));
    test(S(L"abcde"), 2, 4, L"", S(L"ab"));
    test(S(L"abcde"), 2, 4, L"12345", S(L"ab12345"));
    test(S(L"abcde"), 2, 4, L"1234567890", S(L"ab1234567890"));
    test(S(L"abcde"), 2, 4, L"12345678901234567890", S(L"ab12345678901234567890"));
    test(S(L"abcde"), 4, 0, L"", S(L"abcde"));
    test(S(L"abcde"), 4, 0, L"12345", S(L"abcd12345e"));
    test(S(L"abcde"), 4, 0, L"1234567890", S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, 0, L"12345678901234567890", S(L"abcd12345678901234567890e"));
    test(S(L"abcde"), 4, 1, L"", S(L"abcd"));
    test(S(L"abcde"), 4, 1, L"12345", S(L"abcd12345"));
    test(S(L"abcde"), 4, 1, L"1234567890", S(L"abcd1234567890"));
    test(S(L"abcde"), 4, 1, L"12345678901234567890", S(L"abcd12345678901234567890"));
    test(S(L"abcde"), 4, 2, L"", S(L"abcd"));
    test(S(L"abcde"), 4, 2, L"12345", S(L"abcd12345"));
    test(S(L"abcde"), 4, 2, L"1234567890", S(L"abcd1234567890"));
    test(S(L"abcde"), 4, 2, L"12345678901234567890", S(L"abcd12345678901234567890"));
    test(S(L"abcde"), 5, 0, L"", S(L"abcde"));
    test(S(L"abcde"), 5, 0, L"12345", S(L"abcde12345"));
    test(S(L"abcde"), 5, 0, L"1234567890", S(L"abcde1234567890"));
    test(S(L"abcde"), 5, 0, L"12345678901234567890", S(L"abcde12345678901234567890"));
    test(S(L"abcde"), 5, 1, L"", S(L"abcde"));
    test(S(L"abcde"), 5, 1, L"12345", S(L"abcde12345"));
    test(S(L"abcde"), 5, 1, L"1234567890", S(L"abcde1234567890"));
    test(S(L"abcde"), 5, 1, L"12345678901234567890", S(L"abcde12345678901234567890"));
}

template <class S>
void test1()
{
    test(S(L"abcde"), 6, 0, L"", S(L"can't happen"));
    test(S(L"abcde"), 6, 0, L"12345", S(L"can't happen"));
    test(S(L"abcde"), 6, 0, L"1234567890", S(L"can't happen"));
    test(S(L"abcde"), 6, 0, L"12345678901234567890", S(L"can't happen"));
    test(S(L"abcdefghij"), 0, 0, L"", S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, L"12345", S(L"12345abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, L"1234567890", S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, 0, L"12345678901234567890", S(L"12345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, L"", S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, L"12345", S(L"12345bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, L"1234567890", S(L"1234567890bcdefghij"));
    test(S(L"abcdefghij"), 0, 1, L"12345678901234567890", S(L"12345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, L"", S(L"fghij"));
    test(S(L"abcdefghij"), 0, 5, L"12345", S(L"12345fghij"));
    test(S(L"abcdefghij"), 0, 5, L"1234567890", S(L"1234567890fghij"));
    test(S(L"abcdefghij"), 0, 5, L"12345678901234567890", S(L"12345678901234567890fghij"));
    test(S(L"abcdefghij"), 0, 9, L"", S(L"j"));
    test(S(L"abcdefghij"), 0, 9, L"12345", S(L"12345j"));
    test(S(L"abcdefghij"), 0, 9, L"1234567890", S(L"1234567890j"));
    test(S(L"abcdefghij"), 0, 9, L"12345678901234567890", S(L"12345678901234567890j"));
    test(S(L"abcdefghij"), 0, 10, L"", S(L""));
    test(S(L"abcdefghij"), 0, 10, L"12345", S(L"12345"));
    test(S(L"abcdefghij"), 0, 10, L"1234567890", S(L"1234567890"));
    test(S(L"abcdefghij"), 0, 10, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L"abcdefghij"), 0, 11, L"", S(L""));
    test(S(L"abcdefghij"), 0, 11, L"12345", S(L"12345"));
    test(S(L"abcdefghij"), 0, 11, L"1234567890", S(L"1234567890"));
    test(S(L"abcdefghij"), 0, 11, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L"abcdefghij"), 1, 0, L"", S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 0, L"12345", S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, 0, L"1234567890", S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, 0, L"12345678901234567890", S(L"a12345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, 1, L"", S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 1, L"12345", S(L"a12345cdefghij"));
    test(S(L"abcdefghij"), 1, 1, L"1234567890", S(L"a1234567890cdefghij"));
    test(S(L"abcdefghij"), 1, 1, L"12345678901234567890", S(L"a12345678901234567890cdefghij"));
    test(S(L"abcdefghij"), 1, 4, L"", S(L"afghij"));
    test(S(L"abcdefghij"), 1, 4, L"12345", S(L"a12345fghij"));
    test(S(L"abcdefghij"), 1, 4, L"1234567890", S(L"a1234567890fghij"));
    test(S(L"abcdefghij"), 1, 4, L"12345678901234567890", S(L"a12345678901234567890fghij"));
    test(S(L"abcdefghij"), 1, 8, L"", S(L"aj"));
    test(S(L"abcdefghij"), 1, 8, L"12345", S(L"a12345j"));
    test(S(L"abcdefghij"), 1, 8, L"1234567890", S(L"a1234567890j"));
    test(S(L"abcdefghij"), 1, 8, L"12345678901234567890", S(L"a12345678901234567890j"));
    test(S(L"abcdefghij"), 1, 9, L"", S(L"a"));
    test(S(L"abcdefghij"), 1, 9, L"12345", S(L"a12345"));
    test(S(L"abcdefghij"), 1, 9, L"1234567890", S(L"a1234567890"));
    test(S(L"abcdefghij"), 1, 9, L"12345678901234567890", S(L"a12345678901234567890"));
    test(S(L"abcdefghij"), 1, 10, L"", S(L"a"));
    test(S(L"abcdefghij"), 1, 10, L"12345", S(L"a12345"));
    test(S(L"abcdefghij"), 1, 10, L"1234567890", S(L"a1234567890"));
    test(S(L"abcdefghij"), 1, 10, L"12345678901234567890", S(L"a12345678901234567890"));
    test(S(L"abcdefghij"), 5, 0, L"", S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 0, L"12345", S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, 0, L"1234567890", S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, 0, L"12345678901234567890", S(L"abcde12345678901234567890fghij"));
    test(S(L"abcdefghij"), 5, 1, L"", S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 1, L"12345", S(L"abcde12345ghij"));
    test(S(L"abcdefghij"), 5, 1, L"1234567890", S(L"abcde1234567890ghij"));
    test(S(L"abcdefghij"), 5, 1, L"12345678901234567890", S(L"abcde12345678901234567890ghij"));
    test(S(L"abcdefghij"), 5, 2, L"", S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 2, L"12345", S(L"abcde12345hij"));
    test(S(L"abcdefghij"), 5, 2, L"1234567890", S(L"abcde1234567890hij"));
    test(S(L"abcdefghij"), 5, 2, L"12345678901234567890", S(L"abcde12345678901234567890hij"));
    test(S(L"abcdefghij"), 5, 4, L"", S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 4, L"12345", S(L"abcde12345j"));
    test(S(L"abcdefghij"), 5, 4, L"1234567890", S(L"abcde1234567890j"));
    test(S(L"abcdefghij"), 5, 4, L"12345678901234567890", S(L"abcde12345678901234567890j"));
    test(S(L"abcdefghij"), 5, 5, L"", S(L"abcde"));
    test(S(L"abcdefghij"), 5, 5, L"12345", S(L"abcde12345"));
    test(S(L"abcdefghij"), 5, 5, L"1234567890", S(L"abcde1234567890"));
    test(S(L"abcdefghij"), 5, 5, L"12345678901234567890", S(L"abcde12345678901234567890"));
    test(S(L"abcdefghij"), 5, 6, L"", S(L"abcde"));
    test(S(L"abcdefghij"), 5, 6, L"12345", S(L"abcde12345"));
    test(S(L"abcdefghij"), 5, 6, L"1234567890", S(L"abcde1234567890"));
    test(S(L"abcdefghij"), 5, 6, L"12345678901234567890", S(L"abcde12345678901234567890"));
    test(S(L"abcdefghij"), 9, 0, L"", S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 0, L"12345", S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, 0, L"1234567890", S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, 0, L"12345678901234567890", S(L"abcdefghi12345678901234567890j"));
    test(S(L"abcdefghij"), 9, 1, L"", S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 1, L"12345", S(L"abcdefghi12345"));
    test(S(L"abcdefghij"), 9, 1, L"1234567890", S(L"abcdefghi1234567890"));
    test(S(L"abcdefghij"), 9, 1, L"12345678901234567890", S(L"abcdefghi12345678901234567890"));
    test(S(L"abcdefghij"), 9, 2, L"", S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 2, L"12345", S(L"abcdefghi12345"));
    test(S(L"abcdefghij"), 9, 2, L"1234567890", S(L"abcdefghi1234567890"));
    test(S(L"abcdefghij"), 9, 2, L"12345678901234567890", S(L"abcdefghi12345678901234567890"));
    test(S(L"abcdefghij"), 10, 0, L"", S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 0, L"12345", S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, 0, L"1234567890", S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, 0, L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghij"), 10, 1, L"", S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 1, L"12345", S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, 1, L"1234567890", S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, 1, L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghij"), 11, 0, L"", S(L"can't happen"));
    test(S(L"abcdefghij"), 11, 0, L"12345", S(L"can't happen"));
    test(S(L"abcdefghij"), 11, 0, L"1234567890", S(L"can't happen"));
    test(S(L"abcdefghij"), 11, 0, L"12345678901234567890", S(L"can't happen"));
}

template <class S>
void test2()
{
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"", S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"12345", S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"1234567890", S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, L"12345678901234567890", S(L"12345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"", S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"12345", S(L"12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"1234567890", S(L"1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, L"12345678901234567890", S(L"12345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"", S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"12345", S(L"12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"1234567890", S(L"1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, L"12345678901234567890", S(L"12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"", S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"12345", S(L"12345t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"1234567890", S(L"1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, L"12345678901234567890", S(L"12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"", S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"12345", S(L"12345"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"1234567890", S(L"1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"", S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"12345", S(L"12345"));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"1234567890", S(L"1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, L"12345678901234567890", S(L"12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"", S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"12345", S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"1234567890", S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, L"12345678901234567890", S(L"a12345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"", S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"12345", S(L"a12345cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"1234567890", S(L"a1234567890cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, L"12345678901234567890", S(L"a12345678901234567890cdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"", S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"12345", S(L"a12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"1234567890", S(L"a1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, L"12345678901234567890", S(L"a12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"", S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"12345", S(L"a12345t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"1234567890", S(L"a1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, L"12345678901234567890", S(L"a12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"", S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"12345", S(L"a12345"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"1234567890", S(L"a1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, L"12345678901234567890", S(L"a12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"", S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"12345", S(L"a12345"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"1234567890", S(L"a1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, L"12345678901234567890", S(L"a12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"", S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"12345", S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"1234567890", S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, L"12345678901234567890", S(L"abcdefghij12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"", S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"12345", S(L"abcdefghij12345lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"1234567890", S(L"abcdefghij1234567890lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, L"12345678901234567890", S(L"abcdefghij12345678901234567890lmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"", S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"12345", S(L"abcdefghij12345pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"1234567890", S(L"abcdefghij1234567890pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, L"12345678901234567890", S(L"abcdefghij12345678901234567890pqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"", S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"12345", S(L"abcdefghij12345t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"1234567890", S(L"abcdefghij1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, L"12345678901234567890", S(L"abcdefghij12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"", S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"12345", S(L"abcdefghij12345"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"1234567890", S(L"abcdefghij1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"", S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"12345", S(L"abcdefghij12345"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"1234567890", S(L"abcdefghij1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"", S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"12345", S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"1234567890", S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, L"12345678901234567890", S(L"abcdefghijklmnopqrs12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"", S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"12345", S(L"abcdefghijklmnopqrs12345"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"1234567890", S(L"abcdefghijklmnopqrs1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, L"12345678901234567890", S(L"abcdefghijklmnopqrs12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"", S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"12345", S(L"abcdefghijklmnopqrs12345"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"1234567890", S(L"abcdefghijklmnopqrs1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, L"12345678901234567890", S(L"abcdefghijklmnopqrs12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"", S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"12345", S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"1234567890", S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, L"12345678901234567890", S(L"abcdefghijklmnopqrst12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"", S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"12345", S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"1234567890", S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, L"12345678901234567890", S(L"abcdefghijklmnopqrst12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"", S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"12345", S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"1234567890", S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, L"12345678901234567890", S(L"can't happen"));
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
