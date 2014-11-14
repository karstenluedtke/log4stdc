
#include "barefootc/object.h"
#include "barefootc/string.h"
#include "barefootc/iterator.h"

static int indicate_start_tag(struct bfc_tag_parse_state *st, int tagtype);
static int indicate_end_tag(struct bfc_tag_parse_state *st, int tagtype);

int
bfc_parse_xmltags(bfc_cstrptr_t s, struct bfc_tag_parse_state *st)
{
	int rc;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_STRING_LOGGER);

	if (st->maxlevels < sizeof(st->tags) / sizeof(st->tags[0])) {
		st->maxlevels = sizeof(st->tags) / sizeof(st->tags[0]);
	}
	if (BFC_CLASS(&st->prev) != &bfc_xmltag_forward_iterator_class) {
		bfc_init_xmltag(&st->prev, sizeof(st->prev), s, 0);
	}
	st->curr = st->prev;
	while ((rc = bfc_iterator_advance((bfc_iterptr_t) &st->curr, 1)) >= 0) {
		if (st->curr.level >= st->maxlevels) {
			L4SC_ERROR(logger, "%s: level %d >= max %d",
				__FUNCTION__, st->curr.level, st->maxlevels);
			return (-ENOSPC);
		}
		if (st->on_start_tag && ((rc == BFC_XML_START_TAG)
				      || (rc == BFC_XML_EMPTY_TAG))) {
			indicate_start_tag(st, rc);
		}
		if (st->on_end_tag && ((rc == BFC_XML_END_TAG)
				    || (rc == BFC_XML_EMPTY_TAG))) {
			indicate_end_tag(st, rc);
		}
		st->prev = st->curr;
		st->tags[st->curr.level] = st->curr;
	}
	return (st->prev.level);
}

static int
indicate_start_tag(struct bfc_tag_parse_state *st, int tagtype)
{
	int i, rc, nattrs;
	bfc_ctagptr_t tag = &st->curr;
	void *buf;
	size_t bufsize;
	bfc_string_t namestr, tmp;
	bfc_string_t attrs[40];

	get_xmltag_name(tag, &namestr, sizeof(namestr));

	nattrs = get_xmltag_attrs(tag, attrs, sizeof(attrs));
	for (i=0; i < 2*nattrs; i++) {
		if (bfc_string_find_char(&attrs[i], '&', 0) != BFC_NPOS) {
			bufsize = 4*bfc_strlen(&attrs[i]) + 10;
			buf = alloca(bufsize);
			bfc_string_buffered_substr(&attrs[i], 0, BFC_NPOS,
					   &tmp, sizeof(tmp), buf, bufsize);
			bfc_string_decode_html_entities(&tmp);
			bfc_string_swap(&attrs[i], &tmp);
			bfc_destroy(&tmp);
		}
	}

	if (st->on_start_tag) {
		rc = (*st->on_start_tag)(st, tag, &namestr, nattrs, attrs);
	}

	return (rc);
}

static int
indicate_end_tag(struct bfc_tag_parse_state *st, int tagtype)
{
	int rc;
	bfc_ctagptr_t tag = &st->curr;
	bfc_ctagptr_t starttag = &st->tags[tag->level];
	bfc_string_t namestr;

	get_xmltag_name(tag, &namestr, sizeof(namestr));

	if (st->on_end_tag) {
		rc = (*st->on_end_tag)(st, starttag, tag, &namestr);
	}

	return (rc);
}

