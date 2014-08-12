
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <inttypes.h>

#include "barefootc/object.h"
#include "barefootc/iterator.h"

#define FORWARD_CLASS_NAME			"c-string forward iterator"
#define REVERSE_CLASS_NAME			"c-string reverse iterator"

#define bfc_wstr_forward_iterator_class		bfc_cstr_forward_iterator_class
#define bfc_wstr_reverse_iterator_class		bfc_cstr_reverse_iterator_class

#define bfc_init_wstr_iterator			bfc_init_cstr_iterator
#define bfc_init_wstr_reverse_iterator		bfc_init_cstr_reverse_iterator

#define	wchar_t					char
#define	wcslen					strlen

#include "wstr_iterator.c"

static void
dump_iterator(bfc_citerptr_t it, int depth, struct l4sc_logger *log)
{
	if (it && BFC_CLASS(it)) {
		L4SC_DEBUG(log, "%s @%p", BFC_CLASS(it)->name, it);
		L4SC_DEBUG(log, "pos %ld in %p: %s",
				(long) it->pos, it->obj, (wchar_t *) it->obj);
	}
}
