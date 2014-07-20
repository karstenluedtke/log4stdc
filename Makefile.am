AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Ilog4stdc

lib_LIBRARIES = libbarefootc.a liblog4stdc.a
include_HEADERS = log4stdc/log4stdc.h
nobase_include_HEADERS = barefootc/object.h
nobase_include_HEADERS+= barefootc/mempool.h
nobase_include_HEADERS+= barefootc/vector.h
nobase_include_HEADERS+= barefootc/linkedlist.h
nobase_include_HEADERS+= barefootc/synchronization.h

noinst_PROGRAMS = testlink
noinst_PROGRAMS+= vector_test
noinst_PROGRAMS+= string_test
noinst_PROGRAMS+= cxxwrapper_test

check_PROGRAMS =

libbarefootc_a_SOURCES = barefootc/baseobject.c
libbarefootc_a_SOURCES+= barefootc/bfc_new_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_init_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_clone_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_destroy_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_delete_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_object_size.c
libbarefootc_a_SOURCES+= barefootc/bfc_object_hashcode.c
libbarefootc_a_SOURCES+= barefootc/bfc_object_equal.c
libbarefootc_a_SOURCES+= barefootc/bfc_object_length.c
libbarefootc_a_SOURCES+= barefootc/bfc_object_tostring.c
libbarefootc_a_SOURCES+= barefootc/bfc_object_dump.c
libbarefootc_a_SOURCES+= barefootc/posix_mutex.c
libbarefootc_a_SOURCES+= barefootc/win32_mutex.c
libbarefootc_a_SOURCES+= strings/wstring.c
libbarefootc_a_SOURCES+= strings/wchar_traits.c
libbarefootc_a_SOURCES+= strings/basic_wstring.c
libbarefootc_a_SOURCES+= strings/string.c
libbarefootc_a_SOURCES+= strings/basic_string.c
libbarefootc_a_SOURCES+= strings/char_traits.c
libbarefootc_a_SOURCES+= strings/cxx_init_string.cpp
libbarefootc_a_SOURCES+= strings/cxx_init_wstring.cpp
libbarefootc_a_SOURCES+= containers/vector_ref.c
libbarefootc_a_SOURCES+= containers/vector_set.c
libbarefootc_a_SOURCES+= mempools/stdcpool.c
libbarefootc_a_SOURCES+= mempools/defaultpool.c
libbarefootc_a_SOURCES+= log4stdc/l4sc_logprintf.c 
libbarefootc_a_SOURCES+= log4stdc/l4sc_vlog.c log4stdc/l4sc_log.c 
libbarefootc_a_SOURCES+= log4stdc/l4sc_logger_enabled.c 
libbarefootc_a_SOURCES+= log4stdc/l4sc_formatmsg.c 

liblog4stdc_a_SOURCES = log4stdc/log4stdc.h log4stdc/logobjects.h
liblog4stdc_a_SOURCES+= barefootc/object.h
liblog4stdc_a_SOURCES+= barefootc/linkedlist.h
liblog4stdc_a_SOURCES+= barefootc/synchronization.h

liblog4stdc_a_SOURCES+= log4stdc/l4sc_logprintf.c 
liblog4stdc_a_SOURCES+= log4stdc/l4sc_vlog.c log4stdc/l4sc_log.c 
liblog4stdc_a_SOURCES+= log4stdc/l4sc_logger_enabled.c 
liblog4stdc_a_SOURCES+= log4stdc/l4sc_formatmsg.c 

liblog4stdc_a_SOURCES+= log4stdc/logger.c
liblog4stdc_a_SOURCES+= log4stdc/appender.c log4stdc/fileappender.c
liblog4stdc_a_SOURCES+= log4stdc/layout.c log4stdc/logmessage.c
liblog4stdc_a_SOURCES+= log4stdc/xmlconfigurator.c
liblog4stdc_a_SOURCES+= log4stdc/propertyconfigurator.c
liblog4stdc_a_SOURCES+= log4stdc/object.c 

