
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/treenode.h"
#include "barefootc/container.h"
#include "barefootc/mempool.h"
#include "barefootc/utf8.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

static bfc_mempool_t pool;
static l4sc_logger_ptr_t logger;

#define CRLF "\r\n"
#define ouml "\303\266"

int
main(int argc, char *argv[])
{
	bfc_node_t tmpl;

	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);
	pool = bfc_new_sorting_mempool(pool, __FILE__, __LINE__, __FUNCTION__);

	bfc_init_treenode(&tmpl, sizeof(tmpl), pool);

	do {
		bfc_cobjptr_t tp = (bfc_cobjptr_t) &tmpl;
		bfc_cobjptr_t xml =
		  bfc_node_new_element(tp, "nEE", "transaction",
		    bfc_node_new_element(tp,"ns", "method", "setDeviceAudio"),
		    bfc_node_new_element(tp,"nEEEEE", "parameters",
		      bfc_node_new_element(tp,"ns","inputAudioDevice", "null"),
		      bfc_node_new_element(tp,"ns","outputAudioDevice","null"),
		      bfc_node_new_element(tp,"ns","inputAudioECTail", "0"),
		      bfc_node_new_element(tp,"ns","outputAudioMeter", "false"),
		      bfc_node_new_element(tp,"ns","stack", "VOIP")));
		static const char expectation[] =
			"<transaction>" CRLF
			"  <method>setDeviceAudio</method>" CRLF
			"  <parameters>" CRLF
			"    <inputAudioDevice>null</inputAudioDevice>" CRLF
			"    <outputAudioDevice>null</outputAudioDevice>" CRLF
			"    <inputAudioECTail>0</inputAudioECTail>" CRLF
			"    <outputAudioMeter>false</outputAudioMeter>" CRLF
			"    <stack>VOIP</stack>" CRLF
			"  </parameters>" CRLF
			"</transaction>" CRLF;
		char buf[4000];
		int len, rc;
		len= bfc_object_tostring(xml, NULL, 0, "xml");
		rc = bfc_object_tostring(xml, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc > 0);
		assert(rc < sizeof(buf));
		assert(rc == len);
		assert(strcmp(buf, expectation) == 0);
	} while (0 /*just once*/);

	do {
		bfc_cobjptr_t tp = (bfc_cobjptr_t) &tmpl;
		static bfc_string_t mettag = BFCWSTR(L"method");
		static bfc_string_t method = BFCSTR("setDeviceAudio");
		bfc_objptr_t xml =
		  bfc_node_new_element(tp, "nEE", "transaction",
		    bfc_node_new_element(tp,"NS", &mettag, &method),
		    bfc_node_new_element(tp,"nEEEEE", "parameters",
		      bfc_node_new_element(tp,"ns","inputAudioDevice", "B&O"),
		      bfc_node_new_element(tp,"ns","outputAudioDevice",
							"Kopfh" ouml "rer"),
		      bfc_node_new_element(tp,"ns","inputAudioECTail", "0"),
		      bfc_node_new_element(tp,"ns","outputAudioMeter", "false"),
		      bfc_node_new_element(tp,"ns","stack", "VOIP")));
		static const char expectation[] =
			"<transaction transid=\"8000000010\" userid=\"2\">" CRLF
			"  <method>setDeviceAudio</method>" CRLF
			"  <parameters>" CRLF
			"    <inputAudioDevice>B&amp;O</inputAudioDevice>" CRLF
			"    <outputAudioDevice>Kopfh" ouml "rer</outputAudioDevice>" CRLF
			"    <inputAudioECTail>0</inputAudioECTail>" CRLF
			"    <outputAudioMeter>false</outputAudioMeter>" CRLF
			"    <stack>VOIP</stack>" CRLF
			"  </parameters>" CRLF
			"</transaction>" CRLF;
		static const bfc_string_t tidname = BFCSTR("transid");
		static const bfc_string_t tidvalue= BFCSTR("8000000010");
		static const bfc_string_t uidname = BFCSTR("userid");
		static const bfc_string_t uidvalue= BFCSTR("2");
		char buf[4000];
		int len, rc;

		rc = bfc_node_set_xml_attribute(xml, &tidname, &tidvalue);
		assert (rc >= 0);
		rc = bfc_node_set_xml_attribute(xml, &uidname, &uidvalue);
		assert (rc >= 0);

		len= bfc_object_tostring(xml, NULL, 0, "xml");
		rc = bfc_object_tostring(xml, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc > 0);
		assert(rc < sizeof(buf));
		assert(rc == len);
		assert(strcmp(buf, expectation) == 0);
	} while (0 /*just once*/);

	do {
		bfc_cobjptr_t tp = (bfc_cobjptr_t) &tmpl;
		bfc_cobjptr_t xml =
		  bfc_node_new_element(tp, "nEE", "transaction",
		    bfc_node_new_element(tp,"ns", "method", "setDeviceAudio"),
		    bfc_node_new_element(tp,"nEEEEE", "parameters",
		      bfc_node_new_element(tp,"ns","inputAudioDevice", "B&O"),
		      bfc_node_new_element(tp,"ns","outputAudioDevice","Kopfhörer"),
		      bfc_node_new_integer_element(tp, "inputAudioECTail", 250),
		      bfc_node_new_boolean_element(tp, "outputAudioMeter", 0),
		      bfc_node_new_element(tp,"ns","stack", "VOIP")));
		static const char expectation[] =
			"<transaction>" CRLF
			"  <method>setDeviceAudio</method>" CRLF
			"  <parameters>" CRLF
			"    <inputAudioDevice>B&amp;O</inputAudioDevice>" CRLF
			"    <outputAudioDevice>Kopfhörer</outputAudioDevice>" CRLF
			"    <inputAudioECTail>250</inputAudioECTail>" CRLF
			"    <outputAudioMeter>false</outputAudioMeter>" CRLF
			"    <stack>VOIP</stack>" CRLF
			"  </parameters>" CRLF
			"</transaction>" CRLF;
		char buf[4000];
		int len= bfc_object_tostring(xml, NULL, 0, "xml");
		int rc = bfc_object_tostring(xml, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc > 0);
		assert(rc < sizeof(buf));
		assert(rc == len);
		assert(strcmp(buf, expectation) == 0);
	} while (0 /*just once*/);

	do {
		bfc_cobjptr_t tp = (bfc_cobjptr_t) &tmpl;
		bfc_objptr_t xml =
		  bfc_node_new_element(tp, "nsEsE", "p",
		    "A text with an ",
		    bfc_node_new_element(tp,"ns", "img", "image"),
		    " and a ",
		    bfc_node_new_element(tp,"ns", "a", "link"));
		bfc_objptr_t child;
		static bfc_string_t img_attr  = BFCSTR("src");
		static bfc_string_t img_value = BFCSTR("image.png");
		static bfc_string_t href_attr = BFCSTR("href");
		static bfc_string_t href_value= BFCSTR("file.html");
		static const char expectation[] =
			"<p>A text with an <img src=\"image.png\">image</img>"
			" and a <a href=\"file.html\">link</a>" CRLF
			"</p>" CRLF;
		char buf[4000];

		child = (bfc_objptr_t) bfc_container_index(xml, 1);
		bfc_node_set_xml_attribute(child, &img_attr, &img_value);

		child = (bfc_objptr_t) bfc_container_index(xml, 3);
		bfc_node_set_xml_attribute(child, &href_attr, &href_value);

		int len= bfc_object_tostring(xml, NULL, 0, "xml");
		int rc = bfc_object_tostring(xml, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc > 0);
		assert(rc < sizeof(buf));
		assert(rc <= len);
		assert(strcmp(buf, expectation) == 0);
	} while (0 /*just once*/);

	return (0);
}
