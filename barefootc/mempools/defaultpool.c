
#include <stddef.h>
#include "barefootc/mempool.h"

struct stdc_mempool;
extern struct stdc_mempool bfc_stdc_mempool;

static struct mempool *
bfc_default_mempool = (struct mempool *) &bfc_stdc_mempool;

struct mempool *
bfc_get_default_mempool(void)
{
	return (bfc_default_mempool);
}

void
bfc_set_default_mempool(struct mempool *pool)
{
	bfc_default_mempool = pool;
}

