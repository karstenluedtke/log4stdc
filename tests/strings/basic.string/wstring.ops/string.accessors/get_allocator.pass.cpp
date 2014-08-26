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

// allocator_type get_allocator() const;

#include <string>
#include <cassert>

#include "tests/support/test_allocator.h"
#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const typename S::allocator_type& a)
{
    assert(s.get_allocator() == a);
}

int main()
{
    {
    typedef test_allocator<char> A;
    typedef barefootc::basic_string<char, std::char_traits<char>, A> S;
    test(S(L""), A());
    test(S(L"abcde", A(1)), A(1));
    test(S(L"abcdefghij", A(2)), A(2));
    test(S(L"abcdefghijklmnopqrst", A(3)), A(3));
    }
#if __cplusplus >= 201103L
    {
    typedef min_allocator<char> A;
    typedef barefootc::basic_string<char, std::char_traits<char>, A> S;
    test(S(L""), A());
    test(S(L"abcde", A()), A());
    test(S(L"abcdefghij", A()), A());
    test(S(L"abcdefghijklmnopqrst", A()), A());
    }
#endif
}
