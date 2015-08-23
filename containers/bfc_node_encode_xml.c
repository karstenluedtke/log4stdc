
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <inttypes.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "barefootc/object.h"
#include "barefootc/iterator.h"
#include "barefootc/container.h"
#include "barefootc/treenode.h"
#include "log4stdc.h"

static int encode_xml_entities(char *dest, int size, const char *src, int len);
static int tostring_with_entities(bfc_cnodeptr_t node,
				bfc_cobjptr_t child, char *buf, size_t bufsize);

extern bfc_class_t bfc_treenode_class;

int
bfc_node_encode_xml(bfc_cnodeptr_t node, char *buf, size_t bufsize, int level)
{
	static const char spaces[] = "                ";
	const char *indent = &spaces[16 - (2*level & 14)];
	size_t len = 0, spare;
	int rc, afterstring = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_iterator_t iter, limit;

	L4SC_TRACE(logger, "%s(node @%p, buf %p, bufsize %ld, level %d)",
			__FUNCTION__, node, buf, (long)bufsize, level);

	len = strlen(indent) + 1;
	spare = (bufsize > len)? bufsize - len: 0;
	rc = bfc_object_tostring(&node->tagname, buf+len, spare, NULL);
	if (rc > 0) {
		if ((size_t)rc < spare) {
			L4SC_TRACE(logger, "%s(node @%p): <%s>",
					__FUNCTION__, node, buf+len);
			memcpy(buf, indent, len);
			buf[len-1] = '<';
			len += rc;
			if (len < bufsize) {
				buf[len] = '>';
			}
			len += 1;
		} else {
			len += rc + 1;
		}
	}

	bfc_container_begin_iterator(&node->vec, &iter, sizeof(iter));
	bfc_container_end_iterator(&node->vec, &limit, sizeof(limit));
	if (bfc_iterator_distance(&iter, &limit) < 1) {
		if (bufsize > len+2) {
			memcpy(buf+len-1, "/>\0", 3);
		}
		return (++len);
	} else {
		bfc_cobjptr_t first = bfc_iterator_index(&iter);
		if (bfc_instance_of_class(first, &bfc_treenode_class)) {
			if (len+2 < bufsize) {
				buf[len] = '\r';
				buf[len+1] = '\n';
			}
			len += 2;
		}
	}

	while (bfc_iterator_distance(&iter, &limit) > 0) {
		bfc_cobjptr_t child = bfc_iterator_index(&iter);
		bfc_iterator_advance(&iter, 1);
		if (child == NULL) {
			continue;
		}
		if (bfc_instance_of_class(child, &bfc_treenode_class)) {
			spare = (bufsize > len)? bufsize - len: 0;
			rc = bfc_node_encode_xml((bfc_cnodeptr_t) child,
						 buf+len, spare, level+1);
			if ((rc > 0) && ((size_t)rc < spare)) {
				unsigned char *cp = (unsigned char*) buf + len;
				if (afterstring) {
					unsigned char *dp = cp;
					while ((rc > 0) && (*cp <= ' ')) {
						rc--; cp++;
					}
					if ((cp > dp) && (rc > 0)) {
						memmove(dp, cp, rc);
					}
				}
			}
			if (rc > 0) {
				len += rc;
			}
			afterstring = 0;

		} else if (bfc_instance_of_classname(child, "string")
		        || bfc_instance_of_classname(child, "wstring")
		        || bfc_instance_of_classname(child, "number")
		        || bfc_instance_of_classname(child, "boolean")
		        || bfc_instance_of_classname(child, "datetime")) {
			spare = (bufsize > len)? bufsize - len: 0;
			rc = tostring_with_entities(node, child, buf+len,spare);
			if (rc > 0) {
				len += rc;
			}
			afterstring = 1;
		} else {
			L4SC_ERROR(logger, "%s(node %p): child type %s",
				__FUNCTION__, node, BFC_CLASS(child)->name);
		}
	}

	if (!afterstring) {
		spare = (bufsize > len)? bufsize - len: 0;
		rc = strlen(indent);
		if (rc > 0) {
			if ((size_t)rc < spare) {
				memcpy(buf+len, indent, rc);
			}
			len += rc;
		}
	}

	spare = (bufsize > len+3)? bufsize - len+2: 0;
	rc = bfc_object_tostring(&node->tagname, buf+len+2, spare, NULL);
	if (rc > 0) {
		if ((size_t)rc < spare) {
			buf[len] = '<';
			buf[len+1] = '/';
			buf[len+rc+2] = '>';
		}
		len += rc + 3;
	}

	if (len+2 < bufsize) {
		buf[len] = '\r';
		buf[len+1] = '\n';
	}
	len += 2;

	if (len < bufsize) {
		buf[len] = '\0';
	} else if (bufsize > 0) {
		buf[bufsize-1] = '\0';
	}

	return (len);
}

