AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Ilog4stdc

lib_LIBRARIES = liblog4stdc.a
include_HEADERS = log4stdc/log4stdc.h

noinst_PROGRAMS = testlink

check_PROGRAMS =

liblog4stdc_a_SOURCES = log4stdc/log4stdc.h
liblog4stdc_a_SOURCES+= log4stdc/logobjects.h

liblog4stdc_a_SOURCES+= barefootc/object.h
liblog4stdc_a_SOURCES+= barefootc/mempool.h
liblog4stdc_a_SOURCES+= barefootc/synchronization.h
liblog4stdc_a_SOURCES+= barefootc/atomic.h
liblog4stdc_a_SOURCES+= barefootc/linkedlist.h

liblog4stdc_a_SOURCES+= log4stdc/l4sc_logprintf.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_vlog.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_log.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_logger_enabled.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_formatmsg.c

liblog4stdc_a_SOURCES+= log4stdc/logger.c
liblog4stdc_a_SOURCES+= log4stdc/appender.c
liblog4stdc_a_SOURCES+= log4stdc/fileappender.c
liblog4stdc_a_SOURCES+= log4stdc/layout.c
liblog4stdc_a_SOURCES+= log4stdc/logmessage.c
liblog4stdc_a_SOURCES+= log4stdc/propertyconfigurator.c
liblog4stdc_a_SOURCES+= log4stdc/object.c
liblog4stdc_a_SOURCES+= log4stdc/l4stdcpool.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_posix_mutex.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_win32_mutex.c
liblog4stdc_a_SOURCES+= log4stdc/xmlconfigurator.c

if BFCLOG4CXX
liblog4stdc_a_SOURCES+= log4cxx/repository.h
liblog4stdc_a_SOURCES+= log4cxx/repository.cpp
liblog4stdc_a_SOURCES+= log4cxx/logger.cpp
liblog4stdc_a_SOURCES+= log4cxx/level.cpp
liblog4stdc_a_SOURCES+= log4cxx/objectptr.cpp
liblog4stdc_a_SOURCES+= log4cxx/objectimpl.cpp
liblog4stdc_a_SOURCES+= log4cxx/class.cpp
liblog4stdc_a_SOURCES+= log4cxx/classregistration.cpp
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

testlink_SOURCES = testlink.cpp
testlink_LDADD = liblog4stdc.a
testlink_DEPENDENCIES = liblog4stdc.a

TESTS = testlink

noinst_PROGRAMS += examples/barefootc_log_redirect
examples_barefootc_log_redirect_SOURCES = examples/barefootc_log_redirect.c
examples_barefootc_log_redirect_LDADD = liblog4stdc.a
examples_barefootc_log_redirect_DEPENDENCIES = liblog4stdc.a
TESTS += examples/barefootc_log_redirect

check_PROGRAMS += tests/log4stdc/configure.xml/level
tests_log4stdc_configure_xml_level_SOURCES = tests/log4stdc/configure.xml/level.c
tests_log4stdc_configure_xml_level_LDADD = liblog4stdc.a
tests_log4stdc_configure_xml_level_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/configure.xml/level

check_PROGRAMS += tests/log4stdc/configure.xml/largefile
tests_log4stdc_configure_xml_largefile_SOURCES = tests/log4stdc/configure.xml/largefile.c
tests_log4stdc_configure_xml_largefile_LDADD = liblog4stdc.a
tests_log4stdc_configure_xml_largefile_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/configure.xml/largefile

check_PROGRAMS += tests/log4stdc/configure.prop/level
tests_log4stdc_configure_prop_level_SOURCES = tests/log4stdc/configure.prop/level.c
tests_log4stdc_configure_prop_level_LDADD = liblog4stdc.a
tests_log4stdc_configure_prop_level_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/configure.prop/level

