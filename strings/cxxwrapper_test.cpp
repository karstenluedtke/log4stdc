
#include <stddef.h>
#include <wchar.h>

#include <assert.h>

#include "cxxwrapper.h"

int main (int argc, char *argv[])
{
	barefootc::basic_string<char> str1("ABCD");
	barefootc::basic_string<char> str2 = str1 + "1234";
	assert (str2 == barefootc::basic_string<char>("ABCD1234"));

	// barefootc::basic_string<wchar_t> wstr1;

	return(0);
}

