
#include <string.h>

#include "barefootc/object.h"

int
bfc_instance_of_class(const void *obj, bfc_classptr_t refclass)
{
	bfc_classptr_t cls;

	if (obj && ((cls = BFC_CLASS((bfc_cobjptr_t) obj)) != NULL)) {
		do {
			if (cls == refclass) {
				return (1);
			}
		} while ((cls = cls->super) != NULL);
	}
	return (0);
}

int
bfc_instance_of_classname(const void *obj, const char *classname)
{
	return (bfc_baseclass_by_name(obj, classname) != NULL);
}

bfc_classptr_t
bfc_baseclass_by_name(const void *obj, const char *classname)
{
	bfc_classptr_t cls;

	if (obj && ((cls = BFC_CLASS((bfc_cobjptr_t) obj)) != NULL)) {
		do {
			if (cls->name && (strcmp(cls->name, classname) == 0)) {
				return (cls);
			}
		} while ((cls = cls->super) != NULL);
	}
	return (NULL);
}

