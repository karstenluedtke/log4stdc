
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/vector.h"
#include "barefootc/container.h"
#include "barefootc/mempool.h"
#include "barefootc/utf8.h"

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

static int
test_all_equal(const char *p, char c, size_t n)
{
	size_t i;
	for (i=0; i < n; i++) {
		if (p[i] != c) {
			L4SC_ERROR(logger, "%s: %p: p[%lu] %c != %c",
				__FUNCTION__, p+i, (long)i, p[i], c);
			return (1);
		}
	}
	return (0);
}

static int
test(void *initialized_vector, unsigned elem_size)
{
	int rc;
	char *p0, *p1;
	BFC_VECTOR(testvec, char, 0) *vec = initialized_vector;
	unsigned k, n = CV1_BOUNDARY(vec) + CV1_ELEMENTS(vec) + 5;
	BFC_VECTOR(testv2s, char, 1200) v2;

	L4SC_DEBUG(logger, "%s(vec %p, elem_size %u) n=%u",
		__FUNCTION__, vec, elem_size, n);

	bfc_object_dump(vec, 1, logger);

	p0 = (char *) bfc_vector_have(vec, 0);
	L4SC_DEBUG(logger, "%s: vec[%u] @%p", __FUNCTION__, 0, p0);
	if (CV0_ELEMENTS(vec) > 0) {
		assert(p0 == (char *) vec->direct);
	} else {
		assert(p0 != NULL);
	}
	memset(p0, '@', elem_size);

	for (k=1; k < n; k++) {
		p1 = (char *) bfc_vector_have(vec, k);
		L4SC_DEBUG(logger, "%s: vec[%u] @%p", __FUNCTION__, k, p1);
		if ((k != CV0_BOUNDARY(vec))
		 && (k != CV1_BOUNDARY(vec))
		 && (k < (CV1_BOUNDARY(vec) + CV1_ELEMENTS(vec)))) {
			assert(p1 == p0 + elem_size);
		} else {
			assert(p1 != NULL);
		}
		memset(p1, '@' + (k % 27), elem_size);
		assert(test_all_equal(p1, '@'+(k % 27), elem_size) == 0);
		p0 = p1;
	}

	bfc_object_dump(vec, 1, logger);

	for (k=0; k < n; k++) {
		p1 = (char *) bfc_container_index((bfc_contptr_t) vec, k);
		L4SC_DEBUG(logger, "%s: vec[%u] @%p %c%c%c...%c",
		    __FUNCTION__, k, p1, p1[0], p1[1], p1[2], p1[elem_size-1]);
		assert(p1 != NULL);
		assert(test_all_equal(p1, '@'+(k % 27), elem_size) == 0);
		assert(p1[elem_size] != '@' + (k % 27));
	}

	p1 = vec->zero_element;
	L4SC_DEBUG(logger, "%s: vec.zero_element @%p", __FUNCTION__, p1);
	assert(test_all_equal(p1, 0, elem_size) == 0);

	rc = bfc_init_vector_copy(&v2, sizeof(v2), pool, (bfc_ccontptr_t) vec);
	assert(rc >= 0);
	bfc_object_dump(&v2, 1, logger);

	for (k=0; k < n; k++) {
		p1 = (char *) bfc_container_index((bfc_contptr_t) &v2, k);
		L4SC_DEBUG(logger, "%s: v2[%u] @%p %c%c%c...%c",
		    __FUNCTION__, k, p1, p1[0], p1[1], p1[2], p1[elem_size-1]);
		assert(p1 != NULL);
		if ((k != 0)
		 && (k != CV0_BOUNDARY(&v2))
		 && (k != CV1_BOUNDARY(&v2))
		 && (k < (CV1_BOUNDARY(&v2) + CV1_ELEMENTS(&v2)))) {
			assert(p1 == p0 + elem_size);
		} else {
			assert(p1 != NULL);
		}
		assert(test_all_equal(p1, '@'+(k % 27), elem_size) == 0);
		p0 = p1;
	}

	p1 = v2.zero_element;
	L4SC_DEBUG(logger, "%s: v2.zero_element @%p", __FUNCTION__, p1);
	assert(test_all_equal(p1, 0, elem_size) == 0);
	if (elem_size > 1000) {
		k = CV1_BOUNDARY(&v2) - 1;
		p0 = (char *) bfc_container_index((bfc_contptr_t)&v2, k);
		L4SC_DEBUG(logger, "%s: v2[%u] @%p", __FUNCTION__, k, p0);
		assert(p1 == p0 + elem_size);
	}

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	int rc;
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);

	do {
		typedef char e1t[123];
		BFC_VECTOR(v1s, e1t, 3) v1;
		BFC_VECTOR_INIT_POOL(&v1, pool);
		rc = bfc_init_vector_class(&v1, sizeof(v1), pool);
		assert(rc >= 0);
		test(&v1, 123);
		bfc_destroy(&v1);
	} while (0 /*just once*/);

	do {
		typedef char e2t[12345];
		BFC_VECTOR(v2s, e2t, 3) v2;
		BFC_VECTOR_INIT_POOL(&v2, pool);
		rc = bfc_init_vector_class(&v2, sizeof(v2), pool);
		assert(rc >= 0);
		test(&v2, 12345);
		bfc_destroy(&v2);
	} while (0 /*just once*/);

	do {
		typedef struct e3s {
			char x[12345];
		} e3t;
		BFC_VECTOR(v3s, e3t, 2) v3;
		BFC_VECTOR_INIT_POOL(&v3, pool);
		rc = bfc_init_vector_class(&v3, sizeof(v3), pool);
		assert(rc >= 0);
		test(&v3, sizeof(e3t));
		bfc_destroy(&v3);
	} while (0 /*just once*/);

	do {
		typedef struct e4s {
			size_t n;
			char x[1001];
		} e4t;
		BFC_VECTOR(v4s, e4t, 2) v4;
		BFC_VECTOR_INIT_POOL(&v4, pool);
		rc = bfc_init_vector_class(&v4, sizeof(v4), pool);
		assert(rc >= 0);
		test(&v4, sizeof(e4t));
		bfc_destroy(&v4);
	} while (0 /*just once*/);

	return (0);
}
