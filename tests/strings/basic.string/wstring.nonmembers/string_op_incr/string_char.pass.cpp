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
//   operator+(const basic_string<charT,traits,Allocator>& lhs, charT rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(basic_string<charT,traits,Allocator>&& lhs, charT rhs);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test0(const S& lhs, typename S::value_type rhs, const S& x)
{
    assert(lhs + rhs == x);
}

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

template <class S>
void
test1(S&& lhs, typename S::value_type rhs, const S& x)
{
    assert(move(lhs) + rhs == x);
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0(S(L""), '1', S(L"1"));
    test0(S(L"abcde"), '1', S(L"abcde1"));
    test0(S(L"abcdefghij"), '1', S(L"abcdefghij1"));
    test0(S(L"abcdefghijklmnopqrst"), '1', S(L"abcdefghijklmnopqrst1"));

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    test1(S(L""), '1', S(L"1"));
    test1(S(L"abcde"), '1', S(L"abcde1"));
    test1(S(L"abcdefghij"), '1', S(L"abcdefghij1"));
    test1(S(L"abcdefghijklmnopqrst"), '1', S(L"abcdefghijklmnopqrst1"));

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0(S(L""), '1', S(L"1"));
    test0(S(L"abcde"), '1', S(L"abcde1"));
    test0(S(L"abcdefghij"), '1', S(L"abcdefghij1"));
    test0(S(L"abcdefghijklmnopqrst"), '1', S(L"abcdefghijklmnopqrst1"));

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    test1(S(L""), '1', S(L"1"));
    test1(S(L"abcde"), '1', S(L"abcde1"));
    test1(S(L"abcdefghij"), '1', S(L"abcdefghij1"));
    test1(S(L"abcdefghijklmnopqrst"), '1', S(L"abcdefghijklmnopqrst1"));

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
    }
#endif
}
