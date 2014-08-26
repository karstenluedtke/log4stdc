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

// iterator insert(const_iterator p, charT c);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S& s, typename S::const_iterator p, typename S::value_type c, S expected)
{
    bool sufficient_cap = s.size() < s.capacity();
    typename S::difference_type pos = p - s.begin();
    typename S::iterator i = s.insert(p, c);
    assert(s.__invariants());
    assert(s == expected);
    assert(i - s.begin() == pos);
    assert(*i == c);
    if (sufficient_cap)
        assert(i == p);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    S s;
    test(s, s.begin(), '1', S(L"1"));
    test(s, s.begin(), 'a', S(L"a1"));
    test(s, s.end(), 'b', S(L"a1b"));
    test(s, s.end()-1, 'c', S(L"a1cb"));
    test(s, s.end()-2, 'd', S(L"a1dcb"));
    test(s, s.end()-3, '2', S(L"a12dcb"));
    test(s, s.end()-4, '3', S(L"a132dcb"));
    test(s, s.end()-5, '4', S(L"a1432dcb"));
    test(s, s.begin()+1, '5', S(L"a51432dcb"));
    test(s, s.begin()+2, '6', S(L"a561432dcb"));
    test(s, s.begin()+3, '7', S(L"a5671432dcb"));
    test(s, s.begin()+4, 'A', S(L"a567A1432dcb"));
    test(s, s.begin()+5, 'B', S(L"a567AB1432dcb"));
    test(s, s.begin()+6, 'C', S(L"a567ABC1432dcb"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    S s;
    test(s, s.begin(), '1', S(L"1"));
    test(s, s.begin(), 'a', S(L"a1"));
    test(s, s.end(), 'b', S(L"a1b"));
    test(s, s.end()-1, 'c', S(L"a1cb"));
    test(s, s.end()-2, 'd', S(L"a1dcb"));
    test(s, s.end()-3, '2', S(L"a12dcb"));
    test(s, s.end()-4, '3', S(L"a132dcb"));
    test(s, s.end()-5, '4', S(L"a1432dcb"));
    test(s, s.begin()+1, '5', S(L"a51432dcb"));
    test(s, s.begin()+2, '6', S(L"a561432dcb"));
    test(s, s.begin()+3, '7', S(L"a5671432dcb"));
    test(s, s.begin()+4, 'A', S(L"a567A1432dcb"));
    test(s, s.begin()+5, 'B', S(L"a567AB1432dcb"));
    test(s, s.begin()+6, 'C', S(L"a567ABC1432dcb"));
    }
#endif
#if _LIBCPP_DEBUG >= 1
    {
        typedef barefootc::basic_string<wchar_t> S;
        S s;
        S s2;
        s.insert(s2.begin(), '1');
        assert(false);
    }
#endif
}
