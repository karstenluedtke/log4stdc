
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/xmltag.h"
#include "barefootc/iterator.h"

static l4sc_logger_ptr_t logger;

struct tag_expectations {
	unsigned level;
	const char *name;
	int nattrs;
	const char *attrs[10];
};

static int
on_start_tag(struct bfc_tag_parse_state *st, bfc_ctagptr_t tag,
	     bfc_cstrptr_t name, int nattrs, bfc_string_t attrs[])
{
	int i, rc;
	struct tag_expectations *expectations = st->userdata;
	char nm[80], a[80], v[80];

	assert(expectations->name); /* otherwise too many tags found */

	rc = bfc_object_tostring(name, nm, sizeof(nm));
	assert(rc >= 0);
	assert(strcmp(nm, expectations->name) == 0);
	st->userdata = &expectations[1];

	L4SC_DEBUG(logger, "%s: <%s>", __FUNCTION__, nm);

	assert(tag->level == expectations->level);
	assert(nattrs == expectations->nattrs);

	for (i=0; i < nattrs; i++) {
		rc = bfc_object_tostring(&attrs[2*i], a, sizeof(a));
		assert(rc >= 0);
		rc = bfc_object_tostring(&attrs[2*i+1], v, sizeof(v));
		assert(rc >= 0);
		L4SC_DEBUG(logger, "%s: %s=\"%s\"", __FUNCTION__, a, v);
		assert(strcmp(a, expectations->attrs[2*i]) == 0);
		assert(strcmp(v, expectations->attrs[2*i+1]) == 0);
	}

	return (BFC_SUCCESS);
}

static int
on_end_tag(struct bfc_tag_parse_state *st, bfc_ctagptr_t starttag,
				bfc_ctagptr_t endtag, bfc_cstrptr_t name)
{
	struct tag_expectations *expectations = st->userdata;
	char nm[80];

	assert(expectations->name); /* otherwise too many tags found */

	bfc_object_tostring(name, nm, sizeof(nm));
	assert(strcmp(nm, expectations->name) == 0);
	st->userdata = &expectations[1];

	L4SC_DEBUG(logger, "%s: <%s>", __FUNCTION__, nm);

	assert(endtag->level == expectations->level);
	assert(starttag->level == expectations->level);

	return (BFC_SUCCESS);
}

static int
test(const char *s1, struct tag_expectations expectations[])
{
	int rc;
	bfc_string_t doc;
	struct bfc_tag_parse_state parser_state;

	L4SC_TRACE(logger, "%s(\"%s\")", __FUNCTION__, s1);

	rc = bfc_init_shared_string_c_str(&doc, sizeof(doc), s1);
	assert(rc >= 0);

	memset(&parser_state, 0, sizeof(parser_state));

	parser_state.on_start_tag = on_start_tag;
	parser_state.on_end_tag = on_end_tag;
	parser_state.userdata = expectations;

	rc = bfc_parse_xmltags(&doc, &parser_state);
	assert(rc >= 0);

	bfc_destroy(&doc);

	L4SC_TRACE(logger, "%s passed", __FUNCTION__);

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_STRING_LOGGER);
	
