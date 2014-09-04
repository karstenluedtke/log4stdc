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

// const_reverse_iterator crend() const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s)
{
    typename S::const_reverse_iterator ce = s.crend();
    assert(ce == s.rend());
}

int main()
{
    {
    typedef barefootc::basic_string<char> S;
    test(S());
    test(S("123"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S());
    test(S("123"));
    }
#endif
}