#define NEED_XML_ENTITIES(buf,len) \
	(memchr(buf,'<',len) || memchr(buf,'&',len) || memchr(buf,'>',len))

static int
tostring_with_entities(bfc_cnodeptr_t node,
		       bfc_cobjptr_t child, char *buf, size_t bufsize)
{
	int rc;
	int specials_in_buf = 0;
	size_t tmpsize;
	char *tmp;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if ((rc = bfc_object_tostring(child, buf, bufsize, NULL)) <= 0) {
		return (rc);
	} else if (((size_t) rc < bufsize)
		&& ((specials_in_buf = NEED_XML_ENTITIES(buf, rc)) == 0)) {
		L4SC_DEBUG(logger, "%s: fit and no entities in %s, rc = %d",
							__FUNCTION__, buf, rc);
		return (rc);
	}

	tmpsize = rc + 100;
	if (tmpsize < 1000) {
		tmp = alloca(tmpsize);
		if (specials_in_buf) {
			memcpy(tmp, buf, rc);
			tmp[rc] = '\0';
		} else {
			rc = bfc_object_tostring(child, tmp, tmpsize, NULL);
			if ((rc <= 0) || !NEED_XML_ENTITIES(tmp, rc)) {
				L4SC_DEBUG(logger, "%s: no entities in %s, rc = %d",
						__FUNCTION__, tmp, rc);
				return (rc);
			}
		}
		rc = encode_xml_entities(buf, bufsize, tmp, rc);
		L4SC_DEBUG(logger, "%s: with entities in %s, rc = %d",
						__FUNCTION__, tmp, rc);

	} else if ((tmp = bfc_mempool_alloc(node->vec.pool, rc)) != NULL) {
		if (specials_in_buf) {
			memcpy(tmp, buf, rc);
			tmp[rc] = '\0';
		} else {
			rc = bfc_object_tostring(child, tmp, tmpsize, NULL);
			if ((rc <= 0) || !NEED_XML_ENTITIES(tmp, rc)) {
				L4SC_DEBUG(logger, "%s: no entities in %s, rc = %d",
						__FUNCTION__, tmp, rc);
				return (rc);
			}
		}
		rc = encode_xml_entities(buf, bufsize, tmp, rc);
		L4SC_DEBUG(logger, "%s: with entities in %s, rc = %d",
						__FUNCTION__, tmp, rc);
		bfc_mempool_free(node->vec.pool, tmp);
	}
	return (rc);
}

static int
encode_xml_entities(char *dest, int size, const char *src, int len)
{
	const char *cp = src;
	const char *ep = src + len;
	const char *xp = dest + size;
	char c;
	char *dp = dest;

	while (cp < ep) {
		c = *cp++;
		switch (c) {
		case '<':
			if (dp+4 < xp) {
				memcpy (dp, "&lt;", 4);
			}
			dp += 4;
			break;
		case '>':
			if (dp+4 < xp) {
				memcpy (dp, "&gt;", 4);
			}
			dp += 4;
			break;
		case '&':
			if (dp+5 < xp) {
				memcpy (dp, "&amp;", 5);
			}
			dp += 5;
			break;
		default:
			if (dp < xp) {
				*dp = c;
			}
			dp += 1;
		}
	}
	if (dp < xp) {
		*dp = '\0';
	}
	return (dp - dest);
}