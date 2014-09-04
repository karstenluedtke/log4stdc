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

// size_type find_first_not_of(const charT* s, size_type pos = 0) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_first_not_of(str, pos) == x);
    if (x != S::npos)
        assert(pos <= x && x < s.size());
}

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type x)
{
    assert(s.find_first_not_of(str) == x);
    if (x != S::npos)
        assert(x < s.size());
}

template <class S>
void test0()
{
    test(S(L""), L"", 0, S::npos);
    test(S(L""), L"laenf", 0, S::npos);
    test(S(L""), L"pqlnkmbdjo", 0, S::npos);
    test(S(L""), L"qkamfogpnljdcshbreti", 0, S::npos);
    test(S(L""), L"", 1, S::npos);
    test(S(L""), L"bjaht", 1, S::npos);
    test(S(L""), L"hjlcmgpket", 1, S::npos);
    test(S(L""), L"htaobedqikfplcgjsmrn", 1, S::npos);
    test(S(L"fodgq"), L"", 0, 0);
    test(S(L"qanej"), L"dfkap", 0, 0);
    test(S(L"clbao"), L"ihqrfebgad", 0, 0);
    test(S(L"mekdn"), L"ngtjfcalbseiqrphmkdo", 0, S::npos);
    test(S(L"srdfq"), L"", 1, 1);
    test(S(L"oemth"), L"ikcrq", 1, 1);
    test(S(L"cdaih"), L"dmajblfhsg", 1, 3);
    test(S(L"qohtk"), L"oqftjhdmkgsblacenirp", 1, S::npos);
    test(S(L"cshmd"), L"", 2, 2);
    test(S(L"lhcdo"), L"oebqi", 2, 2);
    test(S(L"qnsoh"), L"kojhpmbsfe", 2, S::npos);
    test(S(L"pkrof"), L"acbsjqogpltdkhinfrem", 2, S::npos);
    test(S(L"fmtsp"), L"", 4, 4);
    test(S(L"khbpm"), L"aobjd", 4, 4);
    test(S(L"pbsji"), L"pcbahntsje", 4, 4);
    test(S(L"mprdj"), L"fhepcrntkoagbmldqijs", 4, S::npos);
    test(S(L"eqmpa"), L"", 5, S::npos);
    test(S(L"omigs"), L"kocgb", 5, S::npos);
    test(S(L"onmje"), L"fbslrjiqkm", 5, S::npos);
    test(S(L"oqmrj"), L"jeidpcmalhfnqbgtrsko", 5, S::npos);
    test(S(L"schfa"), L"", 6, S::npos);
    test(S(L"igdsc"), L"qngpd", 6, S::npos);
    test(S(L"brqgo"), L"rodhqklgmb", 6, S::npos);
    test(S(L"tnrph"), L"thdjgafrlbkoiqcspmne", 6, S::npos);
    test(S(L"hcjitbfapl"), L"", 0, 0);
    test(S(L"daiprenocl"), L"ashjd", 0, 2);
    test(S(L"litpcfdghe"), L"mgojkldsqh", 0, 1);
    test(S(L"aidjksrolc"), L"imqnaghkfrdtlopbjesc", 0, S::npos);
    test(S(L"qpghtfbaji"), L"", 1, 1);
    test(S(L"gfshlcmdjr"), L"nadkh", 1, 1);
    test(S(L"nkodajteqp"), L"ofdrqmkebl", 1, 4);
    test(S(L"gbmetiprqd"), L"bdfjqgatlksriohemnpc", 1, S::npos);
    test(S(L"crnklpmegd"), L"", 5, 5);
    test(S(L"jsbtafedoc"), L"prqgn", 5, 5);
    test(S(L"qnmodrtkeb"), L"pejafmnokr", 5, 6);
    test(S(L"cpebqsfmnj"), L"odnqkgijrhabfmcestlp", 5, S::npos);
    test(S(L"lmofqdhpki"), L"", 9, 9);
    test(S(L"hnefkqimca"), L"rtjpa", 9, S::npos);
    test(S(L"drtasbgmfp"), L"ktsrmnqagd", 9, 9);
    test(S(L"lsaijeqhtr"), L"rtdhgcisbnmoaqkfpjle", 9, S::npos);
    test(S(L"elgofjmbrq"), L"", 10, S::npos);
    test(S(L"mjqdgalkpc"), L"dplqa", 10, S::npos);
    test(S(L"kthqnfcerm"), L"dkacjoptns", 10, S::npos);
    test(S(L"dfsjhanorc"), L"hqfimtrgnbekpdcsjalo", 10, S::npos);
    test(S(L"eqsgalomhb"), L"", 11, S::npos);
    test(S(L"akiteljmoh"), L"lofbc", 11, S::npos);
    test(S(L"hlbdfreqjo"), L"astoegbfpn", 11, S::npos);
    test(S(L"taqobhlerg"), L"pdgreqomsncafklhtibj", 11, S::npos);
    test(S(L"snafbdlghrjkpqtoceim"), L"", 0, 0);
    test(S(L"aemtbrgcklhndjisfpoq"), L"lbtqd", 0, 0);
    test(S(L"pnracgfkjdiholtbqsem"), L"tboimldpjh", 0, 1);
    test(S(L"dicfltehbsgrmojnpkaq"), L"slcerthdaiqjfnobgkpm", 0, S::npos);
    test(S(L"jlnkraeodhcspfgbqitm"), L"", 1, 1);
    test(S(L"lhosrngtmfjikbqpcade"), L"aqibs", 1, 1);
    test(S(L"rbtaqjhgkneisldpmfoc"), L"gtfblmqinc", 1, 3);
    test(S(L"gpifsqlrdkbonjtmheca"), L"mkqpbtdalgniorhfescj", 1, S::npos);
    test(S(L"hdpkobnsalmcfijregtq"), L"", 10, 10);
    test(S(L"jtlshdgqaiprkbcoenfm"), L"pblas", 10, 11);
    test(S(L"fkdrbqltsgmcoiphneaj"), L"arosdhcfme", 10, 13);
    test(S(L"crsplifgtqedjohnabmk"), L"blkhjeogicatqfnpdmsr", 10, S::npos);
    test(S(L"niptglfbosehkamrdqcj"), L"", 19, 19);
    test(S(L"copqdhstbingamjfkler"), L"djkqc", 19, 19);
    test(S(L"mrtaefilpdsgocnhqbjk"), L"lgokshjtpb", 19, S::npos);
    test(S(L"kojatdhlcmigpbfrqnes"), L"bqjhtkfepimcnsgrlado", 19, S::npos);
    test(S(L"eaintpchlqsbdgrkjofm"), L"", 20, S::npos);
    test(S(L"gjnhidfsepkrtaqbmclo"), L"nocfa", 20, S::npos);
    test(S(L"spocfaktqdbiejlhngmr"), L"bgtajmiedc", 20, S::npos);
    test(S(L"rphmlekgfscndtaobiqj"), L"lsckfnqgdahejiopbtmr", 20, S::npos);
    test(S(L"liatsqdoegkmfcnbhrpj"), L"", 21, S::npos);
    test(S(L"binjagtfldkrspcomqeh"), L"gfsrt", 21, S::npos);
    test(S(L"latkmisecnorjbfhqpdg"), L"pfsocbhjtm", 21, S::npos);
    test(S(L"lecfratdjkhnsmqpoigb"), L"tpflmdnoicjgkberhqsa", 21, S::npos);
}

