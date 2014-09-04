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

// size_type find_first_of(const charT* s, size_type pos, size_type n) const;

#include <string>
#include <cassert>

#include "tests/support/min_allocator.h"

template <class S>
void
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type n, typename S::size_type x)
{
    assert(s.find_first_of(str, pos, n) == x);
    if (x != S::npos)
        assert(pos <= x && x < s.size());
}

template <class S>
void test0()
{
    test(S(L""), L"", 0, 0, S::npos);
    test(S(L""), L"irkhs", 0, 0, S::npos);
    test(S(L""), L"kante", 0, 1, S::npos);
    test(S(L""), L"oknlr", 0, 2, S::npos);
    test(S(L""), L"pcdro", 0, 4, S::npos);
    test(S(L""), L"bnrpe", 0, 5, S::npos);
    test(S(L""), L"jtdaefblso", 0, 0, S::npos);
    test(S(L""), L"oselktgbca", 0, 1, S::npos);
    test(S(L""), L"eqgaplhckj", 0, 5, S::npos);
    test(S(L""), L"bjahtcmnlp", 0, 9, S::npos);
    test(S(L""), L"hjlcmgpket", 0, 10, S::npos);
    test(S(L""), L"htaobedqikfplcgjsmrn", 0, 0, S::npos);
    test(S(L""), L"hpqiarojkcdlsgnmfetb", 0, 1, S::npos);
    test(S(L""), L"dfkaprhjloqetcsimnbg", 0, 10, S::npos);
    test(S(L""), L"ihqrfebgadntlpmjksoc", 0, 19, S::npos);
    test(S(L""), L"ngtjfcalbseiqrphmkdo", 0, 20, S::npos);
    test(S(L""), L"", 1, 0, S::npos);
    test(S(L""), L"lbtqd", 1, 0, S::npos);
    test(S(L""), L"tboim", 1, 1, S::npos);
    test(S(L""), L"slcer", 1, 2, S::npos);
    test(S(L""), L"cbjfs", 1, 4, S::npos);
    test(S(L""), L"aqibs", 1, 5, S::npos);
    test(S(L""), L"gtfblmqinc", 1, 0, S::npos);
    test(S(L""), L"mkqpbtdalg", 1, 1, S::npos);
    test(S(L""), L"kphatlimcd", 1, 5, S::npos);
    test(S(L""), L"pblasqogic", 1, 9, S::npos);
    test(S(L""), L"arosdhcfme", 1, 10, S::npos);
    test(S(L""), L"blkhjeogicatqfnpdmsr", 1, 0, S::npos);
    test(S(L""), L"bmhineprjcoadgstflqk", 1, 1, S::npos);
    test(S(L""), L"djkqcmetslnghpbarfoi", 1, 10, S::npos);
    test(S(L""), L"lgokshjtpbemarcdqnfi", 1, 19, S::npos);
    test(S(L""), L"bqjhtkfepimcnsgrlado", 1, 20, S::npos);
    test(S(L"eaint"), L"", 0, 0, S::npos);
    test(S(L"binja"), L"gfsrt", 0, 0, S::npos);
    test(S(L"latkm"), L"pfsoc", 0, 1, S::npos);
    test(S(L"lecfr"), L"tpflm", 0, 2, S::npos);
    test(S(L"eqkst"), L"sgkec", 0, 4, 0);
    test(S(L"cdafr"), L"romds", 0, 5, 1);
    test(S(L"prbhe"), L"qhjistlgmr", 0, 0, S::npos);
    test(S(L"lbisk"), L"pedfirsglo", 0, 1, S::npos);
    test(S(L"hrlpd"), L"aqcoslgrmk", 0, 5, S::npos);
    test(S(L"ehmja"), L"dabckmepqj", 0, 9, 0);
    test(S(L"mhqgd"), L"pqscrjthli", 0, 10, 1);
    test(S(L"tgklq"), L"kfphdcsjqmobliagtren", 0, 0, S::npos);
    test(S(L"bocjs"), L"rokpefncljibsdhqtagm", 0, 1, S::npos);
    test(S(L"grbsd"), L"afionmkphlebtcjqsgrd", 0, 10, S::npos);
    test(S(L"ofjqr"), L"aenmqplidhkofrjbctsg", 0, 19, 0);
    test(S(L"btlfi"), L"osjmbtcadhiklegrpqnf", 0, 20, 0);
    test(S(L"clrgb"), L"", 1, 0, S::npos);
    test(S(L"tjmek"), L"osmia", 1, 0, S::npos);
    test(S(L"bgstp"), L"ckonl", 1, 1, S::npos);
    test(S(L"hstrk"), L"ilcaj", 1, 2, S::npos);
    test(S(L"kmspj"), L"lasiq", 1, 4, 2);
    test(S(L"tjboh"), L"kfqmr", 1, 5, S::npos);
    test(S(L"ilbcj"), L"klnitfaobg", 1, 0, S::npos);
    test(S(L"jkngf"), L"gjhmdlqikp", 1, 1, 3);
    test(S(L"gfcql"), L"skbgtahqej", 1, 5, S::npos);
    test(S(L"dqtlg"), L"bjsdgtlpkf", 1, 9, 2);
    test(S(L"bthpg"), L"bjgfmnlkio", 1, 10, 4);
    test(S(L"dgsnq"), L"lbhepotfsjdqigcnamkr", 1, 0, S::npos);
    test(S(L"rmfhp"), L"tebangckmpsrqdlfojhi", 1, 1, S::npos);
    test(S(L"jfdam"), L"joflqbdkhtegimscpanr", 1, 10, 1);
    test(S(L"edapb"), L"adpmcohetfbsrjinlqkg", 1, 19, 1);
    test(S(L"brfsm"), L"iacldqjpfnogbsrhmetk", 1, 20, 1);
    test(S(L"ndrhl"), L"", 2, 0, S::npos);
    test(S(L"mrecp"), L"otkgb", 2, 0, S::npos);
    test(S(L"qlasf"), L"cqsjl", 2, 1, S::npos);
    test(S(L"smaqd"), L"dpifl", 2, 2, 4);
    test(S(L"hjeni"), L"oapht", 2, 4, S::npos);
    test(S(L"ocmfj"), L"cifts", 2, 5, 3);
    test(S(L"hmftq"), L"nmsckbgalo", 2, 0, S::npos);
    test(S(L"fklad"), L"tpksqhamle", 2, 1, S::npos);
    test(S(L"dirnm"), L"tpdrchmkji", 2, 5, 2);
    test(S(L"hrgdc"), L"ijagfkblst", 2, 9, 2);
    test(S(L"ifakg"), L"kpocsignjb", 2, 10, 3);
    test(S(L"ebrgd"), L"pecqtkjsnbdrialgmohf", 2, 0, S::npos);
    test(S(L"rcjml"), L"aiortphfcmkjebgsndql", 2, 1, S::npos);
    test(S(L"peqmt"), L"sdbkeamglhipojqftrcn", 2, 10, 3);
    test(S(L"frehn"), L"ljqncehgmfktroapidbs", 2, 19, 2);
    test(S(L"tqolf"), L"rtcfodilamkbenjghqps", 2, 20, 2);
    test(S(L"cjgao"), L"", 4, 0, S::npos);
    test(S(L"kjplq"), L"mabns", 4, 0, S::npos);
    test(S(L"herni"), L"bdnrp", 4, 1, S::npos);
    test(S(L"tadrb"), L"scidp", 4, 2, S::npos);
    test(S(L"pkfeo"), L"agbjl", 4, 4, S::npos);
    test(S(L"hoser"), L"jfmpr", 4, 5, 4);
    test(S(L"kgrsp"), L"rbpefghsmj", 4, 0, S::npos);
    test(S(L"pgejb"), L"apsfntdoqc", 4, 1, S::npos);
    test(S(L"thlnq"), L"ndkjeisgcl", 4, 5, S::npos);
    test(S(L"nbmit"), L"rnfpqatdeo", 4, 9, 4);
    test(S(L"jgmib"), L"bntjlqrfik", 4, 10, 4);
    test(S(L"ncrfj"), L"kcrtmpolnaqejghsfdbi", 4, 0, S::npos);
    test(S(L"ncsik"), L"lobheanpkmqidsrtcfgj", 4, 1, S::npos);
    test(S(L"sgbfh"), L"athdkljcnreqbgpmisof", 4, 10, 4);
    test(S(L"dktbn"), L"qkdmjialrscpbhefgont", 4, 19, 4);
    test(S(L"fthqm"), L"dmasojntqleribkgfchp", 4, 20, 4);
    test(S(L"klopi"), L"", 5, 0, S::npos);
    test(S(L"dajhn"), L"psthd", 5, 0, S::npos);
    test(S(L"jbgno"), L"rpmjd", 5, 1, S::npos);
    test(S(L"hkjae"), L"dfsmk", 5, 2, S::npos);
}

