/**
 * @file bfc_string_decode_html_entities.c
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

struct entity_table {
	const char *ent;
	unsigned short unicode;
};

static const struct entity_table html_entities[] = {
	{ "quot", 	  34 }, /* " 	Anführungszeichen oben  */
	{ "amp",	  38 }, /* & 	Ampersand, kaufmännisches Und */
	{ "apos",	  39 }, /* ' 	Apostroph */
	{ "lt", 	  60 }, /* < 	öffnende spitze Klammer  */
	{ "gt", 	  62 }, /* > 	schließende spitze Klammer  */
	{ "nbsp", 	 160 }, /*   	Erzwungenes Leerzeichen  */
	{ "iexcl", 	 161 }, /* ¡ 	umgekehrtes Ausrufezeichen  */
	{ "cent", 	 162 }, /* ¢ 	Cent-Zeichen  */
	{ "pound", 	 163 }, /* £ 	Pfund-Zeichen  */
	{ "curren", 	 164 }, /* ¤ 	Währungszeichen  */
	{ "yen", 	 165 }, /* ¥ 	Yen-Zeichen  */
	{ "brvbar", 	 166 }, /* ¦ 	durchbrochener Strich  */
	{ "sect", 	 167 }, /* § 	Paragraph-Zeichen  */
	{ "uml", 	 168 }, /* ¨ 	Pünktchen oben  */
	{ "copy", 	 169 }, /* © 	Copyright-Zeichen  */
	{ "ordf", 	 170 }, /* ª 	Ordinal-Zeichen weiblich  */
	{ "laquo", 	 171 }, /* « 	angewinkelte Anführungszeichen links */
	{ "not", 	 172 }, /* ¬ 	Verneinungs-Zeichen  */
	{ "shy", 	 173 }, /* ­ 	bedingter Trennstrich  */
	{ "reg", 	 174 }, /* ® 	Registriermarke-Zeichen  */
	{ "macr", 	 175 }, /* ¯ 	Überstrich  */
	{ "deg", 	 176 }, /* ° 	Grad-Zeichen  */
	{ "plusmn", 	 177 }, /* ± 	Plusminus-Zeichen  */
	{ "sup2", 	 178 }, /* ² 	Hoch-2-Zeichen  */
	{ "sup3", 	 179 }, /* ³ 	Hoch-3-Zeichen  */
	{ "acute", 	 180 }, /* ´ 	Akut-Zeichen  */
	{ "micro", 	 181 }, /* µ 	Mikro-Zeichen  */
	{ "para", 	 182 }, /* ¶ 	Absatz-Zeichen  */
	{ "middot", 	 183 }, /* · 	Mittelpunkt  */
	{ "cedil", 	 184 }, /* ¸ 	Häkchen unten  */
	{ "sup1", 	 185 }, /* ¹ 	Hoch-1-Zeichen  */
	{ "ordm", 	 186 }, /* º 	Ordinal-Zeichen männlich  */
	{ "raquo", 	 187 }, /* » 	angewinkelte Anführungszeichen rechts*/
	{ "frac14", 	 188 }, /* ¼ 	ein Viertel  */
	{ "frac12", 	 189 }, /* ½ 	ein Halb  */
	{ "frac34", 	 190 }, /* ¾ 	drei Viertel  */
	{ "iquest", 	 191 }, /* ¿ 	umgekehrtes Fragezeichen  */
	{ "Agrave", 	 192 }, /* À 	A mit accent grave (Gravis)  */
	{ "Aacute", 	 193 }, /* Á 	A mit accent aigu (Akut)  */
	{ "Acirc", 	 194 }, /* Â 	A mit Zirkumflex  */
	{ "Atilde", 	 195 }, /* Ã 	A mit Tilde  */
	{ "Auml", 	 196 }, /* Ä 	A Umlaut  */
	{ "Aring", 	 197 }, /* Å 	A mit Ring  */
	{ "AElig", 	 198 }, /* Æ 	A mit legiertem E  */
	{ "Ccedil", 	 199 }, /* Ç 	C mit Häkchen  */
	{ "Egrave", 	 200 }, /* È 	E mit accent grave (Gravis)  */
	{ "Eacute", 	 201 }, /* É 	E mit accent aigu (Akut)  */
	{ "Ecirc", 	 202 }, /* Ê 	E mit Zirkumflex  */
	{ "Euml", 	 203 }, /* Ë 	E Umlaut  */
	{ "Igrave", 	 204 }, /* Ì 	I mit accent grave (Gravis)  */
	{ "Iacute", 	 205 }, /* Í 	I mit accent aigu (Akut)  */
	{ "Icirc", 	 206 }, /* Î 	I mit Zirkumflex  */
	{ "Iuml", 	 207 }, /* Ï 	I Umlaut  */
	{ "ETH", 	 208 }, /* Ð 	großes Eth (isländisch)  */
	{ "Ntilde", 	 209 }, /* Ñ 	N mit Tilde  */
	{ "Ograve", 	 210 }, /* Ò 	O mit accent grave (Gravis)  */
	{ "Oacute", 	 211 }, /* Ó 	O mit accent aigu (Akut)  */
	{ "Ocirc", 	 212 }, /* Ô 	O mit Zirkumflex  */
	{ "Otilde", 	 213 }, /* Õ 	O mit Tilde  */
	{ "Ouml", 	 214 }, /* Ö 	O Umlaut  */
	{ "times", 	 215 }, /* × 	Mal-Zeichen  */
	{ "Oslash", 	 216 }, /* Ø 	O mit Schrägstrich  */
	{ "Ugrave", 	 217 }, /* Ù 	U mit accent grave (Gravis)  */
	{ "Uacute", 	 218 }, /* Ú 	U mit accent aigu (Akut)  */
	{ "Ucirc", 	 219 }, /* Û 	U mit Zirkumflex  */
	{ "Uuml", 	 220 }, /* Ü 	U Umlaut  */
	{ "Yacute", 	 221 }, /* Ý 	Y mit accent aigu (Akut)  */
	{ "THORN", 	 222 }, /* Þ 	großes Thorn (isländisch)  */
	{ "szlig", 	 223 }, /* ß 	scharfes S  */
	{ "agrave", 	 224 }, /* à 	a mit accent grave (Gravis)  */
	{ "aacute", 	 225 }, /* á 	a mit accent aigu (Akut)  */
	{ "acirc", 	 226 }, /* â 	a mit Zirkumflex  */
	{ "atilde", 	 227 }, /* ã 	a mit Tilde  */
	{ "auml", 	 228 }, /* ä 	a Umlaut  */
	{ "aring", 	 229 }, /* å 	a mit Ring  */
	{ "aelig", 	 230 }, /* æ 	a mit legiertem e  */
	{ "ccedil", 	 231 }, /* ç 	c mit Häkchen  */
	{ "egrave", 	 232 }, /* è 	e mit accent grave (Gravis)  */
	{ "eacute", 	 233 }, /* é 	e mit accent aigu (Akut)  */
	{ "ecirc", 	 234 }, /* ê 	e mit Zirkumflex  */
	{ "euml", 	 235 }, /* ë 	e Umlaut  */
	{ "igrave", 	 236 }, /* ì 	i mit accent grave (Gravis)  */
	{ "iacute", 	 237 }, /* í 	i mit accent aigu (Akut)  */
	{ "icirc", 	 238 }, /* î 	i mit Zirkumflex  */
	{ "iuml", 	 239 }, /* ï 	i Umlaut  */
	{ "eth", 	 240 }, /* ð 	kleines Eth (isländisch)  */
	{ "ntilde", 	 241 }, /* ñ 	n mit Tilde  */
	{ "ograve", 	 242 }, /* ò 	o mit accent grave (Gravis)  */
	{ "oacute", 	 243 }, /* ó 	o mit accent aigu (Akut)  */
	{ "ocirc", 	 244 }, /* ô 	o mit Zirkumflex  */
	{ "otilde", 	 245 }, /* õ 	o mit Tilde  */
	{ "ouml", 	 246 }, /* ö 	o Umlaut  */
	{ "divide", 	 247 }, /* ÷ 	Divisions-Zeichen  */
	{ "oslash", 	 248 }, /* ø 	o mit Schrägstrich  */
	{ "ugrave", 	 249 }, /* ù 	u mit accent grave (Gravis)  */
	{ "uacute", 	 250 }, /* ú 	u mit accent aigu (Akut)  */
	{ "ucirc", 	 251 }, /* û 	u mit Zirkumflex  */
	{ "uuml", 	 252 }, /* ü 	u Umlaut  */
	{ "yacute", 	 253 }, /* ý 	y mit accent aigu (Akut)  */
	{ "thorn", 	 254 }, /* þ 	kleines Thorn (isländisch)  */
	{ "yuml", 	 255 }, /* ÿ 	y Umlaut  */
	{ "Alpha", 	 913 }, /* Α 	Alpha groß  */
	{ "alpha", 	 945 }, /* α 	alpha klein  */
	{ "Beta", 	 914 }, /* Β 	Beta groß  */
	{ "beta", 	 946 }, /* β 	beta klein  */
	{ "Gamma", 	 915 }, /* Γ 	Gamma groß  */
	{ "gamma", 	 947 }, /* γ 	gamma klein  */
	{ "Delta", 	 916 }, /* Δ 	Delta groß  */
	{ "delta", 	 948 }, /* δ 	delta klein  */
	{ "Epsilon", 	 917 }, /* Ε 	Epsilon groß  */
	{ "epsilon", 	 949 }, /* ε 	epsilon klein  */
	{ "Zeta", 	 918 }, /* Ζ 	Zeta groß  */
	{ "zeta", 	 950 }, /* ζ 	zeta klein  */
	{ "Eta", 	 919 }, /* Η 	Eta groß  */
	{ "eta", 	 951 }, /* η 	eta klein  */
	{ "Theta", 	 920 }, /* Θ 	Theta groß  */
	{ "theta", 	 952 }, /* θ 	theta klein  */
	{ "Iota", 	 921 }, /* Ι 	Iota groß  */
	{ "iota", 	 953 }, /* ι 	iota klein  */
	{ "Kappa", 	 922 }, /* Κ 	Kappa groß  */
	{ "kappa", 	 954 }, /* κ 	kappa klein  */
	{ "Lambda", 	 923 }, /* Λ 	Lambda groß  */
	{ "lambda", 	 955 }, /* λ 	lambda klein  */
	{ "Mu", 	 924 }, /* Μ 	My groß  */
	{ "mu", 	 956 }, /* μ 	my klein  */
	{ "Nu", 	 925 }, /* Ν 	Ny groß  */
	{ "nu", 	 957 }, /* ν 	ny klein  */
	{ "Xi", 	 926 }, /* Ξ 	Xi groß  */
	{ "xi", 	 958 }, /* ξ 	xi klein  */
	{ "Omicron", 	 927 }, /* Ο 	Omikron groß  */
	{ "omicron", 	 959 }, /* ο 	omikron klein  */
	{ "Pi", 	 928 }, /* Π 	Pi groß  */
	{ "pi", 	 960 }, /* π 	pi klein  */
	{ "Rho", 	 929 }, /* Ρ 	Rho groß  */
	{ "rho", 	 961 }, /* ρ 	rho klein  */
	{ "Sigma", 	 931 }, /* Σ 	Sigma groß  */
	{ "sigmaf", 	 962 }, /* ς 	Schluss-Sigma  */
	{ "sigma", 	 963 }, /* σ 	sigma klein  */
	{ "Tau", 	 932 }, /* Τ 	Tau groß  */
	{ "tau", 	 964 }, /* τ 	tau klein  */
	{ "Upsilon", 	 933 }, /* Υ 	Ypsilon groß  */
	{ "upsilon", 	 965 }, /* υ 	ypsilon klein  */
	{ "Phi", 	 934 }, /* Φ 	Phi groß  */
	{ "phi", 	 966 }, /* φ 	phi klein  */
	{ "Chi", 	 935 }, /* Χ 	Chi groß  */
	{ "chi", 	 967 }, /* χ 	chi klein  */
	{ "Psi", 	 936 }, /* Ψ 	Psi groß  */
	{ "psi", 	 968 }, /* ψ 	psi klein  */
	{ "Omega", 	 937 }, /* Ω 	Omega groß  */
	{ "omega", 	 969 }, /* ω 	omega klein  */
	{ "thetasym", 	 977 }, /* ϑ 	theta Symbol  */
	{ "upsih", 	 978 }, /* ϒ 	ypsilon mit Haken  */
	{ "piv", 	 982 }, /* ϖ 	pi Symbol  */
	{ "forall", 	8704 }, /* ∀ 	für alle  */
	{ "part", 	8706 }, /* ∂ 	partiell  */
	{ "exist", 	8707 }, /* ∃ 	existiert  */
	{ "empty", 	8709 }, /* ∅ 	leere Menge  */
	{ "nabla", 	8711 }, /* ∇ 	Nabla  */
	{ "isin", 	8712 }, /* ∈ 	Element von  */
	{ "notin", 	8713 }, /* ∉ 	kein Element von  */
	{ "ni", 	8715 }, /* ∋ 	Element von (gespiegelt)  */
	{ "prod", 	8719 }, /* ∏ 	Produkt  */
	{ "sum", 	8721 }, /* ∑ 	Summe  */
	{ "minus", 	8722 }, /* − 	minus  */
	{ "lowast", 	8727 }, /* ∗ 	Asterisk  */
	{ "radic", 	8730 }, /* √ 	Wurzel  */
	{ "prop", 	8733 }, /* ∝ 	proportional zu  */
	{ "infin", 	8734 }, /* ∞ 	unendlich  */
	{ "ang", 	8736 }, /* ∠ 	Winkel  */
	{ "and", 	8743 }, /* ∧ 	und  */
	{ "or", 	8744 }, /* ∨ 	oder  */
	{ "cap", 	8745 }, /* ∩ 	Schnittmenge  */
	{ "cup", 	8746 }, /* ∪ 	Vereinigungsmenge  */
	{ "int", 	8747 }, /* ∫ 	Integral  */
	{ "there4", 	8756 }, /* ∴ 	deshalb  */
	{ "sim", 	8764 }, /* ∼ 	ähnlich wie  */
	{ "cong", 	8773 }, /* ≅ 	kongruent  */
	{ "asymp", 	8776 }, /* ≈ 	beinahe gleich  */
	{ "ne", 	8800 }, /* ≠ 	ungleich  */
	{ "equiv", 	8801 }, /* ≡ 	identisch mit  */
	{ "le", 	8804 }, /* ≤ 	kleiner gleich  */
	{ "ge", 	8805 }, /* ≥ 	größer gleich  */
	{ "sub", 	8834 }, /* ⊂ 	Untermenge von  */
	{ "sup", 	8835 }, /* ⊃ 	Obermenge von  */
	{ "nsub", 	8836 }, /* ⊄ 	keine Untermenge von  */
	{ "sube", 	8838 }, /* ⊆ 	Untermenge von oder gleich mit  */
	{ "supe", 	8839 }, /* ⊇ 	Obermenge von oder gleich mit  */
	{ "oplus", 	8853 }, /* ⊕ 	Restklassenaddition  */
	{ "otimes", 	8855 }, /* ⊗ 	Kronecker-Produkt  */
	{ "perp", 	8869 }, /* ⊥ 	senkrecht zu  */
	{ "sdot", 	8901 }, /* ⋅ 	Punkt-Operator  */
	{ "loz", 	9674 }, /* ◊ 	Raute  */
	{ "lceil", 	8968 }, /* ⌈ 	links oben  */
	{ "rceil", 	8969 }, /* ⌉ 	rechts oben  */
	{ "lfloor", 	8970 }, /* ⌊ 	links unten  */
	{ "rfloor", 	8971 }, /* ⌋ 	rechts unten  */
	{ "lang", 	9001 }, /* ⟨ 	spitze Klammer links  */
	{ "rang", 	9002 }, /* ⟩ 	spitze Klammer rechts  */
	{ "larr", 	8592 }, /* ← 	Pfeil links  */
	{ "uarr", 	8593 }, /* ↑ 	Pfeil oben  */
	{ "rarr", 	8594 }, /* → 	Pfeil rechts  */
	{ "darr", 	8595 }, /* ↓ 	Pfeil unten  */
	{ "harr", 	8596 }, /* ↔ 	Pfeil links/rechts  */
	{ "crarr", 	8629 }, /* ↵ 	Pfeil unten-Knick-links  */
	{ "lArr", 	8656 }, /* ⇐ 	Doppelpfeil links  */
	{ "uArr", 	8657 }, /* ⇑ 	Doppelpfeil oben  */
	{ "rArr", 	8658 }, /* ⇒ 	Doppelpfeil rechts  */
	{ "dArr", 	8659 }, /* ⇓ 	Doppelpfeil unten  */
	{ "hArr", 	8660 }, /* ⇔ 	Doppelpfeil links/rechts  */
	{ "bull", 	8226 }, /* • 	Bullet-Zeichen  */
	{ "prime", 	8242 }, /* ′ 	Minutenzeichen  */
	{ "Prime", 	8243 }, /* ″ 	Sekundenzeichen  */
	{ "oline", 	8254 }, /* ‾ 	Überstrich  */
	{ "frasl", 	8260 }, /* ⁄ 	Bruchstrich  */
	{ "weierp", 	8472 }, /* ℘ 	Weierstraß p  */
	{ "image", 	8465 }, /* ℑ 	Zeichen für "imaginär"  */
	{ "real", 	8476 }, /* ℜ 	Zeichen für "real"  */
	{ "trade", 	8482 }, /* ™ 	Trademark-Zeichen  */
	{ "euro", 	8364 }, /* € 	Euro-Zeichen  */
	{ "alefsym", 	8501 }, /* ℵ 	Alef-Symbol  */
	{ "spades", 	9824 }, /* ♠ 	Pik-Zeichen  */
	{ "clubs", 	9827 }, /* ♣ 	Kreuz-Zeichen  */
	{ "hearts", 	9829 }, /* ♥ 	Herz-Zeichen  */
	{ "diams", 	9830 }, /* ♦ 	Karo-Zeichen  */
	{ "OElig", 	 338 }, /* Œ 	OE-Ligatur  */
	{ "oelig", 	 339 }, /* œ 	oe-Ligatur  */
	{ "Scaron", 	 352 }, /* Š 	S mit Hatschek (Caron)  */
	{ "scaron", 	 353 }, /* š 	s mit Hatschek (Caron)  */
	{ "Yuml", 	 376 }, /* Ÿ 	Y Umlaut  */
	{ "fnof", 	 402 }, /* ƒ 	Funktion  */
	{ "ensp", 	8194 }, /*   	Leerzeichen Breite n  */
	{ "emsp", 	8195 }, /*   	Leerzeichen Breite m  */
	{ "thinsp", 	8201 }, /*   	Schmales Leerzeichen  */
	{ "zwnj", 	8204 }, /*  	null breiter Nichtverbinder  */
	{ "zwj", 	8205 }, /*  	null breiter Verbinder  */
	{ "lrm", 	8206 }, /*  	links-nach-rechts-Zeichen  */
	{ "rlm", 	8207 }, /*  	rechts-nach-links-Zeichen  */
	{ "ndash", 	8211 }, /* – 	Gedankenstrich Breite n  */
	{ "mdash", 	8212 }, /* — 	Gedankenstrich Breite m  */
	{ "lsquo", 	8216 }, /* ‘ 	einfaches Anführungszeichen links  */
	{ "rsquo", 	8217 }, /* ’ 	einfaches Anführungszeichen rechts  */
	{ "sbquo", 	8218 }, /* ‚ 	einfaches low-9-Zeichen  */
	{ "ldquo", 	8220 }, /* “ 	doppeltes Anführungszeichen links  */
	{ "rdquo", 	8221 }, /* ” 	doppeltes Anführungszeichen rechts  */
	{ "bdquo", 	8222 }, /* „ 	doppeltes low-9-Zeichen rechts  */
	{ "dagger", 	8224 }, /* † 	Kreuz  */
	{ "Dagger", 	8225 }, /* ‡ 	Doppelkreuz  */
	{ "hellip", 	8230 }, /* … 	Horizontale Ellipse (Auslassungszchn)*/
	{ "permil", 	8240 }, /* ‰ 	Promille  */
	{ "lsaquo", 	8249 }, /* ‹ 	angewinkeltes einzelnes Anf.zeichen */
	{ "rsaquo", 	8250 }, /* › 	angewinkeltes einzelnes Anf.zeichen */
	{ "circ", 	 710 }, /* ˆ 	Zirkumflex  */
	{ "tilde", 	 732 }, /* ˜ 	kleine Tilde  */
	{  NULL, 	   0 }
};

