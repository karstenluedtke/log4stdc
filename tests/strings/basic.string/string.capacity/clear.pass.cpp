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

// void clear();

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s)
{
    s.clear();
    assert(s.size() == 0);
}

int main()
{
    {
    typedef barefootc::basic_string<char> S;
    S s;
    test(s);

    s.assign(10, 'a');
    s.erase(5);
    test(s);

    s.assign(100, 'a');
    s.erase(50);
    test(s);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    S s;
    test(s);

    s.assign(10, 'a');
    s.erase(5);
    test(s);

    s.assign(100, 'a');
    s.erase(50);
    test(s);
    }
#endif
}
