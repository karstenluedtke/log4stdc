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
//   basic_string& assign(InputIterator first, InputIterator last);

#include <string>
#include <cassert>

#include "log4stdc.h"

#if 0
#include "../../input_iterator.h"
#define INPUT_ITERATOR(s) \
 	input_iterator<const char*>(s)
#else
#define INPUT_ITERATOR(s) \
	barefootc::iterator<wchar_t>(s)
#endif

#include "tests/support/min_allocator.h"


template <class S, class It>
void
test(S s, It first, It last, S expected)
{
    s.assign(first, last);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
  l4sc_configure_from_xml_file("log4j.xml");
    {
    typedef barefootc::basic_string<wchar_t> S;
    const wchar_t* s = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    test(S(), s, s, S());
    test(S(), s, s+1, S(L"A"));
    test(S(), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), s, s, S());
    test(S(L"12345"), s, s+1, S(L"A"));
    test(S(L"12345"), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(L"12345"), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), s, s, S());
    test(S(L"1234567890"), s, s+1, S(L"A"));
    test(S(L"1234567890"), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(L"1234567890"), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), s, s, S());
    test(S(L"12345678901234567890"), s, s+1, S(L"A"));
    test(S(L"12345678901234567890"), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), s, s+52,
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s), S());
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"A"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S());
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"A"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S());
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"A"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S());
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"A"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    test(S(), s, s, S());
    test(S(), s, s+1, S(L"A"));
    test(S(), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), s, s, S());
    test(S(L"12345"), s, s+1, S(L"A"));
    test(S(L"12345"), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(L"12345"), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), s, s, S());
    test(S(L"1234567890"), s, s+1, S(L"A"));
    test(S(L"1234567890"), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(L"1234567890"), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), s, s, S());
    test(S(L"12345678901234567890"), s, s+1, S(L"A"));
    test(S(L"12345678901234567890"), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), s, s+52,
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s), S());
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"A"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S());
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"A"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S());
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"A"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S());
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"A"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#endif
}
