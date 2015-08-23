
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

#define STRING_CLASS_NAME			"string"

#define INIT_FORWARD_ITERATOR			bfc_init_utf8_iterator
#define INIT_REVERSE_ITERATOR			bfc_init_utf8_reverse_iterator

#define	wchar_t					char
#define bfc_wstring_class			bfc_string_class
#define bfc_wchar_traits_class			bfc_char_traits_class

#define	bfc_init_wstring			bfc_init_string
#define	bfc_destroy_wstring			bfc_destroy_string
#define	bfc_wstring_objsize			bfc_string_objsize
#define	bfc_init_wstring_copy			bfc_init_string_copy
#define	bfc_init_wstring_move			bfc_init_string_move
#define	bfc_init_wstring_substr			bfc_init_string_substr
#define	bfc_init_wstring_buffer			bfc_init_string_buffer
#define	bfc_init_wstring_c_str			bfc_init_string_c_str
#define	bfc_init_wstring_fill			bfc_init_string_fill
#define	bfc_init_wstring_range			bfc_init_string_range
#define clone_wstring				clone_string
#define wstring_hashcode			cstring_hashcode
#define wstring_equals				cstring_equals
#define wstring_tostring			cstring_tostring
#define dump_wstring				dump_cstring
#define	bfc_wstring_length			bfc_cstring_length
#define	bfc_wstring_max_size			bfc_cstring_max_size
#define	bfc_wstring_charsize			bfc_cstring_charsize
#define	bfc_wstring_resize			bfc_cstring_resize
#define	bfc_wstring_capacity			bfc_cstring_capacity
#define	bfc_wstring_reserve			bfc_cstring_reserve
#define	bfc_wstring_at				bfc_cstring_at
#define	bfc_wstring_ref				bfc_cstring_ref
#define	bfc_wstring_data			bfc_cstring_data
#define	bfc_wstring_index			bfc_cstring_index
#define	bfc_wstring_getlong			bfc_cstring_getlong
#define	bfc_wstring_setlong			bfc_cstring_setlong
#define	bfc_wstring_assign_copy			bfc_cstring_assign_copy
#define	bfc_wstring_assign_substr		bfc_cstring_assign_substr
#define	bfc_wstring_assign_c_str		bfc_cstring_assign_c_str
#define	bfc_wstring_assign_buffer		bfc_cstring_assign_buffer
#define	bfc_wstring_assign_fill			bfc_cstring_assign_fill
#define	bfc_wstring_assign_range		bfc_cstring_assign_range
#define	bfc_wstring_append_copy			bfc_cstring_append_copy
#define	bfc_wstring_append_substr		bfc_cstring_append_substr
#define	bfc_wstring_append_c_str		bfc_cstring_append_c_str
#define	bfc_wstring_append_buffer		bfc_cstring_append_buffer
#define	bfc_wstring_append_fill			bfc_cstring_append_fill
#define	bfc_wstring_append_range		bfc_cstring_append_range
#define	bfc_wstring_push_back			bfc_cstring_push_back
#define	bfc_wstring_insert_copy			bfc_cstring_insert_copy
#define	bfc_wstring_insert_substr		bfc_cstring_insert_substr
#define	bfc_wstring_insert_c_str		bfc_cstring_insert_c_str
#define	bfc_wstring_insert_buffer		bfc_cstring_insert_buffer
#define	bfc_wstring_insert_fill			bfc_cstring_insert_fill
#define	bfc_wstring_insert_fillit		bfc_cstring_insert_fillit
#define	bfc_wstring_insert_char			bfc_cstring_insert_char
#define	bfc_wstring_insert_range		bfc_cstring_insert_range
#define	bfc_wstring_erase_seq			bfc_cstring_erase_seq
#define	bfc_wstring_erase_tail			bfc_cstring_erase_tail
#define	bfc_wstring_erase_char			bfc_cstring_erase_char
#define	bfc_wstring_erase_range			bfc_cstring_erase_range
#define	bfc_wstring_pop_back			bfc_cstring_pop_back
#define	bfc_wstring_replace_copy		bfc_cstring_replace_copy
#define	bfc_wstring_replace_substr		bfc_cstring_replace_substr
#define	bfc_wstring_replace_buffer		bfc_cstring_replace_buffer
#define	bfc_wstring_replace_c_str		bfc_cstring_replace_c_str
#define	bfc_wstring_replace_fill		bfc_cstring_replace_fill
#define	bfc_wstring_replace_range_copy		bfc_cstring_replace_range_copy
#define	bfc_wstring_replace_range_buffer	bfc_cstring_replace_range_buffer
#define	bfc_wstring_replace_range_c_str		bfc_cstring_replace_range_c_str
#define	bfc_wstring_replace_range_fill		bfc_cstring_replace_range_fill
#define	bfc_wstring_replace_ranges		bfc_cstring_replace_ranges
#define	bfc_wstring_copy			bfc_cstring_copy
#define	bfc_wstring_swap			bfc_cstring_swap
#define	bfc_wstring_find_bfstr			bfc_cstring_find_bfstr
#define	bfc_wstring_find_buffer			bfc_cstring_find_buffer
#define	bfc_wstring_find_c_str			bfc_cstring_find_c_str
#define	bfc_wstring_find_char			bfc_cstring_find_char
#define	bfc_wstring_rfind_bfstr			bfc_cstring_rfind_bfstr
#define	bfc_wstring_rfind_buffer		bfc_cstring_rfind_buffer
#define	bfc_wstring_rfind_c_str			bfc_cstring_rfind_c_str
#define	bfc_wstring_rfind_char			bfc_cstring_rfind_char
#define	bfc_wstring_find_first_of_bfstr		bfc_cstring_find_first_of_bfstr
#define	bfc_wstring_find_first_of_buffer	bfc_cstring_find_first_of_buffer
#define	bfc_wstring_find_first_of_c_str		bfc_cstring_find_first_of_c_str
#define	bfc_wstring_find_first_of_char		bfc_cstring_find_first_of_char
#define	bfc_wstring_find_last_of_bfstr		bfc_cstring_find_last_of_bfstr
#define	bfc_wstring_find_last_of_buffer		bfc_cstring_find_last_of_buffer
#define	bfc_wstring_find_last_of_c_str		bfc_cstring_find_last_of_c_str
#define	bfc_wstring_find_last_of_char		bfc_cstring_find_last_of_char
#define	bfc_wstring_find_first_not_of_bfstr	bfc_cstring_find_first_not_of_bfstr
#define	bfc_wstring_find_first_not_of_buffer	bfc_cstring_find_first_not_of_buffer
#define	bfc_wstring_find_first_not_of_c_str	bfc_cstring_find_first_not_of_c_str
#define	bfc_wstring_find_first_not_of_char	bfc_cstring_find_first_not_of_char
#define	bfc_wstring_find_last_not_of_bfstr	bfc_cstring_find_last_not_of_bfstr
#define	bfc_wstring_find_last_not_of_buffer	bfc_cstring_find_last_not_of_buffer
#define	bfc_wstring_find_last_not_of_c_str	bfc_cstring_find_last_not_of_c_str
#define	bfc_wstring_find_last_not_of_char	bfc_cstring_find_last_not_of_char
#define	bfc_wstring_shared_substr		bfc_cstring_shared_substr
#define	bfc_wstring_mutable_substr		bfc_cstring_mutable_substr
#define	bfc_wstring_buffered_substr		bfc_cstring_buffered_substr
#define	bfc_wstring_compare_buffers		bfc_cstring_compare_buffers
#define	bfc_wstring_compare_bfstr		bfc_cstring_compare_bfstr
#define	bfc_wstring_compare_substr		bfc_cstring_compare_substr
#define	bfc_wstring_compare_substrs		bfc_cstring_compare_substrs
#define	bfc_wstring_compare_c_str		bfc_cstring_compare_c_str
#define	bfc_wstring_compare_substr_c_str	bfc_cstring_compare_substr_c_str
#define	bfc_wstring_compare_buffer		bfc_cstring_compare_buffer

