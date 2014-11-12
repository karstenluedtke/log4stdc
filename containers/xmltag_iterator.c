
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/string.h"
#include "barefootc/xmltag.h"
#include "log4stdc.h"

static int  init_iterator(void *buf,size_t bufsize,struct mempool *pool);
static int  iterator_tostring(bfc_ctagptr_t tag, char *buf, size_t bufsize);
static void dump_iterator(bfc_ctagptr_t tag,int depth,struct l4sc_logger *log);
static int  advance_forward(bfc_tagptr_t tag, ptrdiff_t n);
static int  get_xmltag_name(bfc_ctagptr_t tag,bfc_strptr_t buf,size_t bufsize);
static int  get_xmltag_attrs(bfc_ctagptr_t tag,bfc_strptr_t buf,size_t bufsize);
static int  get_namespace_prefix(bfc_ctagptr_t tag,
					bfc_strptr_t buf, size_t bufsize);

extern struct bfc_iterator_class bfc_forward_iterator_class;

#define FORWARD_CLASS_NAME	"XML tag forward iterator"

struct bfc_xmltag_iterator_class bfc_xmltag_forward_iterator_class = {
	.super	= (const void *) &bfc_forward_iterator_class,
	.name	= FORWARD_CLASS_NAME,
	.init 	= init_iterator,
	/* Iterator functions */
	.advance    = advance_forward,
	/* Tag methods */
	.find_nexttag = bfc_find_next_xmltag,
	.find_endtag  = bfc_find_xml_endtag,
	.get_name   = get_xmltag_name,
};

static int
init_iterator(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_tagptr_t tag = (bfc_tagptr_t) buf;
	if (bufsize < sizeof(*tag)) {
		return (-ENOSPC);
	}
	memset(tag, 0, sizeof(*tag));
	tag->vptr = &bfc_xmltag_forward_iterator_class;
	return (BFC_SUCCESS);
}

int
bfc_init_xmltag(void *buf, size_t bufsize, bfc_cstrptr_t doc, size_t offset)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	L4SC_TRACE(logger, "%s(%p, %ld, %p, %ld)",
		__FUNCTION__, buf, (long) bufsize, doc, (long) offset);

	if ((rc = init_iterator(buf, bufsize, NULL)) == BFC_SUCCESS) {
		bfc_tagptr_t tag = (bfc_tagptr_t) buf;
		size_t len = bfc_strlen(doc);
		tag->obj = (bfc_strptr_t) (uintptr_t) doc;
		tag->pos = offset;
		if (offset == BFC_NPOS) {
			tag->pos = len;
		} else if (offset > len) {
			rc = -ERANGE;
		}
	}
	bfc_object_dump(buf, 1, logger);
	return (rc);
}

static int
advance_forward(bfc_tagptr_t tag, ptrdiff_t n)
{
	int i, rc = BFC_SUCCESS;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);
	bfc_xmltag_t prev, next;

	L4SC_TRACE(logger, "%s(xmltag %p, %ld)", __FUNCTION__, tag, (long) n);

	if (n > 0) {
		rc = bfc_find_next_xmltag(tag, &next, sizeof(next));
		if (rc < 0) {
			L4SC_DEBUG(logger, "%s(xmltag %p, %ld): result %d",
					__FUNCTION__, tag, (long) n, rc);
			return (rc);
		}
		for (i=1; i < (int) n; i++) {
			prev = next;
			rc = bfc_find_next_xmltag(&prev, &next, sizeof(next));
			if (rc < 0) {
				L4SC_ERROR(logger, "%s(xmltag %p, %ld): "
					"error %d on iteration #%d",
					__FUNCTION__, tag, (long) n, rc, i+1);
				return (rc);
			}
		}
		*tag = next;
	} else if (n < 0) {
		L4SC_ERROR(logger, "%s(xmltag %p, %ld): cannot move backward",
			__FUNCTION__, tag, (long) n);
		return (-EINVAL);
	}

	L4SC_DEBUG(logger, "%s(xmltag %p, %ld): result %d",
				__FUNCTION__, tag, (long) n, rc);
	bfc_object_dump(tag, 1, logger);
	return (rc);
}

