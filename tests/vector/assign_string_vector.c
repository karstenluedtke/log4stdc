
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

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

static int
test(int n1, const size_t s1[])
{
	int i, rc;
	BFC_VECTOR(v1s, bfc_string_t, 0) v1;
	BFC_VECTOR(v2s, bfc_string_t, 1) v2;
	const size_t expected_size = (n1 > 0)? s1[n1-1]+1: 0;
	const size_t initial_poolsize = bfc_object_length(pool);
	char buf[200];

	L4SC_DEBUG(logger, "%s(n1 %d, s1 %ld %ld %ld ...)",
		__FUNCTION__, n1, (long)s1[0], (long)s1[1], (long)s1[2]);

	BFC_VECTOR_INIT_POOL(&v1, pool);
	rc = bfc_init_object_vector_class(&v1, sizeof(v1), pool);
	assert(rc >= 0);

	BFC_VECTOR_INIT_POOL(&v2, pool);
	rc = bfc_init_object_vector_class(&v2, sizeof(v2), pool);
	assert(rc >= 0);

	for (i=0; i < n1; i++) {
		bfc_strptr_t s = (bfc_strptr_t) bfc_vector_have(&v1, s1[i]);
		assert(s != NULL);
		snprintf(buf, sizeof(buf), "element #%d @%ld", i, (long)s1[i]);
		rc = bfc_init_basic_string_c_str(s, v1.elem_size, pool, buf);
		assert(rc >= 0);
	}
	bfc_object_dump(&v1, 2, logger);
	L4SC_DEBUG(logger, "%s: v1 size is %ld, expect %ld", __FUNCTION__,
		(long)bfc_object_length(&v1), (long)expected_size);
	assert(bfc_object_length(&v1) == expected_size);

	bfc_container_assign_copy((bfc_contptr_t) &v2, (bfc_ccontptr_t) &v1);
	bfc_object_dump(&v2, 2, logger);
	L4SC_DEBUG(logger, "%s: v2 size is %ld, expect %ld", __FUNCTION__,
		(long)bfc_object_length(&v2), (long)bfc_object_length(&v1));
	assert(bfc_object_length(&v2) == bfc_object_length(&v1));
	assert(bfc_equal_object(&v2, &v1));

	bfc_destroy(&v1);

	for (i=0; i < n1; i++) {
		bfc_cstrptr_t s = (bfc_cstrptr_t) bfc_vector_ref(&v2, s1[i]);
		assert(s != NULL);
		snprintf(buf, sizeof(buf), "element #%d @%ld", i, (long)s1[i]);
		L4SC_DEBUG(logger, "%s: expecting \"%s\"", __FUNCTION__, buf);
		bfc_object_dump(s, 1, logger);
		assert(bfc_string_compare_c_str(s, buf) == 0);
	}

	bfc_destroy(&v2);

	assert(bfc_object_length(pool) == initial_poolsize);

	L4SC_DEBUG(logger, "%s(n1 %d, s1 %ld %ld %ld ...)",
		"PASS", n1, (long)s1[0], (long)s1[1], (long)s1[2]);

	return (BFC_SUCCESS);
}

int
main(int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
	pool = bfc_get_stdc_mempool(__FILE__, __LINE__, __FUNCTION__);
	pool = bfc_new_sorting_mempool(pool, __FILE__, __LINE__, __FUNCTION__);

	do {
		size_t s1[] = { 0, 1, 2, 3 };
		test(0, s1);
		test(1, s1);
		test(2, s1);
		test(3, s1);
		test(4, s1);
	} while (0 /*just once*/);

	do {
		size_t s1[] = { 0, 10, 100, 1000, 10000, 100000, 1000000 };
		test(1, s1);
		test(3, s1);
		test(7, s1);
	} while (0 /*just once*/);

	do {
		size_t s1[] = { 10, 11, 1000, 1001, 1000000, 1000001 };
		test(6, s1);
	} while (0 /*just once*/);

	return (0);
}