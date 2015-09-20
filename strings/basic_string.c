
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

#define STRING_CLASS_NAME			"basic_string"

#define	wchar_t					char
#define bfc_objptr_t				bfc_objptr_t
#define bfc_cobjptr_t				bfc_cobjptr_t
#define bfc_basic_wstring			bfc_basic_string

#define bfc_wstring_class			bfc_string_class
#define bfc_basic_wstring_class			bfc_basic_string_class
#define bfc_wchar_traits_class			bfc_char_traits_class

#define	bfc_init_basic_wstring			bfc_init_basic_string
#define	bfc_destroy_basic_wstring		bfc_destroy_basic_string
#define	bfc_basic_wstring_objsize		bfc_basic_string_objsize
#define	bfc_init_basic_wstring_copy		bfc_init_basic_string_copy
#define	bfc_init_basic_wstring_move		bfc_init_basic_string_move
#define	bfc_init_basic_wstring_substr		bfc_init_basic_string_substr
#define	bfc_init_basic_wstring_buffer		bfc_init_basic_string_buffer
#define	bfc_init_basic_wstring_c_str		bfc_init_basic_string_c_str
#define	bfc_init_basic_wstring_fill		bfc_init_basic_string_fill
#define	bfc_init_basic_wstring_range		bfc_init_basic_string_range
#define	bfc_basic_wstring_length		bfc_basic_string_length
#define	bfc_basic_wstring_max_size		bfc_basic_string_max_size
#define	bfc_basic_wstring_resize		bfc_basic_string_resize
#define	bfc_basic_wstring_capacity		bfc_basic_string_capacity
#define	bfc_basic_wstring_reserve		bfc_basic_string_reserve

#define	bfc_wstrlen				bfc_strlen
#define	bfc_wstrbuf				bfc_strbuf
#define	bfc_wstrdata				bfc_strdata
#define	bfc_wstring_sublen			bfc_string_sublen
#define	bfc_wstring_length			bfc_string_length
#define	bfc_wstring_data			bfc_string_data
#define	bfc_wstr_reserve			bfc_str_reserve

#include "basic_wstring.c"

