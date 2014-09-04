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

// size_type find_first_of(charT c, size_type pos = 0) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_first_of(c, pos) == x);
    if (x != S::npos)
        assert(pos <= x && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_first_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 'e', 0, S::npos);
    test(S(L""), 'e', 1, S::npos);
    test(S(L"kitcj"), 'e', 0, S::npos);
    test(S(L"qkamf"), 'e', 1, S::npos);
    test(S(L"nhmko"), 'e', 2, S::npos);
    test(S(L"tpsaf"), 'e', 4, S::npos);
    test(S(L"lahfb"), 'e', 5, S::npos);
    test(S(L"irkhs"), 'e', 6, S::npos);
    test(S(L"gmfhdaipsr"), 'e', 0, S::npos);
    test(S(L"kantesmpgj"), 'e', 1, 4);
    test(S(L"odaftiegpm"), 'e', 5, 6);
    test(S(L"oknlrstdpi"), 'e', 9, S::npos);
    test(S(L"eolhfgpjqk"), 'e', 10, S::npos);
    test(S(L"pcdrofikas"), 'e', 11, S::npos);
    test(S(L"nbatdlmekrgcfqsophij"), 'e', 0, 7);
    test(S(L"bnrpehidofmqtcksjgla"), 'e', 1, 4);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'e', 10, S::npos);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'e', 19, S::npos);
    test(S(L"hkbgspofltajcnedqmri"), 'e', 20, S::npos);
    test(S(L"oselktgbcapndfjihrmq"), 'e', 21, S::npos);

    test(S(L""), 'e', S::npos);
    test(S(L"csope"), 'e', 4);
    test(S(L"gfsmthlkon"), 'e', S::npos);
    test(S(L"laenfsbridchgotmkqpj"), 'e', 2);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 'e', 0, S::npos);
    test(S(L""), 'e', 1, S::npos);
    test(S(L"kitcj"), 'e', 0, S::npos);
    test(S(L"qkamf"), 'e', 1, S::npos);
    test(S(L"nhmko"), 'e', 2, S::npos);
    test(S(L"tpsaf"), 'e', 4, S::npos);
    test(S(L"lahfb"), 'e', 5, S::npos);
    test(S(L"irkhs"), 'e', 6, S::npos);
    test(S(L"gmfhdaipsr"), 'e', 0, S::npos);
    test(S(L"kantesmpgj"), 'e', 1, 4);
    test(S(L"odaftiegpm"), 'e', 5, 6);
    test(S(L"oknlrstdpi"), 'e', 9, S::npos);
    test(S(L"eolhfgpjqk"), 'e', 10, S::npos);
    test(S(L"pcdrofikas"), 'e', 11, S::npos);
    test(S(L"nbatdlmekrgcfqsophij"), 'e', 0, 7);
    test(S(L"bnrpehidofmqtcksjgla"), 'e', 1, 4);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'e', 10, S::npos);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'e', 19, S::npos);
    test(S(L"hkbgspofltajcnedqmri"), 'e', 20, S::npos);
    test(S(L"oselktgbcapndfjihrmq"), 'e', 21, S::npos);

    test(S(L""), 'e', S::npos);
    test(S(L"csope"), 'e', 4);
    test(S(L"gfsmthlkon"), 'e', S::npos);
    test(S(L"laenfsbridchgotmkqpj"), 'e', 2);
    }
#endif
}
