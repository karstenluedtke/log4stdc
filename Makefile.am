AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Ilog4stdc

lib_LIBRARIES = libbarefootc.a liblog4stdc.a
include_HEADERS = log4stdc/log4stdc.h
nobase_include_HEADERS = barefootc/object.h
nobase_include_HEADERS+= barefootc/mempool.h
nobase_include_HEADERS+= barefootc/string.h
nobase_include_HEADERS+= barefootc/vector.h
nobase_include_HEADERS+= barefootc/iterator.h
nobase_include_HEADERS+= barefootc/linkedlist.h
nobase_include_HEADERS+= barefootc/synchronization.h
nobase_include_HEADERS+= barefootc/atomic.h
nobase_include_HEADERS+= barefootc/datetime.h
nobase_include_HEADERS+= barefootc/utf8.h

noinst_PROGRAMS = testlink
noinst_PROGRAMS+= vector_test
noinst_PROGRAMS+= string_test
noinst_PROGRAMS+= cxxwrapper_test

check_PROGRAMS =

libbarefootc_a_SOURCES = barefootc/baseobject.c
libbarefootc_a_SOURCES+= barefootc/bfc_new_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_init_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_clone_object.c
libbarefootc_a_SOURCES+= barefootc/bfc_swap_objects.c
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
libbarefootc_a_SOURCES+= datetime/datetime.c
libbarefootc_a_SOURCES+= datetime/umul32_hiword.h
libbarefootc_a_SOURCES+= datetime/bfc_init_current_datetime.c
libbarefootc_a_SOURCES+= datetime/bfc_init_datetime_from_isotime.c
libbarefootc_a_SOURCES+= datetime/bfc_datetime_frac_from_decimal.c
libbarefootc_a_SOURCES+= datetime/bfc_datetime_days_to_date.c
libbarefootc_a_SOURCES+= strings/wstring.c
libbarefootc_a_SOURCES+= strings/wchar_traits.c
libbarefootc_a_SOURCES+= strings/basic_wstring.c
libbarefootc_a_SOURCES+= strings/shared_wstring.c
libbarefootc_a_SOURCES+= strings/string.c
libbarefootc_a_SOURCES+= strings/basic_string.c
libbarefootc_a_SOURCES+= strings/shared_string.c
libbarefootc_a_SOURCES+= strings/char_traits.c
libbarefootc_a_SOURCES+= strings/utf8_from_wchar.c
libbarefootc_a_SOURCES+= strings/utf8_from_utf16.c
libbarefootc_a_SOURCES+= strings/utf16_from_utf8.c
libbarefootc_a_SOURCES+= strings/wchar_from_utf8.c
libbarefootc_a_SOURCES+= strings/bfc_init_string_copy.c
libbarefootc_a_SOURCES+= strings/bfc_init_string_move.c
libbarefootc_a_SOURCES+= strings/bfc_init_string_substr.c
libbarefootc_a_SOURCES+= strings/bfc_init_string_fill.c
libbarefootc_a_SOURCES+= strings/bfc_init_string_range.c
libbarefootc_a_SOURCES+= strings/bfc_init_wstring_copy.c
libbarefootc_a_SOURCES+= strings/bfc_init_wstring_move.c
libbarefootc_a_SOURCES+= strings/bfc_init_wstring_substr.c
libbarefootc_a_SOURCES+= strings/bfc_init_wstring_fill.c
libbarefootc_a_SOURCES+= strings/bfc_init_wstring_range.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_string_copy.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_string_move.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_string_substr.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_string_range.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_wstring_copy.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_wstring_move.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_wstring_substr.c
libbarefootc_a_SOURCES+= strings/bfc_init_basic_wstring_range.c
libbarefootc_a_SOURCES+= strings/bfc_strlen.c
libbarefootc_a_SOURCES+= strings/bfc_strdata.c
libbarefootc_a_SOURCES+= strings/bfc_string_sublen.c
libbarefootc_a_SOURCES+= strings/bfc_string_subdata.c
libbarefootc_a_SOURCES+= strings/bfc_string_reserve.c
libbarefootc_a_SOURCES+= strings/bfc_string_append_copy.c
libbarefootc_a_SOURCES+= strings/bfc_string_append_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_append_range.c
libbarefootc_a_SOURCES+= strings/bfc_string_append_substr.c
libbarefootc_a_SOURCES+= strings/bfc_string_assign_copy.c
libbarefootc_a_SOURCES+= strings/bfc_string_assign_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_assign_range.c
libbarefootc_a_SOURCES+= strings/bfc_string_assign_substr.c
libbarefootc_a_SOURCES+= strings/bfc_string_compare_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_compare_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_compare_substr.c
libbarefootc_a_SOURCES+= strings/bfc_string_compare_substr_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_compare_substrs.c
libbarefootc_a_SOURCES+= strings/bfc_string_erase_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_erase_range.c
libbarefootc_a_SOURCES+= strings/bfc_string_erase_seq.c
libbarefootc_a_SOURCES+= strings/bfc_string_erase_tail.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_not_of_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_not_of_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_of_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_of_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_not_of_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_not_of_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_of_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_of_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_copy.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_range.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_substr.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_copy.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_range_copy.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_range_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_substr.c
libbarefootc_a_SOURCES+= strings/bfc_string_rfind_bfstr.c
libbarefootc_a_SOURCES+= strings/bfc_string_rfind_c_str.c
libbarefootc_a_SOURCES+= strings/bfc_string_length.c
libbarefootc_a_SOURCES+= strings/bfc_string_max_size.c
libbarefootc_a_SOURCES+= strings/bfc_string_resize.c
libbarefootc_a_SOURCES+= strings/bfc_string_capacity.c
libbarefootc_a_SOURCES+= strings/bfc_string_reserve.c
libbarefootc_a_SOURCES+= strings/bfc_string_index.c
libbarefootc_a_SOURCES+= strings/bfc_string_data.c
libbarefootc_a_SOURCES+= strings/bfc_string_begin_iterator.c
libbarefootc_a_SOURCES+= strings/bfc_string_end_iterator.c
libbarefootc_a_SOURCES+= strings/bfc_string_reverse_end_iterator.c
libbarefootc_a_SOURCES+= strings/bfc_string_reverse_begin_iterator.c
libbarefootc_a_SOURCES+= strings/bfc_string_get_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_set_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_assign_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_assign_fill.c
libbarefootc_a_SOURCES+= strings/bfc_string_append_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_append_fill.c
libbarefootc_a_SOURCES+= strings/bfc_string_push_back.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_fill.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_fillit.c
libbarefootc_a_SOURCES+= strings/bfc_string_insert_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_pop_back.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_fill.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_range_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_range_fill.c
libbarefootc_a_SOURCES+= strings/bfc_string_replace_ranges.c
libbarefootc_a_SOURCES+= strings/bfc_string_copy.c
libbarefootc_a_SOURCES+= strings/bfc_string_swap.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_rfind_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_rfind_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_of_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_of_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_of_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_of_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_not_of_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_first_not_of_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_not_of_char.c
libbarefootc_a_SOURCES+= strings/bfc_string_find_last_not_of_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_string_substr.c
libbarefootc_a_SOURCES+= strings/bfc_string_compare_buffer.c
libbarefootc_a_SOURCES+= strings/bfc_wstrlen.c
libbarefootc_a_SOURCES+= strings/bfc_wstrdata.c
libbarefootc_a_SOURCES+= strings/bfc_wstring_sublen.c
libbarefootc_a_SOURCES+= strings/bfc_wstring_subdata.c
libbarefootc_a_SOURCES+= containers/vector_ref.c
libbarefootc_a_SOURCES+= containers/vector_set.c
libbarefootc_a_SOURCES+= containers/iterator.c
libbarefootc_a_SOURCES+= containers/wstr_iterator.c
libbarefootc_a_SOURCES+= containers/cstr_iterator.c
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
liblog4stdc_a_SOURCES+= log4stdc/propertyconfigurator.c
liblog4stdc_a_SOURCES+= log4stdc/object.c 
if EXPATXML
liblog4stdc_a_SOURCES+= log4stdc/xmlconfigurator.c
endif EXPATXML

if BFCLOG4CXX
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
endif BFCLOG4CXX

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
cxxwrapper_test_SOURCES+= tests/strings/cxxwrapper.h
cxxwrapper_test_SOURCES+= tests/support/min_allocator.h
cxxwrapper_test_SOURCES+= tests/support/test_allocator.h
cxxwrapper_test_SOURCES+= tests/input_iterator.h
cxxwrapper_test_LDADD = libbarefootc.a liblog4stdc.a
cxxwrapper_test_DEPENDENCIES = libbarefootc.a liblog4stdc.a

vector_test_SOURCES = containers/vector_test.c
vector_test_LDADD = libbarefootc.a
vector_test_DEPENDENCIES = libbarefootc.a

TESTS = testlink vector_test string_test cxxwrapper_test

noinst_PROGRAMS += examples/barefootc_log_redirect
examples_barefootc_log_redirect_SOURCES = examples/barefootc_log_redirect.c
examples_barefootc_log_redirect_LDADD = liblog4stdc.a
examples_barefootc_log_redirect_DEPENDENCIES = liblog4stdc.a
TESTS += examples/barefootc_log_redirect

check_PROGRAMS += tests/log4stdc/configure.xml/level
tests_log4stdc_configure_xml_level_SOURCES = tests/log4stdc/configure.xml/level.c
tests_log4stdc_configure_xml_level_LDADD = liblog4stdc.a
tests_log4stdc_configure_xml_level_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/log4stdc/configure.xml/level

check_PROGRAMS += tests/log4stdc/configure.prop/level
tests_log4stdc_configure_prop_level_SOURCES = tests/log4stdc/configure.prop/level.c
tests_log4stdc_configure_prop_level_LDADD = liblog4stdc.a
tests_log4stdc_configure_prop_level_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/log4stdc/configure.prop/level

check_PROGRAMS += tests/synchronization/atomic_counter
tests_synchronization_atomic_counter_SOURCES = tests/synchronization/atomic_counter.c
tests_synchronization_atomic_counter_LDADD = libbarefootc.a liblog4stdc.a
tests_synchronization_atomic_counter_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/synchronization/atomic_counter

check_PROGRAMS += tests/string.ops/string.accessors/data.pass
tests_string_ops_string_accessors_data_pass_SOURCES = tests/string.ops/string.accessors/data.pass.cpp
tests_string_ops_string_accessors_data_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_accessors_data_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string.accessors/data.pass

check_PROGRAMS += tests/string.ops/string.accessors/c_str.pass
tests_string_ops_string_accessors_c_str_pass_SOURCES = tests/string.ops/string.accessors/c_str.pass.cpp
tests_string_ops_string_accessors_c_str_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_accessors_c_str_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string.accessors/c_str.pass

check_PROGRAMS += tests/string.ops/string_substr/substr.pass
tests_string_ops_string_substr_substr_pass_SOURCES = tests/string.ops/string_substr/substr.pass.cpp
tests_string_ops_string_substr_substr_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_substr_substr_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_substr/substr.pass

check_PROGRAMS += tests/string.ops/string_find.last.of/pointer_size.pass
tests_string_ops_string_find_last_of_pointer_size_pass_SOURCES = tests/string.ops/string_find.last.of/pointer_size.pass.cpp
tests_string_ops_string_find_last_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.of/pointer_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.of/char_size.pass
tests_string_ops_string_find_last_of_char_size_pass_SOURCES = tests/string.ops/string_find.last.of/char_size.pass.cpp
tests_string_ops_string_find_last_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.of/char_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.of/pointer_size_size.pass
tests_string_ops_string_find_last_of_pointer_size_size_pass_SOURCES = tests/string.ops/string_find.last.of/pointer_size_size.pass.cpp
tests_string_ops_string_find_last_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.of/pointer_size_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.of/string_size.pass
tests_string_ops_string_find_last_of_string_size_pass_SOURCES = tests/string.ops/string_find.last.of/string_size.pass.cpp
tests_string_ops_string_find_last_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.of/string_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.not.of/pointer_size.pass
tests_string_ops_string_find_first_not_of_pointer_size_pass_SOURCES = tests/string.ops/string_find.first.not.of/pointer_size.pass.cpp
tests_string_ops_string_find_first_not_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_not_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.not.of/pointer_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.not.of/char_size.pass
tests_string_ops_string_find_first_not_of_char_size_pass_SOURCES = tests/string.ops/string_find.first.not.of/char_size.pass.cpp
tests_string_ops_string_find_first_not_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_not_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.not.of/char_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.not.of/pointer_size_size.pass
tests_string_ops_string_find_first_not_of_pointer_size_size_pass_SOURCES = tests/string.ops/string_find.first.not.of/pointer_size_size.pass.cpp
tests_string_ops_string_find_first_not_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_not_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.not.of/pointer_size_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.not.of/string_size.pass
tests_string_ops_string_find_first_not_of_string_size_pass_SOURCES = tests/string.ops/string_find.first.not.of/string_size.pass.cpp
tests_string_ops_string_find_first_not_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_not_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.not.of/string_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.not.of/pointer_size.pass
tests_string_ops_string_find_last_not_of_pointer_size_pass_SOURCES = tests/string.ops/string_find.last.not.of/pointer_size.pass.cpp
tests_string_ops_string_find_last_not_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_not_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.not.of/pointer_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.not.of/char_size.pass
tests_string_ops_string_find_last_not_of_char_size_pass_SOURCES = tests/string.ops/string_find.last.not.of/char_size.pass.cpp
tests_string_ops_string_find_last_not_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_not_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.not.of/char_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.not.of/pointer_size_size.pass
tests_string_ops_string_find_last_not_of_pointer_size_size_pass_SOURCES = tests/string.ops/string_find.last.not.of/pointer_size_size.pass.cpp
tests_string_ops_string_find_last_not_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_not_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.not.of/pointer_size_size.pass

check_PROGRAMS += tests/string.ops/string_find.last.not.of/string_size.pass
tests_string_ops_string_find_last_not_of_string_size_pass_SOURCES = tests/string.ops/string_find.last.not.of/string_size.pass.cpp
tests_string_ops_string_find_last_not_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_last_not_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.last.not.of/string_size.pass

check_PROGRAMS += tests/string.ops/string_find/pointer_size.pass
tests_string_ops_string_find_pointer_size_pass_SOURCES = tests/string.ops/string_find/pointer_size.pass.cpp
tests_string_ops_string_find_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find/pointer_size.pass

check_PROGRAMS += tests/string.ops/string_find/char_size.pass
tests_string_ops_string_find_char_size_pass_SOURCES = tests/string.ops/string_find/char_size.pass.cpp
tests_string_ops_string_find_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find/char_size.pass

check_PROGRAMS += tests/string.ops/string_find/pointer_size_size.pass
tests_string_ops_string_find_pointer_size_size_pass_SOURCES = tests/string.ops/string_find/pointer_size_size.pass.cpp
tests_string_ops_string_find_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find/pointer_size_size.pass

