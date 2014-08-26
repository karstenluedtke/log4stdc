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

// basic_string<charT,traits,Allocator>& append(const charT* s);

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, const typename S::value_type* str, S expected)
{
    s.append(str);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), L"", S());
    test(S(), L"12345", S(L"12345"));
    test(S(), L"12345678901234567890", S(L"12345678901234567890"));

    test(S(L"12345"), L"", S(L"12345"));
    test(S(L"12345"), L"12345", S(L"1234512345"));
    test(S(L"12345"), L"1234567890", S(L"123451234567890"));

    test(S(L"12345678901234567890"), L"", S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), L"12345", S(L"1234567890123456789012345"));
    test(S(L"12345678901234567890"), L"12345678901234567890",
         S(L"1234567890123456789012345678901234567890"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), L"", S());
    test(S(), L"12345", S(L"12345"));
    test(S(), L"12345678901234567890", S(L"12345678901234567890"));

    test(S(L"12345"), L"", S(L"12345"));
    test(S(L"12345"), L"12345", S(L"1234512345"));
    test(S(L"12345"), L"1234567890", S(L"123451234567890"));

    test(S(L"12345678901234567890"), L"", S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), L"12345", S(L"1234567890123456789012345"));
    test(S(L"12345678901234567890"), L"12345678901234567890",
         S(L"1234567890123456789012345678901234567890"));
    }
#endif
}
