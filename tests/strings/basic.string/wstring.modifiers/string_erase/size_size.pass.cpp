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
//   erase(size_type pos = 0, size_type n = npos);

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type pos, typename S::size_type n, S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.erase(pos, n);
        assert(s.__invariants());
        assert(pos <= old_size);
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos > old_size);
        assert(s == s0);
    }
}

template <class S>
void
test(S s, typename S::size_type pos, S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.erase(pos);
        assert(s.__invariants());
        assert(pos <= old_size);
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos > old_size);
        assert(s == s0);
    }
}

template <class S>
void
test(S s, S expected)
{
    s.erase();
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 0, 0, S(L""));
    test(S(L""), 0, 1, S(L""));
    test(S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, 1, S(L"bcde"));
    test(S(L"abcde"), 0, 2, S(L"cde"));
    test(S(L"abcde"), 0, 4, S(L"e"));
    test(S(L"abcde"), 0, 5, S(L""));
    test(S(L"abcde"), 0, 6, S(L""));
    test(S(L"abcde"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, 1, S(L"acde"));
    test(S(L"abcde"), 1, 2, S(L"ade"));
    test(S(L"abcde"), 1, 3, S(L"ae"));
    test(S(L"abcde"), 1, 4, S(L"a"));
    test(S(L"abcde"), 1, 5, S(L"a"));
    test(S(L"abcde"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 2, 1, S(L"abde"));
    test(S(L"abcde"), 2, 2, S(L"abe"));
    test(S(L"abcde"), 2, 3, S(L"ab"));
    test(S(L"abcde"), 2, 4, S(L"ab"));
    test(S(L"abcde"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 4, 1, S(L"abcd"));
    test(S(L"abcde"), 4, 2, S(L"abcd"));
    test(S(L"abcde"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, S(L"fghij"));
    test(S(L"abcdefghij"), 0, 9, S(L"j"));
    test(S(L"abcdefghij"), 0, 10, S(L""));
    test(S(L"abcdefghij"), 0, 11, S(L""));
    test(S(L"abcdefghij"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 1, S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 4, S(L"afghij"));
    test(S(L"abcdefghij"), 1, 8, S(L"aj"));
    test(S(L"abcdefghij"), 1, 9, S(L"a"));
    test(S(L"abcdefghij"), 1, 10, S(L"a"));
    test(S(L"abcdefghij"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 1, S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 2, S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 4, S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 5, S(L"abcde"));
    test(S(L"abcdefghij"), 5, 6, S(L"abcde"));
    test(S(L"abcdefghij"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 1, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 2, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, S(L"can't happen"));

    test(S(L""), 0, S(L""));
    test(S(L""), 1, S(L"can't happen"));
    test(S(L"abcde"), 0, S(L""));
    test(S(L"abcde"), 1, S(L"a"));
    test(S(L"abcde"), 2, S(L"ab"));
    test(S(L"abcde"), 4, S(L"abcd"));
    test(S(L"abcde"), 5, S(L"abcde"));
    test(S(L"abcde"), 6, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L""));
    test(S(L"abcdefghij"), 1, S(L"a"));
    test(S(L"abcdefghij"), 5, S(L"abcde"));
    test(S(L"abcdefghij"), 9, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 10, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 11, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"can't happen"));

    test(S(L""), S(L""));
    test(S(L"abcde"), S(L""));
    test(S(L"abcdefghij"), S(L""));
    test(S(L"abcdefghijklmnopqrst"), S(L""));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 0, 0, S(L""));
    test(S(L""), 0, 1, S(L""));
    test(S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, 1, S(L"bcde"));
    test(S(L"abcde"), 0, 2, S(L"cde"));
    test(S(L"abcde"), 0, 4, S(L"e"));
    test(S(L"abcde"), 0, 5, S(L""));
    test(S(L"abcde"), 0, 6, S(L""));
    test(S(L"abcde"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, 1, S(L"acde"));
    test(S(L"abcde"), 1, 2, S(L"ade"));
    test(S(L"abcde"), 1, 3, S(L"ae"));
    test(S(L"abcde"), 1, 4, S(L"a"));
    test(S(L"abcde"), 1, 5, S(L"a"));
    test(S(L"abcde"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 2, 1, S(L"abde"));
    test(S(L"abcde"), 2, 2, S(L"abe"));
    test(S(L"abcde"), 2, 3, S(L"ab"));
    test(S(L"abcde"), 2, 4, S(L"ab"));
    test(S(L"abcde"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 4, 1, S(L"abcd"));
    test(S(L"abcde"), 4, 2, S(L"abcd"));
    test(S(L"abcde"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 1, S(L"bcdefghij"));
    test(S(L"abcdefghij"), 0, 5, S(L"fghij"));
    test(S(L"abcdefghij"), 0, 9, S(L"j"));
    test(S(L"abcdefghij"), 0, 10, S(L""));
    test(S(L"abcdefghij"), 0, 11, S(L""));
    test(S(L"abcdefghij"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 1, S(L"acdefghij"));
    test(S(L"abcdefghij"), 1, 4, S(L"afghij"));
    test(S(L"abcdefghij"), 1, 8, S(L"aj"));
    test(S(L"abcdefghij"), 1, 9, S(L"a"));
    test(S(L"abcdefghij"), 1, 10, S(L"a"));
    test(S(L"abcdefghij"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 1, S(L"abcdeghij"));
    test(S(L"abcdefghij"), 5, 2, S(L"abcdehij"));
    test(S(L"abcdefghij"), 5, 4, S(L"abcdej"));
    test(S(L"abcdefghij"), 5, 5, S(L"abcde"));
    test(S(L"abcdefghij"), 5, 6, S(L"abcde"));
    test(S(L"abcdefghij"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 1, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 9, 2, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 1, S(L"bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, S(L"klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 19, S(L"t"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 0, 21, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 1, S(L"acdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 9, S(L"aklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 18, S(L"at"));
    test(S(L"abcdefghijklmnopqrst"), 1, 19, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 1, S(L"abcdefghijlmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, S(L"abcdefghijpqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 9, S(L"abcdefghijt"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 10, 11, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 1, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 19, 2, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 21, 0, S(L"can't happen"));

    test(S(L""), 0, S(L""));
    test(S(L""), 1, S(L"can't happen"));
    test(S(L"abcde"), 0, S(L""));
    test(S(L"abcde"), 1, S(L"a"));
    test(S(L"abcde"), 2, S(L"ab"));
    test(S(L"abcde"), 4, S(L"abcd"));
    test(S(L"abcde"), 5, S(L"abcde"));
    test(S(L"abcde"), 6, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L""));
    test(S(L"abcdefghij"), 1, S(L"a"));
    test(S(L"abcdefghij"), 5, S(L"abcde"));
    test(S(L"abcdefghij"), 9, S(L"abcdefghi"));
    test(S(L"abcdefghij"), 10, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 11, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"a"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"abcdefghij"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"abcdefghijklmnopqrs"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"can't happen"));

    test(S(L""), S(L""));
    test(S(L"abcde"), S(L""));
    test(S(L"abcdefghij"), S(L""));
    test(S(L"abcdefghijklmnopqrst"), S(L""));
    }
#endif
}
