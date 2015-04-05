
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
	int i;
	bfc_string_map_t map;
	const size_t initial_poolsize = bfc_object_length(pool);

	L4SC_DEBUG(logger, "%s(n1 %d, kv %p)", __FUNCTION__, n1, kv);
	for (i=0; i < n1; i++) {
		L4SC_DEBUG(logger, "  %s: %s", kv[i].k, kv[i].v);
	}

	BFC_STRING_MAP_INIT(&map, n1, pool);

	bfc_object_dump(&map, 2, logger);

	bfc_destroy(&map);

	L4SC_DEBUG(logger, "%s: final pool size %ld",
			__FUNCTION__, (long) bfc_object_length(pool));
	assert(bfc_object_length(pool) == initial_poolsize);

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
		static struct test_kv kv[] = { };
		test(0, kv);
	} while (0 /*just once*/);

	do {
		static struct test_kv kv[] = {
			{ "Kaaa", "Vaaa" },
		};
		test(1, kv);
	} while (0 /*just once*/);

	return (0);
}
