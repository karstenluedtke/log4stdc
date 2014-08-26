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

// iterator erase(const_iterator p);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::difference_type pos, S expected)
{
    typename S::const_iterator p = s.begin() + pos;
    typename S::iterator i = s.erase(p);
    assert(s.__invariants());
    assert(s == expected);
    assert(i - s.begin() == pos);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L"abcde"), 0, S(L"bcde"));
    test(S(L"abcde"), 1, S(L"acde"));
    test(S(L"abcde"), 2, S(L"abde"));
    test(S(L"abcde"), 4, S(L"abcd"));
    test(S(L"abcdefghij"), 0, S(L"bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"acdefghij"));
    test(S(L"abcdefghij"), 5, S(L"abcdeghij"));
    test(S(L"abcdefghij"), 9, S(L"abcdefghi"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"abcdefghijklmnopqrs"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L"abcde"), 0, S(L"bcde"));
    test(S(L"abcde"), 1, S(L"acde"));
    test(S(L"abcde"), 2, S(L"abde"));
    test(S(L"abcde"), 4, S(L"abcd"));
    test(S(L"abcdefghij"), 0, S(L"bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"acdefghij"));
    test(S(L"abcdefghij"), 5, S(L"abcdeghij"));
    test(S(L"abcdefghij"), 9, S(L"abcdefghi"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"abcdefghijklmnopqrs"));
    }
#endif
}
