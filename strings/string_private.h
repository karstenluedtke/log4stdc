#define STRING_CLASS(s)		((bfc_string_classptr_t) BFC_CLASS(s))
#define STRING_TRAITS(s)	(STRING_CLASS(s)->traits)
#define STRING_LEN(s)		(s)->length
#define STRING_OFFSET(s)	(s)->private_6
#define STRING_BUFSIZE(s)	(s)->private_7
#define GET_STRBUF(s)		BFC_UNCONST(wchar_t*,(s)->name)
#define SET_STRBUF(s,buf)	(s)->name = (const char *)(buf)

#define bfc_strdata(s)	((STRING_OFFSET(s) == 0)? (s)->name: bfc_string_data(s))
