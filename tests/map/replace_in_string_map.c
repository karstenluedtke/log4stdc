
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
test(int n1, const struct test_kv init[],
     int n2, const struct test_kv repl[],
     int n3, const struct test_kv expect[])
{
	int i, rc;
	bfc_string_map_t map;
	const size_t initial_poolsize = bfc_object_length(pool);

	L4SC_DEBUG(logger, "%s(init[%d] %p, repl[%d] %p, expect[%d] %p)",
		__FUNCTION__, n1, init, n2, repl, n3, expect);

	BFC_STRING_MAP_INIT(&map, n1, pool);

	for (i=0; i < n1; i++) {
		bfc_string_t kstr, vstr;
		L4SC_DEBUG(logger, "%s: init %s = %s",
			__FUNCTION__, init[i].k, init[i].v);
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), init[i].k);
		bfc_init_shared_string_c_str(&vstr, sizeof(vstr), init[i].v);
		rc = bfc_map_insert_objects((bfc_contptr_t) &map,
					    (bfc_objptr_t) &kstr,
					    (bfc_objptr_t) &vstr,
					    NULL, 0);
		assert(rc >= 0);
	}
	assert(bfc_map_size((bfc_ccontptr_t)&map) == n1);

	for (i=0; i < n2; i++) {
		bfc_string_t kstr, vstr;
		L4SC_DEBUG(logger, "%s: replace %s = %s",
			__FUNCTION__, repl[i].k, repl[i].v);
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), repl[i].k);
		bfc_init_shared_string_c_str(&vstr, sizeof(vstr), repl[i].v);
		rc = bfc_map_replace_objects((bfc_contptr_t) &map,
					     (bfc_objptr_t) &kstr,
					     (bfc_objptr_t) &vstr,
					     NULL, 0);
		assert(rc >= 0);
	}

	bfc_object_dump(&map, 99, logger);

	for (i=0; i < n3; i++) {
		bfc_string_t kstr;
		bfc_objptr_t vp;
		bfc_cstrptr_t value;
		char vbuf[80];
		bfc_init_shared_string_c_str(&kstr, sizeof(kstr), expect[i].k);
		vp = bfc_map_find_value((bfc_contptr_t) &map,
					(bfc_objptr_t) &kstr);
		assert(vp != NULL);
		value = (bfc_cstrptr_t) vp;
		bfc_object_tostring(vp, vbuf, sizeof(vbuf), NULL);
		L4SC_DEBUG(logger, "%s: retrieved %s = %s",
				__FUNCTION__, expect[i].k, vbuf);
		assert(bfc_string_compare_c_str(value, expect[i].v) == 0);
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
