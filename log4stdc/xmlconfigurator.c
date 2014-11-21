
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/xmltag.h"
#include "barefootc/mempool.h"
#include "logobjects.h"

static int init_xml_configurator(void *, size_t, struct mempool *);
static size_t get_xml_configurator_size(l4sc_configurator_cptr_t obj);

static int configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path);
static int configure_from_string(l4sc_configurator_ptr_t cfgtr,
						const char *s, size_t n);

const struct l4sc_configurator_class l4sc_xml_configurator_class = {
	.super = (l4sc_configurator_class_ptr_t) &l4sc_object_class,
	.name = "xml configurator",
	.init = init_xml_configurator,
	.clonesize = get_xml_configurator_size,

	.configure = configure_from_string,
	.configure_from_file = configure_from_file,
};

static int
init_xml_configurator(void *buf, size_t bufsize, struct mempool *pool)
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
	bfc_cstrptr_t name;
	bfc_cstrptr_t value;
	bfc_cstrptr_t ref;
	bfc_cstrptr_t class;
};

static int
on_start_tag(struct parsing_state *ps, bfc_ctagptr_t tag)
{
	int i, nattrs;
	struct element_values values;
	bfc_string_t namestr, tmp;
	bfc_string_t attrs[10];
	char name[80], a[80], v[80];

	bfc_tag_get_name(tag, &namestr, sizeof(namestr));
	nattrs = bfc_tag_get_attrs(tag, attrs, sizeof(attrs));

	if (ps->depth == 0) {
		for (i=0; i < nattrs; i++) {
			bfc_object_tostring(&attrs[2*i], a, sizeof(a));
			bfc_object_tostring(&attrs[2*i+1],v,sizeof(v));
			if ((strncasecmp(a, "debug", 5) == 0)
			 || (strncasecmp(a, "internalDebug", 13) == 0)
			 || (strncasecmp(a, "configDebug",   11) == 0)) {
				l4sc_set_internal_logging(v, strlen(v));
				break;
			}
		}
	}

	bfc_object_tostring(&namestr, name, sizeof(name));
	LOGDEBUG(("%s: <%s>", __FUNCTION__, name));

	for (i=0; i < 2*nattrs; i++) {
		if (bfc_string_find_char(&attrs[i], '&', 0) != BFC_NPOS) {
			void *buf;
			size_t bufsize = 4*bfc_strlen(&attrs[i]) + 10;
			buf = alloca(bufsize);
			bfc_string_buffered_substr(&attrs[i], 0, BFC_NPOS,
					   &tmp, sizeof(tmp), buf, bufsize);
			bfc_string_decode_html_entities(&tmp);
			bfc_string_swap(&attrs[i], &tmp);
			bfc_destroy(&tmp);
		}
	}

	memset(&values, 0, sizeof(values));
	for (i=0; i < nattrs; i++) {
		bfc_object_tostring(&attrs[2*i], a, sizeof(a));
		bfc_object_tostring(&attrs[2*i+1],v,sizeof(v));
		LOGDEBUG(("%s: attr #%d \"%s\" = \"%s\"",__FUNCTION__,i,a,v));
		if (strncasecmp(a, "name", 4) == 0) {
			values.name = &attrs[2*i+1];
		} else if (strncasecmp(a, "value", 5) == 0) {
			values.value = &attrs[2*i+1];
		} else if (strncasecmp(a, "class", 5) == 0) {
			values.class = &attrs[2*i+1];
		} else if (strncasecmp(a, "ref", 3) == 0) {
			values.ref = &attrs[2*i+1];
		}
	}
	ps->depth = tag->level + (tag->tagtype == BFC_XML_START_TAG)? 1: 0;
	if (strncasecmp(name, "logger", 6) == 0) {
		bfc_object_tostring(values.name, v, sizeof(v));
		ps->current_logger = l4sc_get_logger(v, strlen(v));
		for (i=0; i < nattrs; i++) {
			bfc_object_tostring(&attrs[2*i], a, sizeof(a));
			bfc_object_tostring(&attrs[2*i+1],v,sizeof(v));
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				a, strlen(a), v, strlen(v));
		}
	} else if (strncasecmp(name, "appender-ref", 12) == 0) {
		l4sc_appender_ptr_t appender;
		bfc_object_tostring(values.ref, v, sizeof(v));
		appender = l4sc_get_appender(v, strlen(v), NULL, 0);
		if (appender && ps->current_logger) {
			l4sc_set_logger_appender(ps->current_logger, appender);
		}
	} else if (strncasecmp(name, "appender", 8) == 0) {
		bfc_object_tostring(values.name, a, sizeof(a));
		bfc_object_tostring(values.class, v, sizeof(v));
		ps->current_appender = l4sc_get_appender(a, strlen(a),
							 v, strlen(v));
		for (i=0; i < nattrs; i++) {
			bfc_object_tostring(&attrs[2*i], a, sizeof(a));
			bfc_object_tostring(&attrs[2*i+1],v,sizeof(v));
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				a, strlen(a), v, strlen(v));
		}
	} else if (strncasecmp(name, "layout", 6) == 0) {
		if (ps->current_appender) {
			ps->current_layout = l4sc_get_appender_layout(
						ps->current_appender);
			for (i=0; i < nattrs; i++) {
				bfc_object_tostring(&attrs[2*i], a, sizeof(a));
				bfc_object_tostring(&attrs[2*i+1],v,sizeof(v));
				l4sc_set_object_option(
					(l4sc_objptr_t) ps->current_layout,
					a, strlen(a), v, strlen(v));
			}
		}
	} else if (strncasecmp(name, "param", 5) == 0) {
		bfc_object_tostring(values.name, a, sizeof(a));
		bfc_object_tostring(values.value, v, sizeof(v));
		if (ps->current_layout) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_layout,
				a, strlen(a), v, strlen(v));
		} else if (ps->current_appender) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				a, strlen(a), v, strlen(v));
		} else if (ps->current_logger) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				a, strlen(a), v, strlen(v));
		}
	} else if (values.value) {
		int nmlen = strlen(name);
		bfc_object_tostring(values.value, v, sizeof(v));
		if (ps->current_layout) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_layout,
				name, nmlen, v, strlen(v));
		} else if (ps->current_appender) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				name, nmlen, v, strlen(v));
		} else if (ps->current_logger) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				name, nmlen, v, strlen(v));
		}
	}
	return (BFC_SUCCESS);
}

