
#include "barefootc/object.h"
#include "barefootc/datetime.h"

struct tm;

int
bfc_datetime_to_worldtime(bfc_cdateptr_t date, int offs, struct tm *tm)
{
	RETURN_METHCALL(bfc_datetime_classptr_t, date,
			to_worldtime, (date, offs, tm),
			-ENOSYS);
}

