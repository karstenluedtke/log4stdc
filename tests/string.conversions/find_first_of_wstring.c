
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/mempool.h"
#include "barefootc/utf8.h"

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

static int
test(const char *s1, const wchar_t *s2, size_t pos1, size_t expected)
{
	int rc;
	bfc_string_t s, ws;
	size_t pos, wslen, bufsize;
	char *u8buf;

	wslen = wcslen(s2);
	bufsize = 2*wslen + 20;
	u8buf = alloca(bufsize);
	bfc_utf8_from_wchar(u8buf, bufsize, s2, wslen);

	L4SC_TRACE(logger, "%s(\"%s\", L\"%s\", %lu, expected %lu)",
		__FUNCTION__, s1, u8buf, (unsigned long) pos1,
		(unsigned long) expected);

	rc = bfc_init_basic_string_c_str(&s, sizeof(s), pool, s1);
	assert(rc >= 0);

	rc = bfc_init_shared_wstring_c_str(&ws, sizeof(ws), s2);
	assert(rc >= 0);

	pos = bfc_string_find_first_of_bfstr(&s, &ws, pos1);
	assert (pos == expected);

	L4SC_TRACE(logger, "%s: bufsize %lu",
		__FUNCTION__, (unsigned long) bufsize);
	L4SC_TRACE(logger, "%s(\"%s\", L\"%s\", %lu, expected %lu): pos %lu",
		"PASS", s1, u8buf, (unsigned long) pos1,
		(unsigned long)expected, (unsigned long)pos);

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);