static int
on_end_tag(struct parsing_state *ps, bfc_ctagptr_t endtag)
{
	bfc_string_t namestr;
	char name[80];

	bfc_tag_get_name(endtag, &namestr, sizeof(namestr));
	bfc_object_tostring(&namestr, name, sizeof(name));
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
	int rc, iterations = 0, err = 0, bytes_read = 0;
	FILE *fp;
	struct parsing_state state;
	bfc_string_t doc;
	bfc_xmltag_t curr;
	char buff[256];

	LOGINFO(("%s: \"%s\"", __FUNCTION__, path));

	fp = fopen(path, "r");
	if (fp == NULL) {
		err = errno;
		LOGERROR(("%s: opening file \"%s\" failed, error %d: %s",
			__FUNCTION__, path, err, strerror(err)));
		return (-err);
	}

	memset(&state, 0, sizeof(state));
	state.configurator = cfgtr;

	bfc_init_basic_string(&doc, sizeof(doc), get_stdc_mempool());

	while ((bytes_read = fread(buff, 1, sizeof(buff), fp)) > 0) {
		bfc_string_append_buffer(&doc, buff, bytes_read);
	}
	fclose(fp);

	bfc_init_xmltag(&curr, sizeof(curr), &doc, 0);

	while ((rc = bfc_iterator_advance((bfc_iterptr_t) &curr, 1)) >= 0) {
		iterations++;
		LOGDEBUG(("%s: iteration #%d, found %d",
				__FUNCTION__, iterations, rc));
		if ((rc == BFC_XML_START_TAG) || (rc == BFC_XML_EMPTY_TAG)) {
			on_start_tag(&state, &curr);
		}
		if ((rc == BFC_XML_END_TAG) || (rc == BFC_XML_EMPTY_TAG)) {
			on_end_tag(&state, &curr);
		}
	}
	bfc_destroy(&curr);
	bfc_destroy(&doc);

	LOGINFO(("%s: \"%s\" done, error %d", __FUNCTION__, path, err));

	return ((err == 0)? 0: (err > 0)? -err: err);
}

static int
configure_from_string(l4sc_configurator_ptr_t cfgtr, const char *s, size_t n)
{
	int rc, iterations = 0, err = 0;
	size_t len = (n > 0)? n: strlen(s);
	struct parsing_state state;
	bfc_string_t doc;
	bfc_xmltag_t curr;

	LOGINFO(("%s: %ld bytes", __FUNCTION__, (long) len));

	memset(&state, 0, sizeof(state));
	state.configurator = cfgtr;

	bfc_init_shared_string_buffer(&doc, sizeof(doc), s, len);

	bfc_init_xmltag(&curr, sizeof(curr), &doc, 0);

	while ((rc = bfc_iterator_advance((bfc_iterptr_t) &curr, 1)) >= 0) {
		iterations++;
		LOGDEBUG(("%s: iteration #%d, found %d",
				__FUNCTION__, iterations, rc));
		if ((rc == BFC_XML_START_TAG) || (rc == BFC_XML_EMPTY_TAG)) {
			on_start_tag(&state, &curr);
		}
		if ((rc == BFC_XML_END_TAG) || (rc == BFC_XML_EMPTY_TAG)) {
			on_end_tag(&state, &curr);
		}
	}
	bfc_destroy(&curr);
	bfc_destroy(&doc);

	LOGINFO(("%s: done, error %d", __FUNCTION__, err));

	return ((err == 0)? 0: (err > 0)? -err: err);
}

int
l4sc_configure_from_xml_file(const char *path)
{
	int rc;
	struct l4sc_configurator obj;
	const int pathlen = strlen(path);
	char pathbuf[256 + pathlen];

	l4sc_close_appenders();

	init_xml_configurator(&obj, sizeof(obj), NULL);
	l4sc_merge_base_directory_path(pathbuf, sizeof(pathbuf), path, pathlen);
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

