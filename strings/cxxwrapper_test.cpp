
#include <stddef.h>
#include <wchar.h>

#include <assert.h>

#include "tests/strings/cxxwrapper.h"
#include "log4stdc.h"

int main (int argc, char *argv[])
{
	l4sc_configure_from_xml_file("log4j.xml");

	barefootc::basic_string<char> str1("ABCD");
	barefootc::basic_string<char> str2 = str1 + "1234";
	assert (str1.length() == 4);
	assert (str2.length() == 8);
	assert (str2 == barefootc::basic_string<char>("ABCD1234"));

	// barefootc::basic_string<wchar_t> wstr1;

	return(0);
}

