
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
#include "barefootc/unconst.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

struct test_kv {
	size_t pos;
	const char *val;
};

static bfc_objptr_t create_test_object(const char *v, struct mempool *pool);

static int  init_test_object(void *buf, size_t bufsize, struct mempool *pool);
static void destroy_test_object(bfc_objptr_t obj);
static int  test_object_tostring(bfc_cobjptr_t obj,
				 char *buf, size_t bufsize, const char *fmt);

bfc_class_t test_object_class = {
	.super	  = &bfc_object_class,
	.name	  = "test object",
	.init	  = init_test_object,
	.destroy  = destroy_test_object,
	.tostring = test_object_tostring,
};

static bfc_objptr_t
create_test_object(const char *v, struct mempool *pool)
{
	bfc_objptr_t obj;
	int vlen = strlen(v);
	void *p;
	char *nm;

	bfc_new(&p, &test_object_class, pool);
	obj = (bfc_objptr_t) p;
	nm = bfc_mempool_alloc(pool, vlen+1);
	memcpy(nm, v, vlen);
	nm[vlen] = '\0';
	obj->name = nm;
	return (obj);
}

static int
init_test_object(void *buf, size_t bufsize, struct mempool *pool)
{
	bfc_objptr_t obj = (bfc_objptr_t) buf;
	if (bufsize < sizeof(*obj)) {
		return (-ENOSPC);
	}
	memset(obj, 0, sizeof(*obj));
	obj->vptr = &test_object_class;
	return (BFC_SUCCESS);
}

static void
destroy_test_object(bfc_objptr_t obj)
{
	bfc_classptr_t cls;

	if (obj && ((cls = BFC_CLASS(obj)) != NULL)) {
		if (obj->pool && obj->name) {
			char *nm = BFC_UNCONST(char *, obj->name);
			obj->name = NULL;
			bfc_mempool_free(obj->pool, nm);
		}
		BFC_DESTROY_EPILOGUE(obj, cls);
	}
}

static int
test_object_tostring(bfc_cobjptr_t obj,
		     char *buf, size_t bufsize, const char *fmt)
{
	int rc = 0;

	if (obj && BFC_CLASS(obj) && obj->name) {
		rc = snprintf(buf, bufsize, "%s", obj->name);
	}
	return (rc);
}

static int
test(int n1, struct test_kv kv[])
{
	int i, rc;
	bfc_objref_vector_t v1;
	BFC_VECTOR(v2s, bfc_objptr_t, 1) v2;
	const struct mempool_mark *poolmark = bfc_mempool_mark(pool);
	const size_t initial_poolsize = bfc_object_length(pool);
	char buf[200];

	L4SC_DEBUG(logger, "%s(n1 %d, ...)", __FUNCTION__, n1);

	BFC_VECTOR_INIT_POOL(&v1, pool);
	rc = bfc_init_objref_vector_class(&v1, sizeof(v1), pool);
	assert(rc >= 0);

	BFC_VECTOR_INIT_POOL(&v2, pool);
	rc = bfc_init_objref_vector_class(&v2, sizeof(v2), pool);
	assert(rc >= 0);

	for (i=0; i < n1; i++) {
		bfc_objptr_t obj = create_test_object(kv[i].val, pool);
		bfc_objptr_t ret;
		bfc_init_refcount(obj, 1);
		ret = bfc_container_place(&v1, kv[i].pos, obj, NULL);
		assert(ret == obj);
		assert(obj->refc == 2);
		assert(bfc_container_index(&v1, kv[i].pos) == obj);
		bfc_decr_refcount(obj);
		assert(obj->refc == 1);
	}
	bfc_object_dump(&v1, 2, logger);

	L4SC_DEBUG(logger, "%s: current pool size %ld",
			__FUNCTION__, (long) bfc_object_length(pool));

	// bfc_object_dump(pool, 1, logger);

	bfc_container_assign_copy((bfc_contptr_t) &v2, (bfc_ccontptr_t) &v1);
	bfc_object_dump(&v2, 2, logger);
	L4SC_DEBUG(logger, "%s: v2 size is %ld, expect %ld", __FUNCTION__,
		(long)bfc_object_length(&v2), (long)bfc_object_length(&v1));
	assert(bfc_object_length(&v2) == bfc_object_length(&v1));
	assert(bfc_equal_object(&v2, &v1));

	for (i=0; i < n1; i++) {
		bfc_cobjptr_t o1 = bfc_container_cindex(&v1, kv[i].pos);
		bfc_cobjptr_t o2 = bfc_container_cindex(&v2, kv[i].pos);
		assert(o1 != NULL);
		assert(o2 == o1);
		assert(o1->refc == 2);
		bfc_object_tostring(o1, buf, sizeof(buf), NULL);
		assert(strcmp(buf, kv[i].val) == 0);
	}

	L4SC_DEBUG(logger, "%s: current pool size %ld",
			__FUNCTION__, (long) bfc_object_length(pool));
	// bfc_mempool_dump_all(2, logger);

	bfc_destroy(&v1);

	for (i=0; i < n1; i++) {
		bfc_cobjptr_t obj = bfc_container_cindex(&v2, kv[i].pos);
		assert(obj != NULL);
		assert(obj->refc == 1);
		bfc_object_tostring(obj, buf, sizeof(buf), NULL);
		assert(strcmp(buf, kv[i].val) == 0);
	}

	bfc_destroy(&v2);

	L4SC_DEBUG(logger, "%s: final pool size %ld",
			__FUNCTION__, (long) bfc_object_length(pool));
	assert(bfc_object_length(pool) == initial_poolsize);

	bfc_mempool_reset(pool, poolmark);
	// bfc_object_dump(pool, 99, logger);

	L4SC_DEBUG(logger, "%s(n1 %d, ...)", "PASS", n1);

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
		struct test_kv kv[] = {
			{ 0, "zero" },
			{ 1, "one" },
			{ 2, "two" },
			{ 3, "three" },
			{ 4, "four" },
		};
		test(0, kv);
		test(1, kv);
		test(2, kv);
		test(3, kv);
		test(4, kv);
		test(5, kv);
		test(4, kv+1);
		test(3, kv+2);
		test(2, kv+3);
		test(1, kv+4);
	} while (0 /*just once*/);
	do {
		struct test_kv kv[] = {
			{ 0, "zero" },
			{ 1, "one" },
			{ 200, "twohundred" },
			{ 201, "twohundredone" },
			{ 40000, "fourtythousand" },
		};
		test(0, kv);
		test(1, kv);
		test(2, kv);
		test(3, kv);
		test(4, kv);
		test(5, kv);
		test(4, kv+1);
		test(3, kv+2);
		test(2, kv+3);
		test(1, kv+4);
	} while (0 /*just once*/);

	bfc_mempool_reset(pool, 0);
	// bfc_object_dump(pool, 99, logger);

	return (0);
}