template <class S>
void test1()
{
    test(S(L"gbhqo"), L"skqne", 5, 4, S::npos);
    test(S(L"ktdor"), L"kipnf", 5, 5, S::npos);
    test(S(L"ldprn"), L"hmrnqdgifl", 5, 0, S::npos);
    test(S(L"egmjk"), L"fsmjcdairn", 5, 1, S::npos);
    test(S(L"armql"), L"pcdgltbrfj", 5, 5, S::npos);
    test(S(L"cdhjo"), L"aekfctpirg", 5, 9, S::npos);
    test(S(L"jcons"), L"ledihrsgpf", 5, 10, S::npos);
    test(S(L"cbrkp"), L"mqcklahsbtirgopefndj", 5, 0, S::npos);
    test(S(L"fhgna"), L"kmlthaoqgecrnpdbjfis", 5, 1, S::npos);
    test(S(L"ejfcd"), L"sfhbamcdptojlkrenqgi", 5, 10, S::npos);
    test(S(L"kqjhe"), L"pbniofmcedrkhlstgaqj", 5, 19, S::npos);
    test(S(L"pbdjl"), L"mongjratcskbhqiepfdl", 5, 20, S::npos);
    test(S(L"gajqn"), L"", 6, 0, S::npos);
    test(S(L"stedk"), L"hrnat", 6, 0, S::npos);
    test(S(L"tjkaf"), L"gsqdt", 6, 1, S::npos);
    test(S(L"dthpe"), L"bspkd", 6, 2, S::npos);
    test(S(L"klhde"), L"ohcmb", 6, 4, S::npos);
    test(S(L"bhlki"), L"heatr", 6, 5, S::npos);
    test(S(L"lqmoh"), L"pmblckedfn", 6, 0, S::npos);
    test(S(L"mtqin"), L"aceqmsrbik", 6, 1, S::npos);
    test(S(L"dpqbr"), L"lmbtdehjrn", 6, 5, S::npos);
    test(S(L"kdhmo"), L"teqmcrlgib", 6, 9, S::npos);
    test(S(L"jblqp"), L"njolbmspac", 6, 10, S::npos);
    test(S(L"qmjgl"), L"pofnhidklamecrbqjgst", 6, 0, S::npos);
    test(S(L"rothp"), L"jbhckmtgrqnosafedpli", 6, 1, S::npos);
    test(S(L"ghknq"), L"dobntpmqklicsahgjerf", 6, 10, S::npos);
    test(S(L"eopfi"), L"tpdshainjkbfoemlrgcq", 6, 19, S::npos);
    test(S(L"dsnmg"), L"oldpfgeakrnitscbjmqh", 6, 20, S::npos);
    test(S(L"jnkrfhotgl"), L"", 0, 0, S::npos);
    test(S(L"dltjfngbko"), L"rqegt", 0, 0, S::npos);
    test(S(L"bmjlpkiqde"), L"dashm", 0, 1, 8);
    test(S(L"skrflobnqm"), L"jqirk", 0, 2, 8);
    test(S(L"jkpldtshrm"), L"rckeg", 0, 4, 1);
    test(S(L"ghasdbnjqo"), L"jscie", 0, 5, 3);
    test(S(L"igrkhpbqjt"), L"efsphndliq", 0, 0, S::npos);
    test(S(L"ikthdgcamf"), L"gdicosleja", 0, 1, 5);
    test(S(L"pcofgeniam"), L"qcpjibosfl", 0, 5, 0);
    test(S(L"rlfjgesqhc"), L"lrhmefnjcq", 0, 9, 0);
    test(S(L"itphbqsker"), L"dtablcrseo", 0, 10, 1);
    test(S(L"skjafcirqm"), L"apckjsftedbhgomrnilq", 0, 0, S::npos);
    test(S(L"tcqomarsfd"), L"pcbrgflehjtiadnsokqm", 0, 1, S::npos);
    test(S(L"rocfeldqpk"), L"nsiadegjklhobrmtqcpf", 0, 10, 4);
    test(S(L"cfpegndlkt"), L"cpmajdqnolikhgsbretf", 0, 19, 0);
    test(S(L"fqbtnkeasj"), L"jcflkntmgiqrphdosaeb", 0, 20, 0);
    test(S(L"shbcqnmoar"), L"", 1, 0, S::npos);
    test(S(L"bdoshlmfin"), L"ontrs", 1, 0, S::npos);
    test(S(L"khfrebnsgq"), L"pfkna", 1, 1, S::npos);
    test(S(L"getcrsaoji"), L"ekosa", 1, 2, 1);
    test(S(L"fjiknedcpq"), L"anqhk", 1, 4, 4);
    test(S(L"tkejgnafrm"), L"jekca", 1, 5, 1);
    test(S(L"jnakolqrde"), L"ikemsjgacf", 1, 0, S::npos);
    test(S(L"lcjptsmgbe"), L"arolgsjkhm", 1, 1, S::npos);
    test(S(L"itfsmcjorl"), L"oftkbldhre", 1, 5, 1);
    test(S(L"omchkfrjea"), L"gbkqdoeftl", 1, 9, 4);
    test(S(L"cigfqkated"), L"sqcflrgtim", 1, 10, 1);
    test(S(L"tscenjikml"), L"fmhbkislrjdpanogqcet", 1, 0, S::npos);
    test(S(L"qcpaemsinf"), L"rnioadktqlgpbcjsmhef", 1, 1, S::npos);
    test(S(L"gltkojeipd"), L"oakgtnldpsefihqmjcbr", 1, 10, 1);
    test(S(L"qistfrgnmp"), L"gbnaelosidmcjqktfhpr", 1, 19, 1);
    test(S(L"bdnpfcqaem"), L"akbripjhlosndcmqgfet", 1, 20, 1);
    test(S(L"ectnhskflp"), L"", 5, 0, S::npos);
    test(S(L"fgtianblpq"), L"pijag", 5, 0, S::npos);
    test(S(L"mfeqklirnh"), L"jrckd", 5, 1, S::npos);
    test(S(L"astedncjhk"), L"qcloh", 5, 2, 6);
    test(S(L"fhlqgcajbr"), L"thlmp", 5, 4, S::npos);
    test(S(L"epfhocmdng"), L"qidmo", 5, 5, 6);
    test(S(L"apcnsibger"), L"lnegpsjqrd", 5, 0, S::npos);
    test(S(L"aqkocrbign"), L"rjqdablmfs", 5, 1, 5);
    test(S(L"ijsmdtqgce"), L"enkgpbsjaq", 5, 5, 7);
    test(S(L"clobgsrken"), L"kdsgoaijfh", 5, 9, 5);
    test(S(L"jbhcfposld"), L"trfqgmckbe", 5, 10, S::npos);
    test(S(L"oqnpblhide"), L"igetsracjfkdnpoblhqm", 5, 0, S::npos);
    test(S(L"lroeasctif"), L"nqctfaogirshlekbdjpm", 5, 1, S::npos);
    test(S(L"bpjlgmiedh"), L"csehfgomljdqinbartkp", 5, 10, 5);
    test(S(L"pamkeoidrj"), L"qahoegcmplkfsjbdnitr", 5, 19, 5);
    test(S(L"espogqbthk"), L"dpteiajrqmsognhlfbkc", 5, 20, 5);
    test(S(L"shoiedtcjb"), L"", 9, 0, S::npos);
    test(S(L"ebcinjgads"), L"tqbnh", 9, 0, S::npos);
    test(S(L"dqmregkcfl"), L"akmle", 9, 1, S::npos);
    test(S(L"ngcrieqajf"), L"iqfkm", 9, 2, S::npos);
    test(S(L"qosmilgnjb"), L"tqjsr", 9, 4, S::npos);
    test(S(L"ikabsjtdfl"), L"jplqg", 9, 5, 9);
    test(S(L"ersmicafdh"), L"oilnrbcgtj", 9, 0, S::npos);
    test(S(L"fdnplotmgh"), L"morkglpesn", 9, 1, S::npos);
    test(S(L"fdbicojerm"), L"dmicerngat", 9, 5, 9);
    test(S(L"mbtafndjcq"), L"radgeskbtc", 9, 9, S::npos);
    test(S(L"mlenkpfdtc"), L"ljikprsmqo", 9, 10, S::npos);
    test(S(L"ahlcifdqgs"), L"trqihkcgsjamfdbolnpe", 9, 0, S::npos);
    test(S(L"bgjemaltks"), L"lqmthbsrekajgnofcipd", 9, 1, S::npos);
    test(S(L"pdhslbqrfc"), L"jtalmedribkgqsopcnfh", 9, 10, S::npos);
    test(S(L"dirhtsnjkc"), L"spqfoiclmtagejbndkrh", 9, 19, 9);
    test(S(L"dlroktbcja"), L"nmotklspigjrdhcfaebq", 9, 20, 9);
    test(S(L"ncjpmaekbs"), L"", 10, 0, S::npos);
    test(S(L"hlbosgmrak"), L"hpmsd", 10, 0, S::npos);
    test(S(L"pqfhsgilen"), L"qnpor", 10, 1, S::npos);
    test(S(L"gqtjsbdckh"), L"otdma", 10, 2, S::npos);
    test(S(L"cfkqpjlegi"), L"efhjg", 10, 4, S::npos);
    test(S(L"beanrfodgj"), L"odpte", 10, 5, S::npos);
    test(S(L"adtkqpbjfi"), L"bctdgfmolr", 10, 0, S::npos);
    test(S(L"iomkfthagj"), L"oaklidrbqg", 10, 1, S::npos);
}

