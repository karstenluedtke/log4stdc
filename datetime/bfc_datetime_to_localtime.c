
#include "barefootc/object.h"
#include "barefootc/datetime.h"

struct tm;

int
bfc_datetime_to_localtime(bfc_cdateptr_t date, struct tm *tm)
{
	RETURN_METHCALL(bfc_datetime_classptr_t, date,
			to_localtime, (date, tm),
			-ENOSYS);
}