check_PROGRAMS += tests/string.ops/string_find/string_size.pass
tests_string_ops_string_find_string_size_pass_SOURCES = tests/string.ops/string_find/string_size.pass.cpp
tests_string_ops_string_find_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find/string_size.pass

check_PROGRAMS += tests/string.ops/string_compare/string.pass
tests_string_ops_string_compare_string_pass_SOURCES = tests/string.ops/string_compare/string.pass.cpp
tests_string_ops_string_compare_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_compare_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_compare/string.pass

check_PROGRAMS += tests/string.ops/string_compare/size_size_pointer_size.pass
tests_string_ops_string_compare_size_size_pointer_size_pass_SOURCES = tests/string.ops/string_compare/size_size_pointer_size.pass.cpp
tests_string_ops_string_compare_size_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_compare_size_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_compare/size_size_pointer_size.pass

check_PROGRAMS += tests/string.ops/string_compare/size_size_string_size_size.pass
tests_string_ops_string_compare_size_size_string_size_size_pass_SOURCES = tests/string.ops/string_compare/size_size_string_size_size.pass.cpp
tests_string_ops_string_compare_size_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_compare_size_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_compare/size_size_string_size_size.pass

check_PROGRAMS += tests/string.ops/string_compare/size_size_pointer.pass
tests_string_ops_string_compare_size_size_pointer_pass_SOURCES = tests/string.ops/string_compare/size_size_pointer.pass.cpp
tests_string_ops_string_compare_size_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_compare_size_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_compare/size_size_pointer.pass

check_PROGRAMS += tests/string.ops/string_compare/pointer.pass
tests_string_ops_string_compare_pointer_pass_SOURCES = tests/string.ops/string_compare/pointer.pass.cpp
tests_string_ops_string_compare_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_compare_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_compare/pointer.pass

check_PROGRAMS += tests/string.ops/string_compare/size_size_string.pass
tests_string_ops_string_compare_size_size_string_pass_SOURCES = tests/string.ops/string_compare/size_size_string.pass.cpp
tests_string_ops_string_compare_size_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_compare_size_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_compare/size_size_string.pass

check_PROGRAMS += tests/string.ops/string_find.first.of/pointer_size.pass
tests_string_ops_string_find_first_of_pointer_size_pass_SOURCES = tests/string.ops/string_find.first.of/pointer_size.pass.cpp
tests_string_ops_string_find_first_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.of/pointer_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.of/char_size.pass
tests_string_ops_string_find_first_of_char_size_pass_SOURCES = tests/string.ops/string_find.first.of/char_size.pass.cpp
tests_string_ops_string_find_first_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.of/char_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.of/pointer_size_size.pass
tests_string_ops_string_find_first_of_pointer_size_size_pass_SOURCES = tests/string.ops/string_find.first.of/pointer_size_size.pass.cpp
tests_string_ops_string_find_first_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.of/pointer_size_size.pass

check_PROGRAMS += tests/string.ops/string_find.first.of/string_size.pass
tests_string_ops_string_find_first_of_string_size_pass_SOURCES = tests/string.ops/string_find.first.of/string_size.pass.cpp
tests_string_ops_string_find_first_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_find_first_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_find.first.of/string_size.pass

check_PROGRAMS += tests/string.ops/string_rfind/pointer_size.pass
tests_string_ops_string_rfind_pointer_size_pass_SOURCES = tests/string.ops/string_rfind/pointer_size.pass.cpp
tests_string_ops_string_rfind_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_rfind_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_rfind/pointer_size.pass

check_PROGRAMS += tests/string.ops/string_rfind/char_size.pass
tests_string_ops_string_rfind_char_size_pass_SOURCES = tests/string.ops/string_rfind/char_size.pass.cpp
tests_string_ops_string_rfind_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_rfind_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_rfind/char_size.pass

check_PROGRAMS += tests/string.ops/string_rfind/pointer_size_size.pass
tests_string_ops_string_rfind_pointer_size_size_pass_SOURCES = tests/string.ops/string_rfind/pointer_size_size.pass.cpp
tests_string_ops_string_rfind_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_rfind_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_rfind/pointer_size_size.pass

check_PROGRAMS += tests/string.ops/string_rfind/string_size.pass
tests_string_ops_string_rfind_string_size_pass_SOURCES = tests/string.ops/string_rfind/string_size.pass.cpp
tests_string_ops_string_rfind_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_ops_string_rfind_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.ops/string_rfind/string_size.pass

check_PROGRAMS += tests/string.capacity/resize_size_char.pass
tests_string_capacity_resize_size_char_pass_SOURCES = tests/string.capacity/resize_size_char.pass.cpp
tests_string_capacity_resize_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_resize_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/resize_size_char.pass

check_PROGRAMS += tests/string.capacity/max_size.pass
tests_string_capacity_max_size_pass_SOURCES = tests/string.capacity/max_size.pass.cpp
tests_string_capacity_max_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_max_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# takes somewhat long
# TESTS += tests/string.capacity/max_size.pass

check_PROGRAMS += tests/string.capacity/empty.pass
tests_string_capacity_empty_pass_SOURCES = tests/string.capacity/empty.pass.cpp
tests_string_capacity_empty_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_empty_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/empty.pass

check_PROGRAMS += tests/string.capacity/size.pass
tests_string_capacity_size_pass_SOURCES = tests/string.capacity/size.pass.cpp
tests_string_capacity_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/size.pass

check_PROGRAMS += tests/string.capacity/shrink_to_fit.pass
tests_string_capacity_shrink_to_fit_pass_SOURCES = tests/string.capacity/shrink_to_fit.pass.cpp
tests_string_capacity_shrink_to_fit_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_shrink_to_fit_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/shrink_to_fit.pass

check_PROGRAMS += tests/string.capacity/clear.pass
tests_string_capacity_clear_pass_SOURCES = tests/string.capacity/clear.pass.cpp
tests_string_capacity_clear_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_clear_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/clear.pass

check_PROGRAMS += tests/string.capacity/reserve.pass
tests_string_capacity_reserve_pass_SOURCES = tests/string.capacity/reserve.pass.cpp
tests_string_capacity_reserve_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_reserve_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/reserve.pass

check_PROGRAMS += tests/string.capacity/length.pass
tests_string_capacity_length_pass_SOURCES = tests/string.capacity/length.pass.cpp
tests_string_capacity_length_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/length.pass

check_PROGRAMS += tests/string.capacity/resize_size.pass
tests_string_capacity_resize_size_pass_SOURCES = tests/string.capacity/resize_size.pass.cpp
tests_string_capacity_resize_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_capacity_resize_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.capacity/resize_size.pass

check_PROGRAMS += tests/string.modifiers/string_swap/swap.pass
tests_string_modifiers_string_swap_swap_pass_SOURCES = tests/string.modifiers/string_swap/swap.pass.cpp
tests_string_modifiers_string_swap_swap_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_swap_swap_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_swap/swap.pass

check_PROGRAMS += tests/string.modifiers/string_op_plus_equal/string.pass
tests_string_modifiers_string_op_plus_equal_string_pass_SOURCES = tests/string.modifiers/string_op_plus_equal/string.pass.cpp
tests_string_modifiers_string_op_plus_equal_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_op_plus_equal_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_op_plus_equal/string.pass

check_PROGRAMS += tests/string.modifiers/string_op_plus_equal/char.pass
tests_string_modifiers_string_op_plus_equal_char_pass_SOURCES = tests/string.modifiers/string_op_plus_equal/char.pass.cpp
tests_string_modifiers_string_op_plus_equal_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_op_plus_equal_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_op_plus_equal/char.pass

check_PROGRAMS += tests/string.modifiers/string_op_plus_equal/pointer.pass
tests_string_modifiers_string_op_plus_equal_pointer_pass_SOURCES = tests/string.modifiers/string_op_plus_equal/pointer.pass.cpp
tests_string_modifiers_string_op_plus_equal_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_op_plus_equal_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_op_plus_equal/pointer.pass

check_PROGRAMS += tests/string.modifiers/string_replace/iter_iter_pointer_size.pass
tests_string_modifiers_string_replace_iter_iter_pointer_size_pass_SOURCES = tests/string.modifiers/string_replace/iter_iter_pointer_size.pass.cpp
tests_string_modifiers_string_replace_iter_iter_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_iter_iter_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/iter_iter_pointer_size.pass

check_PROGRAMS += tests/string.modifiers/string_replace/size_size_pointer_size.pass
tests_string_modifiers_string_replace_size_size_pointer_size_pass_SOURCES = tests/string.modifiers/string_replace/size_size_pointer_size.pass.cpp
tests_string_modifiers_string_replace_size_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_size_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/size_size_pointer_size.pass

check_PROGRAMS += tests/string.modifiers/string_replace/iter_iter_pointer.pass
tests_string_modifiers_string_replace_iter_iter_pointer_pass_SOURCES = tests/string.modifiers/string_replace/iter_iter_pointer.pass.cpp
tests_string_modifiers_string_replace_iter_iter_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_iter_iter_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/iter_iter_pointer.pass

check_PROGRAMS += tests/string.modifiers/string_replace/size_size_string_size_size.pass
tests_string_modifiers_string_replace_size_size_string_size_size_pass_SOURCES = tests/string.modifiers/string_replace/size_size_string_size_size.pass.cpp
tests_string_modifiers_string_replace_size_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_size_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/size_size_string_size_size.pass

#check_PROGRAMS += tests/string.modifiers/string_replace/iter_iter_initializer_list.pass
#tests_string_modifiers_string_replace_iter_iter_initializer_list_pass_SOURCES = tests/string.modifiers/string_replace/iter_iter_initializer_list.pass.cpp
#tests_string_modifiers_string_replace_iter_iter_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_modifiers_string_replace_iter_iter_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.modifiers/string_replace/iter_iter_initializer_list.pass

check_PROGRAMS += tests/string.modifiers/string_replace/iter_iter_size_char.pass
tests_string_modifiers_string_replace_iter_iter_size_char_pass_SOURCES = tests/string.modifiers/string_replace/iter_iter_size_char.pass.cpp
tests_string_modifiers_string_replace_iter_iter_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_iter_iter_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/iter_iter_size_char.pass

check_PROGRAMS += tests/string.modifiers/string_replace/size_size_size_char.pass
tests_string_modifiers_string_replace_size_size_size_char_pass_SOURCES = tests/string.modifiers/string_replace/size_size_size_char.pass.cpp
tests_string_modifiers_string_replace_size_size_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_size_size_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/size_size_size_char.pass

check_PROGRAMS += tests/string.modifiers/string_replace/size_size_pointer.pass
tests_string_modifiers_string_replace_size_size_pointer_pass_SOURCES = tests/string.modifiers/string_replace/size_size_pointer.pass.cpp
tests_string_modifiers_string_replace_size_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_size_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/size_size_pointer.pass

check_PROGRAMS += tests/string.modifiers/string_replace/size_size_string.pass
tests_string_modifiers_string_replace_size_size_string_pass_SOURCES = tests/string.modifiers/string_replace/size_size_string.pass.cpp
tests_string_modifiers_string_replace_size_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_size_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/size_size_string.pass

check_PROGRAMS += tests/string.modifiers/string_replace/iter_iter_string.pass
tests_string_modifiers_string_replace_iter_iter_string_pass_SOURCES = tests/string.modifiers/string_replace/iter_iter_string.pass.cpp
tests_string_modifiers_string_replace_iter_iter_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_iter_iter_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/iter_iter_string.pass

check_PROGRAMS += tests/string.modifiers/string_replace/iter_iter_iter_iter.pass
tests_string_modifiers_string_replace_iter_iter_iter_iter_pass_SOURCES = tests/string.modifiers/string_replace/iter_iter_iter_iter.pass.cpp
tests_string_modifiers_string_replace_iter_iter_iter_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_replace_iter_iter_iter_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_replace/iter_iter_iter_iter.pass

check_PROGRAMS += tests/string.modifiers/string_copy/copy.pass
tests_string_modifiers_string_copy_copy_pass_SOURCES = tests/string.modifiers/string_copy/copy.pass.cpp
tests_string_modifiers_string_copy_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_copy_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_copy/copy.pass

check_PROGRAMS += tests/string.modifiers/string_assign/string.pass
tests_string_modifiers_string_assign_string_pass_SOURCES = tests/string.modifiers/string_assign/string.pass.cpp
tests_string_modifiers_string_assign_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_assign_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_assign/string.pass

check_PROGRAMS += tests/string.modifiers/string_assign/size_char.pass
tests_string_modifiers_string_assign_size_char_pass_SOURCES = tests/string.modifiers/string_assign/size_char.pass.cpp
tests_string_modifiers_string_assign_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_assign_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_assign/size_char.pass

check_PROGRAMS += tests/string.modifiers/string_assign/pointer_size.pass
tests_string_modifiers_string_assign_pointer_size_pass_SOURCES = tests/string.modifiers/string_assign/pointer_size.pass.cpp
tests_string_modifiers_string_assign_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_assign_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_assign/pointer_size.pass

check_PROGRAMS += tests/string.modifiers/string_assign/string_size_size.pass
tests_string_modifiers_string_assign_string_size_size_pass_SOURCES = tests/string.modifiers/string_assign/string_size_size.pass.cpp
tests_string_modifiers_string_assign_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_assign_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_assign/string_size_size.pass

# move str&&
# check_PROGRAMS += tests/string.modifiers/string_assign/rv_string.pass
# tests_string_modifiers_string_assign_rv_string_pass_SOURCES = tests/string.modifiers/string_assign/rv_string.pass.cpp
# tests_string_modifiers_string_assign_rv_string_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_modifiers_string_assign_rv_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.modifiers/string_assign/rv_string.pass

#check_PROGRAMS += tests/string.modifiers/string_assign/initializer_list.pass
#tests_string_modifiers_string_assign_initializer_list_pass_SOURCES = tests/string.modifiers/string_assign/initializer_list.pass.cpp
#tests_string_modifiers_string_assign_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_modifiers_string_assign_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.modifiers/string_assign/initializer_list.pass

check_PROGRAMS += tests/string.modifiers/string_assign/iterator.pass
tests_string_modifiers_string_assign_iterator_pass_SOURCES = tests/string.modifiers/string_assign/iterator.pass.cpp
tests_string_modifiers_string_assign_iterator_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_assign_iterator_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_assign/iterator.pass

check_PROGRAMS += tests/string.modifiers/string_assign/pointer.pass
tests_string_modifiers_string_assign_pointer_pass_SOURCES = tests/string.modifiers/string_assign/pointer.pass.cpp
tests_string_modifiers_string_assign_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_assign_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_assign/pointer.pass

