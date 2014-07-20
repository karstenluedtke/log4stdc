
#include "barefootc/string.h"
#include "barefootc/mempool.h"

int
barefootc::init_string(void *buf, size_t bufsize, const wchar_t *s, size_t n)
{
	return bfc_init_wstring_buffer(buf, bufsize, 0, s, n);
}

int
barefootc::init_string(void *buf, size_t bufsize,
	struct mempool *pool, const wchar_t *s, size_t n)
{
	return bfc_init_basic_wstring_buffer(buf, bufsize, pool, s, n);
}

int
barefootc::init_string(void *buf, size_t bufsize,
	struct mempool *pool, size_t n, wchar_t c)
{
	return bfc_init_basic_wstring_fill(buf, bufsize, pool, n, c);
}

