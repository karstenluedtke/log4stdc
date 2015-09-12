
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

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

#define CRLF "\r\n"
#define ouml "\303\266"

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);
	pool = bfc_new_sorting_mempool(pool, __FILE__, __LINE__, __FUNCTION__);

	do {
		bfc_node_t root;
		static const bfc_string_t input = BFCSTR(
			"<transaction>" CRLF
			"  <method>setDeviceAudio</method>" CRLF
			"  <parameters>" CRLF
			"    <inputAudioDevice>null</inputAudioDevice>" CRLF
			"    <outputAudioDevice>null</outputAudioDevice>" CRLF
			"    <inputAudioECTail>0</inputAudioECTail>" CRLF
			"    <outputAudioMeter>false</outputAudioMeter>" CRLF
			"    <stack>VOIP</stack>" CRLF
			"  </parameters>" CRLF
			"</transaction>" CRLF);
		char buf[4000];
		int rc;
		
		bfc_init_treenode(&root, sizeof(root), pool);
		rc = bfc_node_decode_xml(&root, &input, 0);
		bfc_object_tostring(&root, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc >= 0);
		assert(strcmp(buf, bfc_string_data(&input)) == 0);
	} while (0 /*just once*/);

	do {
		bfc_node_t root;
		static const bfc_string_t input = BFCSTR(
			"<transaction transid=\"8000000010\" userid=\"2\">" CRLF
			"  <method>setDeviceAudio</method>" CRLF
			"  <parameters>" CRLF
			"    <inputAudioDevice>B&amp;O</inputAudioDevice>" CRLF
			"    <outputAudioDevice>Kopfh" ouml "rer</outputAudioDevice>" CRLF
			"    <inputAudioECTail>0</inputAudioECTail>" CRLF
			"    <outputAudioMeter>false</outputAudioMeter>" CRLF
			"    <stack>VOIP</stack>" CRLF
			"  </parameters>" CRLF
			"</transaction>" CRLF);
		char buf[4000];
		int rc;
		
		bfc_init_treenode(&root, sizeof(root), pool);
		rc = bfc_node_decode_xml(&root, &input, 0);
		bfc_object_tostring(&root, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc >= 0);
		assert(strcmp(buf, bfc_string_data(&input)) == 0);
	} while (0 /*just once*/);

	do {
		bfc_node_t root;
		static const bfc_string_t input = BFCSTR(
			"<transaction>" CRLF
			"  <method>setDeviceAudio</method>" CRLF
			"  <parameters>" CRLF
			"    <inputAudioDevice>B&amp;O</inputAudioDevice>" CRLF
			"    <outputAudioDevice>Kopfhörer</outputAudioDevice>" CRLF
			"    <inputAudioECTail>250</inputAudioECTail>" CRLF
			"    <outputAudioMeter>false</outputAudioMeter>" CRLF
			"    <stack>VOIP</stack>" CRLF
			"  </parameters>" CRLF
			"</transaction>" CRLF);
		char buf[4000];
		int rc;
		
		bfc_init_treenode(&root, sizeof(root), pool);
		rc = bfc_node_decode_xml(&root, &input, 0);
		bfc_object_tostring(&root, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc >= 0);
		assert(strcmp(buf, bfc_string_data(&input)) == 0);
	} while (0 /*just once*/);

	do {
		bfc_node_t root;
		static const bfc_string_t input = BFCSTR(
			"<p>A text with an <img src=\"image.png\">image</img>"
			" and a <a href=\"file.html\">link</a>" CRLF
			"</p>" CRLF);
		char buf[4000];
		int rc;
		
		bfc_init_treenode(&root, sizeof(root), pool);
		rc = bfc_node_decode_xml(&root, &input, 0);
		bfc_object_tostring(&root, buf, sizeof(buf), "xml");
		L4SC_DEBUG(logger, "%s", buf);
		assert(rc >= 0);
		assert(strcmp(buf, bfc_string_data(&input)) == 0);
	} while (0 /*just once*/);

	return (0);
}