#define	bfc_wstr_reserve			bfc_cstring_reserve

#define	bfc_wstrlen				bfc_strlen
#define	bfc_wstrbuf				bfc_strbuf
#define	bfc_wstrdata				bfc_strdata
#define	bfc_wstring_sublen			bfc_string_sublen
#define	bfc_wstring_subdata			bfc_string_subdata

#define	bfc_init_shared_wstring_buffer		bfc_init_shared_string_buffer

#include "wstring.c"

static int
cstring_tostring(bfc_cstrptr_t s, char *buf, size_t bufsize, const char *fmt)
{
	size_t len;

	if (buf && (bufsize > 0)) {
		len = bfc_string_copy(s, buf, bufsize, 0);
		if (len < bufsize) {
			buf[len] = '\0';
		} else {
			buf[bufsize-1] = '\0';
		}
	} else {
		len = bfc_strlen(s);
	}
	return ((int) len);
}

static void
dump_cstring(bfc_cstrptr_t s, int depth, struct l4sc_logger *log)
{
	if (s && BFC_CLASS(s)) {
		const size_t len = bfc_strlen(s);
		const char *data = bfc_strdata(s);
		if (len < 200) {
			L4SC_DEBUG(log, "%s @%p, len %ld: \"%s\"",
				BFC_CLASS(s)->name, s, (long) len, data);
		} else {
			L4SC_DEBUG(log, "%s @%p, len %ld @%p",
				BFC_CLASS(s)->name, s, (long) len, data);
		}
	}
}