liblog4stdc_a_SOURCES+= log4cxx/repository.h log4cxx/repository.cpp
liblog4stdc_a_SOURCES+= log4cxx/logger.cpp log4cxx/level.cpp
liblog4stdc_a_SOURCES+= log4cxx/objectptr.cpp log4cxx/objectimpl.cpp
liblog4stdc_a_SOURCES+= log4cxx/class.cpp log4cxx/classregistration.cpp
liblog4stdc_a_SOURCES+= log4cxx/configurator.cpp
liblog4stdc_a_SOURCES+= log4cxx/basicconfigurator.cpp
liblog4stdc_a_SOURCES+= log4cxx/domconfigurator.cpp
liblog4stdc_a_SOURCES+= log4cxx/propertyconfigurator.cpp
liblog4stdc_a_SOURCES+= log4cxx/messagebuffer.cpp
liblog4stdc_a_SOURCES+= log4cxx/locationinfo.cpp
liblog4stdc_a_SOURCES+= log4cxx/transcoder.cpp
liblog4stdc_a_SOURCES+= log4cxx/exception.cpp
liblog4stdc_a_SOURCES+= log4cxx/file.cpp
liblog4stdc_a_SOURCES+= log4cxx/pool.cpp
liblog4stdc_a_SOURCES+= log4cxx/mutex.cpp
liblog4stdc_a_SOURCES+= log4cxx/properties.cpp
liblog4stdc_a_SOURCES+= log4cxx/loader.cpp
liblog4stdc_a_SOURCES+= barefootc/bfc_new_object.c
liblog4stdc_a_SOURCES+= barefootc/bfc_init_object.c
liblog4stdc_a_SOURCES+= barefootc/bfc_destroy_object.c
liblog4stdc_a_SOURCES+= barefootc/bfc_delete_object.c
liblog4stdc_a_SOURCES+= barefootc/bfc_object_size.c
liblog4stdc_a_SOURCES+= barefootc/baseobject.c
liblog4stdc_a_SOURCES+= barefootc/posix_mutex.c
liblog4stdc_a_SOURCES+= barefootc/win32_mutex.c
liblog4stdc_a_SOURCES+= mempools/stdcpool.c
liblog4stdc_a_SOURCES+= mempools/defaultpool.c

testlink_SOURCES = testlink.cpp
testlink_LDADD = liblog4stdc.a
testlink_DEPENDENCIES = liblog4stdc.a

string_test_SOURCES = strings/string_test.c
string_test_LDADD = libbarefootc.a liblog4stdc.a
string_test_DEPENDENCIES = libbarefootc.a liblog4stdc.a

cxxwrapper_test_SOURCES = strings/cxxwrapper_test.cpp
cxxwrapper_test_LDADD = libbarefootc.a liblog4stdc.a
cxxwrapper_test_DEPENDENCIES = libbarefootc.a liblog4stdc.a

vector_test_SOURCES = containers/vector_test.c
vector_test_LDADD = libbarefootc.a
vector_test_DEPENDENCIES = libbarefootc.a

TESTS = testlink vector_test string_test cxxwrapper_test

check_PROGRAMS += strings/basic.string/string.ops/string.accessors/data.pass
strings_basic_string_string_ops_string_accessors_data_pass_SOURCES = tests/strings/basic.string/string.ops/string.accessors/data.pass.cpp
strings_basic_string_string_ops_string_accessors_data_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_accessors_data_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string.accessors/data.pass

check_PROGRAMS += strings/basic.string/string.ops/string.accessors/get_allocator.pass
strings_basic_string_string_ops_string_accessors_get_allocator_pass_SOURCES = tests/strings/basic.string/string.ops/string.accessors/get_allocator.pass.cpp
strings_basic_string_string_ops_string_accessors_get_allocator_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_accessors_get_allocator_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string.accessors/get_allocator.pass

check_PROGRAMS += strings/basic.string/string.ops/string.accessors/c_str.pass
strings_basic_string_string_ops_string_accessors_c_str_pass_SOURCES = tests/strings/basic.string/string.ops/string.accessors/c_str.pass.cpp
strings_basic_string_string_ops_string_accessors_c_str_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_accessors_c_str_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string.accessors/c_str.pass

