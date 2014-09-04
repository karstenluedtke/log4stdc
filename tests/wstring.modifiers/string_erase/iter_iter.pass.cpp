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

// iterator erase(const_iterator first, const_iterator last);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::difference_type pos, typename S::difference_type n, S expected)
{
    typename S::const_iterator first = s.cbegin() + pos;
    typename S::const_iterator last = s.cbegin() + pos + n;
    typename S::iterator i = s.erase(first, last);
    assert(s.__invariants());
    assert(s == expected);
    assert(i - s.begin() == pos);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 0, 0, S(L""));
    test(S(L"abcde"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, 1, S(L"bcde"));
    test(S(L"abcde"), 0, 2, S(L"cde"));
    test(S(L"abcde"), 0, 4, S(L"e"));
    test(S(L"abcde"), 0, 5, S(L""));
    test(S(L"abcde"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, 1, S(L"acde"));
    test(S(L"abcde"), 1, 2, S(L"ade"));
    test(S(L"abcde"), 1, 3, S(L"ae"));
    test(S(L"abcde"), 1, 4, S(L"a"));
    test(S(L"abcde"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 2, 1, S(L"abde"));
    test(S(L"abcde"), 2, 2, S(L"abe"));
    test(S(L"abcde"), 2, 3, S(L"ab"));
    test(S(L"abcde"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 4, 1, S(L"abcd"));
    test(S(L"abcde"), 5, 0, S(L"abcde"));
    test(S(L"abcdefghij"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, S(L"fghij"));
    test(S(L"abcdefghij"), 0, 9, S(L"j"));
    test(S(L"abcdefghij"), 0, 10, S(L""));
    test(S(L"abcdefghij"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 1, S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 4, S(L"afghij"));
    test(S(L"abcdefghij"), 1, 8, S(L"aj"));
    test(S(L"abcdefghij"), 1, 9, S(L"a"));
    test(S(L"abcdefghij"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 1, S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 2, S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 4, S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 5, S(L"abcde"));
    test(S(L"abcdefghij"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 1, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcdefghijklmnopqrst"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 0, 0, S(L""));
    test(S(L"abcde"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, 1, S(L"bcde"));
    test(S(L"abcde"), 0, 2, S(L"cde"));
    test(S(L"abcde"), 0, 4, S(L"e"));
    test(S(L"abcde"), 0, 5, S(L""));
    test(S(L"abcde"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, 1, S(L"acde"));
    test(S(L"abcde"), 1, 2, S(L"ade"));
    test(S(L"abcde"), 1, 3, S(L"ae"));
    test(S(L"abcde"), 1, 4, S(L"a"));
    test(S(L"abcde"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 2, 1, S(L"abde"));
    test(S(L"abcde"), 2, 2, S(L"abe"));
    test(S(L"abcde"), 2, 3, S(L"ab"));
    test(S(L"abcde"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 4, 1, S(L"abcd"));
    test(S(L"abcde"), 5, 0, S(L"abcde"));
    test(S(L"abcdefghij"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, S(L"fghij"));
    test(S(L"abcdefghij"), 0, 9, S(L"j"));
    test(S(L"abcdefghij"), 0, 10, S(L""));
    test(S(L"abcdefghij"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 1, S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 4, S(L"afghij"));
    test(S(L"abcdefghij"), 1, 8, S(L"aj"));
    test(S(L"abcdefghij"), 1, 9, S(L"a"));
    test(S(L"abcdefghij"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 1, S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 2, S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 4, S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 5, S(L"abcde"));
    test(S(L"abcdefghij"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 1, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcdefghijklmnopqrst"));
    }
#endif
}