check_PROGRAMS += tests/string.modifiers/string_insert/size_string_size_size.pass
tests_string_modifiers_string_insert_size_string_size_size_pass_SOURCES = tests/string.modifiers/string_insert/size_string_size_size.pass.cpp
tests_string_modifiers_string_insert_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/size_string_size_size.pass

check_PROGRAMS += tests/string.modifiers/string_insert/size_pointer_size.pass
tests_string_modifiers_string_insert_size_pointer_size_pass_SOURCES = tests/string.modifiers/string_insert/size_pointer_size.pass.cpp
tests_string_modifiers_string_insert_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/size_pointer_size.pass

check_PROGRAMS += tests/string.modifiers/string_insert/size_pointer.pass
tests_string_modifiers_string_insert_size_pointer_pass_SOURCES = tests/string.modifiers/string_insert/size_pointer.pass.cpp
tests_string_modifiers_string_insert_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/size_pointer.pass

check_PROGRAMS += tests/string.modifiers/string_insert/iter_char.pass
tests_string_modifiers_string_insert_iter_char_pass_SOURCES = tests/string.modifiers/string_insert/iter_char.pass.cpp
tests_string_modifiers_string_insert_iter_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_iter_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/iter_char.pass

check_PROGRAMS += tests/string.modifiers/string_insert/size_size_char.pass
tests_string_modifiers_string_insert_size_size_char_pass_SOURCES = tests/string.modifiers/string_insert/size_size_char.pass.cpp
tests_string_modifiers_string_insert_size_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_size_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/size_size_char.pass

check_PROGRAMS += tests/string.modifiers/string_insert/iter_iter_iter.pass
tests_string_modifiers_string_insert_iter_iter_iter_pass_SOURCES = tests/string.modifiers/string_insert/iter_iter_iter.pass.cpp
tests_string_modifiers_string_insert_iter_iter_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_iter_iter_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/iter_iter_iter.pass

#check_PROGRAMS += tests/string.modifiers/string_insert/iter_initializer_list.pass
#tests_string_modifiers_string_insert_iter_initializer_list_pass_SOURCES = tests/string.modifiers/string_insert/iter_initializer_list.pass.cpp
#tests_string_modifiers_string_insert_iter_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_modifiers_string_insert_iter_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.modifiers/string_insert/iter_initializer_list.pass

check_PROGRAMS += tests/string.modifiers/string_insert/iter_size_char.pass
tests_string_modifiers_string_insert_iter_size_char_pass_SOURCES = tests/string.modifiers/string_insert/iter_size_char.pass.cpp
tests_string_modifiers_string_insert_iter_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_iter_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/iter_size_char.pass

check_PROGRAMS += tests/string.modifiers/string_insert/size_string.pass
tests_string_modifiers_string_insert_size_string_pass_SOURCES = tests/string.modifiers/string_insert/size_string.pass.cpp
tests_string_modifiers_string_insert_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_insert_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_insert/size_string.pass

check_PROGRAMS += tests/string.modifiers/string_erase/size_size.pass
tests_string_modifiers_string_erase_size_size_pass_SOURCES = tests/string.modifiers/string_erase/size_size.pass.cpp
tests_string_modifiers_string_erase_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/size_size.pass

check_PROGRAMS += tests/string.modifiers/string_erase/erase_iter_iter_db1.pass
tests_string_modifiers_string_erase_erase_iter_iter_db1_pass_SOURCES = tests/string.modifiers/string_erase/erase_iter_iter_db1.pass.cpp
tests_string_modifiers_string_erase_erase_iter_iter_db1_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_erase_iter_iter_db1_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/erase_iter_iter_db1.pass

check_PROGRAMS += tests/string.modifiers/string_erase/pop_back.pass
tests_string_modifiers_string_erase_pop_back_pass_SOURCES = tests/string.modifiers/string_erase/pop_back.pass.cpp
tests_string_modifiers_string_erase_pop_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_pop_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/pop_back.pass

check_PROGRAMS += tests/string.modifiers/string_erase/erase_iter_iter_db4.pass
tests_string_modifiers_string_erase_erase_iter_iter_db4_pass_SOURCES = tests/string.modifiers/string_erase/erase_iter_iter_db4.pass.cpp
tests_string_modifiers_string_erase_erase_iter_iter_db4_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_erase_iter_iter_db4_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/erase_iter_iter_db4.pass

check_PROGRAMS += tests/string.modifiers/string_erase/erase_iter_db2.pass
tests_string_modifiers_string_erase_erase_iter_db2_pass_SOURCES = tests/string.modifiers/string_erase/erase_iter_db2.pass.cpp
tests_string_modifiers_string_erase_erase_iter_db2_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_erase_iter_db2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/erase_iter_db2.pass

check_PROGRAMS += tests/string.modifiers/string_erase/iter_iter.pass
tests_string_modifiers_string_erase_iter_iter_pass_SOURCES = tests/string.modifiers/string_erase/iter_iter.pass.cpp
tests_string_modifiers_string_erase_iter_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_iter_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/iter_iter.pass

check_PROGRAMS += tests/string.modifiers/string_erase/erase_iter_db1.pass
tests_string_modifiers_string_erase_erase_iter_db1_pass_SOURCES = tests/string.modifiers/string_erase/erase_iter_db1.pass.cpp
tests_string_modifiers_string_erase_erase_iter_db1_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_erase_iter_db1_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/erase_iter_db1.pass

check_PROGRAMS += tests/string.modifiers/string_erase/erase_iter_iter_db3.pass
tests_string_modifiers_string_erase_erase_iter_iter_db3_pass_SOURCES = tests/string.modifiers/string_erase/erase_iter_iter_db3.pass.cpp
tests_string_modifiers_string_erase_erase_iter_iter_db3_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_erase_iter_iter_db3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/erase_iter_iter_db3.pass

check_PROGRAMS += tests/string.modifiers/string_erase/erase_iter_iter_db2.pass
tests_string_modifiers_string_erase_erase_iter_iter_db2_pass_SOURCES = tests/string.modifiers/string_erase/erase_iter_iter_db2.pass.cpp
tests_string_modifiers_string_erase_erase_iter_iter_db2_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_erase_iter_iter_db2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/erase_iter_iter_db2.pass

check_PROGRAMS += tests/string.modifiers/string_erase/iter.pass
tests_string_modifiers_string_erase_iter_pass_SOURCES = tests/string.modifiers/string_erase/iter.pass.cpp
tests_string_modifiers_string_erase_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_erase_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_erase/iter.pass

check_PROGRAMS += tests/string.modifiers/string_append/string.pass
tests_string_modifiers_string_append_string_pass_SOURCES = tests/string.modifiers/string_append/string.pass.cpp
tests_string_modifiers_string_append_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/string.pass

check_PROGRAMS += tests/string.modifiers/string_append/size_char.pass
tests_string_modifiers_string_append_size_char_pass_SOURCES = tests/string.modifiers/string_append/size_char.pass.cpp
tests_string_modifiers_string_append_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/size_char.pass

check_PROGRAMS += tests/string.modifiers/string_append/pointer_size.pass
tests_string_modifiers_string_append_pointer_size_pass_SOURCES = tests/string.modifiers/string_append/pointer_size.pass.cpp
tests_string_modifiers_string_append_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/pointer_size.pass

check_PROGRAMS += tests/string.modifiers/string_append/string_size_size.pass
tests_string_modifiers_string_append_string_size_size_pass_SOURCES = tests/string.modifiers/string_append/string_size_size.pass.cpp
tests_string_modifiers_string_append_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/string_size_size.pass

#check_PROGRAMS += tests/string.modifiers/string_append/initializer_list.pass
#tests_string_modifiers_string_append_initializer_list_pass_SOURCES = tests/string.modifiers/string_append/initializer_list.pass.cpp
#tests_string_modifiers_string_append_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_modifiers_string_append_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.modifiers/string_append/initializer_list.pass

check_PROGRAMS += tests/string.modifiers/string_append/iterator.pass
tests_string_modifiers_string_append_iterator_pass_SOURCES = tests/string.modifiers/string_append/iterator.pass.cpp
tests_string_modifiers_string_append_iterator_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_iterator_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/iterator.pass

check_PROGRAMS += tests/string.modifiers/string_append/push_back.pass
tests_string_modifiers_string_append_push_back_pass_SOURCES = tests/string.modifiers/string_append/push_back.pass.cpp
tests_string_modifiers_string_append_push_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_push_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/push_back.pass

check_PROGRAMS += tests/string.modifiers/string_append/pointer.pass
tests_string_modifiers_string_append_pointer_pass_SOURCES = tests/string.modifiers/string_append/pointer.pass.cpp
tests_string_modifiers_string_append_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_modifiers_string_append_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.modifiers/string_append/pointer.pass

#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/compare.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoi.pass
#tests_strings_string_conversions_stoi_pass_SOURCES = tests/strings/string.conversions/stoi.pass.cpp
#tests_strings_string_conversions_stoi_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoi_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoi.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stof.pass
#tests_strings_string_conversions_stof_pass_SOURCES = tests/strings/string.conversions/stof.pass.cpp
#tests_strings_string_conversions_stof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stof.pass
#
#check_PROGRAMS += tests/strings/string.conversions/to_wstring.pass
#tests_strings_string_conversions_to_wstring_pass_SOURCES = tests/strings/string.conversions/to_wstring.pass.cpp
#tests_strings_string_conversions_to_wstring_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_to_wstring_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/to_wstring.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoll.pass
#tests_strings_string_conversions_stoll_pass_SOURCES = tests/strings/string.conversions/stoll.pass.cpp
#tests_strings_string_conversions_stoll_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoll_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoll.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoull.pass
#tests_strings_string_conversions_stoull_pass_SOURCES = tests/strings/string.conversions/stoull.pass.cpp
#tests_strings_string_conversions_stoull_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoull_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoull.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stod.pass
#tests_strings_string_conversions_stod_pass_SOURCES = tests/strings/string.conversions/stod.pass.cpp
#tests_strings_string_conversions_stod_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stod_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stod.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stol.pass
#tests_strings_string_conversions_stol_pass_SOURCES = tests/strings/string.conversions/stol.pass.cpp
#tests_strings_string_conversions_stol_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stol_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stol.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stold.pass
#tests_strings_string_conversions_stold_pass_SOURCES = tests/strings/string.conversions/stold.pass.cpp
#tests_strings_string_conversions_stold_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stold_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stold.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoul.pass
#tests_strings_string_conversions_stoul_pass_SOURCES = tests/strings/string.conversions/stoul.pass.cpp
#tests_strings_string_conversions_stoul_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoul_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoul.pass
#
#check_PROGRAMS += tests/strings/string.conversions/to_string.pass
#tests_strings_string_conversions_to_string_pass_SOURCES = tests/strings/string.conversions/to_string.pass.cpp
#tests_strings_string_conversions_to_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_to_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/to_string.pass
#
#check_PROGRAMS += tests.literals/literal.pass
#tests_literals_literal_pass_SOURCES = tests.literals/literal.pass.cpp
#tests_literals_literal_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal.pass
#
#check_PROGRAMS += tests.literals/literal2.pass
#tests_literals_literal2_pass_SOURCES = tests.literals/literal2.pass.cpp
#tests_literals_literal2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal2.pass
#
#check_PROGRAMS += tests.literals/literal1.pass
#tests_literals_literal1_pass_SOURCES = tests.literals/literal1.pass.cpp
#tests_literals_literal1_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal1_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal1.pass
#
#check_PROGRAMS += tests.literals/literal3.pass
#tests_literals_literal3_pass_SOURCES = tests.literals/literal3.pass.cpp
#tests_literals_literal3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal3.pass
#
#check_PROGRAMS += tests.hash/strings.pass
#tests_hash_strings_pass_SOURCES = tests.hash/strings.pass.cpp
#tests_hash_strings_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_hash_strings_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.hash/strings.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_incr/string_pointer.pass
#tests_string_nonmembers_string_op_incr_string_pointer_pass_SOURCES = tests/string.nonmembers/string_op_incr/string_pointer.pass.cpp
#tests_string_nonmembers_string_op_incr_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_incr_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_incr/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_incr/string_char.pass
#tests_string_nonmembers_string_op_incr_string_char_pass_SOURCES = tests/string.nonmembers/string_op_incr/string_char.pass.cpp
#tests_string_nonmembers_string_op_incr_string_char_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_incr_string_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_incr/string_char.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_incr/char_string.pass
#tests_string_nonmembers_string_op_incr_char_string_pass_SOURCES = tests/string.nonmembers/string_op_incr/char_string.pass.cpp
#tests_string_nonmembers_string_op_incr_char_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_incr_char_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_incr/char_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_incr/string_string.pass
#tests_string_nonmembers_string_op_incr_string_string_pass_SOURCES = tests/string.nonmembers/string_op_incr/string_string.pass.cpp
#tests_string_nonmembers_string_op_incr_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_incr_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_incr/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_incr/pointer_string.pass
#tests_string_nonmembers_string_op_incr_pointer_string_pass_SOURCES = tests/string.nonmembers/string_op_incr/pointer_string.pass.cpp
#tests_string_nonmembers_string_op_incr_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_incr_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_incr/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.io/get_line.pass
#tests_string_nonmembers_string_io_get_line_pass_SOURCES = tests/string.nonmembers/string.io/get_line.pass.cpp
#tests_string_nonmembers_string_io_get_line_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_io_get_line_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.io/get_line.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.io/get_line_delim_rv.pass
#tests_string_nonmembers_string_io_get_line_delim_rv_pass_SOURCES = tests/string.nonmembers/string.io/get_line_delim_rv.pass.cpp
#tests_string_nonmembers_string_io_get_line_delim_rv_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_io_get_line_delim_rv_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.io/get_line_delim_rv.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.io/stream_insert.pass
#tests_string_nonmembers_string_io_stream_insert_pass_SOURCES = tests/string.nonmembers/string.io/stream_insert.pass.cpp
#tests_string_nonmembers_string_io_stream_insert_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_io_stream_insert_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.io/stream_insert.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.io/get_line_rv.pass
#tests_string_nonmembers_string_io_get_line_rv_pass_SOURCES = tests/string.nonmembers/string.io/get_line_rv.pass.cpp
#tests_string_nonmembers_string_io_get_line_rv_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_io_get_line_rv_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.io/get_line_rv.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.io/stream_extract.pass
#tests_string_nonmembers_string_io_stream_extract_pass_SOURCES = tests/string.nonmembers/string.io/stream_extract.pass.cpp
#tests_string_nonmembers_string_io_stream_extract_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_io_stream_extract_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.io/stream_extract.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.io/get_line_delim.pass
#tests_string_nonmembers_string_io_get_line_delim_pass_SOURCES = tests/string.nonmembers/string.io/get_line_delim.pass.cpp
#tests_string_nonmembers_string_io_get_line_delim_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_io_get_line_delim_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.io/get_line_delim.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_ne/string_pointer.pass
#tests_string_nonmembers_string_op_ne_string_pointer_pass_SOURCES = tests/string.nonmembers/string_op_ne/string_pointer.pass.cpp
#tests_string_nonmembers_string_op_ne_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_ne_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_ne/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_ne/string_string.pass
#tests_string_nonmembers_string_op_ne_string_string_pass_SOURCES = tests/string.nonmembers/string_op_ne/string_string.pass.cpp
#tests_string_nonmembers_string_op_ne_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_ne_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_ne/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_ne/pointer_string.pass
#tests_string_nonmembers_string_op_ne_pointer_string_pass_SOURCES = tests/string.nonmembers/string_op_ne/pointer_string.pass.cpp
#tests_string_nonmembers_string_op_ne_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_ne_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_ne/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_opgt/string_pointer.pass
#tests_string_nonmembers_string_opgt_string_pointer_pass_SOURCES = tests/string.nonmembers/string_opgt/string_pointer.pass.cpp
#tests_string_nonmembers_string_opgt_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_opgt_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_opgt/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_opgt/string_string.pass
#tests_string_nonmembers_string_opgt_string_string_pass_SOURCES = tests/string.nonmembers/string_opgt/string_string.pass.cpp
#tests_string_nonmembers_string_opgt_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_opgt_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_opgt/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_opgt/pointer_string.pass
#tests_string_nonmembers_string_opgt_pointer_string_pass_SOURCES = tests/string.nonmembers/string_opgt/pointer_string.pass.cpp
#tests_string_nonmembers_string_opgt_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_opgt_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_opgt/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_ge/string_pointer.pass
#tests_string_nonmembers_string_op_ge_string_pointer_pass_SOURCES = tests/string.nonmembers/string_op_ge/string_pointer.pass.cpp
#tests_string_nonmembers_string_op_ge_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_ge_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_ge/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_ge/string_string.pass
#tests_string_nonmembers_string_op_ge_string_string_pass_SOURCES = tests/string.nonmembers/string_op_ge/string_string.pass.cpp
#tests_string_nonmembers_string_op_ge_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_ge_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_ge/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_ge/pointer_string.pass
#tests_string_nonmembers_string_op_ge_pointer_string_pass_SOURCES = tests/string.nonmembers/string_op_ge/pointer_string.pass.cpp
#tests_string_nonmembers_string_op_ge_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_ge_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_ge/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_le/string_pointer.pass
#tests_string_nonmembers_string_op_le_string_pointer_pass_SOURCES = tests/string.nonmembers/string_op_le/string_pointer.pass.cpp
#tests_string_nonmembers_string_op_le_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_le_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_le/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_le/string_string.pass
#tests_string_nonmembers_string_op_le_string_string_pass_SOURCES = tests/string.nonmembers/string_op_le/string_string.pass.cpp
#tests_string_nonmembers_string_op_le_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_le_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_le/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_op_le/pointer_string.pass
#tests_string_nonmembers_string_op_le_pointer_string_pass_SOURCES = tests/string.nonmembers/string_op_le/pointer_string.pass.cpp
#tests_string_nonmembers_string_op_le_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_op_le_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_op_le/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_oplt/string_pointer.pass
#tests_string_nonmembers_string_oplt_string_pointer_pass_SOURCES = tests/string.nonmembers/string_oplt/string_pointer.pass.cpp
#tests_string_nonmembers_string_oplt_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_oplt_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_oplt/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_oplt/string_string.pass
#tests_string_nonmembers_string_oplt_string_string_pass_SOURCES = tests/string.nonmembers/string_oplt/string_string.pass.cpp
#tests_string_nonmembers_string_oplt_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_oplt_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_oplt/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_oplt/pointer_string.pass
#tests_string_nonmembers_string_oplt_pointer_string_pass_SOURCES = tests/string.nonmembers/string_oplt/pointer_string.pass.cpp
#tests_string_nonmembers_string_oplt_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_oplt_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_oplt/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_operator_eq/string_pointer.pass
#tests_string_nonmembers_string_operator_eq_string_pointer_pass_SOURCES = tests/string.nonmembers/string_operator_eq/string_pointer.pass.cpp
#tests_string_nonmembers_string_operator_eq_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_operator_eq_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_operator_eq/string_pointer.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_operator_eq/string_string.pass
#tests_string_nonmembers_string_operator_eq_string_string_pass_SOURCES = tests/string.nonmembers/string_operator_eq/string_string.pass.cpp
#tests_string_nonmembers_string_operator_eq_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_operator_eq_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_operator_eq/string_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string_operator_eq/pointer_string.pass
#tests_string_nonmembers_string_operator_eq_pointer_string_pass_SOURCES = tests/string.nonmembers/string_operator_eq/pointer_string.pass.cpp
#tests_string_nonmembers_string_operator_eq_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_operator_eq_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string_operator_eq/pointer_string.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.special/swap_noexcept.pass
#tests_string_nonmembers_string_special_swap_noexcept_pass_SOURCES = tests/string.nonmembers/string.special/swap_noexcept.pass.cpp
#tests_string_nonmembers_string_special_swap_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_special_swap_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.special/swap_noexcept.pass
#
#check_PROGRAMS += tests/string.nonmembers/string.special/swap.pass
#tests_string_nonmembers_string_special_swap_pass_SOURCES = tests/string.nonmembers/string.special/swap.pass.cpp
#tests_string_nonmembers_string_special_swap_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_nonmembers_string_special_swap_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.nonmembers/string.special/swap.pass
#
check_PROGRAMS += tests/string.iterators/begin.pass
tests_string_iterators_begin_pass_SOURCES = tests/string.iterators/begin.pass.cpp
tests_string_iterators_begin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_begin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/begin.pass

