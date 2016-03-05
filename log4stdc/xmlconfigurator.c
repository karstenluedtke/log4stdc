
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#elif defined(HAVE_ALLOCA_H)
#include <alloca.h>
#endif

#if defined(_MSC_VER)
#define strncasecmp strnicmp
#endif

#include "barefootc/object.h"
#include "barefootc/mempool.h"
#include "barefootc/utf8.h"

#include "log4stdc/logobjects.h"

struct xml_tag {
	unsigned short	tagtype;
	unsigned short	level;
	unsigned short	nameoffs;
	unsigned short	namelen;
	const char *	rawtag;
	unsigned	taglen;
};

/* tag type */
#define	XML_HEADER_TAG	0
#define	XML_START_TAG	1
#define	XML_END_TAG  	2
#define	XML_EMPTY_TAG	3
#define	XML_COMMENT_TAG	4

#define MAX_ATTRS	10
#define MAX_ATTR_LEN	80

struct xml_attr {
	int namelen;
	int vallen;
	char name[MAX_ATTR_LEN];
	char value[MAX_ATTR_LEN];
};

static int init_xml_configurator(void *, size_t, bfc_mempool_t );
static size_t get_xml_configurator_size(l4sc_configurator_cptr_t obj);

static int configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path);
static int configure_from_string(l4sc_configurator_ptr_t cfgtr,
						const char *s, size_t n);

static int xml_find_next_tag (struct xml_tag *tag,
				const char *start, const char *limit);
static int xml_tag_name_tostring(const struct xml_tag *tag, char *buf,int size);
static int xml_tag_get_attrs(const struct xml_tag *tag, struct xml_attr *attrs,
				int maxattrs, int maxattrlen);
static int decode_html_entities(char *dest, const char *src, int len);

const struct l4sc_configurator_class l4sc_xml_configurator_class = {
	/* .super 	*/ (l4sc_configurator_class_ptr_t) &l4sc_object_class,
	/* .name 	*/ "xml configurator",
	/* .spare2 	*/ NULL,
	/* .spare3 	*/ NULL,
	/* .init 	*/ init_xml_configurator,
	/* .initrefc 	*/ NULL, /* inherit */
	/* .incrrefc 	*/ NULL, /* inherit */
	/* .decrrefc 	*/ NULL, /* inherit */
	/* .destroy 	*/ NULL, /* inherit */
	/* .clone 	*/ NULL, /* inherit */
	/* .clonesize 	*/ get_xml_configurator_size,
	/* .hashcode 	*/ NULL, /* inherit */
	/* .equals 	*/ NULL, /* inherit */
	/* .length 	*/ NULL, /* inherit */
	/* .tostring 	*/ NULL, /* inherit */
	/* .dump 	*/ NULL, /* inherit */
	/* .set_name	*/ NULL,
	/* .set_opt	*/ NULL,
	/* .get_opt	*/ NULL,
	/* .apply	*/ NULL,
	/* .close	*/ NULL,
	/* .configure	*/ configure_from_string,
	/* .configure_from_file */ configure_from_file
};

static int
init_xml_configurator(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(l4sc_configurator_class_ptr_t,
			  l4sc_configurator_ptr_t,configurator,buf,bufsize,pool,
			  &l4sc_xml_configurator_class);
	configurator->name = "XML configurator";
	return (BFC_SUCCESS);
}

static size_t
get_xml_configurator_size(l4sc_configurator_cptr_t obj)
{
	return (sizeof(struct l4sc_configurator));
}

#define NS_DELIMITER	0x1d	/* Ascii GS: Group Separator */

struct parsing_state {
	l4sc_configurator_ptr_t	configurator;
	int depth;
	l4sc_logger_ptr_t	current_logger;
	l4sc_appender_ptr_t	current_appender;
	l4sc_layout_ptr_t	current_layout;
};

struct element_values {
	const char *name;
	int namelen;
	const char *value;
	int vallen;
	const char *ref;
	int reflen;
	const char *class;
	int classlen;
};

