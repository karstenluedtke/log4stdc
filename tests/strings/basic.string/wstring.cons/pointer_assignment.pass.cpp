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
//   operator=(const charT* s);

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s1, const typename S::value_type* s2)
{
    typedef typename S::traits_type T;
    s1 = s2;
    assert(s1.__invariants());
    assert(s1.size() == T::length(s2));
    assert(T::compare(s1.data(), s2, s1.size()) == 0);
    assert(s1.capacity() >= s1.size());
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(), L"");
    test(S(L"1"), L"");
    test(S(), L"1");
    test(S(L"1"), L"2");
    test(S(L"1"), L"2");

    test(S(),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S(L"123456789"),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"
          L"1234567890123456789012345678901234567890123456789012345678901234567890"),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(), L"");
    test(S(L"1"), L"");
    test(S(), L"1");
    test(S(L"1"), L"2");
    test(S(L"1"), L"2");

    test(S(),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S(L"123456789"),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890"
          L"1234567890123456789012345678901234567890123456789012345678901234567890"),
        L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
    }
#endif
}