check_PROGRAMS += tests/string.iterators/end.pass
tests_string_iterators_end_pass_SOURCES = tests/string.iterators/end.pass.cpp
tests_string_iterators_end_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_end_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/end.pass

check_PROGRAMS += tests/string.iterators/iterators.pass
tests_string_iterators_iterators_pass_SOURCES = tests/string.iterators/iterators.pass.cpp
tests_string_iterators_iterators_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_iterators_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/iterators.pass

check_PROGRAMS += tests/string.iterators/db_iterators_2.pass
tests_string_iterators_db_iterators_2_pass_SOURCES = tests/string.iterators/db_iterators_2.pass.cpp
tests_string_iterators_db_iterators_2_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_2.pass

check_PROGRAMS += tests/string.iterators/db_iterators_3.pass
tests_string_iterators_db_iterators_3_pass_SOURCES = tests/string.iterators/db_iterators_3.pass.cpp
tests_string_iterators_db_iterators_3_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_3.pass

check_PROGRAMS += tests/string.iterators/db_iterators_4.pass
tests_string_iterators_db_iterators_4_pass_SOURCES = tests/string.iterators/db_iterators_4.pass.cpp
tests_string_iterators_db_iterators_4_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_4_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_4.pass

check_PROGRAMS += tests/string.iterators/db_iterators_5.pass
tests_string_iterators_db_iterators_5_pass_SOURCES = tests/string.iterators/db_iterators_5.pass.cpp
tests_string_iterators_db_iterators_5_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_5_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_5.pass

check_PROGRAMS += tests/string.iterators/db_iterators_6.pass
tests_string_iterators_db_iterators_6_pass_SOURCES = tests/string.iterators/db_iterators_6.pass.cpp
tests_string_iterators_db_iterators_6_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_6_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_6.pass

check_PROGRAMS += tests/string.iterators/db_iterators_7.pass
tests_string_iterators_db_iterators_7_pass_SOURCES = tests/string.iterators/db_iterators_7.pass.cpp
tests_string_iterators_db_iterators_7_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_7_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_7.pass

check_PROGRAMS += tests/string.iterators/db_iterators_8.pass
tests_string_iterators_db_iterators_8_pass_SOURCES = tests/string.iterators/db_iterators_8.pass.cpp
tests_string_iterators_db_iterators_8_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_db_iterators_8_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/db_iterators_8.pass

check_PROGRAMS += tests/string.iterators/cbegin.pass
tests_string_iterators_cbegin_pass_SOURCES = tests/string.iterators/cbegin.pass.cpp
tests_string_iterators_cbegin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_cbegin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/cbegin.pass

check_PROGRAMS += tests/string.iterators/cend.pass
tests_string_iterators_cend_pass_SOURCES = tests/string.iterators/cend.pass.cpp
tests_string_iterators_cend_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_cend_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/cend.pass

check_PROGRAMS += tests/string.iterators/rbegin.pass
tests_string_iterators_rbegin_pass_SOURCES = tests/string.iterators/rbegin.pass.cpp
tests_string_iterators_rbegin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_rbegin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/rbegin.pass

check_PROGRAMS += tests/string.iterators/rend.pass
tests_string_iterators_rend_pass_SOURCES = tests/string.iterators/rend.pass.cpp
tests_string_iterators_rend_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_rend_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/rend.pass

check_PROGRAMS += tests/string.iterators/crbegin.pass
tests_string_iterators_crbegin_pass_SOURCES = tests/string.iterators/crbegin.pass.cpp
tests_string_iterators_crbegin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_crbegin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/crbegin.pass

check_PROGRAMS += tests/string.iterators/crend.pass
tests_string_iterators_crend_pass_SOURCES = tests/string.iterators/crend.pass.cpp
tests_string_iterators_crend_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_iterators_crend_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.iterators/crend.pass

#check_PROGRAMS += tests/types.pass
#tests_types_pass_SOURCES = tests/types.pass.cpp
#tests_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/types.pass

check_PROGRAMS += tests/string.cons/pointer_size_alloc.pass
tests_string_cons_pointer_size_alloc_pass_SOURCES = tests/string.cons/pointer_size_alloc.pass.cpp
tests_string_cons_pointer_size_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_pointer_size_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/pointer_size_alloc.pass

check_PROGRAMS += tests/string.cons/pointer_assignment.pass
tests_string_cons_pointer_assignment_pass_SOURCES = tests/string.cons/pointer_assignment.pass.cpp
tests_string_cons_pointer_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_pointer_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/pointer_assignment.pass

# check_PROGRAMS += tests/string.cons/initializer_list_assignment.pass
# tests_string_cons_initializer_list_assignment_pass_SOURCES = tests/string.cons/initializer_list_assignment.pass.cpp
# tests_string_cons_initializer_list_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_initializer_list_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/initializer_list_assignment.pass

check_PROGRAMS += tests/string.cons/copy_assignment.pass
tests_string_cons_copy_assignment_pass_SOURCES = tests/string.cons/copy_assignment.pass.cpp
tests_string_cons_copy_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_copy_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/copy_assignment.pass

# check_PROGRAMS += tests/string.cons/move.pass
# tests_string_cons_move_pass_SOURCES = tests/string.cons/move.pass.cpp
# tests_string_cons_move_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/move.pass

check_PROGRAMS += tests/string.cons/copy.pass
tests_string_cons_copy_pass_SOURCES = tests/string.cons/copy.pass.cpp
tests_string_cons_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/copy.pass

# check_PROGRAMS += tests/string.cons/alloc.pass
# tests_string_cons_alloc_pass_SOURCES = tests/string.cons/alloc.pass.cpp
# tests_string_cons_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/alloc.pass

# check_PROGRAMS += tests/string.cons/move_assign_noexcept.pass
# tests_string_cons_move_assign_noexcept_pass_SOURCES = tests/string.cons/move_assign_noexcept.pass.cpp
# tests_string_cons_move_assign_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_move_assign_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/move_assign_noexcept.pass

# check_PROGRAMS += tests/string.cons/initializer_list.pass
# tests_string_cons_initializer_list_pass_SOURCES = tests/string.cons/initializer_list.pass.cpp
# tests_string_cons_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/initializer_list.pass

# check_PROGRAMS += tests/string.cons/iter_alloc.pass
# tests_string_cons_iter_alloc_pass_SOURCES = tests/string.cons/iter_alloc.pass.cpp
# tests_string_cons_iter_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_iter_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/iter_alloc.pass

check_PROGRAMS += tests/string.cons/char_assignment.pass
tests_string_cons_char_assignment_pass_SOURCES = tests/string.cons/char_assignment.pass.cpp
tests_string_cons_char_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_char_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/char_assignment.pass

check_PROGRAMS += tests/string.cons/pointer_alloc.pass
tests_string_cons_pointer_alloc_pass_SOURCES = tests/string.cons/pointer_alloc.pass.cpp
tests_string_cons_pointer_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_pointer_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/pointer_alloc.pass

# check_PROGRAMS += tests/string.cons/move_noexcept.pass
# tests_string_cons_move_noexcept_pass_SOURCES = tests/string.cons/move_noexcept.pass.cpp
# tests_string_cons_move_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_move_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/move_noexcept.pass

# check_PROGRAMS += tests/string.cons/move_assignment.pass
# tests_string_cons_move_assignment_pass_SOURCES = tests/string.cons/move_assignment.pass.cpp
# tests_string_cons_move_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_move_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/move_assignment.pass

# check_PROGRAMS += tests/string.cons/dtor_noexcept.pass
# tests_string_cons_dtor_noexcept_pass_SOURCES = tests/string.cons/dtor_noexcept.pass.cpp
# tests_string_cons_dtor_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_dtor_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/dtor_noexcept.pass

# check_PROGRAMS += tests/string.cons/move_alloc.pass
# tests_string_cons_move_alloc_pass_SOURCES = tests/string.cons/move_alloc.pass.cpp
# tests_string_cons_move_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_move_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/move_alloc.pass

check_PROGRAMS += tests/string.cons/size_char_alloc.pass
tests_string_cons_size_char_alloc_pass_SOURCES = tests/string.cons/size_char_alloc.pass.cpp
tests_string_cons_size_char_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_size_char_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/size_char_alloc.pass

check_PROGRAMS += tests/string.cons/substr.pass
tests_string_cons_substr_pass_SOURCES = tests/string.cons/substr.pass.cpp
tests_string_cons_substr_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_substr_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/substr.pass

# check_PROGRAMS += tests/string.cons/default_noexcept.pass
# tests_string_cons_default_noexcept_pass_SOURCES = tests/string.cons/default_noexcept.pass.cpp
# tests_string_cons_default_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_string_cons_default_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/string.cons/default_noexcept.pass

check_PROGRAMS += tests/string.cons/copy_alloc.pass
tests_string_cons_copy_alloc_pass_SOURCES = tests/string.cons/copy_alloc.pass.cpp
tests_string_cons_copy_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_cons_copy_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.cons/copy_alloc.pass

check_PROGRAMS += tests/string.access/back.pass
tests_string_access_back_pass_SOURCES = tests/string.access/back.pass.cpp
tests_string_access_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/back.pass

check_PROGRAMS += tests/string.access/at.pass
tests_string_access_at_pass_SOURCES = tests/string.access/at.pass.cpp
tests_string_access_at_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_at_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/at.pass

