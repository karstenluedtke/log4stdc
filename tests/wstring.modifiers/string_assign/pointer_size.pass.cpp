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
//   assign(const charT* s, size_type n);

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, const typename S::value_type* str, typename S::size_type n, S expected)
{
    s.assign(str, n);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), L"", 0, S());
    test(S(), L"12345", 3, S(L"123"));
    test(S(), L"12345", 4, S(L"1234"));
    test(S(), L"12345678901234567890", 0, S());
    test(S(), L"12345678901234567890", 1, S(L"1"));
    test(S(), L"12345678901234567890", 3, S(L"123"));
    test(S(), L"12345678901234567890", 20, S(L"12345678901234567890"));

    test(S(L"12345"), L"", 0, S());
    test(S(L"12345"), L"12345", 5, S(L"12345"));
    test(S(L"12345"), L"1234567890", 10, S(L"1234567890"));

    test(S(L"12345678901234567890"), L"", 0, S());
    test(S(L"12345678901234567890"), L"12345", 5, S(L"12345"));
    test(S(L"12345678901234567890"), L"12345678901234567890", 20,
         S(L"12345678901234567890"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), L"", 0, S());
    test(S(), L"12345", 3, S(L"123"));
    test(S(), L"12345", 4, S(L"1234"));
    test(S(), L"12345678901234567890", 0, S());
    test(S(), L"12345678901234567890", 1, S(L"1"));
    test(S(), L"12345678901234567890", 3, S(L"123"));
    test(S(), L"12345678901234567890", 20, S(L"12345678901234567890"));

    test(S(L"12345"), L"", 0, S());
    test(S(L"12345"), L"12345", 5, S(L"12345"));
    test(S(L"12345"), L"1234567890", 10, S(L"1234567890"));

    test(S(L"12345678901234567890"), L"", 0, S());
    test(S(L"12345678901234567890"), L"12345", 5, S(L"12345"));
    test(S(L"12345678901234567890"), L"12345678901234567890", 20,
         S(L"12345678901234567890"));
    }
#endif
}
