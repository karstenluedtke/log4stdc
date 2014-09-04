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

// size_type find_last_of(const basic_string& str, size_type pos = npos) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const S& str, typename S::size_type pos, typename S::size_type x)
{
    assert(s.find_last_of(str, pos) == x);
    if (x != S::npos)
        assert(x <= pos && x < s.size());
}

template <class S>
void
test(const S& s, const S& str, typename S::size_type x)
{
    assert(s.find_last_of(str) == x);
    if (x != S::npos)
        assert(x < s.size());
}

template <class S>
void test0()
{
    test(S(L""), S(L""), 0, S::npos);
    test(S(L""), S(L"laenf"), 0, S::npos);
    test(S(L""), S(L"pqlnkmbdjo"), 0, S::npos);
    test(S(L""), S(L"qkamfogpnljdcshbreti"), 0, S::npos);
    test(S(L""), S(L""), 1, S::npos);
    test(S(L""), S(L"bjaht"), 1, S::npos);
    test(S(L""), S(L"hjlcmgpket"), 1, S::npos);
    test(S(L""), S(L"htaobedqikfplcgjsmrn"), 1, S::npos);
    test(S(L"fodgq"), S(L""), 0, S::npos);
    test(S(L"qanej"), S(L"dfkap"), 0, S::npos);
    test(S(L"clbao"), S(L"ihqrfebgad"), 0, S::npos);
    test(S(L"mekdn"), S(L"ngtjfcalbseiqrphmkdo"), 0, 0);
    test(S(L"srdfq"), S(L""), 1, S::npos);
    test(S(L"oemth"), S(L"ikcrq"), 1, S::npos);
    test(S(L"cdaih"), S(L"dmajblfhsg"), 1, 1);
    test(S(L"qohtk"), S(L"oqftjhdmkgsblacenirp"), 1, 1);
    test(S(L"cshmd"), S(L""), 2, S::npos);
    test(S(L"lhcdo"), S(L"oebqi"), 2, S::npos);
    test(S(L"qnsoh"), S(L"kojhpmbsfe"), 2, 2);
    test(S(L"pkrof"), S(L"acbsjqogpltdkhinfrem"), 2, 2);
    test(S(L"fmtsp"), S(L""), 4, S::npos);
    test(S(L"khbpm"), S(L"aobjd"), 4, 2);
    test(S(L"pbsji"), S(L"pcbahntsje"), 4, 3);
    test(S(L"mprdj"), S(L"fhepcrntkoagbmldqijs"), 4, 4);
    test(S(L"eqmpa"), S(L""), 5, S::npos);
    test(S(L"omigs"), S(L"kocgb"), 5, 3);
    test(S(L"onmje"), S(L"fbslrjiqkm"), 5, 3);
    test(S(L"oqmrj"), S(L"jeidpcmalhfnqbgtrsko"), 5, 4);
    test(S(L"schfa"), S(L""), 6, S::npos);
    test(S(L"igdsc"), S(L"qngpd"), 6, 2);
    test(S(L"brqgo"), S(L"rodhqklgmb"), 6, 4);
    test(S(L"tnrph"), S(L"thdjgafrlbkoiqcspmne"), 6, 4);
    test(S(L"hcjitbfapl"), S(L""), 0, S::npos);
    test(S(L"daiprenocl"), S(L"ashjd"), 0, 0);
    test(S(L"litpcfdghe"), S(L"mgojkldsqh"), 0, 0);
    test(S(L"aidjksrolc"), S(L"imqnaghkfrdtlopbjesc"), 0, 0);
    test(S(L"qpghtfbaji"), S(L""), 1, S::npos);
    test(S(L"gfshlcmdjr"), S(L"nadkh"), 1, S::npos);
    test(S(L"nkodajteqp"), S(L"ofdrqmkebl"), 1, 1);
    test(S(L"gbmetiprqd"), S(L"bdfjqgatlksriohemnpc"), 1, 1);
    test(S(L"crnklpmegd"), S(L""), 5, S::npos);
    test(S(L"jsbtafedoc"), S(L"prqgn"), 5, S::npos);
    test(S(L"qnmodrtkeb"), S(L"pejafmnokr"), 5, 5);
    test(S(L"cpebqsfmnj"), S(L"odnqkgijrhabfmcestlp"), 5, 5);
    test(S(L"lmofqdhpki"), S(L""), 9, S::npos);
    test(S(L"hnefkqimca"), S(L"rtjpa"), 9, 9);
    test(S(L"drtasbgmfp"), S(L"ktsrmnqagd"), 9, 7);
    test(S(L"lsaijeqhtr"), S(L"rtdhgcisbnmoaqkfpjle"), 9, 9);
    test(S(L"elgofjmbrq"), S(L""), 10, S::npos);
    test(S(L"mjqdgalkpc"), S(L"dplqa"), 10, 8);
    test(S(L"kthqnfcerm"), S(L"dkacjoptns"), 10, 6);
    test(S(L"dfsjhanorc"), S(L"hqfimtrgnbekpdcsjalo"), 10, 9);
    test(S(L"eqsgalomhb"), S(L""), 11, S::npos);
    test(S(L"akiteljmoh"), S(L"lofbc"), 11, 8);
    test(S(L"hlbdfreqjo"), S(L"astoegbfpn"), 11, 9);
    test(S(L"taqobhlerg"), S(L"pdgreqomsncafklhtibj"), 11, 9);
    test(S(L"snafbdlghrjkpqtoceim"), S(L""), 0, S::npos);
    test(S(L"aemtbrgcklhndjisfpoq"), S(L"lbtqd"), 0, S::npos);
    test(S(L"pnracgfkjdiholtbqsem"), S(L"tboimldpjh"), 0, 0);
    test(S(L"dicfltehbsgrmojnpkaq"), S(L"slcerthdaiqjfnobgkpm"), 0, 0);
    test(S(L"jlnkraeodhcspfgbqitm"), S(L""), 1, S::npos);
    test(S(L"lhosrngtmfjikbqpcade"), S(L"aqibs"), 1, S::npos);
    test(S(L"rbtaqjhgkneisldpmfoc"), S(L"gtfblmqinc"), 1, 1);
    test(S(L"gpifsqlrdkbonjtmheca"), S(L"mkqpbtdalgniorhfescj"), 1, 1);
    test(S(L"hdpkobnsalmcfijregtq"), S(L""), 10, S::npos);
    test(S(L"jtlshdgqaiprkbcoenfm"), S(L"pblas"), 10, 10);
    test(S(L"fkdrbqltsgmcoiphneaj"), S(L"arosdhcfme"), 10, 10);
    test(S(L"crsplifgtqedjohnabmk"), S(L"blkhjeogicatqfnpdmsr"), 10, 10);
    test(S(L"niptglfbosehkamrdqcj"), S(L""), 19, S::npos);
    test(S(L"copqdhstbingamjfkler"), S(L"djkqc"), 19, 16);
    test(S(L"mrtaefilpdsgocnhqbjk"), S(L"lgokshjtpb"), 19, 19);
    test(S(L"kojatdhlcmigpbfrqnes"), S(L"bqjhtkfepimcnsgrlado"), 19, 19);
    test(S(L"eaintpchlqsbdgrkjofm"), S(L""), 20, S::npos);
    test(S(L"gjnhidfsepkrtaqbmclo"), S(L"nocfa"), 20, 19);
    test(S(L"spocfaktqdbiejlhngmr"), S(L"bgtajmiedc"), 20, 18);
    test(S(L"rphmlekgfscndtaobiqj"), S(L"lsckfnqgdahejiopbtmr"), 20, 19);
    test(S(L"liatsqdoegkmfcnbhrpj"), S(L""), 21, S::npos);
    test(S(L"binjagtfldkrspcomqeh"), S(L"gfsrt"), 21, 12);
    test(S(L"latkmisecnorjbfhqpdg"), S(L"pfsocbhjtm"), 21, 17);
    test(S(L"lecfratdjkhnsmqpoigb"), S(L"tpflmdnoicjgkberhqsa"), 21, 19);
}

