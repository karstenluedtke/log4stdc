AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Ilog4stdc

lib_LIBRARIES = liblog4stdc.a
include_HEADERS = log4stdc/log4stdc.h

noinst_PROGRAMS = testlink

check_PROGRAMS =

liblog4stdc_a_SOURCES = log4stdc/log4stdc.h
liblog4stdc_a_SOURCES+= log4stdc/logobjs.h

liblog4stdc_a_SOURCES+= bareftc/object.h
liblog4stdc_a_SOURCES+= bareftc/mempool.h
liblog4stdc_a_SOURCES+= bareftc/mutex.h
liblog4stdc_a_SOURCES+= bareftc/atomic.h
liblog4stdc_a_SOURCES+= bareftc/lnkdlist.h
liblog4stdc_a_SOURCES+= bareftc/utf8.h

liblog4stdc_a_SOURCES+= log4stdc/logprf.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_vlg.c
liblog4stdc_a_SOURCES+= log4stdc/l4sc_log.c
liblog4stdc_a_SOURCES+= log4stdc/enabled.c
liblog4stdc_a_SOURCES+= log4stdc/fmtmsg.c

liblog4stdc_a_SOURCES+= log4stdc/logger.c
liblog4stdc_a_SOURCES+= log4stdc/appender.c
liblog4stdc_a_SOURCES+= log4stdc/fileappd.c
liblog4stdc_a_SOURCES+= log4stdc/layout.c
liblog4stdc_a_SOURCES+= log4stdc/logmsg.c
liblog4stdc_a_SOURCES+= log4stdc/propcfg.c
liblog4stdc_a_SOURCES+= log4stdc/object.c
liblog4stdc_a_SOURCES+= log4stdc/l4scpool.c
liblog4stdc_a_SOURCES+= log4stdc/pmutex.c
liblog4stdc_a_SOURCES+= log4stdc/wmutex.c
liblog4stdc_a_SOURCES+= log4stdc/xmlcfg.c
liblog4stdc_a_SOURCES+= log4stdc/snprintf.c

testlink_SOURCES = testlink.cpp
testlink_LDADD = liblog4stdc.a
testlink_DEPENDENCIES = liblog4stdc.a

TESTS = testlink

if BFCLOG4CXX
lib_LIBRARIES += liblog4cxx2stdc.a

liblog4cxx2stdc_a_SOURCES = log4cxx/repository.h
liblog4cxx2stdc_a_SOURCES+= log4cxx/repository.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/logger.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/level.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/objectptr.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/objectimpl.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/class.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/classregistration.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/configurator.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/basicconfigurator.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/domconfigurator.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/propertyconfigurator.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/messagebuffer.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/locationinfo.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/transcoder.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/exception.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/file.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/pool.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/mutex.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/properties.cpp
liblog4cxx2stdc_a_SOURCES+= log4cxx/loader.cpp

testlink_LDADD += liblog4cxx2stdc.a
testlink_DEPENDENCIES += liblog4cxx2stdc.a
endif BFCLOG4CXX

noinst_PROGRAMS += examples/redirect
examples_redirect_SOURCES = examples/redirect.c
examples_redirect_LDADD = liblog4stdc.a
examples_redirect_DEPENDENCIES = liblog4stdc.a
TESTS += examples/redirect

check_PROGRAMS += tests/log4stdc/xmlcfg/level
tests_log4stdc_xmlcfg_level_SOURCES = tests/log4stdc/xmlcfg/level.c
tests_log4stdc_xmlcfg_level_LDADD = liblog4stdc.a
tests_log4stdc_xmlcfg_level_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/xmlcfg/level

check_PROGRAMS += tests/log4stdc/xmlcfg/bigfile
tests_log4stdc_xmlcfg_bigfile_SOURCES = tests/log4stdc/xmlcfg/bigfile.c
tests_log4stdc_xmlcfg_bigfile_LDADD = liblog4stdc.a
tests_log4stdc_xmlcfg_bigfile_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/xmlcfg/bigfile

check_PROGRAMS += tests/log4stdc/propcfg/level
tests_log4stdc_propcfg_level_SOURCES = tests/log4stdc/propcfg/level.c
tests_log4stdc_propcfg_level_LDADD = liblog4stdc.a
tests_log4stdc_propcfg_level_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/propcfg/level

check_PROGRAMS += tests/log4stdc/format/currtime
tests_log4stdc_format_currtime_SOURCES = tests/log4stdc/format/currtime.c
tests_log4stdc_format_currtime_LDADD = liblog4stdc.a
tests_log4stdc_format_currtime_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/format/currtime

check_PROGRAMS += tests/log4stdc/format/snprintf
tests_log4stdc_format_snprintf_SOURCES = tests/log4stdc/format/snprintf.c
tests_log4stdc_format_snprintf_LDADD = liblog4stdc.a
tests_log4stdc_format_snprintf_DEPENDENCIES = liblog4stdc.a
TESTS += tests/log4stdc/format/snprintf