int
bfc_find_next_xmltag(bfc_ctagptr_t tag, bfc_tagptr_t next, size_t bufsize)
{
	bfc_strptr_t s = tag->obj;
	size_t ti, ni, ne, te;
	size_t offs = tag->pos + tag->length;
	size_t limit = bfc_strlen(s);
	int level = tag->level + ((tag->tagtype == BFC_XML_START_TAG)? 1: 0);
	char tt;
	long c;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(xmltag %p, next %p, bufsize %ld)",
		__FUNCTION__, tag, next, (long) bufsize);

	ti = bfc_string_find_char(s, '<', offs);
	if ((ti != BFC_NPOS) && (ti >= offs) && (ti < limit)) {
		if ((c = bfc_string_get_char(s, ti+1)) == '/') {
			tt = BFC_XML_END_TAG;
			ni = ti+2;
		} else if (c == '?') {
			tt = BFC_XML_HEADER_TAG;
			ni = ti+2;
		} else {
			tt = BFC_XML_START_TAG; /* or EMPTY_TAG */
			ni = ti+1;
		}

		ne = ni;
		while ((ne < limit)
		    && ((c = bfc_string_get_char(s, ne)) > ' ')
		    && (c != '>') && (c != '/')) {
			ne++;
			if (c == ':') {
				ni = ne;
			}
		}

		te = bfc_string_find_char(s, '>', ne);
		if ((te != BFC_NPOS) && (te >= ne) && (te < limit)) {
			if (bfc_string_get_char(s, te-1) == '/') {
				tt = BFC_XML_EMPTY_TAG;
			}
			if ((tt == BFC_XML_END_TAG) && (level > 0)) {
				level--;
			}
			if (next && (bfc_init_xmltag(next,bufsize,s,ti) >= 0)) {
				next->tagtype = tt;
				next->level   = (unsigned short)level;
				next->length  = (unsigned)(te+1-ti);
				next->nameoffs= (unsigned short)(ni - ti);
				next->namelen = (unsigned short)(ne - ni);
				bfc_object_dump(next, 1, logger);
			}
			return (tt);
		}
	}
	return (-ENOENT);
}

int
bfc_find_xml_endtag(bfc_ctagptr_t starttag, bfc_tagptr_t endtag, size_t bufsize)
{
	bfc_strptr_t s = starttag->obj;
	size_t offs = starttag->pos + starttag->length;
	size_t limit = bfc_strlen(s);
	size_t nameoffs= starttag->nameoffs;
	size_t namelen = starttag->namelen;
	size_t ti, patlen, b2size;
	void *databuf;
	bfc_string_t pattern;
	static const bfc_string_t SLASHSTR = BFCSTR("/");
	static const bfc_string_t CLOSESTR = BFCSTR(">");
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(xmltag %p, endtag %p, bufsize %ld)",
		__FUNCTION__, starttag, endtag, (long) bufsize);
	bfc_object_dump(starttag, 1, logger);

	b2size = 4 * (nameoffs + namelen + 5);
	databuf = alloca(b2size);
	bfc_string_buffered_substr(s, starttag->pos, nameoffs + namelen, 
				   &pattern, sizeof(pattern), databuf, b2size);
	bfc_string_insert(&pattern, 1, &SLASHSTR);
	bfc_string_append(&pattern,    &CLOSESTR);
	patlen = bfc_strlen(&pattern);
	bfc_object_dump(&pattern, 1, logger);
	if ((nameoffs = bfc_string_find_char(&pattern, ':', 0)) != BFC_NPOS) {
		nameoffs++;
	} else {
		nameoffs = 2; /* behind "</" */
	}

	ti = bfc_string_find_bfstr(s, &pattern, offs);
	if ((ti != BFC_NPOS) && (ti >= offs) && (ti < limit)) {
		if (endtag && (bfc_init_xmltag(endtag, bufsize, s, ti) >= 0)) {
			endtag->tagtype = BFC_XML_END_TAG;
			endtag->length  = (unsigned short) patlen;
			endtag->nameoffs= (unsigned short) nameoffs;
			endtag->namelen = (unsigned short) namelen;
			bfc_object_dump(endtag, 1, logger);
		}
		return (BFC_XML_END_TAG);
	}
	return (-ENOENT);
}