static int
on_start_tag(struct parsing_state *ps, const struct xml_tag *tag)
{
	int i, nattrs;
	struct element_values values;
	char name[MAX_ATTR_LEN];
	struct xml_attr attrs[MAX_ATTRS];

	xml_tag_name_tostring(tag, name, sizeof(name));
	nattrs = xml_tag_get_attrs(tag, attrs, MAX_ATTRS, MAX_ATTR_LEN);

	if (ps->depth == 0) {
		for (i=0; i < nattrs; i++) {
			if ((strncasecmp(attrs[i].name,"debug", 5) == 0)
			 || (strncasecmp(attrs[i].name,"internalDebug",13)==0)
			 || (strncasecmp(attrs[i].name,"configDebug",11)==0)) {
				l4sc_set_internal_logging(
					attrs[i].value, attrs[i].vallen);
				break;
			}
		}
	}

	LOGDEBUG(("%s: <%s>", __FUNCTION__, name));

	memset(&values, 0, sizeof(values));
	for (i=0; i < nattrs; i++) {
		LOGDEBUG(("%s: attr #%d \"%s\" = \"%s\"",
			__FUNCTION__, i, attrs[i].name, attrs[i].value));
		if (strncasecmp(attrs[i].name, "name", 4) == 0) {
			values.name = attrs[i].value;
			values.namelen = attrs[i].vallen;
		} else if (strncasecmp(attrs[i].name, "value", 5) == 0) {
			values.value = attrs[i].value;
			values.vallen = attrs[i].vallen;
		} else if (strncasecmp(attrs[i].name, "class", 5) == 0) {
			values.class = attrs[i].value;
			values.classlen = attrs[i].vallen;
		} else if (strncasecmp(attrs[i].name, "ref", 3) == 0) {
			values.ref = attrs[i].value;
			values.reflen = attrs[i].vallen;
		}
	}
	ps->depth = tag->level + (tag->tagtype == XML_START_TAG)? 1: 0;
	if (strncasecmp(name, "logger", 6) == 0) {
		ps->current_logger = l4sc_get_logger(
					values.name, values.namelen);
		for (i=0; i < nattrs; i++) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				attrs[i].name, attrs[i].namelen,
				attrs[i].value, attrs[i].vallen);
		}
	} else if (strncasecmp(name, "appender-ref", 12) == 0) {
		l4sc_appender_ptr_t appender;
		appender = l4sc_get_appender(values.ref, values.reflen,
						NULL, 0);
		if (appender && ps->current_logger) {
			l4sc_set_logger_appender(ps->current_logger, appender);
		}
	} else if (strncasecmp(name, "appender", 8) == 0) {
		ps->current_appender = l4sc_get_appender(
					values.name, values.namelen,
					values.class, values.classlen);
		for (i=0; i < nattrs; i++) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				attrs[i].name, attrs[i].namelen,
				attrs[i].value, attrs[i].vallen);
		}
	} else if (strncasecmp(name, "layout", 6) == 0) {
		if (ps->current_appender) {
			ps->current_layout = l4sc_get_appender_layout(
						ps->current_appender);
			for (i=0; i < nattrs; i++) {
				l4sc_set_object_option(
					(l4sc_objptr_t) ps->current_layout,
					attrs[i].name, attrs[i].namelen,
					attrs[i].value, attrs[i].vallen);
			}
		}
	} else if (strncasecmp(name, "param", 5) == 0) {
		if (ps->current_layout) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_layout,
				values.name, values.namelen,
				values.value, values.vallen);
		} else if (ps->current_appender) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				values.name, values.namelen,
				values.value, values.vallen);
		} else if (ps->current_logger) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				values.name, values.namelen,
				values.value, values.vallen);
		}
	} else if (values.value) {
		int nmlen = strlen(name);
		if (ps->current_layout) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_layout,
				name, nmlen, values.value, values.vallen);
		} else if (ps->current_appender) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				name, nmlen, values.value, values.vallen);
		} else if (ps->current_logger) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				name, nmlen, values.value, values.vallen);
		}
	}
	return (BFC_SUCCESS);
}

static int
on_end_tag(struct parsing_state *ps, const struct xml_tag *endtag)
{
	char name[MAX_ATTR_LEN];

	xml_tag_name_tostring(endtag, name, sizeof(name));
	LOGDEBUG(("%s: </%s>", __FUNCTION__, name));

	ps->depth = endtag->level;
	if (strncasecmp(name, "logger", 6) == 0) {
		ps->current_logger = NULL;
	} else if (strncasecmp(name, "appender", 8) == 0) {
		ps->current_appender = NULL;
	} else if (strncasecmp(name, "layout", 6) == 0) {
		ps->current_layout = NULL;
	}
	return (BFC_SUCCESS);
}

