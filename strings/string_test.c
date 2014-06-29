
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#include <assert.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

int main (int argc, char *argv[])
{
	struct bfc_wstring wstr1;

	bfc_init_wstring_c_str(&wstr1, sizeof(wstr1), NULL, L"abcdef");
	assert(wstr1.len == 6);
	assert(bfc_object_length(&wstr1) == 6);

	return(0);
}

