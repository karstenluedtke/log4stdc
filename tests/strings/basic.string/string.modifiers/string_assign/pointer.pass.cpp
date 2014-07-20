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

// basic_string<charT,traits,Allocator>& assign(const charT* s);

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, const typename S::value_type* str, S expected)
{
    s.assign(str);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<char> S;
    test(S(), "", S());
    test(S(), "12345", S("12345"));
    test(S(), "12345678901234567890", S("12345678901234567890"));

    test(S("12345"), "", S());
    test(S("12345"), "12345", S("12345"));
    test(S("12345"), "1234567890", S("1234567890"));

    test(S("12345678901234567890"), "", S());
    test(S("12345678901234567890"), "12345", S("12345"));
    test(S("12345678901234567890"), "12345678901234567890",
         S("12345678901234567890"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), "", S());
    test(S(), "12345", S("12345"));
    test(S(), "12345678901234567890", S("12345678901234567890"));

    test(S("12345"), "", S());
    test(S("12345"), "12345", S("12345"));
    test(S("12345"), "1234567890", S("1234567890"));

    test(S("12345678901234567890"), "", S());
    test(S("12345678901234567890"), "12345", S("12345"));
    test(S("12345678901234567890"), "12345678901234567890",
         S("12345678901234567890"));
    }
#endif
}