template <class S>
void test1()
{
    test(S(L""), L"", S::npos);
    test(S(L""), L"laenf", S::npos);
    test(S(L""), L"pqlnkmbdjo", S::npos);
    test(S(L""), L"qkamfogpnljdcshbreti", S::npos);
    test(S(L"nhmko"), L"", 0);
    test(S(L"lahfb"), L"irkhs", 0);
    test(S(L"gmfhd"), L"kantesmpgj", 2);
    test(S(L"odaft"), L"oknlrstdpiqmjbaghcfe", S::npos);
    test(S(L"eolhfgpjqk"), L"", 0);
    test(S(L"nbatdlmekr"), L"bnrpe", 2);
    test(S(L"jdmciepkaq"), L"jtdaefblso", 2);
    test(S(L"hkbgspoflt"), L"oselktgbcapndfjihrmq", S::npos);
    test(S(L"gprdcokbnjhlsfmtieqa"), L"", 0);
    test(S(L"qjghlnftcaismkropdeb"), L"bjaht", 0);
    test(S(L"pnalfrdtkqcmojiesbhg"), L"hjlcmgpket", 1);
    test(S(L"pniotcfrhqsmgdkjbael"), L"htaobedqikfplcgjsmrn", S::npos);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0<S>();
    test1<S>();
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0<S>();
    test1<S>();
    }
#endif
}
