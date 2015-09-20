
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
#endif

#if defined(_MSC_VER)
#define strncasecmp strnicmp
#endif

#include "logobjects.h"

static int init_property_configurator(void *, size_t, bfc_mempool_t );
static size_t get_property_configurator_size(l4sc_configurator_cptr_t obj);

static int configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path);
static int configure_from_string(l4sc_configurator_ptr_t cfgtr,
					const char *s, size_t n);
static int config_from_property_line(const char *buf, const char *limit,
								int loop);


const struct l4sc_configurator_class l4sc_property_configurator_class = {
	.super = (l4sc_configurator_class_ptr_t) &l4sc_object_class,
	.name = "property configurator",
	.init = init_property_configurator,
	.clonesize = get_property_configurator_size,

	.configure = configure_from_string,
	.configure_from_file = configure_from_file,
};

static int
init_property_configurator(void *buf, size_t bufsize, bfc_mempool_t pool)
{
	BFC_INIT_PROLOGUE(l4sc_configurator_class_ptr_t,
			  l4sc_configurator_ptr_t,configurator,buf,bufsize,pool,
			  &l4sc_property_configurator_class);
	configurator->name = "Property configurator";
	return (BFC_SUCCESS);
}

static size_t
get_property_configurator_size(l4sc_configurator_cptr_t obj)
{
	return (sizeof(struct l4sc_configurator));
}

static const char *trim_front(const char *buf, const char *limit)
{
	const char *cp = buf;

	while ((cp < limit) && (((unsigned char *)cp)[0]  <= ' ')) ++cp;
	return (cp);
}

int trimmed_len(const char *base, const char *pos)
{
	const char *cp = pos;

	while ((cp > base) && (((unsigned char *)cp)[-1] <= ' ')) --cp;
	return ((int) (cp - base));
}

static int
configure_from_string(l4sc_configurator_ptr_t cfgtr, const char *s, size_t n)
{
	const char *line, *eol;
	const char *limit = (n > 0)? s + n: s + strlen(s);
	int loop, err = 0;

	LOGINFO(("%s: len %ld", __FUNCTION__, (long)(limit - s)));

	for (loop = 0; loop < 2; loop++) {
		for (line = s; (line >= s) && (line < limit); line = eol+1) {
			eol = memchr(line, '\n', limit - line);
			if (eol) {
				config_from_property_line(line, eol, loop);
			} else {
				config_from_property_line(line, limit, loop);
				break;
			}
		}
	}

	LOGINFO(("%s: done, error %d", __FUNCTION__, err));

	return ((err == 0)? 0: (err > 0)? -err: err);
}

static int
configure_from_file(l4sc_configurator_ptr_t cfgtr, const char *path)
{
	FILE *fp;
	char buf[256];
	int err = 0;

	LOGINFO(("%s: \"%s\"", __FUNCTION__, path));

	fp = fopen(path, "r");
	if (fp == NULL) {
		err = errno;
		LOGERROR(("%s: opening file \"%s\" failed, error %d: %s",
			__FUNCTION__, path, err, strerror(err)));
		return (-err);
	}

	while (fgets(buf, sizeof(buf), fp)) {
		config_from_property_line(buf, buf+strlen(buf), 0);
	}
	rewind(fp);
	while (fgets(buf, sizeof(buf), fp)) {
		config_from_property_line(buf, buf+strlen(buf), 1);
	}
	fclose(fp);

	LOGINFO(("%s: \"%s\" done, error %d", __FUNCTION__, path, err));

	return ((err == 0)? 0: (err > 0)? -err: err);
}