template <class S>
void test1()
{
    test(S(L""), S(L""), S::npos);
    test(S(L""), S(L"laenf"), S::npos);
    test(S(L""), S(L"pqlnkmbdjo"), S::npos);
    test(S(L""), S(L"qkamfogpnljdcshbreti"), S::npos);
    test(S(L"nhmko"), S(L""), S::npos);
    test(S(L"lahfb"), S(L"irkhs"), 2);
    test(S(L"gmfhd"), S(L"kantesmpgj"), 1);
    test(S(L"odaft"), S(L"oknlrstdpiqmjbaghcfe"), 4);
    test(S(L"eolhfgpjqk"), S(L""), S::npos);
    test(S(L"nbatdlmekr"), S(L"bnrpe"), 9);
    test(S(L"jdmciepkaq"), S(L"jtdaefblso"), 8);
    test(S(L"hkbgspoflt"), S(L"oselktgbcapndfjihrmq"), 9);
    test(S(L"gprdcokbnjhlsfmtieqa"), S(L""), S::npos);
    test(S(L"qjghlnftcaismkropdeb"), S(L"bjaht"), 19);
    test(S(L"pnalfrdtkqcmojiesbhg"), S(L"hjlcmgpket"), 19);
    test(S(L"pniotcfrhqsmgdkjbael"), S(L"htaobedqikfplcgjsmrn"), 19);
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