static int
configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path)
{
	FILE *fp;
	char *buff;
	int rc = 0;
	size_t bufsize = 8000, length = 0;
	struct mempool *pool = get_default_mempool();

	LOGINFO(("%s: \"%s\"", __FUNCTION__, path));

	fp = fopen(path, "r");
	if (fp == NULL) {
		rc = -errno;
		LOGERROR(("%s: opening file \"%s\" failed, error %d: %s",
			__FUNCTION__, path, rc, strerror(-rc)));
		return (rc);
	}

	buff = mempool_alloc(pool, bufsize);
	while ((rc = fread(buff+length, 1, bufsize-length, fp)) > 0) {
		length += rc;
		if (length >= bufsize) {
			bufsize = (3 * bufsize / 2) + 4000;
			LOGDEBUG(("%s: have %ld, realloc(%ld)...",
				__FUNCTION__, (long)length, (long)bufsize));
			buff = mempool_realloc(pool, buff, bufsize);
			if (buff == NULL) {
				rc = -ENOMEM;
				LOGERROR(("%s: realloc(%ld) failed.",
					__FUNCTION__, (long)bufsize));
				fclose(fp);
				return (rc);
			}
		}
	}
	fclose(fp);

	rc = configure_from_string(cfgtr, buff, length);

	mempool_free(pool, buff);

	LOGINFO(("%s: \"%s\" done, error %d", __FUNCTION__, path, rc));

	return ((rc == 0)? 0: (rc > 0)? -rc: rc);
}

static int
configure_from_string(l4sc_configurator_ptr_t cfgtr, const char *s, size_t n)
{
	int rc, iterations = 0, err = 0;
	size_t len = (n > 0)? n: strlen(s);
	const char *cp = s;
	const char *limit = s + len;
	struct parsing_state state;
	struct xml_tag tag;

	LOGINFO(("%s: %ld bytes", __FUNCTION__, (long) len));

	memset(&tag, 0, sizeof(tag));
	memset(&state, 0, sizeof(state));
	state.configurator = cfgtr;

	while ((rc = xml_find_next_tag(&tag, cp, limit)) >= 0) {
		iterations++;
		LOGDEBUG(("%s: iteration #%d, found %d",
				__FUNCTION__, iterations, rc));
		if ((rc == XML_START_TAG) || (rc == XML_EMPTY_TAG)) {
			on_start_tag(&state, &tag);
		}
		if ((rc == XML_END_TAG) || (rc == XML_EMPTY_TAG)) {
			on_end_tag(&state, &tag);
		}
		cp = tag.rawtag + tag.taglen;
	}

	LOGINFO(("%s: done, error %d", __FUNCTION__, err));

	return ((err == 0)? 0: (err > 0)? -err: err);
}

static int
xml_find_next_tag (struct xml_tag *tag, const char *start, const char *limit)
{
	const char *cp, *tp, *np, *ne;
	int tt;

	if ((tp = memchr (start, '<', limit - start)) && (tp < limit)) {
		if (tp[1] == '/') {
			tt = XML_END_TAG;
			np = tp+2;
		} else if (tp[1] == '?') {
			tt = XML_HEADER_TAG;
			np = tp+2;
		} else {
			tt = XML_START_TAG; /* or XML_EMPTY_TAG */
			np = tp+1;
		}

		ne = np;
		while ((ne < limit)
		    && (*ne > ' ') && (*ne != '>') && (*ne != '/')) ne++;

		if ((cp = memchr (ne, '>', limit - ne)) && (cp < limit)) {
			if (cp[-1] == '/') {
				tt = XML_EMPTY_TAG;
			}
			tag->rawtag = tp;
			tag->taglen = cp+1 - tp;
			tag->nameoffs = np - tp;
			tag->namelen  = ne - np;
			tag->tagtype = tt;
			return (tt);
		}
	}

	return (-ENOENT);
}

static int
xml_tag_name_tostring(const struct xml_tag *tag, char *buf, int size)
{
	int len = (tag->namelen < size)? tag->namelen: size-1;

	memcpy(buf, tag->rawtag + tag->nameoffs, len);
	buf[len] = '\0';

	return (len);
}

