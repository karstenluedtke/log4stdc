
#include "barefootc/string.h"
#include "barefootc/mempool.h"

int
barefootc::init_string(void *buf, size_t bufsize, const char *s, size_t n)
{
	return bfc_init_string_buffer(buf, bufsize, 0, s, n);
}

int
barefootc::init_string(void *buf, size_t bufsize,
	struct mempool *pool, const char *s, size_t n)
{
	return bfc_init_basic_string_buffer(buf, bufsize, pool, s, n);
}

