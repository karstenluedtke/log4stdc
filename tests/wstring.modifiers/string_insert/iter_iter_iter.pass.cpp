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

// template<class InputIterator>
//   iterator insert(const_iterator p, InputIterator first, InputIterator last);

#if _LIBCPP_DEBUG >= 1
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>

#if 0
#include "../../input_iterator.h"
#define INPUT_ITERATOR(s)	input_iterator<const char*>(s)
#else
#define INPUT_ITERATOR(s) \
	barefootc::iterator<barefootc::basic_string<wchar_t>,wchar_t>(s)
#endif

#include "tests/support/min_allocator.h"

template <class S, class It>
void
test(S s, typename S::difference_type pos, It first, It last, S expected)
{
    typename S::const_iterator p = s.cbegin() + pos;
    typename S::iterator i = s.insert(p, first, last);
    assert(s.__invariants());
    assert(i - s.begin() == pos);
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    const wchar_t* s = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    test(S(), 0, s, s, S());
    test(S(), 0, s, s+1, S(L"A"));
    test(S(), 0, s, s+10, S(L"ABCDEFGHIJ"));
    test(S(), 0, s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), 0, s, s, S(L"12345"));
    test(S(L"12345"), 1, s, s+1, S(L"1A2345"));
    test(S(L"12345"), 4, s, s+10, S(L"1234ABCDEFGHIJ5"));
    test(S(L"12345"), 5, s, s+52, S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), 0, s, s, S(L"1234567890"));
    test(S(L"1234567890"), 1, s, s+1, S(L"1A234567890"));
    test(S(L"1234567890"), 10, s, s+10, S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), 8, s, s+52, S(L"12345678ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz90"));

    test(S(L"12345678901234567890"), 3, s, s, S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), 3, s, s+1, S(L"123A45678901234567890"));
    test(S(L"12345678901234567890"), 15, s, s+10, S(L"123456789012345ABCDEFGHIJ67890"));
    test(S(L"12345678901234567890"), 20, s, s+52,
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S());
    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"A"));
    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"ABCDEFGHIJ"));
    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52), S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S(L"12345"));
    test(S(L"12345"), 1, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"1A2345"));
    test(S(L"12345"), 4, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"1234ABCDEFGHIJ5"));
    test(S(L"12345"), 5, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52), S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S(L"1234567890"));
    test(S(L"1234567890"), 1, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"1A234567890"));
    test(S(L"1234567890"), 10, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), 8, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52), S(L"12345678ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz90"));

    test(S(L"12345678901234567890"), 3, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), 3, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"123A45678901234567890"));
    test(S(L"12345678901234567890"), 15, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"123456789012345ABCDEFGHIJ67890"));
    test(S(L"12345678901234567890"), 20, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    const wchar_t* s = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    test(S(), 0, s, s, S());
    test(S(), 0, s, s+1, S(L"A"));
    test(S(), 0, s, s+10, S(L"ABCDEFGHIJ"));
    test(S(), 0, s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), 0, s, s, S(L"12345"));
    test(S(L"12345"), 1, s, s+1, S(L"1A2345"));
    test(S(L"12345"), 4, s, s+10, S(L"1234ABCDEFGHIJ5"));
    test(S(L"12345"), 5, s, s+52, S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), 0, s, s, S(L"1234567890"));
    test(S(L"1234567890"), 1, s, s+1, S(L"1A234567890"));
    test(S(L"1234567890"), 10, s, s+10, S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), 8, s, s+52, S(L"12345678ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz90"));

    test(S(L"12345678901234567890"), 3, s, s, S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), 3, s, s+1, S(L"123A45678901234567890"));
    test(S(L"12345678901234567890"), 15, s, s+10, S(L"123456789012345ABCDEFGHIJ67890"));
    test(S(L"12345678901234567890"), 20, s, s+52,
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S());
    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"A"));
    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"ABCDEFGHIJ"));
    test(S(), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52), S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S(L"12345"));
    test(S(L"12345"), 1, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"1A2345"));
    test(S(L"12345"), 4, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"1234ABCDEFGHIJ5"));
    test(S(L"12345"), 5, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52), S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), 0, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S(L"1234567890"));
    test(S(L"1234567890"), 1, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"1A234567890"));
    test(S(L"1234567890"), 10, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), 8, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52), S(L"12345678ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz90"));

    test(S(L"12345678901234567890"), 3, INPUT_ITERATOR(s), INPUT_ITERATOR(s), S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), 3, INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"123A45678901234567890"));
    test(S(L"12345678901234567890"), 15, INPUT_ITERATOR(s), INPUT_ITERATOR(s+10), S(L"123456789012345ABCDEFGHIJ67890"));
    test(S(L"12345678901234567890"), 20, INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#endif
#if _LIBCPP_DEBUG >= 1
    {
        barefootc::basic_string<wchar_t> v;
        barefootc::basic_string<wchar_t> v2;
        char a[] = "123";
        const int N = sizeof(a)/sizeof(a[0]);
        barefootc::basic_string<wchar_t>::iterator i = v.insert(v2.cbegin() + 10, a, a+N);
        assert(false);
    }
#endif
}
