
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
	bfc_string_t str1;
	bfc_string_t wstr1;
	static const bfc_string_t lit1 = BFCSTR("abcdef");
	static const bfc_string_t wlit1 = BFCWSTR(L"abcdef");

	bfc_init_string_c_str(&str1, sizeof(str1), NULL, "abcdef");
	assert(bfc_strlen(&str1) == 6);
	assert(bfc_object_length(&str1) == 6);

	bfc_init_wstring_c_str(&wstr1, sizeof(wstr1), NULL, L"abcdef");
	assert(bfc_strlen(&wstr1) == 6);
	assert(bfc_object_length(&wstr1) == 6);

	assert(bfc_strlen(&lit1) == 6);
	assert(bfc_object_length(&lit1) == 6);
	assert(bfc_string_compare_bfstr(&str1, &lit1) == 0);

	assert(bfc_strlen(&wlit1) == 6);
	assert(bfc_object_length(&wlit1) == 6);
	assert(bfc_string_compare_bfstr(&wstr1, &wlit1) == 0);

	return(0);
}