check_PROGRAMS += tests/string.access/db_cback.pass
tests_string_access_db_cback_pass_SOURCES = tests/string.access/db_cback.pass.cpp
tests_string_access_db_cback_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_db_cback_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/db_cback.pass

check_PROGRAMS += tests/string.access/db_back.pass
tests_string_access_db_back_pass_SOURCES = tests/string.access/db_back.pass.cpp
tests_string_access_db_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_db_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/db_back.pass

check_PROGRAMS += tests/string.access/db_cfront.pass
tests_string_access_db_cfront_pass_SOURCES = tests/string.access/db_cfront.pass.cpp
tests_string_access_db_cfront_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_db_cfront_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/db_cfront.pass

check_PROGRAMS += tests/string.access/db_cindex.pass
tests_string_access_db_cindex_pass_SOURCES = tests/string.access/db_cindex.pass.cpp
tests_string_access_db_cindex_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_db_cindex_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/db_cindex.pass

check_PROGRAMS += tests/string.access/db_index.pass
tests_string_access_db_index_pass_SOURCES = tests/string.access/db_index.pass.cpp
tests_string_access_db_index_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_db_index_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/db_index.pass

check_PROGRAMS += tests/string.access/front.pass
tests_string_access_front_pass_SOURCES = tests/string.access/front.pass.cpp
tests_string_access_front_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_front_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/front.pass

check_PROGRAMS += tests/string.access/index.pass
tests_string_access_index_pass_SOURCES = tests/string.access/index.pass.cpp
tests_string_access_index_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_index_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/index.pass

check_PROGRAMS += tests/string.access/db_front.pass
tests_string_access_db_front_pass_SOURCES = tests/string.access/db_front.pass.cpp
tests_string_access_db_front_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_string_access_db_front_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/string.access/db_front.pass

check_PROGRAMS += tests/wstring.ops/string.accessors/data.pass
tests_wstring_ops_string_accessors_data_pass_SOURCES = tests/wstring.ops/string.accessors/data.pass.cpp
tests_wstring_ops_string_accessors_data_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_accessors_data_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string.accessors/data.pass

check_PROGRAMS += tests/wstring.ops/string.accessors/c_str.pass
tests_wstring_ops_string_accessors_c_str_pass_SOURCES = tests/wstring.ops/string.accessors/c_str.pass.cpp
tests_wstring_ops_string_accessors_c_str_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_accessors_c_str_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string.accessors/c_str.pass

check_PROGRAMS += tests/wstring.ops/string_substr/substr.pass
tests_wstring_ops_string_substr_substr_pass_SOURCES = tests/wstring.ops/string_substr/substr.pass.cpp
tests_wstring_ops_string_substr_substr_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_substr_substr_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_substr/substr.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.of/pointer_size.pass
tests_wstring_ops_string_find_last_of_pointer_size_pass_SOURCES = tests/wstring.ops/string_find.last.of/pointer_size.pass.cpp
tests_wstring_ops_string_find_last_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.of/pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.of/char_size.pass
tests_wstring_ops_string_find_last_of_char_size_pass_SOURCES = tests/wstring.ops/string_find.last.of/char_size.pass.cpp
tests_wstring_ops_string_find_last_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.of/char_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.of/pointer_size_size.pass
tests_wstring_ops_string_find_last_of_pointer_size_size_pass_SOURCES = tests/wstring.ops/string_find.last.of/pointer_size_size.pass.cpp
tests_wstring_ops_string_find_last_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.of/pointer_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.of/string_size.pass
tests_wstring_ops_string_find_last_of_string_size_pass_SOURCES = tests/wstring.ops/string_find.last.of/string_size.pass.cpp
tests_wstring_ops_string_find_last_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.of/string_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.not.of/pointer_size.pass
tests_wstring_ops_string_find_first_not_of_pointer_size_pass_SOURCES = tests/wstring.ops/string_find.first.not.of/pointer_size.pass.cpp
tests_wstring_ops_string_find_first_not_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_not_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.not.of/pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.not.of/char_size.pass
tests_wstring_ops_string_find_first_not_of_char_size_pass_SOURCES = tests/wstring.ops/string_find.first.not.of/char_size.pass.cpp
tests_wstring_ops_string_find_first_not_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_not_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.not.of/char_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.not.of/pointer_size_size.pass
tests_wstring_ops_string_find_first_not_of_pointer_size_size_pass_SOURCES = tests/wstring.ops/string_find.first.not.of/pointer_size_size.pass.cpp
tests_wstring_ops_string_find_first_not_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_not_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.not.of/pointer_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.not.of/string_size.pass
tests_wstring_ops_string_find_first_not_of_string_size_pass_SOURCES = tests/wstring.ops/string_find.first.not.of/string_size.pass.cpp
tests_wstring_ops_string_find_first_not_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_not_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.not.of/string_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.not.of/pointer_size.pass
tests_wstring_ops_string_find_last_not_of_pointer_size_pass_SOURCES = tests/wstring.ops/string_find.last.not.of/pointer_size.pass.cpp
tests_wstring_ops_string_find_last_not_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_not_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.not.of/pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.not.of/char_size.pass
tests_wstring_ops_string_find_last_not_of_char_size_pass_SOURCES = tests/wstring.ops/string_find.last.not.of/char_size.pass.cpp
tests_wstring_ops_string_find_last_not_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_not_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.not.of/char_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.not.of/pointer_size_size.pass
tests_wstring_ops_string_find_last_not_of_pointer_size_size_pass_SOURCES = tests/wstring.ops/string_find.last.not.of/pointer_size_size.pass.cpp
tests_wstring_ops_string_find_last_not_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_not_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.not.of/pointer_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.last.not.of/string_size.pass
tests_wstring_ops_string_find_last_not_of_string_size_pass_SOURCES = tests/wstring.ops/string_find.last.not.of/string_size.pass.cpp
tests_wstring_ops_string_find_last_not_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_last_not_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.last.not.of/string_size.pass

check_PROGRAMS += tests/wstring.ops/string_find/pointer_size.pass
tests_wstring_ops_string_find_pointer_size_pass_SOURCES = tests/wstring.ops/string_find/pointer_size.pass.cpp
tests_wstring_ops_string_find_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find/pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_find/char_size.pass
tests_wstring_ops_string_find_char_size_pass_SOURCES = tests/wstring.ops/string_find/char_size.pass.cpp
tests_wstring_ops_string_find_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find/char_size.pass

check_PROGRAMS += tests/wstring.ops/string_find/pointer_size_size.pass
tests_wstring_ops_string_find_pointer_size_size_pass_SOURCES = tests/wstring.ops/string_find/pointer_size_size.pass.cpp
tests_wstring_ops_string_find_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find/pointer_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_find/string_size.pass
tests_wstring_ops_string_find_string_size_pass_SOURCES = tests/wstring.ops/string_find/string_size.pass.cpp
tests_wstring_ops_string_find_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find/string_size.pass

check_PROGRAMS += tests/wstring.ops/string_compare/string.pass
tests_wstring_ops_string_compare_string_pass_SOURCES = tests/wstring.ops/string_compare/string.pass.cpp
tests_wstring_ops_string_compare_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_compare_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_compare/string.pass

check_PROGRAMS += tests/wstring.ops/string_compare/size_size_pointer_size.pass
tests_wstring_ops_string_compare_size_size_pointer_size_pass_SOURCES = tests/wstring.ops/string_compare/size_size_pointer_size.pass.cpp
tests_wstring_ops_string_compare_size_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_compare_size_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_compare/size_size_pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_compare/size_size_string_size_size.pass
tests_wstring_ops_string_compare_size_size_string_size_size_pass_SOURCES = tests/wstring.ops/string_compare/size_size_string_size_size.pass.cpp
tests_wstring_ops_string_compare_size_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_compare_size_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_compare/size_size_string_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_compare/size_size_pointer.pass
tests_wstring_ops_string_compare_size_size_pointer_pass_SOURCES = tests/wstring.ops/string_compare/size_size_pointer.pass.cpp
tests_wstring_ops_string_compare_size_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_compare_size_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_compare/size_size_pointer.pass

check_PROGRAMS += tests/wstring.ops/string_compare/pointer.pass
tests_wstring_ops_string_compare_pointer_pass_SOURCES = tests/wstring.ops/string_compare/pointer.pass.cpp
tests_wstring_ops_string_compare_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_compare_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_compare/pointer.pass

check_PROGRAMS += tests/wstring.ops/string_compare/size_size_string.pass
tests_wstring_ops_string_compare_size_size_string_pass_SOURCES = tests/wstring.ops/string_compare/size_size_string.pass.cpp
tests_wstring_ops_string_compare_size_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_compare_size_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_compare/size_size_string.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.of/pointer_size.pass
tests_wstring_ops_string_find_first_of_pointer_size_pass_SOURCES = tests/wstring.ops/string_find.first.of/pointer_size.pass.cpp
tests_wstring_ops_string_find_first_of_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_of_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.of/pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.of/char_size.pass
tests_wstring_ops_string_find_first_of_char_size_pass_SOURCES = tests/wstring.ops/string_find.first.of/char_size.pass.cpp
tests_wstring_ops_string_find_first_of_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_of_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.of/char_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.of/pointer_size_size.pass
tests_wstring_ops_string_find_first_of_pointer_size_size_pass_SOURCES = tests/wstring.ops/string_find.first.of/pointer_size_size.pass.cpp
tests_wstring_ops_string_find_first_of_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_of_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.of/pointer_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_find.first.of/string_size.pass
tests_wstring_ops_string_find_first_of_string_size_pass_SOURCES = tests/wstring.ops/string_find.first.of/string_size.pass.cpp
tests_wstring_ops_string_find_first_of_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_find_first_of_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_find.first.of/string_size.pass

check_PROGRAMS += tests/wstring.ops/string_rfind/pointer_size.pass
tests_wstring_ops_string_rfind_pointer_size_pass_SOURCES = tests/wstring.ops/string_rfind/pointer_size.pass.cpp
tests_wstring_ops_string_rfind_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_rfind_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_rfind/pointer_size.pass

check_PROGRAMS += tests/wstring.ops/string_rfind/char_size.pass
tests_wstring_ops_string_rfind_char_size_pass_SOURCES = tests/wstring.ops/string_rfind/char_size.pass.cpp
tests_wstring_ops_string_rfind_char_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_rfind_char_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_rfind/char_size.pass

check_PROGRAMS += tests/wstring.ops/string_rfind/pointer_size_size.pass
tests_wstring_ops_string_rfind_pointer_size_size_pass_SOURCES = tests/wstring.ops/string_rfind/pointer_size_size.pass.cpp
tests_wstring_ops_string_rfind_pointer_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_rfind_pointer_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_rfind/pointer_size_size.pass

check_PROGRAMS += tests/wstring.ops/string_rfind/string_size.pass
tests_wstring_ops_string_rfind_string_size_pass_SOURCES = tests/wstring.ops/string_rfind/string_size.pass.cpp
tests_wstring_ops_string_rfind_string_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_ops_string_rfind_string_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.ops/string_rfind/string_size.pass

check_PROGRAMS += tests/wstring.capacity/resize_size_char.pass
tests_wstring_capacity_resize_size_char_pass_SOURCES = tests/wstring.capacity/resize_size_char.pass.cpp
tests_wstring_capacity_resize_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_resize_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/resize_size_char.pass

check_PROGRAMS += tests/wstring.capacity/max_size.pass
tests_wstring_capacity_max_size_pass_SOURCES = tests/wstring.capacity/max_size.pass.cpp
tests_wstring_capacity_max_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_max_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# takes somewhat long
# TESTS += tests/wstring.capacity/max_size.pass

check_PROGRAMS += tests/wstring.capacity/empty.pass
tests_wstring_capacity_empty_pass_SOURCES = tests/wstring.capacity/empty.pass.cpp
tests_wstring_capacity_empty_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_empty_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/empty.pass

check_PROGRAMS += tests/wstring.capacity/size.pass
tests_wstring_capacity_size_pass_SOURCES = tests/wstring.capacity/size.pass.cpp
tests_wstring_capacity_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/size.pass

check_PROGRAMS += tests/wstring.capacity/shrink_to_fit.pass
tests_wstring_capacity_shrink_to_fit_pass_SOURCES = tests/wstring.capacity/shrink_to_fit.pass.cpp
tests_wstring_capacity_shrink_to_fit_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_shrink_to_fit_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/shrink_to_fit.pass

check_PROGRAMS += tests/wstring.capacity/clear.pass
tests_wstring_capacity_clear_pass_SOURCES = tests/wstring.capacity/clear.pass.cpp
tests_wstring_capacity_clear_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_clear_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/clear.pass

check_PROGRAMS += tests/wstring.capacity/reserve.pass
tests_wstring_capacity_reserve_pass_SOURCES = tests/wstring.capacity/reserve.pass.cpp
tests_wstring_capacity_reserve_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_reserve_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/reserve.pass

check_PROGRAMS += tests/wstring.capacity/length.pass
tests_wstring_capacity_length_pass_SOURCES = tests/wstring.capacity/length.pass.cpp
tests_wstring_capacity_length_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/length.pass

check_PROGRAMS += tests/wstring.capacity/resize_size.pass
tests_wstring_capacity_resize_size_pass_SOURCES = tests/wstring.capacity/resize_size.pass.cpp
tests_wstring_capacity_resize_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_capacity_resize_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.capacity/resize_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_swap/swap.pass
tests_wstring_modifiers_string_swap_swap_pass_SOURCES = tests/wstring.modifiers/string_swap/swap.pass.cpp
tests_wstring_modifiers_string_swap_swap_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_swap_swap_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_swap/swap.pass

check_PROGRAMS += tests/wstring.modifiers/string_op_plus_equal/string.pass
tests_wstring_modifiers_string_op_plus_equal_string_pass_SOURCES = tests/wstring.modifiers/string_op_plus_equal/string.pass.cpp
tests_wstring_modifiers_string_op_plus_equal_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_op_plus_equal_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_op_plus_equal/string.pass

check_PROGRAMS += tests/wstring.modifiers/string_op_plus_equal/char.pass
tests_wstring_modifiers_string_op_plus_equal_char_pass_SOURCES = tests/wstring.modifiers/string_op_plus_equal/char.pass.cpp
tests_wstring_modifiers_string_op_plus_equal_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_op_plus_equal_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_op_plus_equal/char.pass

