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

// const charT& front() const;
//       charT& front();

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s)
{
    const S& cs = s;
    assert(&cs.front() == &cs[0]);
    assert(&s.front() == &s[0]);
    s.front() = typename S::value_type('z');
    assert(s.front() == typename S::value_type('z'));
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L"1"));
    test(S(L"1234567890123456789012345678901234567890"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L"1"));
    test(S(L"1234567890123456789012345678901234567890"));
    }
#endif
#ifdef _LIBCPP_DEBUG
    {
        barefootc::basic_string<wchar_t> s;
        char c = s.front();
        assert(false);
    }
#endif
}
