
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <expat.h>

#include "logobjects.h"

static int init_xml_configurator(void *, size_t, struct mempool *);
static size_t get_xml_configurator_size(l4sc_configurator_cptr_t obj);

static int configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path);

const struct l4sc_configurator_class l4sc_xml_configurator_class = {
	.super = (l4sc_configurator_class_ptr_t) &l4sc_object_class,
	.name = "xml configurator",
	.init = init_xml_configurator,
	.clonesize = get_xml_configurator_size,

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
	const char *name;
	const char *value;
	const char *ref;
	const char *class;
	int         namelen;
	int         valuelen;
	int         reflen;
	int         classlen;
};

static void XMLCALL
StartElementHandler(void *userData, const XML_Char *name,
				    const XML_Char **attrs)
{
	struct parsing_state *ps = (struct parsing_state *) userData;
	const char *nm, *ns = NULL;
	int i, nslen;
	struct element_values values;

	if (ps->depth == 0) {
		for (i=0; attrs[2*i]; i++) {
			const char *a = attrs[2*i];
			const char *v = attrs[2*i+1];
			if ((strncasecmp(a, "debug", 5) == 0)
			 || (strncasecmp(a, "internalDebug", 13) == 0)
			 || (strncasecmp(a, "configDebug",   11) == 0)) {
				l4sc_set_internal_logging(v, strlen(v));
				break;
			}
		}
	}
	if ((nm = strchr(name, NS_DELIMITER)) == NULL) {
		nm = name;
		LOGDEBUG(("%s: <%s>", __FUNCTION__, nm));
	} else {
		ns = name;
		nslen = nm++ - name;
		LOGDEBUG(("%s: <%s xmlns=\"%.*s\">",__FUNCTION__,nm,nslen,ns));
	}
	memset(&values, 0, sizeof(values));
	for (i=0; attrs[2*i]; i++) {
		const char *a = attrs[2*i];
		const char *v = attrs[2*i+1];
		LOGDEBUG(("%s: attr #%d \"%s\" = \"%s\"",
				__FUNCTION__, i, a, v? v: ""));
		if (a && v) {
			if (strncasecmp(a, "name", 4) == 0) {
				values.name = v;
				values.namelen = strlen(v);
			} else if (strncasecmp(a, "value", 5) == 0) {
				values.value = v;
				values.valuelen = strlen(v);
			} else if (strncasecmp(a, "class", 5) == 0) {
				values.class = v;
				values.classlen = strlen(v);
			} else if (strncasecmp(a, "ref", 3) == 0) {
				values.ref = v;
				values.reflen = strlen(v);
			}
		}
	}
	ps->depth++;
	if (strncasecmp(nm, "logger", 6) == 0) {
		ps->current_logger = l4sc_get_logger(
					values.name, values.namelen);
		for (i=0; attrs[2*i]; i++) {
			const char *a = attrs[2*i];
			const char *v = attrs[2*i+1];
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				a, strlen(a), v, v? strlen(v): 0);
		}
	} else if (strncasecmp(nm, "appender-ref", 12) == 0) {
		l4sc_appender_ptr_t appender = l4sc_get_appender(
					values.ref, values.reflen, NULL, 0);
		if (appender && ps->current_logger) {
			l4sc_set_logger_appender(ps->current_logger, appender);
		}
	} else if (strncasecmp(nm, "appender", 8) == 0) {
		ps->current_appender = l4sc_get_appender(
					values.name, values.namelen,
					values.class, values.classlen);
		for (i=0; attrs[2*i]; i++) {
			const char *a = attrs[2*i];
			const char *v = attrs[2*i+1];
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				a, strlen(a), v, v? strlen(v): 0);
		}
	} else if (strncasecmp(nm, "layout", 6) == 0) {
		if (ps->current_appender) {
			ps->current_layout = l4sc_get_appender_layout(
						ps->current_appender);
			for (i=0; attrs[2*i]; i++) {
				const char *a = attrs[2*i];
				const char *v = attrs[2*i+1];
				l4sc_set_object_option(
					(l4sc_objptr_t) ps->current_layout,
					a, strlen(a), v, v? strlen(v): 0);
			}
		}
	} else if (strncasecmp(nm, "param", 5) == 0) {
		if (ps->current_layout) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_layout,
				values.name, values.namelen,
				values.value, values.valuelen);
		} else if (ps->current_appender) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				values.name, values.namelen,
				values.value, values.valuelen);
		} else if (ps->current_logger) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				values.name, values.namelen,
				values.value, values.valuelen);
		}
	} else if (values.value) {
		int nmlen = strlen(nm);
		if (ps->current_layout) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_layout,
				nm, nmlen, values.value, values.valuelen);
		} else if (ps->current_appender) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_appender,
				nm, nmlen, values.value, values.valuelen);
		} else if (ps->current_logger) {
			l4sc_set_object_option(
				(l4sc_objptr_t) ps->current_logger,
				nm, nmlen, values.value, values.valuelen);
		}
	}
}

