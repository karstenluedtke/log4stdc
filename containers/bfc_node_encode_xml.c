
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
static int tostring_with_entities(bfc_cobjptr_t node, bfc_cobjptr_t child,
				char *buf, size_t bufsize);

static int encode_xml_attributes(bfc_cobjptr_t node,
				char *buf, size_t bufsize);
static int encode_xml_attr_entities(char *dest, int size,
				const char *src, int len);
static int tostring_with_attr_entities(bfc_cobjptr_t child,
				char *buf, size_t bufsize);

extern bfc_class_t bfc_treenode_class;

int
bfc_node_encode_xml(bfc_cobjptr_t node, char *buf, size_t bufsize, int level)
{
	static const char spaces[] = "                                ";
	const char *indent = &spaces[32 - (2*level & 30)];
	size_t len = 0, spare;
	int rc, afterstring = 0;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	bfc_iterator_t iter, limit;
	char subfmt[16];

	L4SC_TRACE(logger, "%s(node @%p, buf %p, bufsize %ld, level %d)",
			__FUNCTION__, node, buf, (long)bufsize, level);

	len = strlen(indent) + 1;
	spare = (bufsize > len)? bufsize - len: 0;
	rc = bfc_object_tostring(&((bfc_cnodeptr_t)node)->tagname,
				 buf+len, spare, NULL);
	if (rc > 0) {
		if ((size_t)rc < spare) {
			L4SC_TRACE(logger, "%s(node @%p): <%s>",
					__FUNCTION__, node, buf+len);
			memcpy(buf, indent, len);
			buf[len-1] = '<';
			len += rc;

			spare = (bufsize > len)? bufsize - len: 0;
			rc = encode_xml_attributes(node, buf+len, spare);
			if (rc > 0) {
				len += rc;
			}

			if (len < bufsize) {
				buf[len] = '>';
			}
			len += 1;
		} else {
			len += rc + 1;
			if ((rc = encode_xml_attributes(node, NULL, 0)) > 0) {
				len += rc;
			}
		}
	}

	bfc_container_begin_iterator(node, &iter, sizeof(iter));
	bfc_container_end_iterator(node, &limit, sizeof(limit));
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
			snprintf(subfmt, sizeof(subfmt), "XML@%d", level+1);
			rc = bfc_object_tostring(child, buf+len, spare, subfmt);
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
			if ((spare > 0) && !afterstring) {
				while ((len > 0)
				    && (*(unsigned char*)(buf+len-1) <= ' ')) {
					len--; spare++;
				}
			}
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
	rc = bfc_object_tostring(&((bfc_cnodeptr_t)node)->tagname,
				 buf+len+2, spare, NULL);
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
tostring_with_entities(bfc_cobjptr_t node,
		       bfc_cobjptr_t child, char *buf, size_t bufsize)
{
	int rc;
	int specials_in_buf = 0;
	size_t tmpsize;
	char *tmp;
	struct mempool *pool;
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

	} else if (((pool = bfc_container_pool(node)) != NULL)
	        && ((tmp = bfc_mempool_alloc(pool, rc)) != NULL)) {
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
		bfc_mempool_free(pool, tmp);
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

static int
encode_xml_attributes(bfc_cobjptr_t node, char *buf, size_t bufsize)
{
	size_t len = 0, spare;
	bfc_contptr_t attrmap;
	bfc_iterator_t iter, limit;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	L4SC_TRACE(logger, "%s(node %p, buf %p, bufsize %ld)",
				__FUNCTION__, node, buf, (long) bufsize);

	if (node == NULL) {
		return (-EFAULT);
	} else if ((attrmap = ((bfc_cnodeptr_t)node)->attributes) == NULL) {
		L4SC_DEBUG(logger, "%s(node %p): no attributes",
						__FUNCTION__, node);
		return (0);
	}

	bfc_container_begin_iterator(attrmap, &iter, sizeof(iter));
	bfc_container_end_iterator(attrmap, &limit, sizeof(limit));

	while (bfc_iterator_distance(&iter, &limit) > 0) {
		int rc1, rc2;
		bfc_contptr_t pair = bfc_iterator_index(&iter);
		bfc_cobjptr_t name, value;
		bfc_iterator_advance(&iter, 1);
		if (pair == NULL) {
			L4SC_DEBUG(logger, "%s(node %p): no pair",
						__FUNCTION__, node);
			continue;
		}
		if ((name = bfc_container_first(pair)) == NULL) {
			L4SC_ERROR(logger, "%s(node %p): no name",
						__FUNCTION__, node);
			continue;
		}
		if ((value = bfc_container_index(pair, 1)) == NULL) {
			L4SC_ERROR(logger, "%s(node %p): no value",
						__FUNCTION__, node);
			continue;
		}
		spare = (bufsize > len+4)? bufsize-len-1: 0;
		rc1 = tostring_with_attr_entities(name, buf+len+1, spare);
		if (rc1 < 0) {
			L4SC_ERROR(logger, "%s(node %p): rc1 = %d",
						__FUNCTION__, node, rc1);
			continue;
		}
		spare = (bufsize > len+rc1+4)? bufsize-len-rc1-3: 0;
		rc2 = tostring_with_attr_entities(value, buf+len+rc1+3, spare);
		if (rc2 < 0) {
			L4SC_ERROR(logger, "%s(node %p): rc2 = %d",
						__FUNCTION__, node, rc2);
			continue;
		}
		if (len+rc1+rc2+4 < bufsize) {
			buf[len] = ' ';
			buf[len+rc1+1] = '=';
			buf[len+rc1+2] = '"';
			buf[len+rc1+rc2+3] = '"';
		}
		len += rc1 + rc2 + 4;
		L4SC_DEBUG(logger, "%s(node %p): rc1 %d, rc2 %d, len %ld",
				__FUNCTION__, node, rc1, rc2, (long) len);
	}

	return ((int) len);
}

#define NEED_ATTR_ENTITIES(buf,len) \
	(memchr(buf,'"',len) || NEED_XML_ENTITIES(buf,len))

static int
tostring_with_attr_entities(bfc_cobjptr_t child, char *buf, size_t bufsize)
{
	int rc;
	int specials_in_buf = 0;
	size_t tmpsize;
	char *tmp;
	l4sc_logger_ptr_t logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);

	if ((rc = bfc_object_tostring(child, buf, bufsize, NULL)) <= 0) {
		return (rc);
	} else if (((size_t) rc < bufsize)
		&& ((specials_in_buf = NEED_ATTR_ENTITIES(buf, rc)) == 0)) {
		L4SC_DEBUG(logger, "%s: fit and no entities in %s, rc = %d",
							__FUNCTION__, buf, rc);
		return (rc);
	}

	tmpsize = rc + 100;
	tmp = alloca(tmpsize);
	if (specials_in_buf) {
		memcpy(tmp, buf, rc);
		tmp[rc] = '\0';
	} else {
		rc = bfc_object_tostring(child, tmp, tmpsize, NULL);
		if ((rc <= 0) || !NEED_ATTR_ENTITIES(tmp, rc)) {
			L4SC_DEBUG(logger, "%s: no entities in %s, rc = %d",
						__FUNCTION__, tmp, rc);
			return (rc);
		}
	}
	rc = encode_xml_attr_entities(buf, bufsize, tmp, rc);
	L4SC_DEBUG(logger, "%s: with entities in %s, rc = %d",
						__FUNCTION__, tmp, rc);

	return (rc);
}

static int
encode_xml_attr_entities(char *dest, int size, const char *src, int len)
{
	const char *cp = src;
	const char *ep = src + len;
	const char *xp = dest + size;
	char c;
	char *dp = dest;

	while (cp < ep) {
		c = *cp++;
		switch (c) {
		case '"':
			if (dp+6 < xp) {
				memcpy (dp, "&quot;", 6);
			}
			dp += 6;
			break;
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
