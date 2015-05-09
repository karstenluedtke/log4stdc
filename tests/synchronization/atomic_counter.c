
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "barefootc/atomic.h"
#include "log4stdc.h"

static l4sc_logger_ptr_t logger;

static void 
t1(int n)
{
	int i, rc;
	bfc_atomic_counter_t ctr;

	bfc_init_atomic_counter(ctr, 0);
	assert (ctr == 0);

	for (i=0; i < n; i++) {
		rc = bfc_incr_atomic_counter(ctr);
		assert (rc == i+1);
	}

	assert (ctr == n);

	for (i=0; i < n-1; i++) {
		rc = bfc_decr_atomic_counter(ctr);
		assert (rc > 0);
		assert (rc == ctr);
		assert (rc == n-i-1);
	}

	if (n > 0) {
		rc = bfc_decr_atomic_counter(ctr);
		assert (rc == 0);
	}

	assert (ctr == 0);
}

int
main (int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");
	logger = l4sc_get_logger("barefootc", 0);

	t1(0);
	t1(1);
	t1(2);
	t1(15);
	t1(65000);

	return (0);
}