#define S1(x) x
#define S2(x) L##x
	test(S1(""), S2(""), 0, BFC_NPOS);
	test(S1(""), S2("laenf"), 0, BFC_NPOS);
	test(S1(""), S2("pqlnkmbdjo"), 0, BFC_NPOS);
	test(S1(""), S2("qkamfogpnljdcshbreti"), 0, BFC_NPOS);
	test(S1(""), S2(""), 1, BFC_NPOS);
	test(S1(""), S2("bjaht"), 1, BFC_NPOS);
	test(S1(""), S2("hjlcmgpket"), 1, BFC_NPOS);
	test(S1(""), S2("htaobedqikfplcgjsmrn"), 1, BFC_NPOS);
	test(S1("fodgq"), S2(""), 0, BFC_NPOS);
	test(S1("qanej"), S2("dfkap"), 0, 1);
	test(S1("clbao"), S2("ihqrfebgad"), 0, 2);
	test(S1("mekdn"), S2("ngtjfcalbseiqrphmkdo"), 0, 0);
	test(S1("srdfq"), S2(""), 1, BFC_NPOS);
	test(S1("oemth"), S2("ikcrq"), 1, BFC_NPOS);
	test(S1("cdaih"), S2("dmajblfhsg"), 1, 1);
	test(S1("qohtk"), S2("oqftjhdmkgsblacenirp"), 1, 1);
	test(S1("cshmd"), S2(""), 2, BFC_NPOS);
	test(S1("lhcdo"), S2("oebqi"), 2, 4);
	test(S1("qnsoh"), S2("kojhpmbsfe"), 2, 2);
	test(S1("pkrof"), S2("acbsjqogpltdkhinfrem"), 2, 2);
	test(S1("fmtsp"), S2(""), 4, BFC_NPOS);
	test(S1("khbpm"), S2("aobjd"), 4, BFC_NPOS);
	test(S1("pbsji"), S2("pcbahntsje"), 4, BFC_NPOS);
	test(S1("mprdj"), S2("fhepcrntkoagbmldqijs"), 4, 4);
	test(S1("eqmpa"), S2(""), 5, BFC_NPOS);
	test(S1("omigs"), S2("kocgb"), 5, BFC_NPOS);
	test(S1("onmje"), S2("fbslrjiqkm"), 5, BFC_NPOS);
	test(S1("oqmrj"), S2("jeidpcmalhfnqbgtrsko"), 5, BFC_NPOS);
	test(S1("schfa"), S2(""), 6, BFC_NPOS);
	test(S1("igdsc"), S2("qngpd"), 6, BFC_NPOS);
	test(S1("brqgo"), S2("rodhqklgmb"), 6, BFC_NPOS);
	test(S1("tnrph"), S2("thdjgafrlbkoiqcspmne"), 6, BFC_NPOS);
	test(S1("hcjitbfapl"), S2(""), 0, BFC_NPOS);
	test(S1("daiprenocl"), S2("ashjd"), 0, 0);
	test(S1("litpcfdghe"), S2("mgojkldsqh"), 0, 0);
	test(S1("aidjksrolc"), S2("imqnaghkfrdtlopbjesc"), 0, 0);
	test(S1("qpghtfbaji"), S2(""), 1, BFC_NPOS);
	test(S1("gfshlcmdjr"), S2("nadkh"), 1, 3);
	test(S1("nkodajteqp"), S2("ofdrqmkebl"), 1, 1);
	test(S1("gbmetiprqd"), S2("bdfjqgatlksriohemnpc"), 1, 1);
	test(S1("crnklpmegd"), S2(""), 5, BFC_NPOS);
	test(S1("jsbtafedoc"), S2("prqgn"), 5, BFC_NPOS);
	test(S1("qnmodrtkeb"), S2("pejafmnokr"), 5, 5);
	test(S1("cpebqsfmnj"), S2("odnqkgijrhabfmcestlp"), 5, 5);
	test(S1("lmofqdhpki"), S2(""), 9, BFC_NPOS);
	test(S1("hnefkqimca"), S2("rtjpa"), 9, 9);
	test(S1("drtasbgmfp"), S2("ktsrmnqagd"), 9, BFC_NPOS);
	test(S1("lsaijeqhtr"), S2("rtdhgcisbnmoaqkfpjle"), 9, 9);
	test(S1("elgofjmbrq"), S2(""), 10, BFC_NPOS);
	test(S1("mjqdgalkpc"), S2("dplqa"), 10, BFC_NPOS);
	test(S1("kthqnfcerm"), S2("dkacjoptns"), 10, BFC_NPOS);
	test(S1("dfsjhanorc"), S2("hqfimtrgnbekpdcsjalo"), 10, BFC_NPOS);
	test(S1("eqsgalomhb"), S2(""), 11, BFC_NPOS);
	test(S1("akiteljmoh"), S2("lofbc"), 11, BFC_NPOS);
	test(S1("hlbdfreqjo"), S2("astoegbfpn"), 11, BFC_NPOS);
	test(S1("taqobhlerg"), S2("pdgreqomsncafklhtibj"), 11, BFC_NPOS);
	test(S1("snafbdlghrjkpqtoceim"), S2(""), 0, BFC_NPOS);
	test(S1("aemtbrgcklhndjisfpoq"), S2("lbtqd"), 0, 3);
	test(S1("pnracgfkjdiholtbqsem"), S2("tboimldpjh"), 0, 0);
	test(S1("dicfltehbsgrmojnpkaq"), S2("slcerthdaiqjfnobgkpm"), 0, 0);
	test(S1("jlnkraeodhcspfgbqitm"), S2(""), 1, BFC_NPOS);
	test(S1("lhosrngtmfjikbqpcade"), S2("aqibs"), 1, 3);
	test(S1("rbtaqjhgkneisldpmfoc"), S2("gtfblmqinc"), 1, 1);
	test(S1("gpifsqlrdkbonjtmheca"), S2("mkqpbtdalgniorhfescj"), 1, 1);
	test(S1("hdpkobnsalmcfijregtq"), S2(""), 10, BFC_NPOS);
	test(S1("jtlshdgqaiprkbcoenfm"), S2("pblas"), 10, 10);
	test(S1("fkdrbqltsgmcoiphneaj"), S2("arosdhcfme"), 10, 10);
	test(S1("crsplifgtqedjohnabmk"), S2("blkhjeogicatqfnpdmsr"), 10, 10);
	test(S1("niptglfbosehkamrdqcj"), S2(""), 19, BFC_NPOS);
	test(S1("copqdhstbingamjfkler"), S2("djkqc"), 19, BFC_NPOS);
	test(S1("mrtaefilpdsgocnhqbjk"), S2("lgokshjtpb"), 19, 19);
	test(S1("kojatdhlcmigpbfrqnes"), S2("bqjhtkfepimcnsgrlado"), 19, 19);
	test(S1("eaintpchlqsbdgrkjofm"), S2(""), 20, BFC_NPOS);
	test(S1("gjnhidfsepkrtaqbmclo"), S2("nocfa"), 20, BFC_NPOS);
	test(S1("spocfaktqdbiejlhngmr"), S2("bgtajmiedc"), 20, BFC_NPOS);
	test(S1("rphmlekgfscndtaobiqj"), S2("lsckfnqgdahejiopbtmr"), 20, BFC_NPOS);
	test(S1("liatsqdoegkmfcnbhrpj"), S2(""), 21, BFC_NPOS);
	test(S1("binjagtfldkrspcomqeh"), S2("gfsrt"), 21, BFC_NPOS);
	test(S1("latkmisecnorjbfhqpdg"), S2("pfsocbhjtm"), 21, BFC_NPOS);
	test(S1("lecfratdjkhnsmqpoigb"), S2("tpflmdnoicjgkberhqsa"), 21, BFC_NPOS);

	test(S1(""), S2(""), 0, BFC_NPOS);
	test(S1(""), S2("laenf"), 0, BFC_NPOS);
	test(S1(""), S2("pqlnkmbdjo"), 0, BFC_NPOS);
	test(S1(""), S2("qkamfogpnljdcshbreti"), 0, BFC_NPOS);
	test(S1("nhmko"), S2(""), 0, BFC_NPOS);
	test(S1("lahfb"), S2("irkhs"), 0, 2);
	test(S1("gmfhd"), S2("kantesmpgj"), 0, 0);
	test(S1("odaft"), S2("oknlrstdpiqmjbaghcfe"), 0, 0);
	test(S1("eolhfgpjqk"), S2(""), 0, BFC_NPOS);
	test(S1("nbatdlmekr"), S2("bnrpe"), 0, 0);
	test(S1("jdmciepkaq"), S2("jtdaefblso"), 0, 0);
	test(S1("hkbgspoflt"), S2("oselktgbcapndfjihrmq"), 0, 0);
	test(S1("gprdcokbnjhlsfmtieqa"), S2(""), 0, BFC_NPOS);
	test(S1("qjghlnftcaismkropdeb"), S2("bjaht"), 0, 1);
	test(S1("pnalfrdtkqcmojiesbhg"), S2("hjlcmgpket"), 0, 0);
	test(S1("pniotcfrhqsmgdkjbael"), S2("htaobedqikfplcgjsmrn"), 0, 0);

	test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"),
	     S2("bcd"), 0, 2);

	do {
	    wchar_t s2[] = { 0xF6, 'p', 'q' , 0 };
	    test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"),
		 s2, 0, 15);
	} while (0 /* just once */);

	do {
	    wchar_t s2[] = { 'm', 'n', 0xF6, 'p', 'q' , 0 };
	    test(S1("\303\244bcdefghijklmn\303\266pqrst\303\274"),
		 s2, 0, 13);
	} while (0 /* just once */);

	return (0);
}
