
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

#define STRING_CLASS_NAME			"shared_string"

#define	wchar_t					char
#define bfc_strptr_t				bfc_strptr_t
#define bfc_cstrptr_t				bfc_cstrptr_t
#define bfc_shared_wstring			bfc_shared_string

#define bfc_wstring_class			bfc_string_class
#define bfc_shared_wstring_class			bfc_shared_string_class
#define bfc_wchar_traits_class			bfc_char_traits_class

#define	bfc_init_shared_wstring			bfc_init_shared_string
#define	bfc_destroy_shared_wstring		bfc_destroy_shared_string
#define	bfc_shared_wstring_objsize		bfc_shared_string_objsize
#define	bfc_init_shared_wstring_copy		bfc_init_shared_string_copy
#define	bfc_init_shared_wstring_move		bfc_init_shared_string_move
#define	bfc_init_shared_wstring_substr		bfc_init_shared_string_substr
#define	bfc_init_shared_wstring_buffer		bfc_init_shared_string_buffer
#define	bfc_init_shared_wstring_c_str		bfc_init_shared_string_c_str
#define	bfc_init_shared_wstring_fill		bfc_init_shared_string_fill
#define	bfc_init_shared_wstring_range		bfc_init_shared_string_range
#define	bfc_shared_wstring_length		bfc_shared_string_length
#define	bfc_shared_wstring_max_size		bfc_shared_string_max_size
#define	bfc_shared_wstring_resize		bfc_shared_string_resize
#define	bfc_shared_wstring_capacity		bfc_shared_string_capacity
#define	bfc_shared_wstring_reserve		bfc_shared_string_reserve

#define	bfc_wstring_length			bfc_string_length
#define	bfc_basic_wstring_max_size		bfc_basic_string_max_size

#define	bfc_wstrlen				bfc_strlen
#define	bfc_wstrbuf				bfc_strbuf
#define	bfc_wstrdata				bfc_strdata
#define	bfc_wstring_sublen			bfc_string_sublen
#define	bfc_wstr_reserve			bfc_str_reserve

#include "shared_wstring.c"

