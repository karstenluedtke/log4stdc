AUTOMAKE_OPTIONS = subdir-objects

lib_LIBRARIES = liblog4stdc.a
include_HEADERS = log4stdc/log4stdc.h
nobase_include_HEADERS = barefootc/object.h

noinst_PROGRAMS = testlink

liblog4stdc_a_SOURCES = log4stdc/log4stdc.h log4stdc/logobjects.h
liblog4stdc_a_SOURCES+= barefootc/object.h

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

testlink_SOURCES = testlink.cpp
testlink_LDADD = liblog4stdc.a -lexpat
testlink_DEPENDENCIES = liblog4stdc.a

AM_CPPFLAGS = -Ilog4stdc
