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

// basic_string<charT,traits,Allocator>&
//   insert(size_type pos1, const basic_string& str);

#include <string>
#include <stdexcept>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(S s, typename S::size_type pos, S str, S expected)
{
    typename S::size_type old_size = s.size();
    S s0 = s;
    try
    {
        s.insert(pos, str);
        assert(s.__invariants());
        assert(pos <= old_size);
        assert(s == expected);
    }
    catch (std::out_of_range&)
    {
        assert(pos > old_size);
        assert(s == s0);
    }
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 0, S(L""), S(L""));
    test(S(L""), 0, S(L"12345"), S(L"12345"));
    test(S(L""), 0, S(L"1234567890"), S(L"1234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), S(L"12345678901234567890"));
    test(S(L""), 1, S(L""), S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), S(L"can't happen"));
    test(S(L"abcde"), 0, S(L""), S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), S(L"12345abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), S(L"12345678901234567890abcde"));
    test(S(L"abcde"), 1, S(L""), S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), S(L"a12345bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), S(L"a12345678901234567890bcde"));
    test(S(L"abcde"), 2, S(L""), S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), S(L"ab12345cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), S(L"ab12345678901234567890cde"));
    test(S(L"abcde"), 4, S(L""), S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), S(L"abcd12345e"));
    test(S(L"abcde"), 4, S(L"1234567890"), S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), S(L"abcd12345678901234567890e"));
    test(S(L"abcde"), 5, S(L""), S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), S(L"abcde12345"));
    test(S(L"abcde"), 5, S(L"1234567890"), S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), S(L"abcde12345678901234567890"));
    test(S(L"abcde"), 6, S(L""), S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), S(L"12345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), S(L"12345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), S(L"a12345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 5, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), S(L"abcde12345678901234567890fghij"));
    test(S(L"abcdefghij"), 9, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), S(L"abcdefghi12345678901234567890j"));
    test(S(L"abcdefghij"), 10, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghij"), 11, S(L""), S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), S(L"12345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), S(L"a12345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), S(L"abcdefghijklmnopqrs12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), S(L"abcdefghijklmnopqrst12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L""), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), S(L"can't happen"));
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 0, S(L""), S(L""));
    test(S(L""), 0, S(L"12345"), S(L"12345"));
    test(S(L""), 0, S(L"1234567890"), S(L"1234567890"));
    test(S(L""), 0, S(L"12345678901234567890"), S(L"12345678901234567890"));
    test(S(L""), 1, S(L""), S(L"can't happen"));
    test(S(L""), 1, S(L"12345"), S(L"can't happen"));
    test(S(L""), 1, S(L"1234567890"), S(L"can't happen"));
    test(S(L""), 1, S(L"12345678901234567890"), S(L"can't happen"));
    test(S(L"abcde"), 0, S(L""), S(L"abcde"));
    test(S(L"abcde"), 0, S(L"12345"), S(L"12345abcde"));
    test(S(L"abcde"), 0, S(L"1234567890"), S(L"1234567890abcde"));
    test(S(L"abcde"), 0, S(L"12345678901234567890"), S(L"12345678901234567890abcde"));
    test(S(L"abcde"), 1, S(L""), S(L"abcde"));
    test(S(L"abcde"), 1, S(L"12345"), S(L"a12345bcde"));
    test(S(L"abcde"), 1, S(L"1234567890"), S(L"a1234567890bcde"));
    test(S(L"abcde"), 1, S(L"12345678901234567890"), S(L"a12345678901234567890bcde"));
    test(S(L"abcde"), 2, S(L""), S(L"abcde"));
    test(S(L"abcde"), 2, S(L"12345"), S(L"ab12345cde"));
    test(S(L"abcde"), 2, S(L"1234567890"), S(L"ab1234567890cde"));
    test(S(L"abcde"), 2, S(L"12345678901234567890"), S(L"ab12345678901234567890cde"));
    test(S(L"abcde"), 4, S(L""), S(L"abcde"));
    test(S(L"abcde"), 4, S(L"12345"), S(L"abcd12345e"));
    test(S(L"abcde"), 4, S(L"1234567890"), S(L"abcd1234567890e"));
    test(S(L"abcde"), 4, S(L"12345678901234567890"), S(L"abcd12345678901234567890e"));
    test(S(L"abcde"), 5, S(L""), S(L"abcde"));
    test(S(L"abcde"), 5, S(L"12345"), S(L"abcde12345"));
    test(S(L"abcde"), 5, S(L"1234567890"), S(L"abcde1234567890"));
    test(S(L"abcde"), 5, S(L"12345678901234567890"), S(L"abcde12345678901234567890"));
    test(S(L"abcde"), 6, S(L""), S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345"), S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"1234567890"), S(L"can't happen"));
    test(S(L"abcde"), 6, S(L"12345678901234567890"), S(L"can't happen"));
    test(S(L"abcdefghij"), 0, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345"), S(L"12345abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"1234567890"), S(L"1234567890abcdefghij"));
    test(S(L"abcdefghij"), 0, S(L"12345678901234567890"), S(L"12345678901234567890abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345"), S(L"a12345bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"1234567890"), S(L"a1234567890bcdefghij"));
    test(S(L"abcdefghij"), 1, S(L"12345678901234567890"), S(L"a12345678901234567890bcdefghij"));
    test(S(L"abcdefghij"), 5, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 5, S(L"12345"), S(L"abcde12345fghij"));
    test(S(L"abcdefghij"), 5, S(L"1234567890"), S(L"abcde1234567890fghij"));
    test(S(L"abcdefghij"), 5, S(L"12345678901234567890"), S(L"abcde12345678901234567890fghij"));
    test(S(L"abcdefghij"), 9, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 9, S(L"12345"), S(L"abcdefghi12345j"));
    test(S(L"abcdefghij"), 9, S(L"1234567890"), S(L"abcdefghi1234567890j"));
    test(S(L"abcdefghij"), 9, S(L"12345678901234567890"), S(L"abcdefghi12345678901234567890j"));
    test(S(L"abcdefghij"), 10, S(L""), S(L"abcdefghij"));
    test(S(L"abcdefghij"), 10, S(L"12345"), S(L"abcdefghij12345"));
    test(S(L"abcdefghij"), 10, S(L"1234567890"), S(L"abcdefghij1234567890"));
    test(S(L"abcdefghij"), 10, S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890"));
    test(S(L"abcdefghij"), 11, S(L""), S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345"), S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"1234567890"), S(L"can't happen"));
    test(S(L"abcdefghij"), 11, S(L"12345678901234567890"), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345"), S(L"12345abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"1234567890"), S(L"1234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 0, S(L"12345678901234567890"), S(L"12345678901234567890abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345"), S(L"a12345bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"1234567890"), S(L"a1234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 1, S(L"12345678901234567890"), S(L"a12345678901234567890bcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345"), S(L"abcdefghij12345klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"1234567890"), S(L"abcdefghij1234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 10, S(L"12345678901234567890"), S(L"abcdefghij12345678901234567890klmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345"), S(L"abcdefghijklmnopqrs12345t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"1234567890"), S(L"abcdefghijklmnopqrs1234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 19, S(L"12345678901234567890"), S(L"abcdefghijklmnopqrs12345678901234567890t"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L""), S(L"abcdefghijklmnopqrst"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345"), S(L"abcdefghijklmnopqrst12345"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"1234567890"), S(L"abcdefghijklmnopqrst1234567890"));
    test(S(L"abcdefghijklmnopqrst"), 20, S(L"12345678901234567890"), S(L"abcdefghijklmnopqrst12345678901234567890"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L""), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345"), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"1234567890"), S(L"can't happen"));
    test(S(L"abcdefghijklmnopqrst"), 21, S(L"12345678901234567890"), S(L"can't happen"));
    }
#endif
}
