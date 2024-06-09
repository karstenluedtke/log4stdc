AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Ilog4stdc

lib_LIBRARIES = liblog4stdc.a
include_HEADERS = log4stdc/log4stdc.h

liblog4stdc_a_SOURCES = log4stdc/log4stdc.h
liblog4stdc_a_SOURCES+= log4stdc/logobjs.h
liblog4stdc_a_SOURCES+= log4stdc/compat.h

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
liblog4stdc_a_SOURCES+= log4stdc/sparent.c
liblog4stdc_a_SOURCES+= log4stdc/sappendr.c
liblog4stdc_a_SOURCES+= log4stdc/fmtmsg.c

liblog4stdc_a_SOURCES+= log4stdc/logger.c
liblog4stdc_a_SOURCES+= log4stdc/appender.c
liblog4stdc_a_SOURCES+= log4stdc/fileappd.c
liblog4stdc_a_SOURCES+= log4stdc/sockappd.c
liblog4stdc_a_SOURCES+= log4stdc/sslappdr.c
liblog4stdc_a_SOURCES+= log4stdc/layout.c
liblog4stdc_a_SOURCES+= log4stdc/logmsg.c
liblog4stdc_a_SOURCES+= log4stdc/propcfg.c
liblog4stdc_a_SOURCES+= log4stdc/object.c
liblog4stdc_a_SOURCES+= log4stdc/l4scpool.c
liblog4stdc_a_SOURCES+= log4stdc/pmutex.c
liblog4stdc_a_SOURCES+= log4stdc/wmutex.c
liblog4stdc_a_SOURCES+= log4stdc/xmlcfg.c
liblog4stdc_a_SOURCES+= log4stdc/strcfg.c
liblog4stdc_a_SOURCES+= log4stdc/filecfg.c
liblog4stdc_a_SOURCES+= log4stdc/snprintf.c
liblog4stdc_a_SOURCES+= log4stdc/vsnprntf.c
liblog4stdc_a_SOURCES+= log4stdc/fmtjtime.c
liblog4stdc_a_SOURCES+= log4stdc/jsonstrm.c
liblog4stdc_a_SOURCES+= log4stdc/l4jstrm.c
liblog4stdc_a_SOURCES+= log4stdc/xmlstrm.c

liblog4stdc_a_SOURCES+= LICENSE.txt
liblog4stdc_a_SOURCES+= win32.mak
liblog4stdc_a_SOURCES+= c600.mak
liblog4stdc_a_SOURCES+= log4stdc.sln
liblog4stdc_a_SOURCES+= log4stdc.vcxproj

noinst_PROGRAMS = examples/redirect
examples_redirect_SOURCES = examples/redirect.c
examples_redirect_LDADD = liblog4stdc.a
examples_redirect_DEPENDENCIES = liblog4stdc.a
TESTS = examples/redirect

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

noinst_PROGRAMS += testlink
testlink_SOURCES = testlink.cpp
testlink_LDADD = liblog4stdc.a liblog4cxx2stdc.a
testlink_DEPENDENCIES = liblog4stdc.a liblog4cxx2stdc.a
TESTS += testlink
endif BFCLOG4CXX

check_PROGRAMS = tests/xmlcfg/level
tests_xmlcfg_level_SOURCES = tests/xmlcfg/level.c
tests_xmlcfg_level_LDADD = liblog4stdc.a
tests_xmlcfg_level_DEPENDENCIES = liblog4stdc.a
TESTS += tests/xmlcfg/level

check_PROGRAMS += tests/xmlcfg/bigfile
tests_xmlcfg_bigfile_SOURCES = tests/xmlcfg/bigfile.c
tests_xmlcfg_bigfile_LDADD = liblog4stdc.a
tests_xmlcfg_bigfile_DEPENDENCIES = liblog4stdc.a
TESTS += tests/xmlcfg/bigfile

check_PROGRAMS += tests/propcfg/level
tests_propcfg_level_SOURCES = tests/propcfg/level.c
tests_propcfg_level_LDADD = liblog4stdc.a
tests_propcfg_level_DEPENDENCIES = liblog4stdc.a
TESTS += tests/propcfg/level

check_PROGRAMS += tests/format/snprintf
tests_format_snprintf_SOURCES = tests/format/snprintf.c
tests_format_snprintf_LDADD = liblog4stdc.a
tests_format_snprintf_DEPENDENCIES = liblog4stdc.a
TESTS += tests/format/snprintf

check_PROGRAMS += tests/format/pattern
tests_format_pattern_SOURCES = tests/format/pattern.c
tests_format_pattern_LDADD = liblog4stdc.a
tests_format_pattern_DEPENDENCIES = liblog4stdc.a
TESTS += tests/format/pattern

check_PROGRAMS += tests/format/currtime
tests_format_currtime_SOURCES = tests/format/currtime.c
tests_format_currtime_LDADD = liblog4stdc.a
tests_format_currtime_DEPENDENCIES = liblog4stdc.a
TESTS += tests/format/currtime

check_PROGRAMS += tests/format/l4j2strm
tests_format_l4j2strm_SOURCES = tests/format/l4j2strm.c
tests_format_l4j2strm_LDADD = liblog4stdc.a
tests_format_l4j2strm_DEPENDENCIES = liblog4stdc.a
TESTS += tests/format/l4j2strm

check_PROGRAMS += tests/appender/l4jsock
tests_appender_l4jsock_SOURCES = tests/appender/l4jsock.c
tests_appender_l4jsock_LDADD = liblog4stdc.a
tests_appender_l4jsock_DEPENDENCIES = liblog4stdc.a
TESTS += tests/appender/l4jsock

check_PROGRAMS += tests/appender/l4j2sock
tests_appender_l4j2sock_SOURCES = tests/appender/l4j2sock.c
tests_appender_l4j2sock_LDADD = liblog4stdc.a
tests_appender_l4j2sock_DEPENDENCIES = liblog4stdc.a
TESTS += tests/appender/l4j2sock

check_PROGRAMS += tests/appender/jsonsock
tests_appender_jsonsock_SOURCES = tests/appender/jsonsock.c
tests_appender_jsonsock_LDADD = liblog4stdc.a
tests_appender_jsonsock_DEPENDENCIES = liblog4stdc.a
TESTS += tests/appender/jsonsock

check_PROGRAMS += tests/appender/xmlsock
tests_appender_xmlsock_SOURCES = tests/appender/xmlsock.c
tests_appender_xmlsock_LDADD = liblog4stdc.a
tests_appender_xmlsock_DEPENDENCIES = liblog4stdc.a
TESTS += tests/appender/xmlsock

