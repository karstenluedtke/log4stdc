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
//   basic_string& append(InputIterator first, InputIterator last);

#include <string>
#include <cassert>

#if 0
#include "../../input_iterator.h"
#define INPUT_ITERATOR(s)	input_iterator<const char*>(s)
#else
#define INPUT_ITERATOR(s)	barefootc::iterator<wchar_t>(s)
#endif

#include "tests/support/min_allocator.h"

template <class S, class It>
void
test(S s, It first, It last, S expected)
{
    s.append(first, last);
    assert(s.__invariants());
    assert(s == expected);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    const wchar_t* s = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    test(S(), s, s, S());
    test(S(), s, s+1, S(L"A"));
    test(S(), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), s, s, S(L"12345"));
    test(S(L"12345"), s, s+1, S(L"12345A"));
    test(S(L"12345"), s, s+10, S(L"12345ABCDEFGHIJ"));
    test(S(L"12345"), s, s+52, S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), s, s, S(L"1234567890"));
    test(S(L"1234567890"), s, s+1, S(L"1234567890A"));
    test(S(L"1234567890"), s, s+10, S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), s, s+52, S(L"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), s, s, S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), s, s+1, S(L"12345678901234567890A"));
    test(S(L"12345678901234567890"), s, s+10, S(L"12345678901234567890ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), s, s+52,
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s), S());
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"A"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S(L"12345"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"12345A"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"12345ABCDEFGHIJ"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S(L"1234567890"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"1234567890A"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"12345678901234567890A"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"12345678901234567890ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    const wchar_t* s = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    test(S(), s, s, S());
    test(S(), s, s+1, S(L"A"));
    test(S(), s, s+10, S(L"ABCDEFGHIJ"));
    test(S(), s, s+52, S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), s, s, S(L"12345"));
    test(S(L"12345"), s, s+1, S(L"12345A"));
    test(S(L"12345"), s, s+10, S(L"12345ABCDEFGHIJ"));
    test(S(L"12345"), s, s+52, S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), s, s, S(L"1234567890"));
    test(S(L"1234567890"), s, s+1, S(L"1234567890A"));
    test(S(L"1234567890"), s, s+10, S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), s, s+52, S(L"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), s, s, S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), s, s+1, S(L"12345678901234567890A"));
    test(S(L"12345678901234567890"), s, s+10, S(L"12345678901234567890ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), s, s+52,
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s), S());
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1), S(L"A"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"ABCDEFGHIJ"));
    test(S(), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S(L"12345"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"12345A"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"12345ABCDEFGHIJ"));
    test(S(L"12345"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"12345ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S(L"1234567890"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"1234567890A"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"1234567890ABCDEFGHIJ"));
    test(S(L"1234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));

    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s),
         S(L"12345678901234567890"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+1),
         S(L"12345678901234567890A"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+10),
         S(L"12345678901234567890ABCDEFGHIJ"));
    test(S(L"12345678901234567890"), INPUT_ITERATOR(s), INPUT_ITERATOR(s+52),
         S(L"12345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    }
#endif
}