static void XMLCALL
EndElementHandler(void *userData, const XML_Char *name)
{
	struct parsing_state *ps = (struct parsing_state *) userData;
	const char *nm, *ns = NULL;
	int nslen;

	if ((nm = strchr(name, NS_DELIMITER)) == NULL) {
		nm = name;
		LOGDEBUG(("%s: <%s>", __FUNCTION__, nm));
	} else {
		ns = name;
		nslen = nm++ - name;
		LOGDEBUG(("%s: <%s xmlns=\"%.*s\">",__FUNCTION__,nm,nslen,ns));
	}
	ps->depth--;
	if (strncasecmp(nm, "logger", 6) == 0) {
		ps->current_logger = NULL;
	} else if (strncasecmp(nm, "appender", 8) == 0) {
		ps->current_appender = NULL;
	} else if (strncasecmp(nm, "layout", 6) == 0) {
		ps->current_layout = NULL;
	}
}

static int
configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path)
{
	int err = 0, bytes_read = 0;
	FILE *fp;
	XML_Parser p;
	const int BUFF_SIZE = 256;
	struct parsing_state state;

	LOGINFO(("%s: \"%s\"", __FUNCTION__, path));

	fp = fopen(path, "r");
	if (fp == NULL) {
		err = errno;
		LOGERROR(("%s: opening file \"%s\" failed, error %d: %s",
			__FUNCTION__, path, err, strerror(err)));
		return (-err);
	}

	p = XML_ParserCreateNS(NULL, NS_DELIMITER);
	if (p == NULL) {
		LOGERROR(("%s: creating parser failed", __FUNCTION__));
		return (-ENOMEM);
	}

	memset(&state, 0, sizeof(state));
	state.configurator = cfgtr;

	XML_SetElementHandler(p, StartElementHandler, EndElementHandler);
	XML_SetUserData(p, &state);

	do {
		void *buff = XML_GetBuffer(p, BUFF_SIZE);
		if (buff == NULL) {
			LOGERROR(("%s: creating buffer failed", __FUNCTION__));
			err = ENOMEM;
			break;
		}

		bytes_read = fread(buff, 1, BUFF_SIZE, fp);
		if (bytes_read < 0) {
			err = errno;
			LOGERROR(("%s: reading \"%s\" failed, error %d: %s",
				__FUNCTION__, path, err, strerror(err)));
			break;
		}

		if (! XML_ParseBuffer(p, bytes_read, bytes_read == 0)) {
			err = XML_GetErrorCode(p);
			LOGERROR(("%s: reading \"%s\" failed, error %d: %s",
				__FUNCTION__, path, err, XML_ErrorString(err)));
			break;
		}
	} while (bytes_read > 0);

	XML_ParserFree(p);
	fclose(fp);

	LOGINFO(("%s: \"%s\" done, error %d", __FUNCTION__, path, err));

	return ((err == 0)? 0: (err > 0)? -err: err);
}

int
l4sc_configure_from_xml_file(const char *path)
{
	int rc;
	struct l4sc_configurator obj;
	const int pathlen = strlen(path);
	char pathbuf[256 + pathlen];

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

	init_xml_configurator(&obj, sizeof(obj), NULL);
	rc = VMETHCALL(&obj, configure, (&obj, buf, len), -ENOSYS);
	VMETHCALL(&obj, destroy, (&obj), (void) 0);

	l4sc_set_configured(1);

	return (rc);
}

