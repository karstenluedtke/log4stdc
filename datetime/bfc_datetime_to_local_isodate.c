
#include "barefootc/object.h"
#include "barefootc/datetime.h"

int
bfc_datetime_to_local_isodate(bfc_cobjptr_t date, char *buf, size_t bufsize)
{
	RETURN_METHCALL(bfc_datetime_classptr_t, date,
			to_local_isodate, (date, buf, bufsize),
			-ENOSYS);
}