template <class S>
void test2()
{
    test(S(L"sdpcilonqj"), L"dnjfsagktr", 10, 5, S::npos);
    test(S(L"gtfbdkqeml"), L"nejaktmiqg", 10, 9, S::npos);
    test(S(L"bmeqgcdorj"), L"pjqonlebsf", 10, 10, S::npos);
    test(S(L"etqlcanmob"), L"dshmnbtolcjepgaikfqr", 10, 0, S::npos);
    test(S(L"roqmkbdtia"), L"iogfhpabtjkqlrnemcds", 10, 1, S::npos);
    test(S(L"kadsithljf"), L"ngridfabjsecpqltkmoh", 10, 10, S::npos);
    test(S(L"sgtkpbfdmh"), L"athmknplcgofrqejsdib", 10, 19, S::npos);
    test(S(L"qgmetnabkl"), L"ldobhmqcafnjtkeisgrp", 10, 20, S::npos);
    test(S(L"cqjohampgd"), L"", 11, 0, S::npos);
    test(S(L"hobitmpsan"), L"aocjb", 11, 0, S::npos);
    test(S(L"tjehkpsalm"), L"jbrnk", 11, 1, S::npos);
    test(S(L"ngfbojitcl"), L"tqedg", 11, 2, S::npos);
    test(S(L"rcfkdbhgjo"), L"nqskp", 11, 4, S::npos);
    test(S(L"qghptonrea"), L"eaqkl", 11, 5, S::npos);
    test(S(L"hnprfgqjdl"), L"reaoicljqm", 11, 0, S::npos);
    test(S(L"hlmgabenti"), L"lsftgajqpm", 11, 1, S::npos);
    test(S(L"ofcjanmrbs"), L"rlpfogmits", 11, 5, S::npos);
    test(S(L"jqedtkornm"), L"shkncmiaqj", 11, 9, S::npos);
    test(S(L"rfedlasjmg"), L"fpnatrhqgs", 11, 10, S::npos);
    test(S(L"talpqjsgkm"), L"sjclemqhnpdbgikarfot", 11, 0, S::npos);
    test(S(L"lrkcbtqpie"), L"otcmedjikgsfnqbrhpla", 11, 1, S::npos);
    test(S(L"cipogdskjf"), L"bonsaefdqiprkhlgtjcm", 11, 10, S::npos);
    test(S(L"nqedcojahi"), L"egpscmahijlfnkrodqtb", 11, 19, S::npos);
    test(S(L"hefnrkmctj"), L"kmqbfepjthgilscrndoa", 11, 20, S::npos);
    test(S(L"atqirnmekfjolhpdsgcb"), L"", 0, 0, S::npos);
    test(S(L"echfkmlpribjnqsaogtd"), L"prboq", 0, 0, S::npos);
    test(S(L"qnhiftdgcleajbpkrosm"), L"fjcqh", 0, 1, 4);
    test(S(L"chamfknorbedjitgslpq"), L"fmosa", 0, 2, 3);
    test(S(L"njhqpibfmtlkaecdrgso"), L"qdbok", 0, 4, 3);
    test(S(L"ebnghfsqkprmdcljoiat"), L"amslg", 0, 5, 3);
    test(S(L"letjomsgihfrpqbkancd"), L"smpltjneqb", 0, 0, S::npos);
    test(S(L"nblgoipcrqeaktshjdmf"), L"flitskrnge", 0, 1, 19);
    test(S(L"cehkbngtjoiflqapsmrd"), L"pgqihmlbef", 0, 5, 2);
    test(S(L"mignapfoklbhcqjetdrs"), L"cfpdqjtgsb", 0, 9, 2);
    test(S(L"ceatbhlsqjgpnokfrmdi"), L"htpsiaflom", 0, 10, 2);
    test(S(L"ocihkjgrdelpfnmastqb"), L"kpjfiaceghsrdtlbnomq", 0, 0, S::npos);
    test(S(L"noelgschdtbrjfmiqkap"), L"qhtbomidljgafneksprc", 0, 1, 16);
    test(S(L"dkclqfombepritjnghas"), L"nhtjobkcefldimpsaqgr", 0, 10, 1);
    test(S(L"miklnresdgbhqcojftap"), L"prabcjfqnoeskilmtgdh", 0, 19, 0);
    test(S(L"htbcigojaqmdkfrnlsep"), L"dtrgmchilkasqoebfpjn", 0, 20, 0);
    test(S(L"febhmqtjanokscdirpgl"), L"", 1, 0, S::npos);
    test(S(L"loakbsqjpcrdhftniegm"), L"sqome", 1, 0, S::npos);
    test(S(L"reagphsqflbitdcjmkno"), L"smfte", 1, 1, 6);
    test(S(L"jitlfrqemsdhkopncabg"), L"ciboh", 1, 2, 1);
    test(S(L"mhtaepscdnrjqgbkifol"), L"haois", 1, 4, 1);
    test(S(L"tocesrfmnglpbjihqadk"), L"abfki", 1, 5, 6);
    test(S(L"lpfmctjrhdagneskbqoi"), L"frdkocntmq", 1, 0, S::npos);
    test(S(L"lsmqaepkdhncirbtjfgo"), L"oasbpedlnr", 1, 1, 19);
    test(S(L"epoiqmtldrabnkjhcfsg"), L"kltqmhgand", 1, 5, 4);
    test(S(L"emgasrilpknqojhtbdcf"), L"gdtfjchpmr", 1, 9, 1);
    test(S(L"hnfiagdpcklrjetqbsom"), L"ponmcqblet", 1, 10, 1);
    test(S(L"nsdfebgajhmtricpoklq"), L"sgphqdnofeiklatbcmjr", 1, 0, S::npos);
    test(S(L"atjgfsdlpobmeiqhncrk"), L"ljqprsmigtfoneadckbh", 1, 1, 7);
    test(S(L"sitodfgnrejlahcbmqkp"), L"ligeojhafnkmrcsqtbdp", 1, 10, 1);
    test(S(L"fraghmbiceknltjpqosd"), L"lsimqfnjarbopedkhcgt", 1, 19, 1);
    test(S(L"pmafenlhqtdbkirjsogc"), L"abedmfjlghniorcqptks", 1, 20, 1);
    test(S(L"pihgmoeqtnakrjslcbfd"), L"", 10, 0, S::npos);
    test(S(L"gjdkeprctqblnhiafsom"), L"hqtoa", 10, 0, S::npos);
    test(S(L"mkpnblfdsahrcqijteog"), L"cahif", 10, 1, 12);
    test(S(L"gckarqnelodfjhmbptis"), L"kehis", 10, 2, S::npos);
    test(S(L"gqpskidtbclomahnrjfe"), L"kdlmh", 10, 4, 10);
    test(S(L"pkldjsqrfgitbhmaecno"), L"paeql", 10, 5, 15);
    test(S(L"aftsijrbeklnmcdqhgop"), L"aghoqiefnb", 10, 0, S::npos);
    test(S(L"mtlgdrhafjkbiepqnsoc"), L"jrbqaikpdo", 10, 1, S::npos);
    test(S(L"pqgirnaefthokdmbsclj"), L"smjonaeqcl", 10, 5, 11);
    test(S(L"kpdbgjmtherlsfcqoina"), L"eqbdrkcfah", 10, 9, 10);
    test(S(L"jrlbothiknqmdgcfasep"), L"kapmsienhf", 10, 10, 11);
    test(S(L"mjogldqferckabinptsh"), L"jpqotrlenfcsbhkaimdg", 10, 0, S::npos);
    test(S(L"apoklnefbhmgqcdrisjt"), L"jlbmhnfgtcqprikeados", 10, 1, 18);
    test(S(L"ifeopcnrjbhkdgatmqls"), L"stgbhfmdaljnpqoicker", 10, 10, 10);
    test(S(L"ckqhaiesmjdnrgolbtpf"), L"oihcetflbjagdsrkmqpn", 10, 19, 10);
    test(S(L"bnlgapfimcoterskqdjh"), L"adtclebmnpjsrqfkigoh", 10, 20, 10);
    test(S(L"kgdlrobpmjcthqsafeni"), L"", 19, 0, S::npos);
    test(S(L"dfkechomjapgnslbtqir"), L"beafg", 19, 0, S::npos);
    test(S(L"rloadknfbqtgmhcsipje"), L"iclat", 19, 1, S::npos);
    test(S(L"mgjhkolrnadqbpetcifs"), L"rkhnf", 19, 2, S::npos);
    test(S(L"cmlfakiojdrgtbsphqen"), L"clshq", 19, 4, S::npos);
    test(S(L"kghbfipeomsntdalrqjc"), L"dtcoj", 19, 5, 19);
    test(S(L"eldiqckrnmtasbghjfpo"), L"rqosnjmfth", 19, 0, S::npos);
    test(S(L"abqjcfedgotihlnspkrm"), L"siatdfqglh", 19, 1, S::npos);
    test(S(L"qfbadrtjsimkolcenhpg"), L"mrlshtpgjq", 19, 5, S::npos);
    test(S(L"abseghclkjqifmtodrnp"), L"adlcskgqjt", 19, 9, S::npos);
    test(S(L"ibmsnlrjefhtdokacqpg"), L"drshcjknaf", 19, 10, S::npos);
    test(S(L"mrkfciqjebaponsthldg"), L"etsaqroinghpkjdlfcbm", 19, 0, S::npos);
    test(S(L"mjkticdeoqshpalrfbgn"), L"sgepdnkqliambtrocfhj", 19, 1, S::npos);
    test(S(L"rqnoclbdejgiphtfsakm"), L"nlmcjaqgbsortfdihkpe", 19, 10, 19);
    test(S(L"plkqbhmtfaeodjcrsing"), L"racfnpmosldibqkghjet", 19, 19, 19);
    test(S(L"oegalhmstjrfickpbndq"), L"fjhdsctkqeiolagrnmbp", 19, 20, 19);
    test(S(L"rdtgjcaohpblniekmsfq"), L"", 20, 0, S::npos);
    test(S(L"ofkqbnjetrmsaidphglc"), L"ejanp", 20, 0, S::npos);
    test(S(L"grkpahljcftesdmonqib"), L"odife", 20, 1, S::npos);
    test(S(L"jimlgbhfqkteospardcn"), L"okaqd", 20, 2, S::npos);
    test(S(L"gftenihpmslrjkqadcob"), L"lcdbi", 20, 4, S::npos);
    test(S(L"bmhldogtckrfsanijepq"), L"fsqbj", 20, 5, S::npos);
    test(S(L"nfqkrpjdesabgtlcmoih"), L"bigdomnplq", 20, 0, S::npos);
    test(S(L"focalnrpiqmdkstehbjg"), L"apiblotgcd", 20, 1, S::npos);
    test(S(L"rhqdspkmebiflcotnjga"), L"acfhdenops", 20, 5, S::npos);
    test(S(L"rahdtmsckfboqlpniegj"), L"jopdeamcrk", 20, 9, S::npos);
    test(S(L"fbkeiopclstmdqranjhg"), L"trqncbkgmh", 20, 10, S::npos);
    test(S(L"lifhpdgmbconstjeqark"), L"tomglrkencbsfjqpihda", 20, 0, S::npos);
}