static int
config_from_property_line(const char *buf, const char *limit, int loop)
{
	l4sc_logger_ptr_t logger = NULL;
	l4sc_appender_ptr_t appender = NULL;
	l4sc_layout_ptr_t layout = NULL;
	const char *cp, *ep, *v;
	const char *nodetype = NULL, *nodename = NULL, *optname = NULL;
	int nodelen = 0, optlen = 0, vallen = 0;

	cp = trim_front(buf, limit);
	ep = memchr(cp, '=', limit-cp);
	if ((strncasecmp(cp, "log4", 4) != 0) || (ep == NULL)) {
		return (0);
	}
	v = trim_front(ep+1, limit); /* behind '=' */
	vallen = trimmed_len(v, limit);
	if ((cp = memchr(cp, '.', ep-cp)) == NULL) {
		return (0);
	}
	nodetype = trim_front(++cp, ep);
	if (strncasecmp(nodetype, "debug", 5) == 0) {
		l4sc_set_internal_logging(v, vallen);
		return (1);
	}
	if ((cp = memchr(cp, '.', ep-cp)) == NULL) {
		return (0);
	}
	nodename = trim_front(++cp, ep);
	nodelen  = trimmed_len(nodename, ep);
	if (strncasecmp(nodetype, "logger.", 7) == 0) {
		logger = l4sc_get_logger(nodename, nodelen);
		if (logger) {
			ep = v + vallen;
			cp = memchr(v, ',', ep-v);
			vallen = trimmed_len(v, cp? cp: ep);
			l4sc_set_object_option((l4sc_objptr_t) logger,
						"level", 5, v, vallen);
			while (cp && (cp < ep)) {
				v  = trim_front(++cp, ep);
				cp = memchr(cp, ',', ep-cp);
				vallen = trimmed_len(v, cp? cp: ep);
				if ((appender = l4sc_get_appender(v, vallen,
							NULL, 0)) != NULL) {
					l4sc_set_logger_appender(logger,
								 appender);
				} else if (loop == 0) {
					LOGINFO(("%s: no appender %.*s",
						__FUNCTION__, vallen, v));
				} else {
					LOGERROR(("%s: no appender %.*s",
						__FUNCTION__, vallen, v));
				}
			}
			return (1);
		} else {
			LOGERROR(("%s: no logger %.*s",
				__FUNCTION__, nodelen, nodename));
		}
		return (0);
	} else if (strncasecmp(nodetype, "additivity.", 11) == 0) {
		logger = l4sc_get_logger(nodename, nodelen);
		if (logger) {
			l4sc_set_object_option((l4sc_objptr_t) logger,
						"additivity", 10, v, vallen);
		}
		return (1);
	} else if (strncasecmp(nodetype, "appender.", 9) == 0) {
		if ((cp = memchr(cp, '.', ep-cp)) == NULL) {
			appender = l4sc_get_appender(nodename, nodelen,
								v, vallen);
			if (appender == NULL) {
				LOGERROR(("%s: could not create appender"
					" %.*s, type %.*s", __FUNCTION__,
					nodelen, nodename, vallen, v));
			}
			return (1);
		} else {
			nodelen = trimmed_len(nodename, cp);
			if ((appender = l4sc_get_appender(nodename, nodelen,
							  NULL, 0)) == NULL) {
				return (0);
			}
			optname = trim_front(++cp, ep);
			optlen  = trimmed_len(optname, ep);
			if (strncasecmp(optname, "layout", 6) == 0) {
				layout = l4sc_get_appender_layout(appender);
				if ((cp = memchr(cp, '.', ep-cp)) != NULL) {
					optname = trim_front(++cp, ep);
					optlen  = trimmed_len(optname, ep);
					l4sc_set_object_option(
						(l4sc_objptr_t) layout,
						optname, optlen, v, vallen);
				}
				return (1);
			}
			l4sc_set_object_option((l4sc_objptr_t) appender,
						optname, optlen, v, vallen);
			return (1);
		}
	}
	return (0);
}

int
l4sc_configure_from_property_file(const char *path)
{
	int rc;
	const int pathlen = strlen(path);
	struct l4sc_configurator obj;
	const size_t pathbufsize = 256 + pathlen;
	char *pathbuf;

	l4sc_close_appenders();

	pathbuf = alloca(pathbufsize);
	init_property_configurator(&obj, sizeof(obj), NULL);
	l4sc_merge_base_directory_path(pathbuf, pathbufsize, path, pathlen);
	rc = VMETHCALL(&obj, configure_from_file, (&obj, pathbuf), -ENOSYS);
	VMETHCALL(&obj, destroy, (&obj), (void) 0);

	l4sc_set_configured(1);

	return (rc);
}

int
l4sc_configure_from_property_string(const char *buf, size_t len)
{
	int rc;
	struct l4sc_configurator obj;

	l4sc_close_appenders();

	init_property_configurator(&obj, sizeof(obj), NULL);
	rc = VMETHCALL(&obj, configure, (&obj, buf, len), -ENOSYS);
	VMETHCALL(&obj, destroy, (&obj), (void) 0);

	l4sc_set_configured(1);

	return (rc);
}

