
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
	struct bfc_string str1;
	struct bfc_string wstr1;
	static const bfc_string_t lit1 = BFCSTR("abcdef");
	static const bfc_string_t wlit1 = BFCWSTR(L"abcdef");

	bfc_init_string_c_str(&str1, sizeof(str1), NULL, "abcdef");
	assert(str1.len == 6);
	assert(bfc_object_length(&str1) == 6);

	bfc_init_wstring_c_str(&wstr1, sizeof(wstr1), NULL, L"abcdef");
	assert(wstr1.len == 6);
	assert(bfc_object_length(&wstr1) == 6);

	assert(lit1.len == 6);
	assert(bfc_object_length(&lit1) == 6);
	assert(bfc_string_compare_bfstr(&str1, &lit1) == 0);

	assert(wlit1.len == 6);
	assert(bfc_object_length(&wlit1) == 6);
	assert(bfc_string_compare_bfstr(&wstr1, &wlit1) == 0);

	return(0);
}

