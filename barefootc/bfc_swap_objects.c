
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <malloc.h>  /* for alloca */
#endif

#include "barefootc/object.h"

int
bfc_swap_objects(void *obj1, void *obj2)
{
	size_t size = bfc_object_size(obj1);

	if (size == bfc_object_size(obj2)) {
		void *temp = alloca(size);
		memcpy(temp, obj1, size);
		memcpy(obj1, obj2, size);
		memcpy(obj2, temp, size);
		return (BFC_SUCCESS);
	}

	return (-EINVAL);
}

