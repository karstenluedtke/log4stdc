
#include "barefootc/string.h"
#include "barefootc/mempool.h"

int
barefootc::init_string(void *buf, size_t bufsize,
	struct mempool *pool, wchar_t *s, size_t n)
{
	return bfc_init_wstring_buffer(buf, bufsize, pool, s, n);
}

