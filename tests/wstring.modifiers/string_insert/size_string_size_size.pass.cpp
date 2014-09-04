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
//   insert(size_type pos1, const basic_string<charT,traits,Allocator>& str,
//          size_type pos2, size_type n=npos);
// the "=npos" was added in C++14

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type pos1, S str, typename S::size_type pos2,
     typename S::size_type n, S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.insert(pos1, str, pos2, n);
        assert(s.__invariants());
        assert(pos1 <= old_size && pos2 <= str.size());
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos1 > old_size || pos2 > str.size());
        assert(s == s0);
    }
}

template <class S>
void
test_npos(S s, typename S::size_type pos1, S str, typename S::size_type pos2, S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.insert(pos1, str, pos2);
        assert(s.__invariants());
        assert(pos1 <= old_size && pos2 <= str.size());
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos1 > old_size || pos2 > str.size());
        assert(s == s0);
    }
}


template <class S>
void test0()
{
    test(S(L""), 0, S(L""), 0, 0, S(L""));
    test(S(L""), 0, S(L""), 0, 1, S(L""));
    test(S(L""), 0, S(L""), 1, 0, S(L"can't happen"));
    test(S(L""), 0, S(L"12345"), 0, 0, S(L""));
    test(S(L""), 0, S(L"12345"), 0, 1, S(L"1"));
    test(S(L""), 0, S(L"12345"), 0, 2, S(L"12"));
    test(S(L""), 0, S(L"12345"), 0, 4, S(L"1234"));
    test(S(L""), 0, S(L"12345"), 0, 5, S(L"12345"));
    test(S(L""), 0, S(L"12345"), 0, 6, S(L"12345"));
    test(S(L""), 0, S(L"12345"), 1, 0, S(L""));
    test(S(L""), 0, S(L"12345"), 1, 1, S(L"2"));
    test(S(L""), 0, S(L"12345"), 1, 2, S(L"23"));
    test(S(L""), 0, S(L"12345"), 1, 3, S(L"234"));
    test(S(L""), 0, S(L"12345"), 1, 4, S(L"2345"));
    test(S(L""), 0, S(L"12345"), 1, 5, S(L"2345"));
    test(S(L""), 0, S(L"12345"), 2, 0, S(L""));
    test(S(L""), 0, S(L"12345"), 2, 1, S(L"3"));
    test(S(L""), 0, S(L"12345"), 2, 2, S(L"34"));
    test(S(L""), 0, S(L"12345"), 2, 3, S(L"345"));
    test(S(L""), 0, S(L"12345"), 2, 4, S(L"345"));
    test(S(L""), 0, S(L"12345"), 4, 0, S(L""));
    test(S(L""), 0, S(L"12345"), 4, 1, S(L"5"));
    test(S(L""), 0, S(L"12345"), 4, 2, S(L"5"));
    test(S(L""), 0, S(L"12345"), 5, 0, S(L""));
    test(S(L""), 0, S(L"12345"), 5, 1, S(L""));
    test(S(L""), 0, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L""), 0, S(L"1234567890"), 0, 0, S(L""));
    test(S(L""), 0, S(L"1234567890"), 0, 1, S(L"1"));
    test(S(L""), 0, S(L"1234567890"), 0, 5, S(L"12345"));
    test(S(L""), 0, S(L"1234567890"), 0, 9, S(L"123456789"));
    test(S(L""), 0, S(L"1234567890"), 0, 10, S(L"1234567890"));
    test(S(L""), 0, S(L"1234567890"), 0, 11, S(L"1234567890"));
    test(S(L""), 0, S(L"1234567890"), 1, 0, S(L""));
    test(S(L""), 0, S(L"1234567890"), 1, 1, S(L"2"));
    test(S(L""), 0, S(L"1234567890"), 1, 4, S(L"2345"));
    test(S(L""), 0, S(L"1234567890"), 1, 8, S(L"23456789"));
    test(S(L""), 0, S(L"1234567890"), 1, 9, S(L"234567890"));
    test(S(L""), 0, S(L"1234567890"), 1, 10, S(L"234567890"));
    test(S(L""), 0, S(L"1234567890"), 5, 0, S(L""));
    test(S(L""), 0, S(L"1234567890"), 5, 1, S(L"6"));
    test(S(L""), 0, S(L"1234567890"), 5, 2, S(L"67"));
    test(S(L""), 0, S(L"1234567890"), 5, 4, S(L"6789"));
    test(S(L""), 0, S(L"1234567890"), 5, 5, S(L"67890"));
    test(S(L""), 0, S(L"1234567890"), 5, 6, S(L"67890"));
    test(S(L""), 0, S(L"1234567890"), 9, 0, S(L""));
    test(S(L""), 0, S(L"1234567890"), 9, 1, S(L"0"));
    test(S(L""), 0, S(L"1234567890"), 9, 2, S(L"0"));
    test(S(L""), 0, S(L"1234567890"), 10, 0, S(L""));
    test(S(L""), 0, S(L"1234567890"), 10, 1, S(L""));
    test(S(L""), 0, S(L"1234567890"), 11, 0, S(L"can't happen"));
}

