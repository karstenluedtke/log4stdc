
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "log4stdc.h"
#include "barefootc/string.h"
#include "barefootc/map.h"
#include "barefootc/container.h"
#include "barefootc/mempool.h"
#include "barefootc/utf8.h"
#include "barefootc/unconst.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

struct test_kv {
	const char *k, *v;
};

static bfc_objptr_t create_test_object(const char *v);

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
create_test_object(const char *v)
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
		if (obj->parent_pool && obj->name) {
			char *nm = BFC_UNCONST(char *, obj->name);
			obj->name = NULL;
			bfc_mempool_free(obj->parent_pool, nm);
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
test(int n1, const struct test_kv init[],
     int n2, const struct test_kv repl[],
     int n3, const struct test_kv expect[])
{
	int i, rc;
	bfc_string_objref_map_t map;
	const size_t initial_poolsize = bfc_object_length(pool);

	L4SC_DEBUG(logger, "%s(init[%d] %p, repl[%d] %p, expect[%d] %p)",
		__FUNCTION__, n1, init, n2, repl, n3, expect);

	BFC_STRING_OBJREF_MAP_INIT(&map, n1, pool);

	for (i=0; i < n1; i++) {
		bfc_string_t kstr;
		bfc_objptr_t vobj;
		L4SC_DEBUG(logger, "%s: init %s = test_object(%s)",
			__FUNCTION__, init[i].k, init[i].v);
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), init[i].k);
		vobj = create_test_object(init[i].v);
		bfc_init_refcount(vobj, 1);
		assert(BFC_CLASS(vobj) == &test_object_class);
		assert(strcmp(vobj->name, init[i].v) == 0);
		rc = bfc_map_insert_objects((bfc_contptr_t) &map,
					    (bfc_objptr_t) &kstr, vobj,
					    NULL, 0);
		assert(rc >= 0);
		bfc_decr_refcount(vobj); /* vobj now ref'd by map only */
		vobj = NULL;
	}
	assert(bfc_map_size((bfc_ccontptr_t)&map) == n1);

	for (i=0; i < n2; i++) {
		bfc_string_t kstr;
		bfc_objptr_t vobj;
		L4SC_DEBUG(logger, "%s: replace %s = test_object(%s)",
			__FUNCTION__, repl[i].k, repl[i].v);
		vobj = create_test_object(repl[i].v);
		bfc_init_refcount(vobj, 1);
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), repl[i].k);
		rc = bfc_map_replace_objects((bfc_contptr_t) &map,
					     (bfc_objptr_t) &kstr, vobj,
					     NULL, 0);
		assert(rc >= 0);
		bfc_decr_refcount(vobj); /* vobj now ref'd by map only */
		vobj = NULL;
	}

	bfc_object_dump(&map, 99, logger);

	for (i=0; i < n3; i++) {
		bfc_string_t kstr;
		bfc_objptr_t vp;
		char vbuf[80];
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), expect[i].k);
		vp = bfc_map_find_value((bfc_contptr_t) &map,
					(bfc_objptr_t) &kstr);
		assert(vp != NULL);
		bfc_object_tostring(vp, vbuf, sizeof(vbuf), NULL);
		L4SC_DEBUG(logger, "%s: retrieved %s = test_object(%s)",
				__FUNCTION__, expect[i].k, vbuf);
		assert(strcmp(vbuf, expect[i].v) == 0);
	}
	assert(bfc_map_size((bfc_ccontptr_t)&map) == n3);

	bfc_destroy(&map);

	L4SC_DEBUG(logger, "%s: final pool size %ld",
			__FUNCTION__, (long) bfc_object_length(pool));
	assert(bfc_object_length(pool) == initial_poolsize);

	L4SC_DEBUG(logger, "%s(init[%d] %p, repl[%d] %p, expect[%d] %p)",
		"PASS", n1, init, n2, repl, n3, expect);

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
		static struct test_kv init[] = {
			{ "Kaaa", "Vaaa" },
			{ "Kbbb", "Vbbb" },
			{ "Kccc", "Vccc" },
			{ "Kddd", "Vddd" },
			{ "Keee", "Veee" },
			{ "Kfff", "Vfff" },
		};
		static struct test_kv repl[] = {
			{ "Kbbb", "VBBB" },
		};
		static struct test_kv expect[] = {
			{ "Kaaa", "Vaaa" },
			{ "Kbbb", "VBBB" },
			{ "Kccc", "Vccc" },
			{ "Kddd", "Vddd" },
			{ "Keee", "Veee" },
			{ "Kfff", "Vfff" },
		};
		test(sizeof(init)/sizeof(init[0]), init,
		     sizeof(repl)/sizeof(repl[0]), repl,
		     sizeof(expect)/sizeof(expect[0]), expect);
	} while (0 /*just once*/);

	do {
		static struct test_kv init[] = {
			{ "Kaaa", "Vaaa" },
			{ "Kbbb", "Vbbb" },
			{ "Kccc", "Vccc" },
			{ "Kddd", "Vddd" },
			{ "Keee", "Veee" },
			{ "Kfff", "Vfff" },
		};
		static struct test_kv repl[] = {
			{ "Kxxx", "Vxxx" },
			{ "Kyyy", "Vyyy" },
			{ "Kzzz", "Vzzz" },
			{ "Kbbb", "VBBB" },
		};
		static struct test_kv expect[] = {
			{ "Kaaa", "Vaaa" },
			{ "Kbbb", "VBBB" },
			{ "Kccc", "Vccc" },
			{ "Kddd", "Vddd" },
			{ "Keee", "Veee" },
			{ "Kfff", "Vfff" },
			{ "Kxxx", "Vxxx" },
			{ "Kyyy", "Vyyy" },
			{ "Kzzz", "Vzzz" },
		};
		test(sizeof(init)/sizeof(init[0]), init,
		     sizeof(repl)/sizeof(repl[0]), repl,
		     sizeof(expect)/sizeof(expect[0]), expect);
	} while (0 /*just once*/);

	return (0);
}
