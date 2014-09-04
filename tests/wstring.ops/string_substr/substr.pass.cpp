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

// basic_string substr(size_type pos = 0, size_type n = npos) const;

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, typename S::size_type pos, typename S::size_type n)
{
    try
    {
        S str = s.substr(pos, n);
        assert(str.__invariants());
        assert(pos <= s.size());
        typename S::size_type rlen = std::min(n, s.size() - pos);
        assert(str.size() == rlen);
        assert(S::traits_type::compare(s.data()+pos, str.data(), rlen) == 0);
    }
    catch (std::out_of_range&)
    {
        assert(pos > s.size());
    }
}

int main()
{
   l4sc_configure_from_xml_file("log4j.xml");
   l4sc_logger_ptr_t logger = l4sc_get_logger("barefootc.string", 0);
   L4SC_TRACE(logger,"%s: starting", __FUNCTION__);
    {
    typedef barefootc::basic_string<wchar_t> S;
    test(S(L""), 0, 0);
    test(S(L""), 1, 0);
    test(S(L"pniot"), 0, 0);
    test(S(L"htaob"), 0, 1);
    test(S(L"fodgq"), 0, 2);
    test(S(L"hpqia"), 0, 4);
    test(S(L"qanej"), 0, 5);
    test(S(L"dfkap"), 1, 0);
    test(S(L"clbao"), 1, 1);
    test(S(L"ihqrf"), 1, 2);
    test(S(L"mekdn"), 1, 3);
    test(S(L"ngtjf"), 1, 4);
    test(S(L"srdfq"), 2, 0);
    test(S(L"qkdrs"), 2, 1);
    test(S(L"ikcrq"), 2, 2);
    test(S(L"cdaih"), 2, 3);
    test(S(L"dmajb"), 4, 0);
    test(S(L"karth"), 4, 1);
    test(S(L"lhcdo"), 5, 0);
    test(S(L"acbsj"), 6, 0);
    test(S(L"pbsjikaole"), 0, 0);
    test(S(L"pcbahntsje"), 0, 1);
    test(S(L"mprdjbeiak"), 0, 5);
    test(S(L"fhepcrntko"), 0, 9);
    test(S(L"eqmpaidtls"), 0, 10);
    test(S(L"joidhalcmq"), 1, 0);
    test(S(L"omigsphflj"), 1, 1);
    test(S(L"kocgbphfji"), 1, 4);
    test(S(L"onmjekafbi"), 1, 8);
    test(S(L"fbslrjiqkm"), 1, 9);
    test(S(L"oqmrjahnkg"), 5, 0);
    test(S(L"jeidpcmalh"), 5, 1);
    test(S(L"schfalibje"), 5, 2);
    test(S(L"crliponbqe"), 5, 4);
    test(S(L"igdscopqtm"), 5, 5);
    test(S(L"qngpdkimlc"), 9, 0);
    test(S(L"thdjgafrlb"), 9, 1);
    test(S(L"hcjitbfapl"), 10, 0);
    test(S(L"mgojkldsqh"), 11, 0);
    test(S(L"gfshlcmdjreqipbontak"), 0, 0);
    test(S(L"nadkhpfemgclosibtjrq"), 0, 1);
    test(S(L"nkodajteqplrbifhmcgs"), 0, 10);
    test(S(L"ofdrqmkeblthacpgijsn"), 0, 19);
    test(S(L"gbmetiprqdoasckjfhln"), 0, 20);
    test(S(L"bdfjqgatlksriohemnpc"), 1, 0);
    test(S(L"crnklpmegdqfiashtojb"), 1, 1);
    test(S(L"ejqcnahdrkfsmptilgbo"), 1, 9);
    test(S(L"jsbtafedocnirgpmkhql"), 1, 18);
    test(S(L"prqgnlbaejsmkhdctoif"), 1, 19);
    test(S(L"qnmodrtkebhpasifgcjl"), 10, 0);
    test(S(L"pejafmnokrqhtisbcdgl"), 10, 1);
    test(S(L"cpebqsfmnjdolhkratgi"), 10, 5);
    test(S(L"odnqkgijrhabfmcestlp"), 10, 9);
    test(S(L"lmofqdhpkibagnrcjste"), 10, 10);
    test(S(L"lgjqketopbfahrmnsicd"), 19, 0);
    test(S(L"ktsrmnqagdecfhijpobl"), 19, 1);
    test(S(L"lsaijeqhtrbgcdmpfkno"), 20, 0);
    test(S(L"dplqartnfgejichmoskb"), 21, 0);
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S(L""), 0, 0);
    test(S(L""), 1, 0);
    test(S(L"pniot"), 0, 0);
    test(S(L"htaob"), 0, 1);
    test(S(L"fodgq"), 0, 2);
    test(S(L"hpqia"), 0, 4);
    test(S(L"qanej"), 0, 5);
    test(S(L"dfkap"), 1, 0);
    test(S(L"clbao"), 1, 1);
    test(S(L"ihqrf"), 1, 2);
    test(S(L"mekdn"), 1, 3);
    test(S(L"ngtjf"), 1, 4);
    test(S(L"srdfq"), 2, 0);
    test(S(L"qkdrs"), 2, 1);
    test(S(L"ikcrq"), 2, 2);
    test(S(L"cdaih"), 2, 3);
    test(S(L"dmajb"), 4, 0);
    test(S(L"karth"), 4, 1);
    test(S(L"lhcdo"), 5, 0);
    test(S(L"acbsj"), 6, 0);
    test(S(L"pbsjikaole"), 0, 0);
    test(S(L"pcbahntsje"), 0, 1);
    test(S(L"mprdjbeiak"), 0, 5);
    test(S(L"fhepcrntko"), 0, 9);
    test(S(L"eqmpaidtls"), 0, 10);
    test(S(L"joidhalcmq"), 1, 0);
    test(S(L"omigsphflj"), 1, 1);
    test(S(L"kocgbphfji"), 1, 4);
    test(S(L"onmjekafbi"), 1, 8);
    test(S(L"fbslrjiqkm"), 1, 9);
    test(S(L"oqmrjahnkg"), 5, 0);
    test(S(L"jeidpcmalh"), 5, 1);
    test(S(L"schfalibje"), 5, 2);
    test(S(L"crliponbqe"), 5, 4);
    test(S(L"igdscopqtm"), 5, 5);
    test(S(L"qngpdkimlc"), 9, 0);
    test(S(L"thdjgafrlb"), 9, 1);
    test(S(L"hcjitbfapl"), 10, 0);
    test(S(L"mgojkldsqh"), 11, 0);
    test(S(L"gfshlcmdjreqipbontak"), 0, 0);
    test(S(L"nadkhpfemgclosibtjrq"), 0, 1);
    test(S(L"nkodajteqplrbifhmcgs"), 0, 10);
    test(S(L"ofdrqmkeblthacpgijsn"), 0, 19);
    test(S(L"gbmetiprqdoasckjfhln"), 0, 20);
    test(S(L"bdfjqgatlksriohemnpc"), 1, 0);
    test(S(L"crnklpmegdqfiashtojb"), 1, 1);
    test(S(L"ejqcnahdrkfsmptilgbo"), 1, 9);
    test(S(L"jsbtafedocnirgpmkhql"), 1, 18);
    test(S(L"prqgnlbaejsmkhdctoif"), 1, 19);
    test(S(L"qnmodrtkebhpasifgcjl"), 10, 0);
    test(S(L"pejafmnokrqhtisbcdgl"), 10, 1);
    test(S(L"cpebqsfmnjdolhkratgi"), 10, 5);
    test(S(L"odnqkgijrhabfmcestlp"), 10, 9);
    test(S(L"lmofqdhpkibagnrcjste"), 10, 10);
    test(S(L"lgjqketopbfahrmnsicd"), 19, 0);
    test(S(L"ktsrmnqagdecfhijpobl"), 19, 1);
    test(S(L"lsaijeqhtrbgcdmpfkno"), 20, 0);
    test(S(L"dplqartnfgejichmoskb"), 21, 0);
    }
#endif
}
