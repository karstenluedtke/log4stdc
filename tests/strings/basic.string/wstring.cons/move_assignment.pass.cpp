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
//   operator=(basic_string<charT,traits,Allocator>&& str);

#include <string>
#include <cassert>

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

#include "tests/support/test_allocator.h"
#include "tests/support/min_allocator.h"

template <class S>
void
test(S s1, S s2)
{
    S s0 = s2;
    s1 = std::move(s2);
    assert(s1.__invariants());
    assert(s2.__invariants());
    assert(s1 == s0);
    assert(s1.capacity() >= s1.size());
}

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

int main()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), S());
    test(S(L"1"), S());
    test(S(), S(L"1"));
    test(S(L"1"), S(L"2"));
    test(S(L"1"), S(L"2"));

    test(S(),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S(L"123456789"),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"
           "1234567890123456789012345678901234567890123456789012345678901234567890"),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), S());
    test(S(L"1"), S());
    test(S(), S(L"1"));
    test(S(L"1"), S(L"2"));
    test(S(L"1"), S(L"2"));

    test(S(),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S(L"123456789"),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"
           "1234567890123456789012345678901234567890123456789012345678901234567890"),
         S(L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    }
#endif
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}
