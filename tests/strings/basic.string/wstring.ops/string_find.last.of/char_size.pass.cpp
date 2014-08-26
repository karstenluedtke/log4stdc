#include <wchar.h>
#include "tests/strings/cxxwrapper.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// size_type find_last_of(charT c, size_type pos = npos) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_last_of(c, pos) == x);
    if (x != S::npos)
        assert(x <= pos && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_last_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
  l4sc_configure_from_xml_file("log4j.xml");
  l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 0);
  L4SC_TRACE(logger, "tests/strings/basic.string/string.ops/string_find.last.of/char_size.pass starting");
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 'm', 0, S::npos);
    test(S(L""), 'm', 1, S::npos);
    test(S(L"kitcj"), 'm', 0, S::npos);
    test(S(L"qkamf"), 'm', 1, S::npos);
    test(S(L"nhmko"), 'm', 2, 2);
    test(S(L"tpsaf"), 'm', 4, S::npos);
    test(S(L"lahfb"), 'm', 5, S::npos);
    test(S(L"irkhs"), 'm', 6, S::npos);
    test(S(L"gmfhdaipsr"), 'm', 0, S::npos);
    test(S(L"kantesmpgj"), 'm', 1, S::npos);
    test(S(L"odaftiegpm"), 'm', 5, S::npos);
    test(S(L"oknlrstdpi"), 'm', 9, S::npos);
    test(S(L"eolhfgpjqk"), 'm', 10, S::npos);
    test(S(L"pcdrofikas"), 'm', 11, S::npos);
    test(S(L"nbatdlmekrgcfqsophij"), 'm', 0, S::npos);
    test(S(L"bnrpehidofmqtcksjgla"), 'm', 1, S::npos);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'm', 10, 2);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'm', 19, 12);
    test(S(L"hkbgspofltajcnedqmri"), 'm', 20, 17);
    test(S(L"oselktgbcapndfjihrmq"), 'm', 21, 18);

    test(S(L""), 'm', S::npos);
    test(S(L"csope"), 'm', S::npos);
    test(S(L"gfsmthlkon"), 'm', 3);
    test(S(L"laenfsbridchgotmkqpj"), 'm', 15);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 'm', 0, S::npos);
    test(S(L""), 'm', 1, S::npos);
    test(S(L"kitcj"), 'm', 0, S::npos);
    test(S(L"qkamf"), 'm', 1, S::npos);
    test(S(L"nhmko"), 'm', 2, 2);
    test(S(L"tpsaf"), 'm', 4, S::npos);
    test(S(L"lahfb"), 'm', 5, S::npos);
    test(S(L"irkhs"), 'm', 6, S::npos);
    test(S(L"gmfhdaipsr"), 'm', 0, S::npos);
    test(S(L"kantesmpgj"), 'm', 1, S::npos);
    test(S(L"odaftiegpm"), 'm', 5, S::npos);
    test(S(L"oknlrstdpi"), 'm', 9, S::npos);
    test(S(L"eolhfgpjqk"), 'm', 10, S::npos);
    test(S(L"pcdrofikas"), 'm', 11, S::npos);
    test(S(L"nbatdlmekrgcfqsophij"), 'm', 0, S::npos);
    test(S(L"bnrpehidofmqtcksjgla"), 'm', 1, S::npos);
    test(S(L"jdmciepkaqgotsrfnhlb"), 'm', 10, 2);
    test(S(L"jtdaefblsokrmhpgcnqi"), 'm', 19, 12);
    test(S(L"hkbgspofltajcnedqmri"), 'm', 20, 17);
    test(S(L"oselktgbcapndfjihrmq"), 'm', 21, 18);

    test(S(L""), 'm', S::npos);
    test(S(L"csope"), 'm', S::npos);
    test(S(L"gfsmthlkon"), 'm', 3);
    test(S(L"laenfsbridchgotmkqpj"), 'm', 15);
    }
#endif
}