template <class S>
void test1()
{
    test(S(L""), 0, S(L"12345678901234567890"), 0, 0, S(L""));
    test(S(L""), 0, S(L"12345678901234567890"), 0, 1, S(L"1"));
    test(S(L""), 0, S(L"12345678901234567890"), 0, 10, S(L"1234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 0, 19, S(L"1234567890123456789"));
    test(S(L""), 0, S(L"12345678901234567890"), 0, 20, S(L"12345678901234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 0, 21, S(L"12345678901234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 1, 0, S(L""));
    test(S(L""), 0, S(L"12345678901234567890"), 1, 1, S(L"2"));
    test(S(L""), 0, S(L"12345678901234567890"), 1, 9, S(L"234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 1, 18, S(L"234567890123456789"));
    test(S(L""), 0, S(L"12345678901234567890"), 1, 19, S(L"2345678901234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 1, 20, S(L"2345678901234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 10, 0, S(L""));
    test(S(L""), 0, S(L"12345678901234567890"), 10, 1, S(L"1"));
    test(S(L""), 0, S(L"12345678901234567890"), 10, 5, S(L"12345"));
    test(S(L""), 0, S(L"12345678901234567890"), 10, 9, S(L"123456789"));
    test(S(L""), 0, S(L"12345678901234567890"), 10, 10, S(L"1234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 10, 11, S(L"1234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), 19, 0, S(L""));
    test(S(L""), 0, S(L"12345678901234567890"), 19, 1, S(L"0"));
    test(S(L""), 0, S(L"12345678901234567890"), 19, 2, S(L"0"));
    test(S(L""), 0, S(L"12345678901234567890"), 20, 0, S(L""));
    test(S(L""), 0, S(L"12345678901234567890"), 20, 1, S(L""));
    test(S(L""), 0, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L""), 1, S(L""), 0, 0, S(L"can't happen"));
    test(S(L""), 1, S(L""), 0, 1, S(L"can't happen"));
    test(S(L""), 1, S(L""), 1, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 0, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 0, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 0, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 0, 4, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 0, 5, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 0, 6, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 1, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 1, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 1, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 1, 3, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 1, 4, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 1, 5, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 2, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 2, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 2, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 2, 3, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 2, 4, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 4, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 4, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 4, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 5, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 5, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), 6, 0, S(L"can't happen"));
}

template <class S>
void test2()
{
    test(S(L""), 1, S(L"1234567890"), 0, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 0, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 0, 5, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 0, 9, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 0, 10, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 0, 11, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 1, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 1, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 1, 4, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 1, 8, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 1, 9, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 1, 10, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 5, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 5, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 5, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 5, 4, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 5, 5, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 5, 6, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 9, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 9, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 9, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 10, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 10, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 0, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 0, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 0, 10, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 0, 19, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 0, 20, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 0, 21, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 1, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 1, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 1, 9, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 1, 18, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 1, 19, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 1, 20, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 10, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 10, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 10, 5, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 10, 9, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 10, 10, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 10, 11, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 19, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 19, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 19, 2, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 20, 0, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 20, 1, S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcde"), 0, S(L""), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L""), 0, 1, S(L"abcde"));
}

template <class S>
void test3()
{
    test(S(L"abcde"), 0, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 0, S(L"12345"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 0, 1, S(L"1abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 0, 2, S(L"12abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 0, 4, S(L"1234abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 0, 5, S(L"12345abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 0, 6, S(L"12345abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 1, 1, S(L"2abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 1, 2, S(L"23abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 1, 3, S(L"234abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 1, 4, S(L"2345abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 1, 5, S(L"2345abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 2, 1, S(L"3abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 2, 2, S(L"34abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 2, 3, S(L"345abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 2, 4, S(L"345abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 4, 1, S(L"5abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 4, 2, S(L"5abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcde"), 0, S(L"1234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 0, 1, S(L"1abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 0, 5, S(L"12345abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 0, 9, S(L"123456789abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 0, 10, S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 0, 11, S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 1, 1, S(L"2abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 1, 4, S(L"2345abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 1, 8, S(L"23456789abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 1, 9, S(L"234567890abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 1, 10, S(L"234567890abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 5, 1, S(L"6abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 5, 2, S(L"67abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 5, 4, S(L"6789abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 5, 5, S(L"67890abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 5, 6, S(L"67890abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 9, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 9, 1, S(L"0abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 9, 2, S(L"0abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 10, 1, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 0, 1, S(L"1abcde"));
}

template <class S>
void test4()
{
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 0, 10, S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 0, 19, S(L"1234567890123456789abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 0, 20, S(L"12345678901234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 0, 21, S(L"12345678901234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 1, 1, S(L"2abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 1, 9, S(L"234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 1, 18, S(L"234567890123456789abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 1, 19, S(L"2345678901234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 1, 20, S(L"2345678901234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 10, 1, S(L"1abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 10, 5, S(L"12345abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 10, 9, S(L"123456789abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 10, 10, S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 10, 11, S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 19, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 19, 1, S(L"0abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 19, 2, S(L"0abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 20, 0, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 20, 1, S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcde"), 1, S(L""), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L""), 0, 1, S(L"abcde"));
    test(S(L"abcde"), 1, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 1, S(L"12345"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), 0, 1, S(L"a1bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 0, 2, S(L"a12bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 0, 4, S(L"a1234bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 0, 5, S(L"a12345bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 0, 6, S(L"a12345bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), 1, 1, S(L"a2bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 1, 2, S(L"a23bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 1, 3, S(L"a234bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 1, 4, S(L"a2345bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 1, 5, S(L"a2345bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), 2, 1, S(L"a3bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 2, 2, S(L"a34bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 2, 3, S(L"a345bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 2, 4, S(L"a345bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), 4, 1, S(L"a5bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 4, 2, S(L"a5bcde"));
    test(S(L"abcde"), 1, S(L"12345"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcde"), 1, S(L"1234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 0, 1, S(L"a1bcde"));
}

template <class S>
void test5()
{
    test(S(L"abcde"), 1, S(L"1234567890"), 0, 5, S(L"a12345bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 0, 9, S(L"a123456789bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 0, 10, S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 0, 11, S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 1, 1, S(L"a2bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 1, 4, S(L"a2345bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 1, 8, S(L"a23456789bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 1, 9, S(L"a234567890bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 1, 10, S(L"a234567890bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 5, 1, S(L"a6bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 5, 2, S(L"a67bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 5, 4, S(L"a6789bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 5, 5, S(L"a67890bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 5, 6, S(L"a67890bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 9, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 9, 1, S(L"a0bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 9, 2, S(L"a0bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 10, 1, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 0, 1, S(L"a1bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 0, 10, S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 0, 19, S(L"a1234567890123456789bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 0, 20, S(L"a12345678901234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 0, 21, S(L"a12345678901234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 1, 1, S(L"a2bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 1, 9, S(L"a234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 1, 18, S(L"a234567890123456789bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 1, 19, S(L"a2345678901234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 1, 20, S(L"a2345678901234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 10, 1, S(L"a1bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 10, 5, S(L"a12345bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 10, 9, S(L"a123456789bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 10, 10, S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 10, 11, S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 19, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 19, 1, S(L"a0bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 19, 2, S(L"a0bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 20, 0, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 20, 1, S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcde"), 2, S(L""), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L""), 0, 1, S(L"abcde"));
    test(S(L"abcde"), 2, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 2, S(L"12345"), 0, 0, S(L"abcde"));
}

template <class S>
void test6()
{
    test(S(L"abcde"), 2, S(L"12345"), 0, 1, S(L"ab1cde"));
    test(S(L"abcde"), 2, S(L"12345"), 0, 2, S(L"ab12cde"));
    test(S(L"abcde"), 2, S(L"12345"), 0, 4, S(L"ab1234cde"));
    test(S(L"abcde"), 2, S(L"12345"), 0, 5, S(L"ab12345cde"));
    test(S(L"abcde"), 2, S(L"12345"), 0, 6, S(L"ab12345cde"));
    test(S(L"abcde"), 2, S(L"12345"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), 1, 1, S(L"ab2cde"));
    test(S(L"abcde"), 2, S(L"12345"), 1, 2, S(L"ab23cde"));
    test(S(L"abcde"), 2, S(L"12345"), 1, 3, S(L"ab234cde"));
    test(S(L"abcde"), 2, S(L"12345"), 1, 4, S(L"ab2345cde"));
    test(S(L"abcde"), 2, S(L"12345"), 1, 5, S(L"ab2345cde"));
    test(S(L"abcde"), 2, S(L"12345"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), 2, 1, S(L"ab3cde"));
    test(S(L"abcde"), 2, S(L"12345"), 2, 2, S(L"ab34cde"));
    test(S(L"abcde"), 2, S(L"12345"), 2, 3, S(L"ab345cde"));
    test(S(L"abcde"), 2, S(L"12345"), 2, 4, S(L"ab345cde"));
    test(S(L"abcde"), 2, S(L"12345"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), 4, 1, S(L"ab5cde"));
    test(S(L"abcde"), 2, S(L"12345"), 4, 2, S(L"ab5cde"));
    test(S(L"abcde"), 2, S(L"12345"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcde"), 2, S(L"1234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 0, 1, S(L"ab1cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 0, 5, S(L"ab12345cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 0, 9, S(L"ab123456789cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 0, 10, S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 0, 11, S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 1, 1, S(L"ab2cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 1, 4, S(L"ab2345cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 1, 8, S(L"ab23456789cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 1, 9, S(L"ab234567890cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 1, 10, S(L"ab234567890cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 5, 1, S(L"ab6cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 5, 2, S(L"ab67cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 5, 4, S(L"ab6789cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 5, 5, S(L"ab67890cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 5, 6, S(L"ab67890cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 9, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 9, 1, S(L"ab0cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 9, 2, S(L"ab0cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 10, 1, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 0, 1, S(L"ab1cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 0, 10, S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 0, 19, S(L"ab1234567890123456789cde"));
}

template <class S>
void test7()
{
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 0, 20, S(L"ab12345678901234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 0, 21, S(L"ab12345678901234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 1, 1, S(L"ab2cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 1, 9, S(L"ab234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 1, 18, S(L"ab234567890123456789cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 1, 19, S(L"ab2345678901234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 1, 20, S(L"ab2345678901234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 10, 1, S(L"ab1cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 10, 5, S(L"ab12345cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 10, 9, S(L"ab123456789cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 10, 10, S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 10, 11, S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 19, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 19, 1, S(L"ab0cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 19, 2, S(L"ab0cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 20, 0, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 20, 1, S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcde"), 4, S(L""), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L""), 0, 1, S(L"abcde"));
    test(S(L"abcde"), 4, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 4, S(L"12345"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), 0, 1, S(L"abcd1e"));
    test(S(L"abcde"), 4, S(L"12345"), 0, 2, S(L"abcd12e"));
    test(S(L"abcde"), 4, S(L"12345"), 0, 4, S(L"abcd1234e"));
    test(S(L"abcde"), 4, S(L"12345"), 0, 5, S(L"abcd12345e"));
    test(S(L"abcde"), 4, S(L"12345"), 0, 6, S(L"abcd12345e"));
    test(S(L"abcde"), 4, S(L"12345"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), 1, 1, S(L"abcd2e"));
    test(S(L"abcde"), 4, S(L"12345"), 1, 2, S(L"abcd23e"));
    test(S(L"abcde"), 4, S(L"12345"), 1, 3, S(L"abcd234e"));
    test(S(L"abcde"), 4, S(L"12345"), 1, 4, S(L"abcd2345e"));
    test(S(L"abcde"), 4, S(L"12345"), 1, 5, S(L"abcd2345e"));
    test(S(L"abcde"), 4, S(L"12345"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), 2, 1, S(L"abcd3e"));
    test(S(L"abcde"), 4, S(L"12345"), 2, 2, S(L"abcd34e"));
    test(S(L"abcde"), 4, S(L"12345"), 2, 3, S(L"abcd345e"));
    test(S(L"abcde"), 4, S(L"12345"), 2, 4, S(L"abcd345e"));
    test(S(L"abcde"), 4, S(L"12345"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), 4, 1, S(L"abcd5e"));
    test(S(L"abcde"), 4, S(L"12345"), 4, 2, S(L"abcd5e"));
    test(S(L"abcde"), 4, S(L"12345"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcde"), 4, S(L"1234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"1234567890"), 0, 1, S(L"abcd1e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 0, 5, S(L"abcd12345e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 0, 9, S(L"abcd123456789e"));
}

template <class S>
void test8()
{
    test(S(L"abcde"), 4, S(L"1234567890"), 0, 10, S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 0, 11, S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"1234567890"), 1, 1, S(L"abcd2e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 1, 4, S(L"abcd2345e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 1, 8, S(L"abcd23456789e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 1, 9, S(L"abcd234567890e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 1, 10, S(L"abcd234567890e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"1234567890"), 5, 1, S(L"abcd6e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 5, 2, S(L"abcd67e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 5, 4, S(L"abcd6789e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 5, 5, S(L"abcd67890e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 5, 6, S(L"abcd67890e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 9, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"1234567890"), 9, 1, S(L"abcd0e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 9, 2, S(L"abcd0e"));
    test(S(L"abcde"), 4, S(L"1234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"1234567890"), 10, 1, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 0, 1, S(L"abcd1e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 0, 10, S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 0, 19, S(L"abcd1234567890123456789e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 0, 20, S(L"abcd12345678901234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 0, 21, S(L"abcd12345678901234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 1, 1, S(L"abcd2e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 1, 9, S(L"abcd234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 1, 18, S(L"abcd234567890123456789e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 1, 19, S(L"abcd2345678901234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 1, 20, S(L"abcd2345678901234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 10, 1, S(L"abcd1e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 10, 5, S(L"abcd12345e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 10, 9, S(L"abcd123456789e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 10, 10, S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 10, 11, S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 19, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 19, 1, S(L"abcd0e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 19, 2, S(L"abcd0e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 20, 0, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 20, 1, S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcde"), 5, S(L""), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L""), 0, 1, S(L"abcde"));
    test(S(L"abcde"), 5, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 5, S(L"12345"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), 0, 1, S(L"abcde1"));
    test(S(L"abcde"), 5, S(L"12345"), 0, 2, S(L"abcde12"));
}

template <class S>
void test9()
{
    test(S(L"abcde"), 5, S(L"12345"), 0, 4, S(L"abcde1234"));
    test(S(L"abcde"), 5, S(L"12345"), 0, 5, S(L"abcde12345"));
    test(S(L"abcde"), 5, S(L"12345"), 0, 6, S(L"abcde12345"));
    test(S(L"abcde"), 5, S(L"12345"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), 1, 1, S(L"abcde2"));
    test(S(L"abcde"), 5, S(L"12345"), 1, 2, S(L"abcde23"));
    test(S(L"abcde"), 5, S(L"12345"), 1, 3, S(L"abcde234"));
    test(S(L"abcde"), 5, S(L"12345"), 1, 4, S(L"abcde2345"));
    test(S(L"abcde"), 5, S(L"12345"), 1, 5, S(L"abcde2345"));
    test(S(L"abcde"), 5, S(L"12345"), 2, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), 2, 1, S(L"abcde3"));
    test(S(L"abcde"), 5, S(L"12345"), 2, 2, S(L"abcde34"));
    test(S(L"abcde"), 5, S(L"12345"), 2, 3, S(L"abcde345"));
    test(S(L"abcde"), 5, S(L"12345"), 2, 4, S(L"abcde345"));
    test(S(L"abcde"), 5, S(L"12345"), 4, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), 4, 1, S(L"abcde5"));
    test(S(L"abcde"), 5, S(L"12345"), 4, 2, S(L"abcde5"));
    test(S(L"abcde"), 5, S(L"12345"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), 5, 1, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcde"), 5, S(L"1234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"1234567890"), 0, 1, S(L"abcde1"));
    test(S(L"abcde"), 5, S(L"1234567890"), 0, 5, S(L"abcde12345"));
    test(S(L"abcde"), 5, S(L"1234567890"), 0, 9, S(L"abcde123456789"));
    test(S(L"abcde"), 5, S(L"1234567890"), 0, 10, S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"1234567890"), 0, 11, S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"1234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"1234567890"), 1, 1, S(L"abcde2"));
    test(S(L"abcde"), 5, S(L"1234567890"), 1, 4, S(L"abcde2345"));
    test(S(L"abcde"), 5, S(L"1234567890"), 1, 8, S(L"abcde23456789"));
    test(S(L"abcde"), 5, S(L"1234567890"), 1, 9, S(L"abcde234567890"));
    test(S(L"abcde"), 5, S(L"1234567890"), 1, 10, S(L"abcde234567890"));
    test(S(L"abcde"), 5, S(L"1234567890"), 5, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"1234567890"), 5, 1, S(L"abcde6"));
    test(S(L"abcde"), 5, S(L"1234567890"), 5, 2, S(L"abcde67"));
    test(S(L"abcde"), 5, S(L"1234567890"), 5, 4, S(L"abcde6789"));
    test(S(L"abcde"), 5, S(L"1234567890"), 5, 5, S(L"abcde67890"));
    test(S(L"abcde"), 5, S(L"1234567890"), 5, 6, S(L"abcde67890"));
    test(S(L"abcde"), 5, S(L"1234567890"), 9, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"1234567890"), 9, 1, S(L"abcde0"));
    test(S(L"abcde"), 5, S(L"1234567890"), 9, 2, S(L"abcde0"));
    test(S(L"abcde"), 5, S(L"1234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"1234567890"), 10, 1, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 0, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 0, 1, S(L"abcde1"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 0, 10, S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 0, 19, S(L"abcde1234567890123456789"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 0, 20, S(L"abcde12345678901234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 0, 21, S(L"abcde12345678901234567890"));
}

template <class S>
void test10()
{
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 1, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 1, 1, S(L"abcde2"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 1, 9, S(L"abcde234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 1, 18, S(L"abcde234567890123456789"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 1, 19, S(L"abcde2345678901234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 1, 20, S(L"abcde2345678901234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 10, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 10, 1, S(L"abcde1"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 10, 5, S(L"abcde12345"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 10, 9, S(L"abcde123456789"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 10, 10, S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 10, 11, S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 19, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 19, 1, S(L"abcde0"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 19, 2, S(L"abcde0"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 20, 0, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 20, 1, S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L""), 0, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L""), 0, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 0, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 0, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 0, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 0, 4, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 0, 5, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 0, 6, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 1, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 1, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 1, 3, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 1, 4, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 1, 5, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 2, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 2, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 2, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 2, 3, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 2, 4, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 4, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 4, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 4, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 5, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 5, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 0, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 0, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 0, 5, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 0, 9, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 0, 10, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 0, 11, S(L"can't happen"));
}

template <class S>
void test11()
{
    test(S(L"abcde"), 6, S(L"1234567890"), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 1, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 1, 4, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 1, 8, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 1, 9, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 1, 10, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 5, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 5, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 5, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 5, 4, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 5, 5, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 5, 6, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 9, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 9, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 9, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 10, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 10, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 0, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 0, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 0, 10, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 0, 19, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 0, 20, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 0, 21, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 1, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 1, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 1, 9, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 1, 18, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 1, 19, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 1, 20, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 10, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 10, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 10, 5, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 10, 9, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 10, 10, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 10, 11, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 19, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 19, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 19, 2, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 20, 0, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 20, 1, S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L""), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L""), 0, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 0, 1, S(L"1abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 0, 2, S(L"12abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 0, 4, S(L"1234abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 0, 5, S(L"12345abcdefghij"));
}

template <class S>
void test12()
{
    test(S(L"abcdefghij"), 0, S(L"12345"), 0, 6, S(L"12345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 1, 1, S(L"2abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 1, 2, S(L"23abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 1, 3, S(L"234abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 1, 4, S(L"2345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 1, 5, S(L"2345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 2, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 2, 1, S(L"3abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 2, 2, S(L"34abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 2, 3, S(L"345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 2, 4, S(L"345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 4, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 4, 1, S(L"5abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 4, 2, S(L"5abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 5, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 0, 1, S(L"1abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 0, 5, S(L"12345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 0, 9, S(L"123456789abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 0, 10, S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 0, 11, S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 1, 1, S(L"2abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 1, 4, S(L"2345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 1, 8, S(L"23456789abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 1, 9, S(L"234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 1, 10, S(L"234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 5, 1, S(L"6abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 5, 2, S(L"67abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 5, 4, S(L"6789abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 5, 5, S(L"67890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 5, 6, S(L"67890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 9, 1, S(L"0abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 9, 2, S(L"0abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 0, 1, S(L"1abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 0, 10, S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 0, 19, S(L"1234567890123456789abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 0, 20, S(L"12345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 0, 21, S(L"12345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 1, 1, S(L"2abcdefghij"));
}

template <class S>
void test13()
{
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 1, 9, S(L"234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 1, 18, S(L"234567890123456789abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 1, 19, S(L"2345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 1, 20, S(L"2345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 10, 1, S(L"1abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 10, 5, S(L"12345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 10, 9, S(L"123456789abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 10, 10, S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 10, 11, S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 19, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 19, 1, S(L"0abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 19, 2, S(L"0abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 20, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 20, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 1, S(L""), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L""), 0, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 0, 1, S(L"a1bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 0, 2, S(L"a12bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 0, 4, S(L"a1234bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 0, 5, S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 0, 6, S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 1, 1, S(L"a2bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 1, 2, S(L"a23bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 1, 3, S(L"a234bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 1, 4, S(L"a2345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 1, 5, S(L"a2345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 2, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 2, 1, S(L"a3bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 2, 2, S(L"a34bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 2, 3, S(L"a345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 2, 4, S(L"a345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 4, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 4, 1, S(L"a5bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 4, 2, S(L"a5bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 5, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 0, 1, S(L"a1bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 0, 5, S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 0, 9, S(L"a123456789bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 0, 10, S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 0, 11, S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 1, 1, S(L"a2bcdefghij"));
}

template <class S>
void test14()
{
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 1, 4, S(L"a2345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 1, 8, S(L"a23456789bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 1, 9, S(L"a234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 1, 10, S(L"a234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 5, 1, S(L"a6bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 5, 2, S(L"a67bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 5, 4, S(L"a6789bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 5, 5, S(L"a67890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 5, 6, S(L"a67890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 9, 1, S(L"a0bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 9, 2, S(L"a0bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 0, 1, S(L"a1bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 0, 10, S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 0, 19, S(L"a1234567890123456789bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 0, 20, S(L"a12345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 0, 21, S(L"a12345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 1, 1, S(L"a2bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 1, 9, S(L"a234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 1, 18, S(L"a234567890123456789bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 1, 19, S(L"a2345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 1, 20, S(L"a2345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 10, 1, S(L"a1bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 10, 5, S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 10, 9, S(L"a123456789bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 10, 10, S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 10, 11, S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 19, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 19, 1, S(L"a0bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 19, 2, S(L"a0bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 20, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 20, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 5, S(L""), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L""), 0, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 0, 1, S(L"abcde1fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 0, 2, S(L"abcde12fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 0, 4, S(L"abcde1234fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 0, 5, S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 0, 6, S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 1, 0, S(L"abcdefghij"));
}

template <class S>
void test15()
{
    test(S(L"abcdefghij"), 5, S(L"12345"), 1, 1, S(L"abcde2fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 1, 2, S(L"abcde23fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 1, 3, S(L"abcde234fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 1, 4, S(L"abcde2345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 1, 5, S(L"abcde2345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 2, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 2, 1, S(L"abcde3fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 2, 2, S(L"abcde34fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 2, 3, S(L"abcde345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 2, 4, S(L"abcde345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 4, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 4, 1, S(L"abcde5fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 4, 2, S(L"abcde5fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 5, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 0, 1, S(L"abcde1fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 0, 5, S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 0, 9, S(L"abcde123456789fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 0, 10, S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 0, 11, S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 1, 1, S(L"abcde2fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 1, 4, S(L"abcde2345fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 1, 8, S(L"abcde23456789fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 1, 9, S(L"abcde234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 1, 10, S(L"abcde234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 5, 1, S(L"abcde6fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 5, 2, S(L"abcde67fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 5, 4, S(L"abcde6789fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 5, 5, S(L"abcde67890fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 5, 6, S(L"abcde67890fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 9, 1, S(L"abcde0fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 9, 2, S(L"abcde0fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 0, 1, S(L"abcde1fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 0, 10, S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 0, 19, S(L"abcde1234567890123456789fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 0, 20, S(L"abcde12345678901234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 0, 21, S(L"abcde12345678901234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 1, 1, S(L"abcde2fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 1, 9, S(L"abcde234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 1, 18, S(L"abcde234567890123456789fghij"));
}

template <class S>
void test16()
{
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 1, 19, S(L"abcde2345678901234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 1, 20, S(L"abcde2345678901234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 10, 1, S(L"abcde1fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 10, 5, S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 10, 9, S(L"abcde123456789fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 10, 10, S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 10, 11, S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 19, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 19, 1, S(L"abcde0fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 19, 2, S(L"abcde0fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 20, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 20, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 9, S(L""), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L""), 0, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 0, 1, S(L"abcdefghi1j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 0, 2, S(L"abcdefghi12j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 0, 4, S(L"abcdefghi1234j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 0, 5, S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 0, 6, S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 1, 1, S(L"abcdefghi2j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 1, 2, S(L"abcdefghi23j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 1, 3, S(L"abcdefghi234j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 1, 4, S(L"abcdefghi2345j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 1, 5, S(L"abcdefghi2345j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 2, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 2, 1, S(L"abcdefghi3j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 2, 2, S(L"abcdefghi34j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 2, 3, S(L"abcdefghi345j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 2, 4, S(L"abcdefghi345j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 4, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 4, 1, S(L"abcdefghi5j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 4, 2, S(L"abcdefghi5j"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 5, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 0, 1, S(L"abcdefghi1j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 0, 5, S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 0, 9, S(L"abcdefghi123456789j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 0, 10, S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 0, 11, S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 1, 1, S(L"abcdefghi2j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 1, 4, S(L"abcdefghi2345j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 1, 8, S(L"abcdefghi23456789j"));
}

template <class S>
void test17()
{
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 1, 9, S(L"abcdefghi234567890j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 1, 10, S(L"abcdefghi234567890j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 5, 1, S(L"abcdefghi6j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 5, 2, S(L"abcdefghi67j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 5, 4, S(L"abcdefghi6789j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 5, 5, S(L"abcdefghi67890j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 5, 6, S(L"abcdefghi67890j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 9, 1, S(L"abcdefghi0j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 9, 2, S(L"abcdefghi0j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 0, 1, S(L"abcdefghi1j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 0, 10, S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 0, 19, S(L"abcdefghi1234567890123456789j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 0, 20, S(L"abcdefghi12345678901234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 0, 21, S(L"abcdefghi12345678901234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 1, 1, S(L"abcdefghi2j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 1, 9, S(L"abcdefghi234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 1, 18, S(L"abcdefghi234567890123456789j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 1, 19, S(L"abcdefghi2345678901234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 1, 20, S(L"abcdefghi2345678901234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 10, 1, S(L"abcdefghi1j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 10, 5, S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 10, 9, S(L"abcdefghi123456789j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 10, 10, S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 10, 11, S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 19, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 19, 1, S(L"abcdefghi0j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 19, 2, S(L"abcdefghi0j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 20, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 20, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 10, S(L""), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L""), 0, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 0, 1, S(L"abcdefghij1"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 0, 2, S(L"abcdefghij12"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 0, 4, S(L"abcdefghij1234"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 0, 5, S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 0, 6, S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 1, 1, S(L"abcdefghij2"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 1, 2, S(L"abcdefghij23"));
}

template <class S>
void test18()
{
    test(S(L"abcdefghij"), 10, S(L"12345"), 1, 3, S(L"abcdefghij234"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 1, 4, S(L"abcdefghij2345"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 1, 5, S(L"abcdefghij2345"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 2, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 2, 1, S(L"abcdefghij3"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 2, 2, S(L"abcdefghij34"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 2, 3, S(L"abcdefghij345"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 2, 4, S(L"abcdefghij345"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 4, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 4, 1, S(L"abcdefghij5"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 4, 2, S(L"abcdefghij5"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 5, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 0, 1, S(L"abcdefghij1"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 0, 5, S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 0, 9, S(L"abcdefghij123456789"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 0, 10, S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 0, 11, S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 1, 1, S(L"abcdefghij2"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 1, 4, S(L"abcdefghij2345"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 1, 8, S(L"abcdefghij23456789"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 1, 9, S(L"abcdefghij234567890"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 1, 10, S(L"abcdefghij234567890"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 5, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 5, 1, S(L"abcdefghij6"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 5, 2, S(L"abcdefghij67"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 5, 4, S(L"abcdefghij6789"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 5, 5, S(L"abcdefghij67890"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 5, 6, S(L"abcdefghij67890"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 9, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 9, 1, S(L"abcdefghij0"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 9, 2, S(L"abcdefghij0"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 10, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 0, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 0, 1, S(L"abcdefghij1"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 0, 10, S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 0, 19, S(L"abcdefghij1234567890123456789"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 0, 20, S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 0, 21, S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 1, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 1, 1, S(L"abcdefghij2"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 1, 9, S(L"abcdefghij234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 1, 18, S(L"abcdefghij234567890123456789"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 1, 19, S(L"abcdefghij2345678901234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 1, 20, S(L"abcdefghij2345678901234567890"));
}

template <class S>
void test19()
{
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 10, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 10, 1, S(L"abcdefghij1"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 10, 5, S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 10, 9, S(L"abcdefghij123456789"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 10, 10, S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 10, 11, S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 19, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 19, 1, S(L"abcdefghij0"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 19, 2, S(L"abcdefghij0"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 20, 0, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 20, 1, S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L""), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L""), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 0, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 0, 4, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 0, 5, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 0, 6, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 1, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 1, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 1, 3, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 1, 4, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 1, 5, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 2, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 2, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 2, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 2, 3, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 2, 4, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 4, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 4, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 4, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 5, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 5, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 0, 5, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 0, 9, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 0, 10, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 0, 11, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 1, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 1, 4, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 1, 8, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 1, 9, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 1, 10, S(L"can't happen"));
}

template <class S>
void test20()
{
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 5, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 5, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 5, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 5, 4, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 5, 5, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 5, 6, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 9, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 9, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 9, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 10, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 10, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 0, 10, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 0, 19, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 0, 20, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 0, 21, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 1, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 1, 9, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 1, 18, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 1, 19, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 1, 20, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 10, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 10, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 10, 5, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 10, 9, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 10, 10, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 10, 11, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 19, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 19, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 19, 2, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 20, 0, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 20, 1, S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""), 0, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 0, 1, S(L"1abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 0, 2, S(L"12abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 0, 4, S(L"1234abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 0, 5, S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 0, 6, S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 1, 1, S(L"2abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 1, 2, S(L"23abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 1, 3, S(L"234abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 1, 4, S(L"2345abcdefghijklmnopqrst"));
}

template <class S>
void test21()
{
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 1, 5, S(L"2345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 2, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 2, 1, S(L"3abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 2, 2, S(L"34abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 2, 3, S(L"345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 2, 4, S(L"345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 4, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 4, 1, S(L"5abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 4, 2, S(L"5abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 5, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 0, 1, S(L"1abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 0, 5, S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 0, 9, S(L"123456789abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 0, 10, S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 0, 11, S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 1, 1, S(L"2abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 1, 4, S(L"2345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 1, 8, S(L"23456789abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 1, 9, S(L"234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 1, 10, S(L"234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 5, 1, S(L"6abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 5, 2, S(L"67abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 5, 4, S(L"6789abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 5, 5, S(L"67890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 5, 6, S(L"67890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 9, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 9, 1, S(L"0abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 9, 2, S(L"0abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 10, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 0, 1, S(L"1abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 0, 10, S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 0, 19, S(L"1234567890123456789abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 0, 20, S(L"12345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 0, 21, S(L"12345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 1, 1, S(L"2abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 1, 9, S(L"234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 1, 18, S(L"234567890123456789abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 1, 19, S(L"2345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 1, 20, S(L"2345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 10, 1, S(L"1abcdefghijklmnopqrst"));
}

template <class S>
void test22()
{
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 10, 5, S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 10, 9, S(L"123456789abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 10, 10, S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 10, 11, S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 19, 1, S(L"0abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 19, 2, S(L"0abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L""), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L""), 0, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 0, 1, S(L"a1bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 0, 2, S(L"a12bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 0, 4, S(L"a1234bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 0, 5, S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 0, 6, S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 1, 1, S(L"a2bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 1, 2, S(L"a23bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 1, 3, S(L"a234bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 1, 4, S(L"a2345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 1, 5, S(L"a2345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 2, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 2, 1, S(L"a3bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 2, 2, S(L"a34bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 2, 3, S(L"a345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 2, 4, S(L"a345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 4, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 4, 1, S(L"a5bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 4, 2, S(L"a5bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 5, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 0, 1, S(L"a1bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 0, 5, S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 0, 9, S(L"a123456789bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 0, 10, S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 0, 11, S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 1, 1, S(L"a2bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 1, 4, S(L"a2345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 1, 8, S(L"a23456789bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 1, 9, S(L"a234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 1, 10, S(L"a234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 5, 1, S(L"a6bcdefghijklmnopqrst"));
}

template <class S>
void test23()
{
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 5, 2, S(L"a67bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 5, 4, S(L"a6789bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 5, 5, S(L"a67890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 5, 6, S(L"a67890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 9, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 9, 1, S(L"a0bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 9, 2, S(L"a0bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 10, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 0, 1, S(L"a1bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 0, 10, S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 0, 19, S(L"a1234567890123456789bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 0, 20, S(L"a12345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 0, 21, S(L"a12345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 1, 1, S(L"a2bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 1, 9, S(L"a234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 1, 18, S(L"a234567890123456789bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 1, 19, S(L"a2345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 1, 20, S(L"a2345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 10, 1, S(L"a1bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 10, 5, S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 10, 9, S(L"a123456789bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 10, 10, S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 10, 11, S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 19, 1, S(L"a0bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 19, 2, S(L"a0bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L""), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L""), 0, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, 1, S(L"abcdefghij1klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, 2, S(L"abcdefghij12klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, 4, S(L"abcdefghij1234klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, 5, S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, 6, S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, 1, S(L"abcdefghij2klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, 2, S(L"abcdefghij23klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, 3, S(L"abcdefghij234klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, 4, S(L"abcdefghij2345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, 5, S(L"abcdefghij2345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 2, 0, S(L"abcdefghijklmnopqrst"));
}

template <class S>
void test24()
{
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 2, 1, S(L"abcdefghij3klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 2, 2, S(L"abcdefghij34klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 2, 3, S(L"abcdefghij345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 2, 4, S(L"abcdefghij345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 4, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 4, 1, S(L"abcdefghij5klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 4, 2, S(L"abcdefghij5klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 5, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 0, 1, S(L"abcdefghij1klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 0, 5, S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 0, 9, S(L"abcdefghij123456789klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 0, 10, S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 0, 11, S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 1, 1, S(L"abcdefghij2klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 1, 4, S(L"abcdefghij2345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 1, 8, S(L"abcdefghij23456789klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 1, 9, S(L"abcdefghij234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 1, 10, S(L"abcdefghij234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 5, 1, S(L"abcdefghij6klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 5, 2, S(L"abcdefghij67klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 5, 4, S(L"abcdefghij6789klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 5, 5, S(L"abcdefghij67890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 5, 6, S(L"abcdefghij67890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 9, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 9, 1, S(L"abcdefghij0klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 9, 2, S(L"abcdefghij0klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 10, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 0, 1, S(L"abcdefghij1klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 0, 10, S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 0, 19, S(L"abcdefghij1234567890123456789klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 0, 20, S(L"abcdefghij12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 0, 21, S(L"abcdefghij12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 1, 1, S(L"abcdefghij2klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 1, 9, S(L"abcdefghij234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 1, 18, S(L"abcdefghij234567890123456789klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 1, 19, S(L"abcdefghij2345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 1, 20, S(L"abcdefghij2345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 10, 1, S(L"abcdefghij1klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 10, 5, S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 10, 9, S(L"abcdefghij123456789klmnopqrst"));
}

template <class S>
void test25()
{
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 10, 10, S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 10, 11, S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 19, 1, S(L"abcdefghij0klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 19, 2, S(L"abcdefghij0klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L""), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L""), 0, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 0, 1, S(L"abcdefghijklmnopqrs1t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 0, 2, S(L"abcdefghijklmnopqrs12t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 0, 4, S(L"abcdefghijklmnopqrs1234t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 0, 5, S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 0, 6, S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 1, 1, S(L"abcdefghijklmnopqrs2t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 1, 2, S(L"abcdefghijklmnopqrs23t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 1, 3, S(L"abcdefghijklmnopqrs234t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 1, 4, S(L"abcdefghijklmnopqrs2345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 1, 5, S(L"abcdefghijklmnopqrs2345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 2, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 2, 1, S(L"abcdefghijklmnopqrs3t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 2, 2, S(L"abcdefghijklmnopqrs34t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 2, 3, S(L"abcdefghijklmnopqrs345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 2, 4, S(L"abcdefghijklmnopqrs345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 4, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 4, 1, S(L"abcdefghijklmnopqrs5t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 4, 2, S(L"abcdefghijklmnopqrs5t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 5, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 0, 1, S(L"abcdefghijklmnopqrs1t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 0, 5, S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 0, 9, S(L"abcdefghijklmnopqrs123456789t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 0, 10, S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 0, 11, S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 1, 1, S(L"abcdefghijklmnopqrs2t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 1, 4, S(L"abcdefghijklmnopqrs2345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 1, 8, S(L"abcdefghijklmnopqrs23456789t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 1, 9, S(L"abcdefghijklmnopqrs234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 1, 10, S(L"abcdefghijklmnopqrs234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 5, 1, S(L"abcdefghijklmnopqrs6t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 5, 2, S(L"abcdefghijklmnopqrs67t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 5, 4, S(L"abcdefghijklmnopqrs6789t"));
}

template <class S>
void test26()
{
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 5, 5, S(L"abcdefghijklmnopqrs67890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 5, 6, S(L"abcdefghijklmnopqrs67890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 9, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 9, 1, S(L"abcdefghijklmnopqrs0t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 9, 2, S(L"abcdefghijklmnopqrs0t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 10, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 0, 1, S(L"abcdefghijklmnopqrs1t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 0, 10, S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 0, 19, S(L"abcdefghijklmnopqrs1234567890123456789t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 0, 20, S(L"abcdefghijklmnopqrs12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 0, 21, S(L"abcdefghijklmnopqrs12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 1, 1, S(L"abcdefghijklmnopqrs2t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 1, 9, S(L"abcdefghijklmnopqrs234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 1, 18, S(L"abcdefghijklmnopqrs234567890123456789t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 1, 19, S(L"abcdefghijklmnopqrs2345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 1, 20, S(L"abcdefghijklmnopqrs2345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 10, 1, S(L"abcdefghijklmnopqrs1t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 10, 5, S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 10, 9, S(L"abcdefghijklmnopqrs123456789t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 10, 10, S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 10, 11, S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 19, 1, S(L"abcdefghijklmnopqrs0t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 19, 2, S(L"abcdefghijklmnopqrs0t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L""), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L""), 0, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 0, 1, S(L"abcdefghijklmnopqrst1"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 0, 2, S(L"abcdefghijklmnopqrst12"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 0, 4, S(L"abcdefghijklmnopqrst1234"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 0, 5, S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 0, 6, S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 1, 1, S(L"abcdefghijklmnopqrst2"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 1, 2, S(L"abcdefghijklmnopqrst23"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 1, 3, S(L"abcdefghijklmnopqrst234"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 1, 4, S(L"abcdefghijklmnopqrst2345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 1, 5, S(L"abcdefghijklmnopqrst2345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 2, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 2, 1, S(L"abcdefghijklmnopqrst3"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 2, 2, S(L"abcdefghijklmnopqrst34"));
}

template <class S>
void test27()
{
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 2, 3, S(L"abcdefghijklmnopqrst345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 2, 4, S(L"abcdefghijklmnopqrst345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 4, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 4, 1, S(L"abcdefghijklmnopqrst5"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 4, 2, S(L"abcdefghijklmnopqrst5"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 5, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 0, 1, S(L"abcdefghijklmnopqrst1"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 0, 5, S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 0, 9, S(L"abcdefghijklmnopqrst123456789"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 0, 10, S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 0, 11, S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 1, 1, S(L"abcdefghijklmnopqrst2"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 1, 4, S(L"abcdefghijklmnopqrst2345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 1, 8, S(L"abcdefghijklmnopqrst23456789"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 1, 9, S(L"abcdefghijklmnopqrst234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 1, 10, S(L"abcdefghijklmnopqrst234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 5, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 5, 1, S(L"abcdefghijklmnopqrst6"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 5, 2, S(L"abcdefghijklmnopqrst67"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 5, 4, S(L"abcdefghijklmnopqrst6789"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 5, 5, S(L"abcdefghijklmnopqrst67890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 5, 6, S(L"abcdefghijklmnopqrst67890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 9, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 9, 1, S(L"abcdefghijklmnopqrst0"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 9, 2, S(L"abcdefghijklmnopqrst0"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 10, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 0, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 0, 1, S(L"abcdefghijklmnopqrst1"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 0, 10, S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 0, 19, S(L"abcdefghijklmnopqrst1234567890123456789"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 0, 20, S(L"abcdefghijklmnopqrst12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 0, 21, S(L"abcdefghijklmnopqrst12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 1, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 1, 1, S(L"abcdefghijklmnopqrst2"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 1, 9, S(L"abcdefghijklmnopqrst234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 1, 18, S(L"abcdefghijklmnopqrst234567890123456789"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 1, 19, S(L"abcdefghijklmnopqrst2345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 1, 20, S(L"abcdefghijklmnopqrst2345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 10, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 10, 1, S(L"abcdefghijklmnopqrst1"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 10, 5, S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 10, 9, S(L"abcdefghijklmnopqrst123456789"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 10, 10, S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 10, 11, S(L"abcdefghijklmnopqrst1234567890"));
}

template <class S>
void test28()
{
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 19, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 19, 1, S(L"abcdefghijklmnopqrst0"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 19, 2, S(L"abcdefghijklmnopqrst0"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 20, 0, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 20, 1, S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L""), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L""), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L""), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 0, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 0, 4, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 0, 5, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 0, 6, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 1, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 1, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 1, 3, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 1, 4, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 1, 5, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 2, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 2, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 2, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 2, 3, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 2, 4, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 4, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 4, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 4, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 5, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 5, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), 6, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 0, 5, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 0, 9, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 0, 10, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 0, 11, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 1, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 1, 4, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 1, 8, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 1, 9, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 1, 10, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 5, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 5, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 5, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 5, 4, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 5, 5, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 5, 6, S(L"can't happen"));
}

template <class S>
void test29()
{
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 9, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 9, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 9, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 10, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 10, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), 11, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 0, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 0, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 0, 10, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 0, 19, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 0, 20, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 0, 21, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 1, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 1, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 1, 9, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 1, 18, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 1, 19, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 1, 20, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 10, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 10, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 10, 5, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 10, 9, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 10, 10, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 10, 11, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 19, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 19, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 19, 2, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 20, 0, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 20, 1, S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), 21, 0, S(L"can't happen"));
}

template <class S>
void test30()
{
    test_npos(S(L""), 0, S(L"12345678901234567890"),  0, S(L"12345678901234567890"));
    test_npos(S(L""), 0, S(L"12345678901234567890"),  1, S(L"2345678901234567890"));
    test_npos(S(L""), 0, S(L"12345678901234567890"),  2, S( L"345678901234567890"));
    test_npos(S(L""), 0, S(L"12345678901234567890"),  3, S(  L"45678901234567890"));
    test_npos(S(L""), 0, S(L"12345678901234567890"),  5, S(    L"678901234567890"));
    test_npos(S(L""), 0, S(L"12345678901234567890"), 10, S(         L"1234567890"));
    test_npos(S(L""), 0, S(L"12345678901234567890"), 21, S(L"can't happen"));
    test_npos(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 0, S(L"abcdefghij12345klmnopqrst"));
    test_npos(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 1, S(L"abcdefghij2345klmnopqrst"));
    test_npos(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 3, S(L"abcdefghij45klmnopqrst"));
    test_npos(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 5, S(L"abcdefghijklmnopqrst"));
    test_npos(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), 6, S(L"can't happen"));
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0<S>();
    test1<S>();
    test2<S>();
    test3<S>();
    test4<S>();
    test5<S>();
    test6<S>();
    test7<S>();
    test8<S>();
    test9<S>();
    test10<S>();
    test11<S>();
    test12<S>();
    test13<S>();
    test14<S>();
    test15<S>();
    test16<S>();
    test17<S>();
    test18<S>();
    test19<S>();
    test20<S>();
    test21<S>();
    test22<S>();
    test23<S>();
    test24<S>();
    test25<S>();
    test26<S>();
    test27<S>();
    test28<S>();
    test29<S>();
    test30<S>();
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0<S>();
    test1<S>();
    test2<S>();
    test3<S>();
    test4<S>();
    test5<S>();
    test6<S>();
    test7<S>();
    test8<S>();
    test9<S>();
    test10<S>();
    test11<S>();
    test12<S>();
    test13<S>();
    test14<S>();
    test15<S>();
    test16<S>();
    test17<S>();
    test18<S>();
    test19<S>();
    test20<S>();
    test21<S>();
    test22<S>();
    test23<S>();
    test24<S>();
    test25<S>();
    test26<S>();
    test27<S>();
    test28<S>();
    test29<S>();
    test30<S>();
    }
#endif
}
