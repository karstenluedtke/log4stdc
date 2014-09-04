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
//   operator+(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(basic_string<charT,traits,Allocator>&& lhs, const charT* rhs);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test0(const S& lhs, const typename S::value_type* rhs, const S& x)
{
    assert(lhs + rhs == x);
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class S>
void
test1(S&& lhs, const typename S::value_type* rhs, const S& x)
{
    assert(move(lhs) + rhs == x);
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0(S(L""), L"", S(L""));
    test0(S(L""), L"12345", S(L"12345"));
    test0(S(L""), L"1234567890", S(L"1234567890"));
    test0(S(L""), L"12345678901234567890", S(L"12345678901234567890"));
    test0(S(L"abcde"), L"", S(L"abcde"));
    test0(S(L"abcde"), L"12345", S(L"abcde12345"));
    test0(S(L"abcde"), L"1234567890", S(L"abcde1234567890"));
    test0(S(L"abcde"), L"12345678901234567890", S(L"abcde12345678901234567890"));
    test0(S(L"abcdefghij"), L"", S(L"abcdefghij"));
    test0(S(L"abcdefghij"), L"12345", S(L"abcdefghij12345"));
    test0(S(L"abcdefghij"), L"1234567890", S(L"abcdefghij1234567890"));
    test0(S(L"abcdefghij"), L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test0(S(L"abcdefghijklmnopqrst"), L"", S(L"abcdefghijklmnopqrst"));
    test0(S(L"abcdefghijklmnopqrst"), L"12345", S(L"abcdefghijklmnopqrst12345"));
    test0(S(L"abcdefghijklmnopqrst"), L"1234567890", S(L"abcdefghijklmnopqrst1234567890"));
    test0(S(L"abcdefghijklmnopqrst"), L"12345678901234567890", S(L"abcdefghijklmnopqrst12345678901234567890"));

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    test1(S(L""), L"", S(L""));
    test1(S(L""), L"12345", S(L"12345"));
    test1(S(L""), L"1234567890", S(L"1234567890"));
    test1(S(L""), L"12345678901234567890", S(L"12345678901234567890"));
    test1(S(L"abcde"), L"", S(L"abcde"));
    test1(S(L"abcde"), L"12345", S(L"abcde12345"));
    test1(S(L"abcde"), L"1234567890", S(L"abcde1234567890"));
    test1(S(L"abcde"), L"12345678901234567890", S(L"abcde12345678901234567890"));
    test1(S(L"abcdefghij"), L"", S(L"abcdefghij"));
    test1(S(L"abcdefghij"), L"12345", S(L"abcdefghij12345"));
    test1(S(L"abcdefghij"), L"1234567890", S(L"abcdefghij1234567890"));
    test1(S(L"abcdefghij"), L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test1(S(L"abcdefghijklmnopqrst"), L"", S(L"abcdefghijklmnopqrst"));
    test1(S(L"abcdefghijklmnopqrst"), L"12345", S(L"abcdefghijklmnopqrst12345"));
    test1(S(L"abcdefghijklmnopqrst"), L"1234567890", S(L"abcdefghijklmnopqrst1234567890"));
    test1(S(L"abcdefghijklmnopqrst"), L"12345678901234567890", S(L"abcdefghijklmnopqrst12345678901234567890"));

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0(S(L""), L"", S(L""));
    test0(S(L""), L"12345", S(L"12345"));
    test0(S(L""), L"1234567890", S(L"1234567890"));
    test0(S(L""), L"12345678901234567890", S(L"12345678901234567890"));
    test0(S(L"abcde"), L"", S(L"abcde"));
    test0(S(L"abcde"), L"12345", S(L"abcde12345"));
    test0(S(L"abcde"), L"1234567890", S(L"abcde1234567890"));
    test0(S(L"abcde"), L"12345678901234567890", S(L"abcde12345678901234567890"));
    test0(S(L"abcdefghij"), L"", S(L"abcdefghij"));
    test0(S(L"abcdefghij"), L"12345", S(L"abcdefghij12345"));
    test0(S(L"abcdefghij"), L"1234567890", S(L"abcdefghij1234567890"));
    test0(S(L"abcdefghij"), L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test0(S(L"abcdefghijklmnopqrst"), L"", S(L"abcdefghijklmnopqrst"));
    test0(S(L"abcdefghijklmnopqrst"), L"12345", S(L"abcdefghijklmnopqrst12345"));
    test0(S(L"abcdefghijklmnopqrst"), L"1234567890", S(L"abcdefghijklmnopqrst1234567890"));
    test0(S(L"abcdefghijklmnopqrst"), L"12345678901234567890", S(L"abcdefghijklmnopqrst12345678901234567890"));

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    test1(S(L""), L"", S(L""));
    test1(S(L""), L"12345", S(L"12345"));
    test1(S(L""), L"1234567890", S(L"1234567890"));
    test1(S(L""), L"12345678901234567890", S(L"12345678901234567890"));
    test1(S(L"abcde"), L"", S(L"abcde"));
    test1(S(L"abcde"), L"12345", S(L"abcde12345"));
    test1(S(L"abcde"), L"1234567890", S(L"abcde1234567890"));
    test1(S(L"abcde"), L"12345678901234567890", S(L"abcde12345678901234567890"));
    test1(S(L"abcdefghij"), L"", S(L"abcdefghij"));
    test1(S(L"abcdefghij"), L"12345", S(L"abcdefghij12345"));
    test1(S(L"abcdefghij"), L"1234567890", S(L"abcdefghij1234567890"));
    test1(S(L"abcdefghij"), L"12345678901234567890", S(L"abcdefghij12345678901234567890"));
    test1(S(L"abcdefghijklmnopqrst"), L"", S(L"abcdefghijklmnopqrst"));
    test1(S(L"abcdefghijklmnopqrst"), L"12345", S(L"abcdefghijklmnopqrst12345"));
    test1(S(L"abcdefghijklmnopqrst"), L"1234567890", S(L"abcdefghijklmnopqrst1234567890"));
    test1(S(L"abcdefghijklmnopqrst"), L"12345678901234567890", S(L"abcdefghijklmnopqrst12345678901234567890"));

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    }
#endif
}