check_PROGRAMS += strings/basic.string/string.ops/string_substr/substr.pass
strings_basic_string_string_ops_string_substr_substr_pass_SOURCES = tests/strings/basic.string/string.ops/string_substr/substr.pass.cpp
strings_basic_string_string_ops_string_substr_substr_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_substr_substr_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_substr/substr.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.of/pointer_size.pass
strings_basic_string_string_ops_string_find_last_of_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.of/pointer_size.pass.cpp
strings_basic_string_string_ops_string_find_last_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.of/pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.of/char_size.pass
strings_basic_string_string_ops_string_find_last_of_char_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.of/char_size.pass.cpp
strings_basic_string_string_ops_string_find_last_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.of/char_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.of/pointer_size_size.pass
strings_basic_string_string_ops_string_find_last_of_pointer_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.of/pointer_size_size.pass.cpp
strings_basic_string_string_ops_string_find_last_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.of/pointer_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.of/string_size.pass
strings_basic_string_string_ops_string_find_last_of_string_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.of/string_size.pass.cpp
strings_basic_string_string_ops_string_find_last_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.of/string_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.not.of/pointer_size.pass
strings_basic_string_string_ops_string_find_first_not_of_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.not.of/pointer_size.pass.cpp
strings_basic_string_string_ops_string_find_first_not_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_not_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.not.of/pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.not.of/char_size.pass
strings_basic_string_string_ops_string_find_first_not_of_char_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.not.of/char_size.pass.cpp
strings_basic_string_string_ops_string_find_first_not_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_not_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.not.of/char_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.not.of/pointer_size_size.pass
strings_basic_string_string_ops_string_find_first_not_of_pointer_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.not.of/pointer_size_size.pass.cpp
strings_basic_string_string_ops_string_find_first_not_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_not_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.not.of/pointer_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.not.of/string_size.pass
strings_basic_string_string_ops_string_find_first_not_of_string_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.not.of/string_size.pass.cpp
strings_basic_string_string_ops_string_find_first_not_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_not_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.not.of/string_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.not.of/pointer_size.pass
strings_basic_string_string_ops_string_find_last_not_of_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.not.of/pointer_size.pass.cpp
strings_basic_string_string_ops_string_find_last_not_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_not_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.not.of/pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.not.of/char_size.pass
strings_basic_string_string_ops_string_find_last_not_of_char_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.not.of/char_size.pass.cpp
strings_basic_string_string_ops_string_find_last_not_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_not_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.not.of/char_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.not.of/pointer_size_size.pass
strings_basic_string_string_ops_string_find_last_not_of_pointer_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.not.of/pointer_size_size.pass.cpp
strings_basic_string_string_ops_string_find_last_not_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_not_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.not.of/pointer_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.last.not.of/string_size.pass
strings_basic_string_string_ops_string_find_last_not_of_string_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.last.not.of/string_size.pass.cpp
strings_basic_string_string_ops_string_find_last_not_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_last_not_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.last.not.of/string_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find/pointer_size.pass
strings_basic_string_string_ops_string_find_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find/pointer_size.pass.cpp
strings_basic_string_string_ops_string_find_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find/pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find/char_size.pass
strings_basic_string_string_ops_string_find_char_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find/char_size.pass.cpp
strings_basic_string_string_ops_string_find_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find/char_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find/pointer_size_size.pass
strings_basic_string_string_ops_string_find_pointer_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find/pointer_size_size.pass.cpp
strings_basic_string_string_ops_string_find_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find/pointer_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find/string_size.pass
strings_basic_string_string_ops_string_find_string_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find/string_size.pass.cpp
strings_basic_string_string_ops_string_find_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find/string_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_compare/string.pass
strings_basic_string_string_ops_string_compare_string_pass_SOURCES = tests/strings/basic.string/string.ops/string_compare/string.pass.cpp
strings_basic_string_string_ops_string_compare_string_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_compare_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_compare/string.pass

check_PROGRAMS += strings/basic.string/string.ops/string_compare/size_size_pointer_size.pass
strings_basic_string_string_ops_string_compare_size_size_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_compare/size_size_pointer_size.pass.cpp
strings_basic_string_string_ops_string_compare_size_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_compare_size_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_compare/size_size_pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_compare/size_size_string_size_size.pass
strings_basic_string_string_ops_string_compare_size_size_string_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_compare/size_size_string_size_size.pass.cpp
strings_basic_string_string_ops_string_compare_size_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_compare_size_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_compare/size_size_string_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_compare/size_size_pointer.pass
strings_basic_string_string_ops_string_compare_size_size_pointer_pass_SOURCES = tests/strings/basic.string/string.ops/string_compare/size_size_pointer.pass.cpp
strings_basic_string_string_ops_string_compare_size_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_compare_size_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_compare/size_size_pointer.pass

check_PROGRAMS += strings/basic.string/string.ops/string_compare/pointer.pass
strings_basic_string_string_ops_string_compare_pointer_pass_SOURCES = tests/strings/basic.string/string.ops/string_compare/pointer.pass.cpp
strings_basic_string_string_ops_string_compare_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_compare_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_compare/pointer.pass

