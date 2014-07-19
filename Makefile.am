AUTOMAKE_OPTIONS = subdir-objects

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

AM_CPPFLAGS = -Ilog4stdc
