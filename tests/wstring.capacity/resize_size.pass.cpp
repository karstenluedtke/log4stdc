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

// void resize(size_type n);

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type n, S expected)
{
    try
    {
        s.resize(n);
        assert(s.__invariants());
        assert(n <= s.max_size());
        assert(s == expected);
    }
    catch (std::length_error&)
    {
        assert(n > s.max_size());
    }
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), 0, S());
    test(S(), 1, S(1, '\0'));
    test(S(), 10, S(10, '\0'));
    test(S(), 100, S(100, '\0'));
    test(S(L"12345"), 0, S());
    test(S(L"12345"), 2, S(L"12"));
    test(S(L"12345"), 5, S(L"12345"));
    test(S(L"12345"), 15, S(L"12345\0\0\0\0\0\0\0\0\0\0", 15));
    test(S(L"12345678901234567890123456789012345678901234567890"), 0, S());
    test(S(L"12345678901234567890123456789012345678901234567890"), 10,
         S(L"1234567890"));
    test(S(L"12345678901234567890123456789012345678901234567890"), 50,
         S(L"12345678901234567890123456789012345678901234567890"));
    test(S(L"12345678901234567890123456789012345678901234567890"), 60,
         S(L"12345678901234567890123456789012345678901234567890\0\0\0\0\0\0\0\0\0\0", 60));
    test(S(), S::npos, S(L"not going to happen"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), 0, S());
    test(S(), 1, S(1, '\0'));
    test(S(), 10, S(10, '\0'));
    test(S(), 100, S(100, '\0'));
    test(S(L"12345"), 0, S());
    test(S(L"12345"), 2, S(L"12"));
    test(S(L"12345"), 5, S(L"12345"));
    test(S(L"12345"), 15, S(L"12345\0\0\0\0\0\0\0\0\0\0", 15));
    test(S(L"12345678901234567890123456789012345678901234567890"), 0, S());
    test(S(L"12345678901234567890123456789012345678901234567890"), 10,
         S(L"1234567890"));
    test(S(L"12345678901234567890123456789012345678901234567890"), 50,
         S(L"12345678901234567890123456789012345678901234567890"));
    test(S(L"12345678901234567890123456789012345678901234567890"), 60,
         S(L"12345678901234567890123456789012345678901234567890\0\0\0\0\0\0\0\0\0\0", 60));
    test(S(), S::npos, S(L"not going to happen"));
    }
#endif
}