check_PROGRAMS += strings/basic.string/string.ops/string_compare/size_size_string.pass
strings_basic_string_string_ops_string_compare_size_size_string_pass_SOURCES = tests/strings/basic.string/string.ops/string_compare/size_size_string.pass.cpp
strings_basic_string_string_ops_string_compare_size_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_compare_size_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_compare/size_size_string.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.of/pointer_size.pass
strings_basic_string_string_ops_string_find_first_of_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.of/pointer_size.pass.cpp
strings_basic_string_string_ops_string_find_first_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.of/pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.of/char_size.pass
strings_basic_string_string_ops_string_find_first_of_char_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.of/char_size.pass.cpp
strings_basic_string_string_ops_string_find_first_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.of/char_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.of/pointer_size_size.pass
strings_basic_string_string_ops_string_find_first_of_pointer_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.of/pointer_size_size.pass.cpp
strings_basic_string_string_ops_string_find_first_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.of/pointer_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_find.first.of/string_size.pass
strings_basic_string_string_ops_string_find_first_of_string_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_find.first.of/string_size.pass.cpp
strings_basic_string_string_ops_string_find_first_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_find_first_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_find.first.of/string_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_rfind/pointer_size.pass
strings_basic_string_string_ops_string_rfind_pointer_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_rfind/pointer_size.pass.cpp
strings_basic_string_string_ops_string_rfind_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_rfind_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_rfind/pointer_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_rfind/char_size.pass
strings_basic_string_string_ops_string_rfind_char_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_rfind/char_size.pass.cpp
strings_basic_string_string_ops_string_rfind_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_rfind_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_rfind/char_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_rfind/pointer_size_size.pass
strings_basic_string_string_ops_string_rfind_pointer_size_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_rfind/pointer_size_size.pass.cpp
strings_basic_string_string_ops_string_rfind_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_rfind_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_rfind/pointer_size_size.pass

check_PROGRAMS += strings/basic.string/string.ops/string_rfind/string_size.pass
strings_basic_string_string_ops_string_rfind_string_size_pass_SOURCES = tests/strings/basic.string/string.ops/string_rfind/string_size.pass.cpp
strings_basic_string_string_ops_string_rfind_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_ops_string_rfind_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.ops/string_rfind/string_size.pass

check_PROGRAMS += strings/basic.string/string.capacity/resize_size_char.pass
strings_basic_string_string_capacity_resize_size_char_pass_SOURCES = tests/strings/basic.string/string.capacity/resize_size_char.pass.cpp
strings_basic_string_string_capacity_resize_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_resize_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/resize_size_char.pass

check_PROGRAMS += strings/basic.string/string.capacity/max_size.pass
strings_basic_string_string_capacity_max_size_pass_SOURCES = tests/strings/basic.string/string.capacity/max_size.pass.cpp
strings_basic_string_string_capacity_max_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_max_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/max_size.pass

check_PROGRAMS += strings/basic.string/string.capacity/empty.pass
strings_basic_string_string_capacity_empty_pass_SOURCES = tests/strings/basic.string/string.capacity/empty.pass.cpp
strings_basic_string_string_capacity_empty_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_empty_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/empty.pass

check_PROGRAMS += strings/basic.string/string.capacity/size.pass
strings_basic_string_string_capacity_size_pass_SOURCES = tests/strings/basic.string/string.capacity/size.pass.cpp
strings_basic_string_string_capacity_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/size.pass

check_PROGRAMS += strings/basic.string/string.capacity/shrink_to_fit.pass
strings_basic_string_string_capacity_shrink_to_fit_pass_SOURCES = tests/strings/basic.string/string.capacity/shrink_to_fit.pass.cpp
strings_basic_string_string_capacity_shrink_to_fit_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_shrink_to_fit_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/shrink_to_fit.pass

check_PROGRAMS += strings/basic.string/string.capacity/clear.pass
strings_basic_string_string_capacity_clear_pass_SOURCES = tests/strings/basic.string/string.capacity/clear.pass.cpp
strings_basic_string_string_capacity_clear_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_clear_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/clear.pass

check_PROGRAMS += strings/basic.string/string.capacity/reserve.pass
strings_basic_string_string_capacity_reserve_pass_SOURCES = tests/strings/basic.string/string.capacity/reserve.pass.cpp
strings_basic_string_string_capacity_reserve_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_reserve_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/reserve.pass

check_PROGRAMS += strings/basic.string/string.capacity/length.pass
strings_basic_string_string_capacity_length_pass_SOURCES = tests/strings/basic.string/string.capacity/length.pass.cpp
strings_basic_string_string_capacity_length_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/length.pass

check_PROGRAMS += strings/basic.string/string.capacity/resize_size.pass
strings_basic_string_string_capacity_resize_size_pass_SOURCES = tests/strings/basic.string/string.capacity/resize_size.pass.cpp
strings_basic_string_string_capacity_resize_size_pass_LDADD = libbarefootc.a liblog4stdc.a
strings_basic_string_string_capacity_resize_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += strings/basic.string/string.capacity/resize_size.pass