check_PROGRAMS += tests/wstring.modifiers/string_op_plus_equal/pointer.pass
tests_wstring_modifiers_string_op_plus_equal_pointer_pass_SOURCES = tests/wstring.modifiers/string_op_plus_equal/pointer.pass.cpp
tests_wstring_modifiers_string_op_plus_equal_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_op_plus_equal_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_op_plus_equal/pointer.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/iter_iter_pointer_size.pass
tests_wstring_modifiers_string_replace_iter_iter_pointer_size_pass_SOURCES = tests/wstring.modifiers/string_replace/iter_iter_pointer_size.pass.cpp
tests_wstring_modifiers_string_replace_iter_iter_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_iter_iter_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/iter_iter_pointer_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/size_size_pointer_size.pass
tests_wstring_modifiers_string_replace_size_size_pointer_size_pass_SOURCES = tests/wstring.modifiers/string_replace/size_size_pointer_size.pass.cpp
tests_wstring_modifiers_string_replace_size_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_size_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/size_size_pointer_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/iter_iter_pointer.pass
tests_wstring_modifiers_string_replace_iter_iter_pointer_pass_SOURCES = tests/wstring.modifiers/string_replace/iter_iter_pointer.pass.cpp
tests_wstring_modifiers_string_replace_iter_iter_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_iter_iter_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/iter_iter_pointer.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/size_size_string_size_size.pass
tests_wstring_modifiers_string_replace_size_size_string_size_size_pass_SOURCES = tests/wstring.modifiers/string_replace/size_size_string_size_size.pass.cpp
tests_wstring_modifiers_string_replace_size_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_size_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/size_size_string_size_size.pass

#check_PROGRAMS += tests/wstring.modifiers/string_replace/iter_iter_initializer_list.pass
#tests_wstring_modifiers_string_replace_iter_iter_initializer_list_pass_SOURCES = tests/wstring.modifiers/string_replace/iter_iter_initializer_list.pass.cpp
#tests_wstring_modifiers_string_replace_iter_iter_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_modifiers_string_replace_iter_iter_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.modifiers/string_replace/iter_iter_initializer_list.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/iter_iter_size_char.pass
tests_wstring_modifiers_string_replace_iter_iter_size_char_pass_SOURCES = tests/wstring.modifiers/string_replace/iter_iter_size_char.pass.cpp
tests_wstring_modifiers_string_replace_iter_iter_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_iter_iter_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/iter_iter_size_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/size_size_size_char.pass
tests_wstring_modifiers_string_replace_size_size_size_char_pass_SOURCES = tests/wstring.modifiers/string_replace/size_size_size_char.pass.cpp
tests_wstring_modifiers_string_replace_size_size_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_size_size_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/size_size_size_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/size_size_pointer.pass
tests_wstring_modifiers_string_replace_size_size_pointer_pass_SOURCES = tests/wstring.modifiers/string_replace/size_size_pointer.pass.cpp
tests_wstring_modifiers_string_replace_size_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_size_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/size_size_pointer.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/size_size_string.pass
tests_wstring_modifiers_string_replace_size_size_string_pass_SOURCES = tests/wstring.modifiers/string_replace/size_size_string.pass.cpp
tests_wstring_modifiers_string_replace_size_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_size_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/size_size_string.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/iter_iter_string.pass
tests_wstring_modifiers_string_replace_iter_iter_string_pass_SOURCES = tests/wstring.modifiers/string_replace/iter_iter_string.pass.cpp
tests_wstring_modifiers_string_replace_iter_iter_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_iter_iter_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/iter_iter_string.pass

check_PROGRAMS += tests/wstring.modifiers/string_replace/iter_iter_iter_iter.pass
tests_wstring_modifiers_string_replace_iter_iter_iter_iter_pass_SOURCES = tests/wstring.modifiers/string_replace/iter_iter_iter_iter.pass.cpp
tests_wstring_modifiers_string_replace_iter_iter_iter_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_replace_iter_iter_iter_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_replace/iter_iter_iter_iter.pass

check_PROGRAMS += tests/wstring.modifiers/string_copy/copy.pass
tests_wstring_modifiers_string_copy_copy_pass_SOURCES = tests/wstring.modifiers/string_copy/copy.pass.cpp
tests_wstring_modifiers_string_copy_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_copy_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_copy/copy.pass

check_PROGRAMS += tests/wstring.modifiers/string_assign/string.pass
tests_wstring_modifiers_string_assign_string_pass_SOURCES = tests/wstring.modifiers/string_assign/string.pass.cpp
tests_wstring_modifiers_string_assign_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_assign_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_assign/string.pass

check_PROGRAMS += tests/wstring.modifiers/string_assign/size_char.pass
tests_wstring_modifiers_string_assign_size_char_pass_SOURCES = tests/wstring.modifiers/string_assign/size_char.pass.cpp
tests_wstring_modifiers_string_assign_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_assign_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_assign/size_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_assign/pointer_size.pass
tests_wstring_modifiers_string_assign_pointer_size_pass_SOURCES = tests/wstring.modifiers/string_assign/pointer_size.pass.cpp
tests_wstring_modifiers_string_assign_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_assign_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_assign/pointer_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_assign/string_size_size.pass
tests_wstring_modifiers_string_assign_string_size_size_pass_SOURCES = tests/wstring.modifiers/string_assign/string_size_size.pass.cpp
tests_wstring_modifiers_string_assign_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_assign_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_assign/string_size_size.pass

# move str&&
# check_PROGRAMS += tests/wstring.modifiers/string_assign/rv_string.pass
# tests_wstring_modifiers_string_assign_rv_string_pass_SOURCES = tests/wstring.modifiers/string_assign/rv_string.pass.cpp
# tests_wstring_modifiers_string_assign_rv_string_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_modifiers_string_assign_rv_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.modifiers/string_assign/rv_string.pass

#check_PROGRAMS += tests/wstring.modifiers/string_assign/initializer_list.pass
#tests_wstring_modifiers_string_assign_initializer_list_pass_SOURCES = tests/wstring.modifiers/string_assign/initializer_list.pass.cpp
#tests_wstring_modifiers_string_assign_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_modifiers_string_assign_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.modifiers/string_assign/initializer_list.pass

check_PROGRAMS += tests/wstring.modifiers/string_assign/iterator.pass
tests_wstring_modifiers_string_assign_iterator_pass_SOURCES = tests/wstring.modifiers/string_assign/iterator.pass.cpp
tests_wstring_modifiers_string_assign_iterator_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_assign_iterator_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_assign/iterator.pass

check_PROGRAMS += tests/wstring.modifiers/string_assign/pointer.pass
tests_wstring_modifiers_string_assign_pointer_pass_SOURCES = tests/wstring.modifiers/string_assign/pointer.pass.cpp
tests_wstring_modifiers_string_assign_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_assign_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_assign/pointer.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/size_string_size_size.pass
tests_wstring_modifiers_string_insert_size_string_size_size_pass_SOURCES = tests/wstring.modifiers/string_insert/size_string_size_size.pass.cpp
tests_wstring_modifiers_string_insert_size_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_size_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/size_string_size_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/size_pointer_size.pass
tests_wstring_modifiers_string_insert_size_pointer_size_pass_SOURCES = tests/wstring.modifiers/string_insert/size_pointer_size.pass.cpp
tests_wstring_modifiers_string_insert_size_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_size_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/size_pointer_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/size_pointer.pass
tests_wstring_modifiers_string_insert_size_pointer_pass_SOURCES = tests/wstring.modifiers/string_insert/size_pointer.pass.cpp
tests_wstring_modifiers_string_insert_size_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_size_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/size_pointer.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/iter_char.pass
tests_wstring_modifiers_string_insert_iter_char_pass_SOURCES = tests/wstring.modifiers/string_insert/iter_char.pass.cpp
tests_wstring_modifiers_string_insert_iter_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_iter_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/iter_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/size_size_char.pass
tests_wstring_modifiers_string_insert_size_size_char_pass_SOURCES = tests/wstring.modifiers/string_insert/size_size_char.pass.cpp
tests_wstring_modifiers_string_insert_size_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_size_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/size_size_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/iter_iter_iter.pass
tests_wstring_modifiers_string_insert_iter_iter_iter_pass_SOURCES = tests/wstring.modifiers/string_insert/iter_iter_iter.pass.cpp
tests_wstring_modifiers_string_insert_iter_iter_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_iter_iter_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/iter_iter_iter.pass

#check_PROGRAMS += tests/wstring.modifiers/string_insert/iter_initializer_list.pass
#tests_wstring_modifiers_string_insert_iter_initializer_list_pass_SOURCES = tests/wstring.modifiers/string_insert/iter_initializer_list.pass.cpp
#tests_wstring_modifiers_string_insert_iter_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_modifiers_string_insert_iter_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.modifiers/string_insert/iter_initializer_list.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/iter_size_char.pass
tests_wstring_modifiers_string_insert_iter_size_char_pass_SOURCES = tests/wstring.modifiers/string_insert/iter_size_char.pass.cpp
tests_wstring_modifiers_string_insert_iter_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_iter_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/iter_size_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_insert/size_string.pass
tests_wstring_modifiers_string_insert_size_string_pass_SOURCES = tests/wstring.modifiers/string_insert/size_string.pass.cpp
tests_wstring_modifiers_string_insert_size_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_insert_size_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_insert/size_string.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/size_size.pass
tests_wstring_modifiers_string_erase_size_size_pass_SOURCES = tests/wstring.modifiers/string_erase/size_size.pass.cpp
tests_wstring_modifiers_string_erase_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/size_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/erase_iter_iter_db1.pass
tests_wstring_modifiers_string_erase_erase_iter_iter_db1_pass_SOURCES = tests/wstring.modifiers/string_erase/erase_iter_iter_db1.pass.cpp
tests_wstring_modifiers_string_erase_erase_iter_iter_db1_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_erase_iter_iter_db1_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/erase_iter_iter_db1.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/pop_back.pass
tests_wstring_modifiers_string_erase_pop_back_pass_SOURCES = tests/wstring.modifiers/string_erase/pop_back.pass.cpp
tests_wstring_modifiers_string_erase_pop_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_pop_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/pop_back.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/erase_iter_iter_db4.pass
tests_wstring_modifiers_string_erase_erase_iter_iter_db4_pass_SOURCES = tests/wstring.modifiers/string_erase/erase_iter_iter_db4.pass.cpp
tests_wstring_modifiers_string_erase_erase_iter_iter_db4_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_erase_iter_iter_db4_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/erase_iter_iter_db4.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/erase_iter_db2.pass
tests_wstring_modifiers_string_erase_erase_iter_db2_pass_SOURCES = tests/wstring.modifiers/string_erase/erase_iter_db2.pass.cpp
tests_wstring_modifiers_string_erase_erase_iter_db2_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_erase_iter_db2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/erase_iter_db2.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/iter_iter.pass
tests_wstring_modifiers_string_erase_iter_iter_pass_SOURCES = tests/wstring.modifiers/string_erase/iter_iter.pass.cpp
tests_wstring_modifiers_string_erase_iter_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_iter_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/iter_iter.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/erase_iter_db1.pass
tests_wstring_modifiers_string_erase_erase_iter_db1_pass_SOURCES = tests/wstring.modifiers/string_erase/erase_iter_db1.pass.cpp
tests_wstring_modifiers_string_erase_erase_iter_db1_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_erase_iter_db1_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/erase_iter_db1.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/erase_iter_iter_db3.pass
tests_wstring_modifiers_string_erase_erase_iter_iter_db3_pass_SOURCES = tests/wstring.modifiers/string_erase/erase_iter_iter_db3.pass.cpp
tests_wstring_modifiers_string_erase_erase_iter_iter_db3_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_erase_iter_iter_db3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/erase_iter_iter_db3.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/erase_iter_iter_db2.pass
tests_wstring_modifiers_string_erase_erase_iter_iter_db2_pass_SOURCES = tests/wstring.modifiers/string_erase/erase_iter_iter_db2.pass.cpp
tests_wstring_modifiers_string_erase_erase_iter_iter_db2_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_erase_iter_iter_db2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/erase_iter_iter_db2.pass

check_PROGRAMS += tests/wstring.modifiers/string_erase/iter.pass
tests_wstring_modifiers_string_erase_iter_pass_SOURCES = tests/wstring.modifiers/string_erase/iter.pass.cpp
tests_wstring_modifiers_string_erase_iter_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_erase_iter_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_erase/iter.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/string.pass
tests_wstring_modifiers_string_append_string_pass_SOURCES = tests/wstring.modifiers/string_append/string.pass.cpp
tests_wstring_modifiers_string_append_string_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/string.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/size_char.pass
tests_wstring_modifiers_string_append_size_char_pass_SOURCES = tests/wstring.modifiers/string_append/size_char.pass.cpp
tests_wstring_modifiers_string_append_size_char_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_size_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/size_char.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/pointer_size.pass
tests_wstring_modifiers_string_append_pointer_size_pass_SOURCES = tests/wstring.modifiers/string_append/pointer_size.pass.cpp
tests_wstring_modifiers_string_append_pointer_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_pointer_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/pointer_size.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/string_size_size.pass
tests_wstring_modifiers_string_append_string_size_size_pass_SOURCES = tests/wstring.modifiers/string_append/string_size_size.pass.cpp
tests_wstring_modifiers_string_append_string_size_size_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_string_size_size_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/string_size_size.pass

#check_PROGRAMS += tests/wstring.modifiers/string_append/initializer_list.pass
#tests_wstring_modifiers_string_append_initializer_list_pass_SOURCES = tests/wstring.modifiers/string_append/initializer_list.pass.cpp
#tests_wstring_modifiers_string_append_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_modifiers_string_append_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.modifiers/string_append/initializer_list.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/iterator.pass
tests_wstring_modifiers_string_append_iterator_pass_SOURCES = tests/wstring.modifiers/string_append/iterator.pass.cpp
tests_wstring_modifiers_string_append_iterator_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_iterator_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/iterator.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/push_back.pass
tests_wstring_modifiers_string_append_push_back_pass_SOURCES = tests/wstring.modifiers/string_append/push_back.pass.cpp
tests_wstring_modifiers_string_append_push_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_push_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/push_back.pass

check_PROGRAMS += tests/wstring.modifiers/string_append/pointer.pass
tests_wstring_modifiers_string_append_pointer_pass_SOURCES = tests/wstring.modifiers/string_append/pointer.pass.cpp
tests_wstring_modifiers_string_append_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_modifiers_string_append_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.modifiers/string_append/pointer.pass

