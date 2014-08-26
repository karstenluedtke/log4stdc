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

// iterator insert(const_iterator p, size_type n, charT c);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::difference_type pos, typename S::size_type n,
     typename S::value_type c, S expected)
{
    typename S::const_iterator p = s.cbegin() + pos;
    typename S::iterator i = s.insert(p, n, c);
    assert(s.__invariants());
    assert(i - s.begin() == pos);
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 0, 0, '1', S(L""));
    test(S(L""), 0, 5, '1', S(L"11111"));
    test(S(L""), 0, 10, '1', S(L"1111111111"));
    test(S(L""), 0, 20, '1', S(L"11111111111111111111"));
    test(S(L"abcde"), 0, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 0, 5, '1', S(L"11111abcde"));
    test(S(L"abcde"), 0, 10, '1', S(L"1111111111abcde"));
    test(S(L"abcde"), 0, 20, '1', S(L"11111111111111111111abcde"));
    test(S(L"abcde"), 1, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 1, 5, '1', S(L"a11111bcde"));
    test(S(L"abcde"), 1, 10, '1', S(L"a1111111111bcde"));
    test(S(L"abcde"), 1, 20, '1', S(L"a11111111111111111111bcde"));
    test(S(L"abcde"), 2, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 2, 5, '1', S(L"ab11111cde"));
    test(S(L"abcde"), 2, 10, '1', S(L"ab1111111111cde"));
    test(S(L"abcde"), 2, 20, '1', S(L"ab11111111111111111111cde"));
    test(S(L"abcde"), 4, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 4, 5, '1', S(L"abcd11111e"));
    test(S(L"abcde"), 4, 10, '1', S(L"abcd1111111111e"));
    test(S(L"abcde"), 4, 20, '1', S(L"abcd11111111111111111111e"));
    test(S(L"abcde"), 5, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 5, 5, '1', S(L"abcde11111"));
    test(S(L"abcde"), 5, 10, '1', S(L"abcde1111111111"));
    test(S(L"abcde"), 5, 20, '1', S(L"abcde11111111111111111111"));
    test(S(L"abcdefghij"), 0, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 5, '1', S(L"11111abcdefghij"));
    test(S(L"abcdefghij"), 0, 10, '1', S(L"1111111111abcdefghij"));
    test(S(L"abcdefghij"), 0, 20, '1', S(L"11111111111111111111abcdefghij"));
    test(S(L"abcdefghij"), 1, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 5, '1', S(L"a11111bcdefghij"));
    test(S(L"abcdefghij"), 1, 10, '1', S(L"a1111111111bcdefghij"));
    test(S(L"abcdefghij"), 1, 20, '1', S(L"a11111111111111111111bcdefghij"));
    test(S(L"abcdefghij"), 5, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 5, '1', S(L"abcde11111fghij"));
    test(S(L"abcdefghij"), 5, 10, '1', S(L"abcde1111111111fghij"));
    test(S(L"abcdefghij"), 5, 20, '1', S(L"abcde11111111111111111111fghij"));
    test(S(L"abcdefghij"), 9, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 5, '1', S(L"abcdefghi11111j"));
    test(S(L"abcdefghij"), 9, 10, '1', S(L"abcdefghi1111111111j"));
    test(S(L"abcdefghij"), 9, 20, '1', S(L"abcdefghi11111111111111111111j"));
    test(S(L"abcdefghij"), 10, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 5, '1', S(L"abcdefghij11111"));
    test(S(L"abcdefghij"), 10, 10, '1', S(L"abcdefghij1111111111"));
    test(S(L"abcdefghij"), 10, 20, '1', S(L"abcdefghij11111111111111111111"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 5, '1', S(L"11111abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, '1', S(L"1111111111abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, '1', S(L"11111111111111111111abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 5, '1', S(L"a11111bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 10, '1', S(L"a1111111111bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, '1', S(L"a11111111111111111111bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, '1', S(L"abcdefghij11111klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, '1', S(L"abcdefghij1111111111klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 20, '1', S(L"abcdefghij11111111111111111111klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 5, '1', S(L"abcdefghijklmnopqrs11111t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 10, '1', S(L"abcdefghijklmnopqrs1111111111t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 20, '1', S(L"abcdefghijklmnopqrs11111111111111111111t"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 5, '1', S(L"abcdefghijklmnopqrst11111"));
    test(S(L"abcdefghijklmnopqrst"), 20, 10, '1', S(L"abcdefghijklmnopqrst1111111111"));
    test(S(L"abcdefghijklmnopqrst"), 20, 20, '1', S(L"abcdefghijklmnopqrst11111111111111111111"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 0, 0, '1', S(L""));
    test(S(L""), 0, 5, '1', S(L"11111"));
    test(S(L""), 0, 10, '1', S(L"1111111111"));
    test(S(L""), 0, 20, '1', S(L"11111111111111111111"));
    test(S(L"abcde"), 0, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 0, 5, '1', S(L"11111abcde"));
    test(S(L"abcde"), 0, 10, '1', S(L"1111111111abcde"));
    test(S(L"abcde"), 0, 20, '1', S(L"11111111111111111111abcde"));
    test(S(L"abcde"), 1, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 1, 5, '1', S(L"a11111bcde"));
    test(S(L"abcde"), 1, 10, '1', S(L"a1111111111bcde"));
    test(S(L"abcde"), 1, 20, '1', S(L"a11111111111111111111bcde"));
    test(S(L"abcde"), 2, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 2, 5, '1', S(L"ab11111cde"));
    test(S(L"abcde"), 2, 10, '1', S(L"ab1111111111cde"));
    test(S(L"abcde"), 2, 20, '1', S(L"ab11111111111111111111cde"));
    test(S(L"abcde"), 4, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 4, 5, '1', S(L"abcd11111e"));
    test(S(L"abcde"), 4, 10, '1', S(L"abcd1111111111e"));
    test(S(L"abcde"), 4, 20, '1', S(L"abcd11111111111111111111e"));
    test(S(L"abcde"), 5, 0, '1', S(L"abcde"));
    test(S(L"abcde"), 5, 5, '1', S(L"abcde11111"));
    test(S(L"abcde"), 5, 10, '1', S(L"abcde1111111111"));
    test(S(L"abcde"), 5, 20, '1', S(L"abcde11111111111111111111"));
    test(S(L"abcdefghij"), 0, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, 5, '1', S(L"11111abcdefghij"));
    test(S(L"abcdefghij"), 0, 10, '1', S(L"1111111111abcdefghij"));
    test(S(L"abcdefghij"), 0, 20, '1', S(L"11111111111111111111abcdefghij"));
    test(S(L"abcdefghij"), 1, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, 5, '1', S(L"a11111bcdefghij"));
    test(S(L"abcdefghij"), 1, 10, '1', S(L"a1111111111bcdefghij"));
    test(S(L"abcdefghij"), 1, 20, '1', S(L"a11111111111111111111bcdefghij"));
    test(S(L"abcdefghij"), 5, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, 5, '1', S(L"abcde11111fghij"));
    test(S(L"abcdefghij"), 5, 10, '1', S(L"abcde1111111111fghij"));
    test(S(L"abcdefghij"), 5, 20, '1', S(L"abcde11111111111111111111fghij"));
    test(S(L"abcdefghij"), 9, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, 5, '1', S(L"abcdefghi11111j"));
    test(S(L"abcdefghij"), 9, 10, '1', S(L"abcdefghi1111111111j"));
    test(S(L"abcdefghij"), 9, 20, '1', S(L"abcdefghi11111111111111111111j"));
    test(S(L"abcdefghij"), 10, 0, '1', S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, 5, '1', S(L"abcdefghij11111"));
    test(S(L"abcdefghij"), 10, 10, '1', S(L"abcdefghij1111111111"));
    test(S(L"abcdefghij"), 10, 20, '1', S(L"abcdefghij11111111111111111111"));
    test(S(L"abcdefghijklmnopqrst"), 0, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 5, '1', S(L"11111abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 10, '1', S(L"1111111111abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, 20, '1', S(L"11111111111111111111abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 5, '1', S(L"a11111bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 10, '1', S(L"a1111111111bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, 20, '1', S(L"a11111111111111111111bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 5, '1', S(L"abcdefghij11111klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 10, '1', S(L"abcdefghij1111111111klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, 20, '1', S(L"abcdefghij11111111111111111111klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, 5, '1', S(L"abcdefghijklmnopqrs11111t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 10, '1', S(L"abcdefghijklmnopqrs1111111111t"));
    test(S(L"abcdefghijklmnopqrst"), 19, 20, '1', S(L"abcdefghijklmnopqrs11111111111111111111t"));
    test(S(L"abcdefghijklmnopqrst"), 20, 0, '1', S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, 5, '1', S(L"abcdefghijklmnopqrst11111"));
    test(S(L"abcdefghijklmnopqrst"), 20, 10, '1', S(L"abcdefghijklmnopqrst1111111111"));
    test(S(L"abcdefghijklmnopqrst"), 20, 20, '1', S(L"abcdefghijklmnopqrst11111111111111111111"));
    }
#endif
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::basic_string<wchar_t> s;
        barefootc::basic_string<wchar_t> s2;
        s.insert(s2.begin(), 1, 'a');
        assert(false);
    }
#endif
}
