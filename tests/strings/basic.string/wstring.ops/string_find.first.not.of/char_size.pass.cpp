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

// size_type find_first_not_of(charT c, size_type pos = 0) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_first_not_of(c, pos) == x);
    if (x != S::npos)
        assert(pos <= x && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_first_not_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 'q', 0, S::npos);
    test(S(L""), 'q', 1, S::npos);
    test(S(L"kitcj"), 'q', 0, 0);
    test(S(L"qkamf"), 'q', 1, 1);
    test(S(L"nhmko"), 'q', 2, 2);
    test(S(L"tpsaf"), 'q', 4, 4);
    test(S(L"lahfb"), 'q', 5, S::npos);
    test(S(L"irkhs"), 'q', 6, S::npos);
    test(S(L"gmfhdaipsr"), 'q', 0, 0);
    test(S(L"kantesmpgj"), 'q', 1, 1);
    test(S(L"odaftiegpm"), 'q', 5, 5);
    test(S(L"oknlrstdpi"), 'q', 9, 9);
    test(S(L"eolhfgpjqk"), 'q', 10, S::npos);
    test(S(L"pcdrofikas"), 'q', 11, S::npos);
    test(S(L"nbatdlmekrgcfqsophij"), 'q', 0, 0);
    test(S(L"bnrpehidofmqtcksjgla"), 'q', 1, 1);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'q', 10, 10);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'q', 19, 19);
    test(S(L"hkbgspofltajcnedqmri"), 'q', 20, S::npos);
    test(S(L"oselktgbcapndfjihrmq"), 'q', 21, S::npos);

    test(S(L""), 'q', S::npos);
    test(S(L"q"), 'q', S::npos);
    test(S(L"qqq"), 'q', S::npos);
    test(S(L"csope"), 'q', 0);
    test(S(L"gfsmthlkon"), 'q', 0);
    test(S(L"laenfsbridchgotmkqpj"), 'q', 0);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 'q', 0, S::npos);
    test(S(L""), 'q', 1, S::npos);
    test(S(L"kitcj"), 'q', 0, 0);
    test(S(L"qkamf"), 'q', 1, 1);
    test(S(L"nhmko"), 'q', 2, 2);
    test(S(L"tpsaf"), 'q', 4, 4);
    test(S(L"lahfb"), 'q', 5, S::npos);
    test(S(L"irkhs"), 'q', 6, S::npos);
    test(S(L"gmfhdaipsr"), 'q', 0, 0);
    test(S(L"kantesmpgj"), 'q', 1, 1);
    test(S(L"odaftiegpm"), 'q', 5, 5);
    test(S(L"oknlrstdpi"), 'q', 9, 9);
    test(S(L"eolhfgpjqk"), 'q', 10, S::npos);
    test(S(L"pcdrofikas"), 'q', 11, S::npos);
    test(S(L"nbatdlmekrgcfqsophij"), 'q', 0, 0);
    test(S(L"bnrpehidofmqtcksjgla"), 'q', 1, 1);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'q', 10, 10);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'q', 19, 19);
    test(S(L"hkbgspofltajcnedqmri"), 'q', 20, S::npos);
    test(S(L"oselktgbcapndfjihrmq"), 'q', 21, S::npos);

    test(S(L""), 'q', S::npos);
    test(S(L"q"), 'q', S::npos);
    test(S(L"qqq"), 'q', S::npos);
    test(S(L"csope"), 'q', 0);
    test(S(L"gfsmthlkon"), 'q', 0);
    test(S(L"laenfsbridchgotmkqpj"), 'q', 0);
    }
#endif
}