#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/compare.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_lt_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_lt_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_lt_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign2_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_find_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_find_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_find_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_move_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_move_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_copy_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eof_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign3_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_assign3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_int_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_int_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_eq_int_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_length_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_char_type_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_char_type_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_to_char_type_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass
#
#check_PROGRAMS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_compare_pass_SOURCES = tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass.cpp
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_compare_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_char_traits_char_traits_specializations_char_traits_specializations_char_compare_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoi.pass
#tests_strings_string_conversions_stoi_pass_SOURCES = tests/strings/string.conversions/stoi.pass.cpp
#tests_strings_string_conversions_stoi_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoi_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoi.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stof.pass
#tests_strings_string_conversions_stof_pass_SOURCES = tests/strings/string.conversions/stof.pass.cpp
#tests_strings_string_conversions_stof_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stof_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stof.pass
#
#check_PROGRAMS += tests/strings/string.conversions/to_wstring.pass
#tests_strings_string_conversions_to_wstring_pass_SOURCES = tests/strings/string.conversions/to_wstring.pass.cpp
#tests_strings_string_conversions_to_wstring_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_to_wstring_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/to_wstring.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoll.pass
#tests_strings_string_conversions_stoll_pass_SOURCES = tests/strings/string.conversions/stoll.pass.cpp
#tests_strings_string_conversions_stoll_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoll_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoll.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoull.pass
#tests_strings_string_conversions_stoull_pass_SOURCES = tests/strings/string.conversions/stoull.pass.cpp
#tests_strings_string_conversions_stoull_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoull_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoull.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stod.pass
#tests_strings_string_conversions_stod_pass_SOURCES = tests/strings/string.conversions/stod.pass.cpp
#tests_strings_string_conversions_stod_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stod_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stod.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stol.pass
#tests_strings_string_conversions_stol_pass_SOURCES = tests/strings/string.conversions/stol.pass.cpp
#tests_strings_string_conversions_stol_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stol_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stol.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stold.pass
#tests_strings_string_conversions_stold_pass_SOURCES = tests/strings/string.conversions/stold.pass.cpp
#tests_strings_string_conversions_stold_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stold_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stold.pass
#
#check_PROGRAMS += tests/strings/string.conversions/stoul.pass
#tests_strings_string_conversions_stoul_pass_SOURCES = tests/strings/string.conversions/stoul.pass.cpp
#tests_strings_string_conversions_stoul_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_stoul_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/stoul.pass
#
#check_PROGRAMS += tests/strings/string.conversions/to_string.pass
#tests_strings_string_conversions_to_string_pass_SOURCES = tests/strings/string.conversions/to_string.pass.cpp
#tests_strings_string_conversions_to_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_conversions_to_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.conversions/to_string.pass
#
#check_PROGRAMS += tests.literals/literal.pass
#tests_literals_literal_pass_SOURCES = tests.literals/literal.pass.cpp
#tests_literals_literal_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal.pass
#
#check_PROGRAMS += tests.literals/literal2.pass
#tests_literals_literal2_pass_SOURCES = tests.literals/literal2.pass.cpp
#tests_literals_literal2_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal2.pass
#
#check_PROGRAMS += tests.literals/literal1.pass
#tests_literals_literal1_pass_SOURCES = tests.literals/literal1.pass.cpp
#tests_literals_literal1_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal1_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal1.pass
#
#check_PROGRAMS += tests.literals/literal3.pass
#tests_literals_literal3_pass_SOURCES = tests.literals/literal3.pass.cpp
#tests_literals_literal3_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_literals_literal3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.literals/literal3.pass
#
#check_PROGRAMS += tests.hash/strings.pass
#tests_hash_strings_pass_SOURCES = tests.hash/strings.pass.cpp
#tests_hash_strings_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_hash_strings_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests.hash/strings.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_incr/string_pointer.pass
#tests_wstring_nonmembers_string_op_incr_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_op_incr/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_op_incr_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_incr_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_incr/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_incr/string_char.pass
#tests_wstring_nonmembers_string_op_incr_string_char_pass_SOURCES = tests/wstring.nonmembers/string_op_incr/string_char.pass.cpp
#tests_wstring_nonmembers_string_op_incr_string_char_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_incr_string_char_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_incr/string_char.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_incr/char_string.pass
#tests_wstring_nonmembers_string_op_incr_char_string_pass_SOURCES = tests/wstring.nonmembers/string_op_incr/char_string.pass.cpp
#tests_wstring_nonmembers_string_op_incr_char_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_incr_char_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_incr/char_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_incr/string_string.pass
#tests_wstring_nonmembers_string_op_incr_string_string_pass_SOURCES = tests/wstring.nonmembers/string_op_incr/string_string.pass.cpp
#tests_wstring_nonmembers_string_op_incr_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_incr_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_incr/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_incr/pointer_string.pass
#tests_wstring_nonmembers_string_op_incr_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_op_incr/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_op_incr_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_incr_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_incr/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.io/get_line.pass
#tests_wstring_nonmembers_string_io_get_line_pass_SOURCES = tests/wstring.nonmembers/string.io/get_line.pass.cpp
#tests_wstring_nonmembers_string_io_get_line_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_io_get_line_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.io/get_line.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.io/get_line_delim_rv.pass
#tests_wstring_nonmembers_string_io_get_line_delim_rv_pass_SOURCES = tests/wstring.nonmembers/string.io/get_line_delim_rv.pass.cpp
#tests_wstring_nonmembers_string_io_get_line_delim_rv_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_io_get_line_delim_rv_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.io/get_line_delim_rv.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.io/stream_insert.pass
#tests_wstring_nonmembers_string_io_stream_insert_pass_SOURCES = tests/wstring.nonmembers/string.io/stream_insert.pass.cpp
#tests_wstring_nonmembers_string_io_stream_insert_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_io_stream_insert_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.io/stream_insert.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.io/get_line_rv.pass
#tests_wstring_nonmembers_string_io_get_line_rv_pass_SOURCES = tests/wstring.nonmembers/string.io/get_line_rv.pass.cpp
#tests_wstring_nonmembers_string_io_get_line_rv_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_io_get_line_rv_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.io/get_line_rv.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.io/stream_extract.pass
#tests_wstring_nonmembers_string_io_stream_extract_pass_SOURCES = tests/wstring.nonmembers/string.io/stream_extract.pass.cpp
#tests_wstring_nonmembers_string_io_stream_extract_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_io_stream_extract_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.io/stream_extract.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.io/get_line_delim.pass
#tests_wstring_nonmembers_string_io_get_line_delim_pass_SOURCES = tests/wstring.nonmembers/string.io/get_line_delim.pass.cpp
#tests_wstring_nonmembers_string_io_get_line_delim_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_io_get_line_delim_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.io/get_line_delim.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_ne/string_pointer.pass
#tests_wstring_nonmembers_string_op_ne_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_op_ne/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_op_ne_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_ne_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_ne/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_ne/string_string.pass
#tests_wstring_nonmembers_string_op_ne_string_string_pass_SOURCES = tests/wstring.nonmembers/string_op_ne/string_string.pass.cpp
#tests_wstring_nonmembers_string_op_ne_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_ne_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_ne/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_ne/pointer_string.pass
#tests_wstring_nonmembers_string_op_ne_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_op_ne/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_op_ne_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_ne_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_ne/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_opgt/string_pointer.pass
#tests_wstring_nonmembers_string_opgt_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_opgt/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_opgt_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_opgt_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_opgt/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_opgt/string_string.pass
#tests_wstring_nonmembers_string_opgt_string_string_pass_SOURCES = tests/wstring.nonmembers/string_opgt/string_string.pass.cpp
#tests_wstring_nonmembers_string_opgt_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_opgt_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_opgt/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_opgt/pointer_string.pass
#tests_wstring_nonmembers_string_opgt_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_opgt/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_opgt_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_opgt_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_opgt/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_ge/string_pointer.pass
#tests_wstring_nonmembers_string_op_ge_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_op_ge/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_op_ge_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_ge_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_ge/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_ge/string_string.pass
#tests_wstring_nonmembers_string_op_ge_string_string_pass_SOURCES = tests/wstring.nonmembers/string_op_ge/string_string.pass.cpp
#tests_wstring_nonmembers_string_op_ge_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_ge_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_ge/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_ge/pointer_string.pass
#tests_wstring_nonmembers_string_op_ge_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_op_ge/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_op_ge_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_ge_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_ge/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_le/string_pointer.pass
#tests_wstring_nonmembers_string_op_le_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_op_le/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_op_le_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_le_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_le/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_le/string_string.pass
#tests_wstring_nonmembers_string_op_le_string_string_pass_SOURCES = tests/wstring.nonmembers/string_op_le/string_string.pass.cpp
#tests_wstring_nonmembers_string_op_le_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_le_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_le/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_op_le/pointer_string.pass
#tests_wstring_nonmembers_string_op_le_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_op_le/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_op_le_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_op_le_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_op_le/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_oplt/string_pointer.pass
#tests_wstring_nonmembers_string_oplt_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_oplt/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_oplt_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_oplt_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_oplt/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_oplt/string_string.pass
#tests_wstring_nonmembers_string_oplt_string_string_pass_SOURCES = tests/wstring.nonmembers/string_oplt/string_string.pass.cpp
#tests_wstring_nonmembers_string_oplt_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_oplt_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_oplt/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_oplt/pointer_string.pass
#tests_wstring_nonmembers_string_oplt_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_oplt/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_oplt_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_oplt_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_oplt/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_operator_eq/string_pointer.pass
#tests_wstring_nonmembers_string_operator_eq_string_pointer_pass_SOURCES = tests/wstring.nonmembers/string_operator_eq/string_pointer.pass.cpp
#tests_wstring_nonmembers_string_operator_eq_string_pointer_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_operator_eq_string_pointer_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_operator_eq/string_pointer.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_operator_eq/string_string.pass
#tests_wstring_nonmembers_string_operator_eq_string_string_pass_SOURCES = tests/wstring.nonmembers/string_operator_eq/string_string.pass.cpp
#tests_wstring_nonmembers_string_operator_eq_string_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_operator_eq_string_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_operator_eq/string_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string_operator_eq/pointer_string.pass
#tests_wstring_nonmembers_string_operator_eq_pointer_string_pass_SOURCES = tests/wstring.nonmembers/string_operator_eq/pointer_string.pass.cpp
#tests_wstring_nonmembers_string_operator_eq_pointer_string_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_operator_eq_pointer_string_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string_operator_eq/pointer_string.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.special/swap_noexcept.pass
#tests_wstring_nonmembers_string_special_swap_noexcept_pass_SOURCES = tests/wstring.nonmembers/string.special/swap_noexcept.pass.cpp
#tests_wstring_nonmembers_string_special_swap_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_special_swap_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.special/swap_noexcept.pass
#
#check_PROGRAMS += tests/wstring.nonmembers/string.special/swap.pass
#tests_wstring_nonmembers_string_special_swap_pass_SOURCES = tests/wstring.nonmembers/string.special/swap.pass.cpp
#tests_wstring_nonmembers_string_special_swap_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_wstring_nonmembers_string_special_swap_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/wstring.nonmembers/string.special/swap.pass
#
check_PROGRAMS += tests/wstring.iterators/begin.pass
tests_wstring_iterators_begin_pass_SOURCES = tests/wstring.iterators/begin.pass.cpp
tests_wstring_iterators_begin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_begin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/begin.pass

check_PROGRAMS += tests/wstring.iterators/end.pass
tests_wstring_iterators_end_pass_SOURCES = tests/wstring.iterators/end.pass.cpp
tests_wstring_iterators_end_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_end_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/end.pass

check_PROGRAMS += tests/wstring.iterators/iterators.pass
tests_wstring_iterators_iterators_pass_SOURCES = tests/wstring.iterators/iterators.pass.cpp
tests_wstring_iterators_iterators_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_iterators_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/iterators.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_2.pass
tests_wstring_iterators_db_iterators_2_pass_SOURCES = tests/wstring.iterators/db_iterators_2.pass.cpp
tests_wstring_iterators_db_iterators_2_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_2_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_2.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_3.pass
tests_wstring_iterators_db_iterators_3_pass_SOURCES = tests/wstring.iterators/db_iterators_3.pass.cpp
tests_wstring_iterators_db_iterators_3_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_3_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_3.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_4.pass
tests_wstring_iterators_db_iterators_4_pass_SOURCES = tests/wstring.iterators/db_iterators_4.pass.cpp
tests_wstring_iterators_db_iterators_4_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_4_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_4.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_5.pass
tests_wstring_iterators_db_iterators_5_pass_SOURCES = tests/wstring.iterators/db_iterators_5.pass.cpp
tests_wstring_iterators_db_iterators_5_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_5_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_5.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_6.pass
tests_wstring_iterators_db_iterators_6_pass_SOURCES = tests/wstring.iterators/db_iterators_6.pass.cpp
tests_wstring_iterators_db_iterators_6_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_6_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_6.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_7.pass
tests_wstring_iterators_db_iterators_7_pass_SOURCES = tests/wstring.iterators/db_iterators_7.pass.cpp
tests_wstring_iterators_db_iterators_7_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_7_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_7.pass

check_PROGRAMS += tests/wstring.iterators/db_iterators_8.pass
tests_wstring_iterators_db_iterators_8_pass_SOURCES = tests/wstring.iterators/db_iterators_8.pass.cpp
tests_wstring_iterators_db_iterators_8_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_db_iterators_8_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/db_iterators_8.pass

check_PROGRAMS += tests/wstring.iterators/cbegin.pass
tests_wstring_iterators_cbegin_pass_SOURCES = tests/wstring.iterators/cbegin.pass.cpp
tests_wstring_iterators_cbegin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_cbegin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/cbegin.pass

check_PROGRAMS += tests/wstring.iterators/cend.pass
tests_wstring_iterators_cend_pass_SOURCES = tests/wstring.iterators/cend.pass.cpp
tests_wstring_iterators_cend_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_cend_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/cend.pass

check_PROGRAMS += tests/wstring.iterators/rbegin.pass
tests_wstring_iterators_rbegin_pass_SOURCES = tests/wstring.iterators/rbegin.pass.cpp
tests_wstring_iterators_rbegin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_rbegin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/rbegin.pass

check_PROGRAMS += tests/wstring.iterators/rend.pass
tests_wstring_iterators_rend_pass_SOURCES = tests/wstring.iterators/rend.pass.cpp
tests_wstring_iterators_rend_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_rend_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/rend.pass

check_PROGRAMS += tests/wstring.iterators/crbegin.pass
tests_wstring_iterators_crbegin_pass_SOURCES = tests/wstring.iterators/crbegin.pass.cpp
tests_wstring_iterators_crbegin_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_crbegin_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/crbegin.pass

check_PROGRAMS += tests/wstring.iterators/crend.pass
tests_wstring_iterators_crend_pass_SOURCES = tests/wstring.iterators/crend.pass.cpp
tests_wstring_iterators_crend_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_iterators_crend_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.iterators/crend.pass

#check_PROGRAMS += tests/types.pass
#tests_types_pass_SOURCES = tests/types.pass.cpp
#tests_types_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_types_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/types.pass

check_PROGRAMS += tests/wstring.cons/pointer_size_alloc.pass
tests_wstring_cons_pointer_size_alloc_pass_SOURCES = tests/wstring.cons/pointer_size_alloc.pass.cpp
tests_wstring_cons_pointer_size_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_pointer_size_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/pointer_size_alloc.pass

