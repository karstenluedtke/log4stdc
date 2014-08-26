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

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>
//   operator+(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(const charT* lhs, basic_string<charT,traits,Allocator>&& rhs);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test0(const typename S::value_type* lhs, const S& rhs, const S& x)
{
    assert(lhs + rhs == x);
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class S>
void
test1(const typename S::value_type* lhs, S&& rhs, const S& x)
{
    assert(lhs + move(rhs) == x);
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0(L"", S(L""), S(L""));
    test0(L"", S(L"12345"), S(L"12345"));
    test0(L"", S(L"1234567890"), S(L"1234567890"));
    test0(L"", S(L"12345678901234567890"), S(L"12345678901234567890"));
    test0(L"abcde", S(L""), S(L"abcde"));
    test0(L"abcde", S(L"12345"), S(L"abcde12345"));
    test0(L"abcde", S(L"1234567890"), S(L"abcde1234567890"));
    test0(L"abcde", S(L"12345678901234567890"), S(L"abcde12345678901234567890"));
    test0(L"abcdefghij", S(L""), S(L"abcdefghij"));
    test0(L"abcdefghij", S(L"12345"), S(L"abcdefghij12345"));
    test0(L"abcdefghij", S(L"1234567890"), S(L"abcdefghij1234567890"));
    test0(L"abcdefghij", S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890"));
    test0(L"abcdefghijklmnopqrst", S(L""), S(L"abcdefghijklmnopqrst"));
    test0(L"abcdefghijklmnopqrst", S(L"12345"), S(L"abcdefghijklmnopqrst12345"));
    test0(L"abcdefghijklmnopqrst", S(L"1234567890"), S(L"abcdefghijklmnopqrst1234567890"));
    test0(L"abcdefghijklmnopqrst", S(L"12345678901234567890"), S(L"abcdefghijklmnopqrst12345678901234567890"));

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    test1(L"", S(L""), S(L""));
    test1(L"", S(L"12345"), S(L"12345"));
    test1(L"", S(L"1234567890"), S(L"1234567890"));
    test1(L"", S(L"12345678901234567890"), S(L"12345678901234567890"));
    test1(L"abcde", S(L""), S(L"abcde"));
    test1(L"abcde", S(L"12345"), S(L"abcde12345"));
    test1(L"abcde", S(L"1234567890"), S(L"abcde1234567890"));
    test1(L"abcde", S(L"12345678901234567890"), S(L"abcde12345678901234567890"));
    test1(L"abcdefghij", S(L""), S(L"abcdefghij"));
    test1(L"abcdefghij", S(L"12345"), S(L"abcdefghij12345"));
    test1(L"abcdefghij", S(L"1234567890"), S(L"abcdefghij1234567890"));
    test1(L"abcdefghij", S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890"));
    test1(L"abcdefghijklmnopqrst", S(L""), S(L"abcdefghijklmnopqrst"));
    test1(L"abcdefghijklmnopqrst", S(L"12345"), S(L"abcdefghijklmnopqrst12345"));
    test1(L"abcdefghijklmnopqrst", S(L"1234567890"), S(L"abcdefghijklmnopqrst1234567890"));
    test1(L"abcdefghijklmnopqrst", S(L"12345678901234567890"), S(L"abcdefghijklmnopqrst12345678901234567890"));

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0(L"", S(L""), S(L""));
    test0(L"", S(L"12345"), S(L"12345"));
    test0(L"", S(L"1234567890"), S(L"1234567890"));
    test0(L"", S(L"12345678901234567890"), S(L"12345678901234567890"));
    test0(L"abcde", S(L""), S(L"abcde"));
    test0(L"abcde", S(L"12345"), S(L"abcde12345"));
    test0(L"abcde", S(L"1234567890"), S(L"abcde1234567890"));
    test0(L"abcde", S(L"12345678901234567890"), S(L"abcde12345678901234567890"));
    test0(L"abcdefghij", S(L""), S(L"abcdefghij"));
    test0(L"abcdefghij", S(L"12345"), S(L"abcdefghij12345"));
    test0(L"abcdefghij", S(L"1234567890"), S(L"abcdefghij1234567890"));
    test0(L"abcdefghij", S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890"));
    test0(L"abcdefghijklmnopqrst", S(L""), S(L"abcdefghijklmnopqrst"));
    test0(L"abcdefghijklmnopqrst", S(L"12345"), S(L"abcdefghijklmnopqrst12345"));
    test0(L"abcdefghijklmnopqrst", S(L"1234567890"), S(L"abcdefghijklmnopqrst1234567890"));
    test0(L"abcdefghijklmnopqrst", S(L"12345678901234567890"), S(L"abcdefghijklmnopqrst12345678901234567890"));

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    test1(L"", S(L""), S(L""));
    test1(L"", S(L"12345"), S(L"12345"));
    test1(L"", S(L"1234567890"), S(L"1234567890"));
    test1(L"", S(L"12345678901234567890"), S(L"12345678901234567890"));
    test1(L"abcde", S(L""), S(L"abcde"));
    test1(L"abcde", S(L"12345"), S(L"abcde12345"));
    test1(L"abcde", S(L"1234567890"), S(L"abcde1234567890"));
    test1(L"abcde", S(L"12345678901234567890"), S(L"abcde12345678901234567890"));
    test1(L"abcdefghij", S(L""), S(L"abcdefghij"));
    test1(L"abcdefghij", S(L"12345"), S(L"abcdefghij12345"));
    test1(L"abcdefghij", S(L"1234567890"), S(L"abcdefghij1234567890"));
    test1(L"abcdefghij", S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890"));
    test1(L"abcdefghijklmnopqrst", S(L""), S(L"abcdefghijklmnopqrst"));
    test1(L"abcdefghijklmnopqrst", S(L"12345"), S(L"abcdefghijklmnopqrst12345"));
    test1(L"abcdefghijklmnopqrst", S(L"1234567890"), S(L"abcdefghijklmnopqrst1234567890"));
    test1(L"abcdefghijklmnopqrst", S(L"12345678901234567890"), S(L"abcdefghijklmnopqrst12345678901234567890"));

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    }
#endif
}