static int
get_xmltag_name(bfc_ctagptr_t tag, bfc_strptr_t buf, size_t bufsize)
{
	return (bfc_string_shared_substr(tag->obj,
		  tag->pos + tag->nameoffs, tag->namelen, buf, bufsize));
}

static int
get_xmltag_attrs(bfc_ctagptr_t tag, bfc_strptr_t attrs, size_t bufsize)
{
	bfc_strptr_t ap, s = tag->obj;
	size_t pos = tag->pos + tag->nameoffs + tag->namelen;
	size_t limit = tag->pos + tag->length;
	unsigned n=0, maxattrs = (unsigned) (bufsize / 2*sizeof(attrs[0]));
	size_t start, assign, quote, endquote;
	long c;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	L4SC_TRACE(logger, "%s(xmltag %p, buf %p, size %ld): max %u",
		__FUNCTION__, tag, attrs, (long) bufsize, maxattrs);
	bfc_object_dump(tag, 1, logger);

	while ((pos < limit) && (n < maxattrs)) {
		while ((pos < limit) && (bfc_string_get_char(s, pos) > ' ')) {
			pos++;
		}
		while ((pos < limit) && (bfc_string_get_char(s, pos) < ' ')) {
			pos++;
		}
		if (pos >= limit) {
			break;
		}
		start = pos;
		assign = quote = endquote = 0;
		c = bfc_string_get_char(s, pos);
		if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) {
			while (!endquote && (pos < limit)) {
				c = bfc_string_get_char(s, pos);
				if (assign != 0) {
					if (c == '"') {
						if (quote == 0) {
							quote = pos;
						} else {
							endquote = pos;
						}
					}
				} else if (c == '=') {
					assign = pos;
				}
				pos++;
			}
		}
		if (assign && (quote > assign) && (endquote > quote)) {
			L4SC_DEBUG(logger, "%s(xmltag) #%d:", __FUNCTION__, n);
			ap = &attrs[2*(n++)];
			bfc_shared_string_substr(s, start, assign-start,
						 ap, sizeof(*ap));
			bfc_string_trim(ap);
			bfc_object_dump(ap, 1, logger);
			ap++;
			bfc_string_shared_substr(s, quote+1, endquote-quote-1,
						 ap, sizeof(*ap));
			bfc_object_dump(ap, 1, logger);
		}
	}
	return ((int) n);
}

static int
get_namespace_prefix(bfc_ctagptr_t tag, bfc_strptr_t buf, size_t bufsize)
{
	if (tag->nameoffs > 2) {
		return (bfc_string_shared_substr(tag->obj,
			  tag->pos+1, tag->nameoffs-2, buf, bufsize));
	}
	bfc_string_shared_substr(tag->obj, 0, 0, buf, bufsize);
	return (-ENOENT);
}

static int
iterator_tostring(bfc_ctagptr_t tag, char *buf, size_t bufsize)
{
	bfc_string_t tagstr;

	if (tag && BFC_CLASS(tag) && tag->obj) {
		bfc_string_shared_substring(tag->obj, tag->pos, tag->length,
					    &tagstr, sizeof(tagstr));
		return (bfc_object_tostring(&tagstr, buf, bufsize));
	}
	return (0);
}

static void
dump_iterator(bfc_ctagptr_t tag, int depth, struct l4sc_logger *log)
{
	char buf[200];
	size_t maxsize;

	if (tag && BFC_CLASS(tag)) {
		L4SC_DEBUG(log, "%s type %d len %d @%p, name @%d len %d",
			BFC_CLASS(tag)->name, tag->tagtype, tag->length, tag,
			tag->nameoffs, tag->namelen);

		iterator_tostring(tag, buf, sizeof(buf));

		L4SC_DEBUG(log, "pos %ld in %p: %s",
			(long) tag->pos, tag->obj, buf);
	}
}

