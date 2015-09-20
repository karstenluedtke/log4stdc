
#include "barefootc/object.h"
#include "barefootc/datetime.h"

int
bfc_datetime_to_world_isodate(bfc_cobjptr_t date, int offs,
					char *buf, size_t bufsize)
{
	RETURN_METHCALL(bfc_datetime_classptr_t, date,
			to_world_isodate, (date, offs, buf, bufsize),
			-ENOSYS);
}