check_PROGRAMS += tests/wstring.cons/pointer_assignment.pass
tests_wstring_cons_pointer_assignment_pass_SOURCES = tests/wstring.cons/pointer_assignment.pass.cpp
tests_wstring_cons_pointer_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_pointer_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/pointer_assignment.pass

# check_PROGRAMS += tests/wstring.cons/initializer_list_assignment.pass
# tests_wstring_cons_initializer_list_assignment_pass_SOURCES = tests/wstring.cons/initializer_list_assignment.pass.cpp
# tests_wstring_cons_initializer_list_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_initializer_list_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/initializer_list_assignment.pass

check_PROGRAMS += tests/wstring.cons/copy_assignment.pass
tests_wstring_cons_copy_assignment_pass_SOURCES = tests/wstring.cons/copy_assignment.pass.cpp
tests_wstring_cons_copy_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_copy_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/copy_assignment.pass

# check_PROGRAMS += tests/wstring.cons/move.pass
# tests_wstring_cons_move_pass_SOURCES = tests/wstring.cons/move.pass.cpp
# tests_wstring_cons_move_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_move_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/move.pass

check_PROGRAMS += tests/wstring.cons/copy.pass
tests_wstring_cons_copy_pass_SOURCES = tests/wstring.cons/copy.pass.cpp
tests_wstring_cons_copy_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_copy_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/copy.pass

# check_PROGRAMS += tests/wstring.cons/alloc.pass
# tests_wstring_cons_alloc_pass_SOURCES = tests/wstring.cons/alloc.pass.cpp
# tests_wstring_cons_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/alloc.pass

# check_PROGRAMS += tests/wstring.cons/move_assign_noexcept.pass
# tests_wstring_cons_move_assign_noexcept_pass_SOURCES = tests/wstring.cons/move_assign_noexcept.pass.cpp
# tests_wstring_cons_move_assign_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_move_assign_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/move_assign_noexcept.pass

# check_PROGRAMS += tests/wstring.cons/initializer_list.pass
# tests_wstring_cons_initializer_list_pass_SOURCES = tests/wstring.cons/initializer_list.pass.cpp
# tests_wstring_cons_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/initializer_list.pass

# check_PROGRAMS += tests/wstring.cons/iter_alloc.pass
# tests_wstring_cons_iter_alloc_pass_SOURCES = tests/wstring.cons/iter_alloc.pass.cpp
# tests_wstring_cons_iter_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_iter_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/iter_alloc.pass

check_PROGRAMS += tests/wstring.cons/char_assignment.pass
tests_wstring_cons_char_assignment_pass_SOURCES = tests/wstring.cons/char_assignment.pass.cpp
tests_wstring_cons_char_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_char_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/char_assignment.pass

check_PROGRAMS += tests/wstring.cons/pointer_alloc.pass
tests_wstring_cons_pointer_alloc_pass_SOURCES = tests/wstring.cons/pointer_alloc.pass.cpp
tests_wstring_cons_pointer_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_pointer_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/pointer_alloc.pass

# check_PROGRAMS += tests/wstring.cons/move_noexcept.pass
# tests_wstring_cons_move_noexcept_pass_SOURCES = tests/wstring.cons/move_noexcept.pass.cpp
# tests_wstring_cons_move_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_move_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/move_noexcept.pass

# check_PROGRAMS += tests/wstring.cons/move_assignment.pass
# tests_wstring_cons_move_assignment_pass_SOURCES = tests/wstring.cons/move_assignment.pass.cpp
# tests_wstring_cons_move_assignment_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_move_assignment_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/move_assignment.pass

# check_PROGRAMS += tests/wstring.cons/dtor_noexcept.pass
# tests_wstring_cons_dtor_noexcept_pass_SOURCES = tests/wstring.cons/dtor_noexcept.pass.cpp
# tests_wstring_cons_dtor_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_dtor_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/dtor_noexcept.pass

# check_PROGRAMS += tests/wstring.cons/move_alloc.pass
# tests_wstring_cons_move_alloc_pass_SOURCES = tests/wstring.cons/move_alloc.pass.cpp
# tests_wstring_cons_move_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_move_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/move_alloc.pass

check_PROGRAMS += tests/wstring.cons/size_char_alloc.pass
tests_wstring_cons_size_char_alloc_pass_SOURCES = tests/wstring.cons/size_char_alloc.pass.cpp
tests_wstring_cons_size_char_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_size_char_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/size_char_alloc.pass

check_PROGRAMS += tests/wstring.cons/substr.pass
tests_wstring_cons_substr_pass_SOURCES = tests/wstring.cons/substr.pass.cpp
tests_wstring_cons_substr_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_substr_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/substr.pass

# check_PROGRAMS += tests/wstring.cons/default_noexcept.pass
# tests_wstring_cons_default_noexcept_pass_SOURCES = tests/wstring.cons/default_noexcept.pass.cpp
# tests_wstring_cons_default_noexcept_pass_LDADD = libbarefootc.a liblog4stdc.a
# tests_wstring_cons_default_noexcept_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
# TESTS += tests/wstring.cons/default_noexcept.pass

check_PROGRAMS += tests/wstring.cons/copy_alloc.pass
tests_wstring_cons_copy_alloc_pass_SOURCES = tests/wstring.cons/copy_alloc.pass.cpp
tests_wstring_cons_copy_alloc_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_cons_copy_alloc_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.cons/copy_alloc.pass

check_PROGRAMS += tests/wstring.access/back.pass
tests_wstring_access_back_pass_SOURCES = tests/wstring.access/back.pass.cpp
tests_wstring_access_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/back.pass

check_PROGRAMS += tests/wstring.access/at.pass
tests_wstring_access_at_pass_SOURCES = tests/wstring.access/at.pass.cpp
tests_wstring_access_at_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_at_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/at.pass

check_PROGRAMS += tests/wstring.access/db_cback.pass
tests_wstring_access_db_cback_pass_SOURCES = tests/wstring.access/db_cback.pass.cpp
tests_wstring_access_db_cback_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_db_cback_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/db_cback.pass

check_PROGRAMS += tests/wstring.access/db_back.pass
tests_wstring_access_db_back_pass_SOURCES = tests/wstring.access/db_back.pass.cpp
tests_wstring_access_db_back_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_db_back_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/db_back.pass

check_PROGRAMS += tests/wstring.access/db_cfront.pass
tests_wstring_access_db_cfront_pass_SOURCES = tests/wstring.access/db_cfront.pass.cpp
tests_wstring_access_db_cfront_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_db_cfront_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/db_cfront.pass

check_PROGRAMS += tests/wstring.access/db_cindex.pass
tests_wstring_access_db_cindex_pass_SOURCES = tests/wstring.access/db_cindex.pass.cpp
tests_wstring_access_db_cindex_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_db_cindex_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/db_cindex.pass

check_PROGRAMS += tests/wstring.access/db_index.pass
tests_wstring_access_db_index_pass_SOURCES = tests/wstring.access/db_index.pass.cpp
tests_wstring_access_db_index_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_db_index_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/db_index.pass

check_PROGRAMS += tests/wstring.access/front.pass
tests_wstring_access_front_pass_SOURCES = tests/wstring.access/front.pass.cpp
tests_wstring_access_front_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_front_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/front.pass

check_PROGRAMS += tests/wstring.access/index.pass
tests_wstring_access_index_pass_SOURCES = tests/wstring.access/index.pass.cpp
tests_wstring_access_index_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_index_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/index.pass

check_PROGRAMS += tests/wstring.access/db_front.pass
tests_wstring_access_db_front_pass_SOURCES = tests/wstring.access/db_front.pass.cpp
tests_wstring_access_db_front_pass_LDADD = libbarefootc.a liblog4stdc.a
tests_wstring_access_db_front_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/wstring.access/db_front.pass

check_PROGRAMS += tests/shared_string.erase/erase_seq
tests_shared_string_erase_erase_seq_SOURCES = tests/shared_string.erase/erase_seq.c
tests_shared_string_erase_erase_seq_LDADD = libbarefootc.a liblog4stdc.a
tests_shared_string_erase_erase_seq_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/shared_string.erase/erase_seq

check_PROGRAMS += tests/shared_string.erase/erase_char
tests_shared_string_erase_erase_char_SOURCES = tests/shared_string.erase/erase_char.c
tests_shared_string_erase_erase_char_LDADD = libbarefootc.a liblog4stdc.a
tests_shared_string_erase_erase_char_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/shared_string.erase/erase_char

check_PROGRAMS += tests/shared_string.erase/erase_range
tests_shared_string_erase_erase_range_SOURCES = tests/shared_string.erase/erase_range.c
tests_shared_string_erase_erase_range_LDADD = libbarefootc.a liblog4stdc.a
tests_shared_string_erase_erase_range_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/shared_string.erase/erase_range

check_PROGRAMS += tests/datetime/init_isotime
tests_datetime_init_isotime_SOURCES = tests/datetime/init_isotime.c
tests_datetime_init_isotime_LDADD = libbarefootc.a liblog4stdc.a
tests_datetime_init_isotime_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/datetime/init_isotime

check_PROGRAMS += tests/datetime/secs_between
tests_datetime_secs_between_SOURCES = tests/datetime/secs_between.c
tests_datetime_secs_between_LDADD = libbarefootc.a liblog4stdc.a
tests_datetime_secs_between_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/datetime/secs_between

check_PROGRAMS += tests/datetime/msecs_between
tests_datetime_msecs_between_SOURCES = tests/datetime/msecs_between.c
tests_datetime_msecs_between_LDADD = libbarefootc.a liblog4stdc.a
tests_datetime_msecs_between_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/datetime/msecs_between

check_PROGRAMS += tests/datetime/usecs_between
tests_datetime_usecs_between_SOURCES = tests/datetime/usecs_between.c
tests_datetime_usecs_between_LDADD = libbarefootc.a liblog4stdc.a
tests_datetime_usecs_between_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/datetime/usecs_between

check_PROGRAMS += tests/datetime/nsecs_between
tests_datetime_nsecs_between_SOURCES = tests/datetime/nsecs_between.c
tests_datetime_nsecs_between_LDADD = libbarefootc.a liblog4stdc.a
tests_datetime_nsecs_between_DEPENDENCIES = libbarefootc.a liblog4stdc.a
TESTS += tests/datetime/nsecs_between

#check_PROGRAMS += tests/strings/version.pass
#tests_strings_version_pass_SOURCES = tests/strings/version.pass.cpp
#tests_strings_version_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_version_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/version.pass
#
#check_PROGRAMS += tests/strings/c.strings/version_cwctype.pass
#tests_strings_c_strings_version_cwctype_pass_SOURCES = tests/strings/c.strings/version_cwctype.pass.cpp
#tests_strings_c_strings_version_cwctype_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_version_cwctype_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/version_cwctype.pass
#
#check_PROGRAMS += tests/strings/c.strings/cctype.pass
#tests_strings_c_strings_cctype_pass_SOURCES = tests/strings/c.strings/cctype.pass.cpp
#tests_strings_c_strings_cctype_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_cctype_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/cctype.pass
#
#check_PROGRAMS += tests/strings/c.strings/cwctype.pass
#tests_strings_c_strings_cwctype_pass_SOURCES = tests/strings/c.strings/cwctype.pass.cpp
#tests_strings_c_strings_cwctype_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_cwctype_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/cwctype.pass
#
#check_PROGRAMS += tests/strings/c.strings/cstring.pass
#tests_strings_c_strings_cstring_pass_SOURCES = tests/strings/c.strings/cstring.pass.cpp
#tests_strings_c_strings_cstring_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_cstring_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/cstring.pass
#
#check_PROGRAMS += tests/strings/c.strings/cwchar.pass
#tests_strings_c_strings_cwchar_pass_SOURCES = tests/strings/c.strings/cwchar.pass.cpp
#tests_strings_c_strings_cwchar_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_cwchar_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/cwchar.pass
#
#check_PROGRAMS += tests/strings/c.strings/version_cwchar.pass
#tests_strings_c_strings_version_cwchar_pass_SOURCES = tests/strings/c.strings/version_cwchar.pass.cpp
#tests_strings_c_strings_version_cwchar_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_version_cwchar_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/version_cwchar.pass
#
#check_PROGRAMS += tests/strings/c.strings/version_cstring.pass
#tests_strings_c_strings_version_cstring_pass_SOURCES = tests/strings/c.strings/version_cstring.pass.cpp
#tests_strings_c_strings_version_cstring_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_version_cstring_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/version_cstring.pass
#
#check_PROGRAMS += tests/strings/c.strings/version_cctype.pass
#tests_strings_c_strings_version_cctype_pass_SOURCES = tests/strings/c.strings/version_cctype.pass.cpp
#tests_strings_c_strings_version_cctype_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_version_cctype_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/version_cctype.pass
#
#check_PROGRAMS += tests/strings/c.strings/version_cuchar.pass
#tests_strings_c_strings_version_cuchar_pass_SOURCES = tests/strings/c.strings/version_cuchar.pass.cpp
#tests_strings_c_strings_version_cuchar_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_version_cuchar_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/version_cuchar.pass
#
#check_PROGRAMS += tests/strings/c.strings/cuchar.pass
#tests_strings_c_strings_cuchar_pass_SOURCES = tests/strings/c.strings/cuchar.pass.cpp
#tests_strings_c_strings_cuchar_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_c_strings_cuchar_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/c.strings/cuchar.pass
#
#check_PROGRAMS += tests/strings/string.classes/typedefs.pass
#tests_strings_string_classes_typedefs_pass_SOURCES = tests/strings/string.classes/typedefs.pass.cpp
#tests_strings_string_classes_typedefs_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_strings_string_classes_typedefs_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/strings/string.classes/typedefs.pass
#
#check_PROGRAMS += tests/string.ops/string.accessors/get_allocator.pass
#tests_string_ops_string_accessors_get_allocator_pass_SOURCES = tests/string.ops/string.accessors/get_allocator.pass.cpp
#tests_string_ops_string_accessors_get_allocator_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_ops_string_accessors_get_allocator_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.ops/string.accessors/get_allocator.pass
#
#check_PROGRAMS += tests/string.capacity/capacity.pass
#tests_string_capacity_capacity_pass_SOURCES = tests/string.capacity/capacity.pass.cpp
#tests_string_capacity_capacity_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_capacity_capacity_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.capacity/capacity.pass
#
#check_PROGRAMS += tests/string.modifiers/string_op_plus_equal/initializer_list.pass
#tests_string_modifiers_string_op_plus_equal_initializer_list_pass_SOURCES = tests/string.modifiers/string_op_plus_equal/initializer_list.pass.cpp
#tests_string_modifiers_string_op_plus_equal_initializer_list_pass_LDADD = libbarefootc.a liblog4stdc.a
#tests_string_modifiers_string_op_plus_equal_initializer_list_pass_DEPENDENCIES = libbarefootc.a liblog4stdc.a
#TESTS += tests/string.modifiers/string_op_plus_equal/initializer_list.pass
#
