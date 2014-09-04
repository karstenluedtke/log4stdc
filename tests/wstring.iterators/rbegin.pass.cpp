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

//       reverse_iterator rbegin();
// const_reverse_iterator rbegin() const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s)
{
    const S& cs = s;
    typename S::reverse_iterator b = s.rbegin();
    typename S::const_reverse_iterator cb = cs.rbegin();
    if (!s.empty())
    {
        assert(*b == s.back());
    }
    assert(b == cb);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S());
    test(S(L"123"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S());
    test(S(L"123"));
    }
#endif
}