/**
 * @brief    bfc_string_decode_html_entities
 */
int
bfc_string_decode_html_entities(bfc_strptr_t s)
{
	size_t pos = 0;
	size_t len, amp, sem;
	const struct entity_table *tp;
	unsigned long unicode = 0;
	int n;
	char c1, c2;
	bfc_string_t sub;
	bfc_iterator_t i1, i2;
	char buf[20];

	while (((len = bfc_strlen(s)) > pos)
	    && ((amp = bfc_string_find_char(s, '&', pos)) != BFC_NPOS)
	    && ((sem = bfc_string_find_char(s, ';', amp)) != BFC_NPOS)) {
		bfc_string_shared_substr(s, amp, sem-amp, &sub, sizeof(sub));
		bfc_object_tostring(&sub, buf, sizeof(buf));
		pos = sem+1;
		unicode = 0;
		if ((c1 = buf[1]) == '#') {
			if (((c2 = buf[2]) == 'x') || (c2 == 'X')) {
				unicode = strtoul(buf+3, NULL, 16);
			} else if ((c2 >= '0') && (c2 <= '9')) {
				unicode = strtoul(buf+2, NULL, 10);
			}
		} else if ((n = sem - amp - 1) > 0) {
			for (tp = html_entities; tp->ent; tp++) {
				if ((tp->ent[0] == c1)
				 && (strlen(tp->ent) == n)
				 && (memcmp(buf+1, tp->ent, n) == 0)) {
					unicode = tp->unicode;
					break;
				}
			}
		}
		if (unicode) {
			bfc_string_begin_iterator(s, &i1, sizeof(i1));
			bfc_string_begin_iterator(s, &i2, sizeof(i2));
			bfc_iterator_set_position(&i1, amp);
			bfc_iterator_set_position(&i2, sem+1);
			bfc_string_replace_range_fill(s, &i1, &i2, 1, unicode);
			pos += bfc_strlen(s) - len;
		}
	}
	return (len);
}

