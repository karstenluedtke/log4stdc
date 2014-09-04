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

// size_type find_last_not_of(charT c, size_type pos = npos) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_last_not_of(c, pos) == x);
    if (x != S::npos)
        assert(x <= pos && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_last_not_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 'i', 0, S::npos);
    test(S(L""), 'i', 1, S::npos);
    test(S(L"kitcj"), 'i', 0, 0);
    test(S(L"qkamf"), 'i', 1, 1);
    test(S(L"nhmko"), 'i', 2, 2);
    test(S(L"tpsaf"), 'i', 4, 4);
    test(S(L"lahfb"), 'i', 5, 4);
    test(S(L"irkhs"), 'i', 6, 4);
    test(S(L"gmfhdaipsr"), 'i', 0, 0);
    test(S(L"kantesmpgj"), 'i', 1, 1);
    test(S(L"odaftiegpm"), 'i', 5, 4);
    test(S(L"oknlrstdpi"), 'i', 9, 8);
    test(S(L"eolhfgpjqk"), 'i', 10, 9);
    test(S(L"pcdrofikas"), 'i', 11, 9);
    test(S(L"nbatdlmekrgcfqsophij"), 'i', 0, 0);
    test(S(L"bnrpehidofmqtcksjgla"), 'i', 1, 1);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'i', 10, 10);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'i', 19, 18);
    test(S(L"hkbgspofltajcnedqmri"), 'i', 20, 18);
    test(S(L"oselktgbcapndfjihrmq"), 'i', 21, 19);

    test(S(L""), 'i', S::npos);
    test(S(L"csope"), 'i', 4);
    test(S(L"gfsmthlkon"), 'i', 9);
    test(S(L"laenfsbridchgotmkqpj"), 'i', 19);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 'i', 0, S::npos);
    test(S(L""), 'i', 1, S::npos);
    test(S(L"kitcj"), 'i', 0, 0);
    test(S(L"qkamf"), 'i', 1, 1);
    test(S(L"nhmko"), 'i', 2, 2);
    test(S(L"tpsaf"), 'i', 4, 4);
    test(S(L"lahfb"), 'i', 5, 4);
    test(S(L"irkhs"), 'i', 6, 4);
    test(S(L"gmfhdaipsr"), 'i', 0, 0);
    test(S(L"kantesmpgj"), 'i', 1, 1);
    test(S(L"odaftiegpm"), 'i', 5, 4);
    test(S(L"oknlrstdpi"), 'i', 9, 8);
    test(S(L"eolhfgpjqk"), 'i', 10, 9);
    test(S(L"pcdrofikas"), 'i', 11, 9);
    test(S(L"nbatdlmekrgcfqsophij"), 'i', 0, 0);
    test(S(L"bnrpehidofmqtcksjgla"), 'i', 1, 1);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'i', 10, 10);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'i', 19, 18);
    test(S(L"hkbgspofltajcnedqmri"), 'i', 20, 18);
    test(S(L"oselktgbcapndfjihrmq"), 'i', 21, 19);

    test(S(L""), 'i', S::npos);
    test(S(L"csope"), 'i', 4);
    test(S(L"gfsmthlkon"), 'i', 9);
    test(S(L"laenfsbridchgotmkqpj"), 'i', 19);
    }
#endif
}
