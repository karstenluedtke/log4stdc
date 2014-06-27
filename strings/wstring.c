
#include <stddef.h>

#include "barefootc/object.h"
#include "barefootc/string.h"

struct bfc_wchar_string_class {
	BFC_STRING_CLASS_DEF(const struct bfc_wchar_string_class *,
				bfc_strptr_t, bfc_cstrptr_t, wchar_t)
};

struct bfc_wchar_string_class bfc_wstring_class = {
	.super = NULL,
	.name = "wstring",
};