static int
xml_tag_get_attrs(const struct xml_tag *tag, struct xml_attr *attrs,
		  int maxattrs, int maxattrlen)
{
	const unsigned char *s = (const unsigned char *)tag->rawtag;
	unsigned pos = tag->nameoffs + tag->namelen;
	unsigned limit = tag->taglen;
	unsigned start, assign, quote, endquote;
	unsigned n=0;
	long c;

	for (; (pos < limit) && (n < maxattrs); pos++) {
		LOGDEBUG(("%s: pos %u/%u, have %u/%u",
			__FUNCTION__, pos, limit, n, maxattrs));
		while ((pos < limit) && (s[pos] > ' ')) {
			pos++;
		}
		while ((pos < limit) && (s[pos] <= ' ')) {
			pos++;
		}
		if (pos >= limit) {
			break;
		}
		start = pos;
		assign = quote = endquote = 0;
		c = s[pos];
		LOGDEBUG(("%s: pos %ld, char %c",
				__FUNCTION__, (long) pos, (char)c));
		if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
			while (!endquote && (++pos < limit)) {
				c = s[pos];
				if (assign != 0) {
					if (c == '"') {
						if (quote == 0) {
							quote = pos;
						} else {
							endquote = pos;
							break;
						}
					}
				} else if (c == '=') {
					assign = pos;
				}
				LOGDEBUG(("%s: pos %u, char %c, "
					"assign %u, quote %u, endquote %u",
					__FUNCTION__, pos, (char)c,
					assign, quote, endquote));
			}
		}
		if ((assign > start) && (quote > assign) && (endquote > quote)){
			int len = assign - start;
			while ((len > 0) && (s[start+len-1] <= ' ')) {
				len--;
			}
			if (len >= maxattrlen) {
				len = maxattrlen-1;
			}
			memcpy(attrs[n].name, s+start, len);
			attrs[n].name[len] = '\0';
			attrs[n].namelen = len;

			len = endquote-quote-1;
			if (len >= maxattrlen) {
				len = maxattrlen-1;
			}
			if (memchr(s+quote+1, '&', len) == NULL) {
				memcpy(attrs[n].value, s+quote+1, len);
			} else {
				len = decode_html_entities(attrs[n].value,
						(const char *)s+quote+1, len);
			}
			attrs[n].value[len] = '\0';
			attrs[n].vallen = len;
			LOGDEBUG(("%s(xmltag) #%d: %s=\"%s\"", __FUNCTION__,
					n, attrs[n].name, attrs[n].value));
			n++;
		}
	}
	return ((int) n);
}

int
l4sc_configure_from_xml_file(const char *path)
{
	int rc;
	struct l4sc_configurator obj;
	const size_t pathlen = strlen(path);
	const size_t pathbufsize = 256 + pathlen;
	char *pathbuf;

	l4sc_close_appenders();

	pathbuf = alloca(pathbufsize);
	init_xml_configurator(&obj, sizeof(obj), NULL);
	l4sc_merge_base_directory_path(pathbuf, pathbufsize, path, pathlen);
	rc = VMETHCALL(&obj, configure_from_file, (&obj, pathbuf), -ENOSYS);
	VMETHCALL(&obj, destroy, (&obj), (void) 0);

	l4sc_set_configured(1);

	return (rc);
}

int
l4sc_configure_from_xml_string(const char *buf, size_t len)
{
	int rc;
	struct l4sc_configurator obj;

	l4sc_close_appenders();

	init_xml_configurator(&obj, sizeof(obj), NULL);
	rc = VMETHCALL(&obj, configure, (&obj, buf, len), -ENOSYS);
	VMETHCALL(&obj, destroy, (&obj), (void) 0);

	l4sc_set_configured(1);

	return (rc);
}

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

static int
decode_html_entities(char *dest, const char *src, int len)
{
	const char *cp = src;
	const char *ep = src + len;
	const char *amp, *sem;
	char *dp = dest;
	const struct entity_table *tp;
	unsigned long unicode = 0;
	int n;

	while (((amp = memchr(cp, '&', ep-cp))  != NULL)
	    && ((sem = memchr(amp,';', ep-amp)) != NULL)) {
		if ((n = amp - cp) > 0) {
			memcpy (dp, cp, n);
			dp += n;
			cp = amp;
		}
		if (amp[1] == '#') {
			if ((amp[2] == 'x') || (amp[2] == 'X')) {
				unicode = strtoul(amp+3, NULL, 16);
			} else if ((amp[2] >= '0') && (amp[2] <= '9')) {
				unicode = strtoul(amp+2, NULL, 10);
			}
		} else if ((n = sem - amp - 1) > 0) {
			for (tp = html_entities; tp->ent; tp++) {
				if ((amp[1] == tp->ent[0])
				 && (strlen(tp->ent) == n)
				 && (memcmp(amp+1,tp->ent,n) == 0)) {
					unicode = tp->unicode;
					break;
				}
			}
		}
		if (unicode) {
			BFC_PUT_UTF8(dp, dest+len, unicode);

		} else if ((n = sem+1 - cp) > 0) {
			memcpy (dp, cp, n);
			dp += n;
		}
		cp = sem + 1;
	}
	if ((n = ep - cp) > 0) {
		memcpy (dp, cp, n);
		dp += n;
		cp = ep;
	}
	if (dp < dest+len) {
		dp[0] = '\0';
	}
	return (dp - dest);
}

