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

// basic_string(const basic_string<charT,traits,Allocator>& str);

#include <string>
#include <cassert>

#include "tests/support/test_allocator.h"
#include "tests/support/min_allocator.h"

template <class S>
void
test(S s1)
{
    S s2 = s1;
    assert(s2.__invariants());
    assert(s2 == s1);
    assert(s2.capacity() >= s2.size());
    assert(s2.get_allocator() == s1.get_allocator());
}

int main()
{
    {
    typedef test_allocator<wchar_t> A;
    typedef barefootc::basic_string<wchar_t, std::char_traits<wchar_t>, A> S;
    test(S(A(3)));
    test(S(L"1", A(5)));
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890", A(7)));
    }
#if __cplusplus >= 201103L
    {
    typedef min_allocator<wchar_t> A;
    typedef barefootc::basic_string<wchar_t, std::char_traits<wchar_t>, A> S;
    test(S(A{}));
    test(S(L"1", A()));
    test(S(L"1234567890123456789012345678901234567890123456789012345678901234567890", A()));
    }
#endif
}
