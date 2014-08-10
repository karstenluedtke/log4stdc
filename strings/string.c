
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

#define STRING_CLASS_NAME			"string"

#define	wchar_t					char
#define bfc_wstrptr_t				bfc_strptr_t
#define bfc_cwstrptr_t				bfc_cstrptr_t

#define bfc_wstring_class			bfc_string_class
#define bfc_wchar_traits_class			bfc_char_traits_class

#define	bfc_init_wstring			bfc_init_string
#define	bfc_destroy_wstring			bfc_destroy_string
#define	bfc_wstring_objsize			bfc_string_objsize
#define	bfc_init_wstring_bfstr			bfc_init_string_bfstr
#define	bfc_init_wstring_move			bfc_init_string_move
#define	bfc_init_wstring_substr			bfc_init_string_substr
#define	bfc_init_wstring_buffer			bfc_init_string_buffer
#define	bfc_init_wstring_c_str			bfc_init_string_c_str
#define	bfc_init_wstring_fill			bfc_init_string_fill
#define	bfc_init_wstring_range			bfc_init_string_range
#define	bfc_wstring_length			bfc_string_length
#define	bfc_wstring_max_size			bfc_string_max_size
#define	bfc_wstring_resize			bfc_string_resize
#define	bfc_wstring_capacity			bfc_string_capacity
#define	bfc_wstring_reserve			bfc_string_reserve
#define	bfc_wstring_at				bfc_string_at
#define	bfc_wstring_ref				bfc_string_ref
#define	bfc_wstring_data			bfc_string_data
#define	bfc_wstring_index			bfc_string_index
#define	bfc_wstring_assign_bfstr		bfc_string_assign_bfstr
#define	bfc_wstring_assign_substr		bfc_string_assign_substr
#define	bfc_wstring_assign_c_str		bfc_string_assign_c_str
#define	bfc_wstring_assign_buffer		bfc_string_assign_buffer
#define	bfc_wstring_assign_fill			bfc_string_assign_fill
#define	bfc_wstring_assign_range		bfc_string_assign_range
#define	bfc_wstring_append_bfstr		bfc_string_append_bfstr
#define	bfc_wstring_append_substr		bfc_string_append_substr
#define	bfc_wstring_append_c_str		bfc_string_append_c_str
#define	bfc_wstring_append_buffer		bfc_string_append_buffer
#define	bfc_wstring_append_fill			bfc_string_append_fill
#define	bfc_wstring_append_range		bfc_string_append_range
#define	bfc_wstring_push_back			bfc_string_push_back
#define	bfc_wstring_insert_bfstr		bfc_string_insert_bfstr
#define	bfc_wstring_insert_substr		bfc_string_insert_substr
#define	bfc_wstring_insert_c_str		bfc_string_insert_c_str
#define	bfc_wstring_insert_buffer		bfc_string_insert_buffer
#define	bfc_wstring_insert_fill			bfc_string_insert_fill
#define	bfc_wstring_insert_fillit		bfc_string_insert_fillit
#define	bfc_wstring_insert_char			bfc_string_insert_char
#define	bfc_wstring_insert_range		bfc_string_insert_range
#define	bfc_wstring_erase_seq			bfc_string_erase_seq
#define	bfc_wstring_erase_tail			bfc_string_erase_tail
#define	bfc_wstring_erase_char			bfc_string_erase_char
#define	bfc_wstring_erase_range			bfc_string_erase_range
#define	bfc_wstring_pop_back			bfc_string_pop_back
#define	bfc_wstring_replace_bfstr		bfc_string_replace_bfstr
#define	bfc_wstring_replace_substr		bfc_string_replace_substr
#define	bfc_wstring_replace_buffer		bfc_string_replace_buffer
#define	bfc_wstring_replace_c_str		bfc_string_replace_c_str
#define	bfc_wstring_replace_fill		bfc_string_replace_fill
#define	bfc_wstring_replace_range_bfstr		bfc_string_replace_range_bfstr
#define	bfc_wstring_replace_range_buffer	bfc_string_replace_range_buffer
#define	bfc_wstring_replace_range_c_str		bfc_string_replace_range_c_str
#define	bfc_wstring_replace_range_fill		bfc_string_replace_range_fill
#define	bfc_wstring_replace_ranges		bfc_string_replace_ranges
#define	bfc_wstring_copy			bfc_string_copy
#define	bfc_wstring_swap			bfc_string_swap
#define	bfc_wstring_find_bfstr			bfc_string_find_bfstr
#define	bfc_wstring_find_buffer			bfc_string_find_buffer
#define	bfc_wstring_find_c_str			bfc_string_find_c_str
#define	bfc_wstring_find_char			bfc_string_find_char
#define	bfc_wstring_rfind_bfstr			bfc_string_rfind_bfstr
#define	bfc_wstring_rfind_buffer		bfc_string_rfind_buffer
#define	bfc_wstring_rfind_c_str			bfc_string_rfind_c_str
#define	bfc_wstring_rfind_char			bfc_string_rfind_char
#define	bfc_wstring_find_first_of_bfstr		bfc_string_find_first_of_bfstr
#define	bfc_wstring_find_first_of_buffer	bfc_string_find_first_of_buffer
#define	bfc_wstring_find_first_of_c_str		bfc_string_find_first_of_c_str
#define	bfc_wstring_find_first_of_char		bfc_string_find_first_of_char
#define	bfc_wstring_find_last_of_bfstr		bfc_string_find_last_of_bfstr
#define	bfc_wstring_find_last_of_buffer		bfc_string_find_last_of_buffer
#define	bfc_wstring_find_last_of_c_str		bfc_string_find_last_of_c_str
#define	bfc_wstring_find_last_of_char		bfc_string_find_last_of_char
#define	bfc_wstring_find_first_not_of_bfstr	bfc_string_find_first_not_of_bfstr
#define	bfc_wstring_find_first_not_of_buffer	bfc_string_find_first_not_of_buffer
#define	bfc_wstring_find_first_not_of_c_str	bfc_string_find_first_not_of_c_str
#define	bfc_wstring_find_first_not_of_char	bfc_string_find_first_not_of_char
#define	bfc_wstring_find_last_not_of_bfstr	bfc_string_find_last_not_of_bfstr
#define	bfc_wstring_find_last_not_of_buffer	bfc_string_find_last_not_of_buffer
#define	bfc_wstring_find_last_not_of_c_str	bfc_string_find_last_not_of_c_str
#define	bfc_wstring_find_last_not_of_char	bfc_string_find_last_not_of_char
#define	bfc_wstring_substr			bfc_string_substr
#define	bfc_wstring_compare_buffers		bfc_string_compare_buffers
#define	bfc_wstring_compare_bfstr		bfc_string_compare_bfstr
#define	bfc_wstring_compare_substr		bfc_string_compare_substr
#define	bfc_wstring_compare_substrs		bfc_string_compare_substrs
#define	bfc_wstring_compare_c_str		bfc_string_compare_c_str
#define	bfc_wstring_compare_substr_c_str	bfc_string_compare_substr_c_str
#define	bfc_wstring_compare_buffer		bfc_string_compare_buffer

#define	bfc_wstrlen				bfc_strlen
#define	bfc_wstrbuf				bfc_strbuf
#define	bfc_wstrdata				bfc_strdata
#define	bfc_wstring_sublen			bfc_string_sublen
#define	bfc_wstr_reserve			bfc_str_reserve

#include "wstring.c"