#define S1(x) x
#define CRLF "\r\n"
	do {
	struct tag_expectations expectations[] = {
		{ 0, NULL, 0, { NULL, }, },
	};
	test(S1(""), expectations);
	} while (0);

	do {
	struct tag_expectations expectations[] = {
		{ 0, NULL, 0, { NULL, }, },
	};
	test(S1(
	"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
			), expectations);
	} while (0);

	do {
	struct tag_expectations expectations[] = {
		{ 0, "configuration",
		  2, { "xmlns:log4j", "http://jakarta.apache.org/log4j/",
		       "debug", "1", } },
		{ 1, "appender",
		  2, { "class", "org.apache.log4j.ConsoleAppender",
		       "name",  "Console", } },
		{ 2, "layout",
		  1, { "class", "org.apache.log4j.PatternLayout", } },
		{ 3, "param",
		  2, { "name", "ConversionPattern",
		       "value", "%d [%t] %-5p %c - %m%n", } },
		{ 3, "param", 0, { NULL } },
		{ 2, "layout", 0, { NULL } },
		{ 1, "appender", 0, { NULL } },
		{ 1, "logger",
		  2, { "additivity", "false", "name", "barefootc" } },
		{ 2, "level",
		  1, { "value", "INFO", } },
		{ 2, "level", 0, { NULL } },
		{ 2, "appender-ref",
		  1, { "ref", "Console", } },
		{ 2, "appender-ref", 0, { NULL } },
		{ 1, "logger", 0, { NULL } },
		{ 0, "configuration", 0, { NULL }, },
		{ 0, NULL, 0, { NULL, }, },
	};
	test(S1(
	"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
	"<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\" debug=\"1\">" CRLF
	"  <appender class=\"org.apache.log4j.ConsoleAppender\" name=\"Console\">" CRLF
	"    <layout class=\"org.apache.log4j.PatternLayout\">" CRLF
	"      <param name=\"ConversionPattern\" value=\"%d [%t] %-5p %c - %m%n\"/>" CRLF
	"    </layout>" CRLF
	"  </appender>" CRLF
	"  <logger additivity=\"false\" name=\"barefootc\">" CRLF
	"    <level value=\"INFO\"/>" CRLF
	"    <appender-ref ref=\"Console\"/>" CRLF
	"  </logger>" CRLF
	"</log4j:configuration>" CRLF), expectations);
	} while (0);

	do {
	struct tag_expectations expectations[] = {
		{ 0, "configuration",
		  2, { "xmlns:log4j", "http://jakarta.apache.org/log4j/",
		       "debug", "1", } },
		{ 1, "appender",
		  2, { "class", "org.apache.log4j.RollingFileAppender",
		       "name", "bfclog", } },
		{ 2, "param",
		  2, { "name", "File", "value", "bfc.log", } },
		{ 2, "param", 0, { NULL } },
		{ 2, "param",
		  2, { "name", "MaxBackupIndex", "value", "9", } },
		{ 2, "param", 0, { NULL } },
		{ 2, "param",
		  2, { "name", "MaxFileSize", "value", "10MB", } },
		{ 2, "param", 0, { NULL } },
		{ 2, "layout",
		  1, { "class", "org.apache.log4j.PatternLayout", } },
		{ 3, "param",
		  2, { "name", "ConversionPattern",
		       "value", "%d{ABSOLUTE} %-5p> %m%n", } },
		{ 3, "param", 0, { NULL } },
		{ 2, "layout", 0, { NULL } },
		{ 1, "appender", 0, { NULL } },
		{ 1, "logger",
		  2, { "additivity", "false", "name", "barefootc" } },
		{ 2, "level",
		  1, { "value", "WARN", } },
		{ 2, "level", 0, { NULL } },
		{ 2, "appender-ref",
		  1, { "ref", "bfclog", } },
		{ 2, "appender-ref", 0, { NULL } },
		{ 1, "logger", 0, { NULL } },
		{ 0, "configuration", 0, { NULL }, },
		{ 0, NULL, 0, { NULL, }, },
	};
	test(S1(
	"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
	"<log4j:configuration xmlns:log4j=\"http://jakarta.apache.org/log4j/\" debug=\"1\">" CRLF
	"  <appender class=\"org.apache.log4j.RollingFileAppender\" name=\"bfclog\">" CRLF
	"    <param name=\"File\" value=\"bfc.log\"/>" CRLF
	"    <param name=\"MaxBackupIndex\" value=\"9\"/>" CRLF
	"    <param name=\"MaxFileSize\" value=\"10MB\"/>" CRLF
	"    <layout class=\"org.apache.log4j.PatternLayout\">" CRLF
	"      <param name=\"ConversionPattern\" value=\"%d{ABSOLUTE} %-5p&gt; %m%n\"/>" CRLF
	"    </layout>" CRLF
	"  </appender>" CRLF
	"  <logger additivity=\"false\" name=\"barefootc\">" CRLF
	"    <level value=\"WARN\"/>" CRLF
	"    <appender-ref ref=\"bfclog\"/>" CRLF
	"  </logger>" CRLF
	"</log4j:configuration>" CRLF), expectations);
	} while (0);

	return (0);
}

