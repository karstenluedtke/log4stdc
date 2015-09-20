
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

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

static struct mempool *pool;
static l4sc_logger_ptr_t logger;

struct test_kv {
	const char *k, *v;
};

static int
test(int n1, const struct test_kv kv[])
{
	int i, rc;
	bfc_string_map_t map;
	const size_t initial_poolsize = bfc_object_length((bfc_cobjptr_t)pool);

	L4SC_DEBUG(logger, "%s(n1 %d, kv %p)", __FUNCTION__, n1, kv);

	BFC_STRING_MAP_INIT(&map, n1, pool);

	for (i=0; i < n1; i++) {
		bfc_string_t kstr, vstr;
		L4SC_DEBUG(logger, "%s: %s = %s",__FUNCTION__,kv[i].k,kv[i].v);
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), kv[i].k);
		bfc_init_shared_string_c_str(&vstr, sizeof(vstr), kv[i].v);
		rc = bfc_map_insert_objects((bfc_contptr_t) &map,
					    (bfc_objptr_t) &kstr,
					    (bfc_objptr_t) &vstr,
					    NULL, 0);
		assert(rc >= 0);
	}

	bfc_object_dump((bfc_cobjptr_t)&map, 99, logger);
	assert(bfc_map_size((bfc_ccontptr_t)&map) == n1);

	for (i=0; i < n1; i++) {
		bfc_string_t kstr;
		bfc_objptr_t vp;
		char vbuf[80];
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), kv[i].k);
		vp = bfc_map_find_value((bfc_contptr_t) &map,
					(bfc_objptr_t) &kstr);
		assert(vp != NULL);
		bfc_object_tostring(vp, vbuf, sizeof(vbuf), NULL);
		L4SC_DEBUG(logger, "%s: %s = %s",__FUNCTION__, kv[i].k, vbuf);
		assert(bfc_string_compare_c_str((bfc_cstrptr_t)vp,kv[i].v)==0);
	}

	bfc_destroy((bfc_objptr_t) &map);

	L4SC_DEBUG(logger, "%s: final pool size %ld",
		__FUNCTION__, (long) bfc_object_length((bfc_cobjptr_t)pool));
	assert(bfc_object_length((bfc_cobjptr_t)pool) == initial_poolsize);

	L4SC_DEBUG(logger, "%s(n1 %d, kv %p)", "PASS", n1, kv);

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
		static struct test_kv kv[] = {
			{ NULL, NULL },
		};
		test(0, kv);
	} while (0 /*just once*/);

	do {
		static struct test_kv kv[] = {
			{ "Kaaa", "Vaaa" },
		};
		test(1, kv);
	} while (0 /*just once*/);

	do {
		static struct test_kv kv[] = {
			{ "Kaaa", "Vaaa" },
			{ "Kbbb", "Vbbb" },
			{ "Kccc", "Vccc" },
			{ "Kddd", "Vddd" },
			{ "Keee", "Veee" },
			{ "Kfff", "Vfff" },
		};
		test(sizeof(kv)/sizeof(kv[0]), kv);
	} while (0 /*just once*/);

	return (0);
}