template <class S>
void test3()
{
    test(S(L"pboqganrhedjmltsicfk"), L"gbkhdnpoietfcmrslajq", 20, 1, S::npos);
    test(S(L"klchabsimetjnqgorfpd"), L"rtfnmbsglkjaichoqedp", 20, 10, S::npos);
    test(S(L"sirfgmjqhctndbklaepo"), L"ohkmdpfqbsacrtjnlgei", 20, 19, S::npos);
    test(S(L"rlbdsiceaonqjtfpghkm"), L"dlbrteoisgphmkncajfq", 20, 20, S::npos);
    test(S(L"ecgdanriptblhjfqskom"), L"", 21, 0, S::npos);
    test(S(L"fdmiarlpgcskbhoteqjn"), L"sjrlo", 21, 0, S::npos);
    test(S(L"rlbstjqopignecmfadkh"), L"qjpor", 21, 1, S::npos);
    test(S(L"grjpqmbshektdolcafni"), L"odhfn", 21, 2, S::npos);
    test(S(L"sakfcohtqnibprjmlged"), L"qtfin", 21, 4, S::npos);
    test(S(L"mjtdglasihqpocebrfkn"), L"hpqfo", 21, 5, S::npos);
    test(S(L"okaplfrntghqbmeicsdj"), L"fabmertkos", 21, 0, S::npos);
    test(S(L"sahngemrtcjidqbklfpo"), L"brqtgkmaej", 21, 1, S::npos);
    test(S(L"dlmsipcnekhbgoaftqjr"), L"nfrdeihsgl", 21, 5, S::npos);
    test(S(L"ahegrmqnoiklpfsdbcjt"), L"hlfrosekpi", 21, 9, S::npos);
    test(S(L"hdsjbnmlegtkqripacof"), L"atgbkrjdsm", 21, 10, S::npos);
    test(S(L"pcnedrfjihqbalkgtoms"), L"blnrptjgqmaifsdkhoec", 21, 0, S::npos);
    test(S(L"qjidealmtpskrbfhocng"), L"ctpmdahebfqjgknloris", 21, 1, S::npos);
    test(S(L"qeindtagmokpfhsclrbj"), L"apnkeqthrmlbfodiscgj", 21, 10, S::npos);
    test(S(L"kpfegbjhsrnodltqciam"), L"jdgictpframeoqlsbknh", 21, 19, S::npos);
    test(S(L"hnbrcplsjfgiktoedmaq"), L"qprlsfojamgndekthibc", 21, 20, S::npos);
}

int main()
{
    {
    typedef barefootc::basic_string<wchar_t> S;
    test0<S>();
    test1<S>();
    test2<S>();
    test3<S>();
    }
#if __cplusplus >= 201103L
    {
    typedef barefootc::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test0<S>();
    test1<S>();
    test2<S>();
    test3<S>();
    }
#endif
}
