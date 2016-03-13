
CC=cl
CPPFLAGS=-I. -Ilog4stdc -DHAVE_CONFIG_H=1
CXXFLAGS=-EHsc

HEADERS= \
	log4stdc/log4stdc.h \


OFILES= \
	log4stdc/logprf.obj \
	log4stdc/l4sc_vlg.obj \
	log4stdc/l4sc_log.obj \
	log4stdc/enabled.obj \
	log4stdc/fmtmsg.obj \
	log4stdc/logger.obj \
	log4stdc/appender.obj \
	log4stdc/fileappd.obj \
	log4stdc/layout.obj \
	log4stdc/logmsg.obj \
	log4stdc/propcfg.obj \
	log4stdc/object.obj \
	log4stdc/l4scpool.obj \
	log4stdc/pmutex.obj \
	log4stdc/wmutex.obj \
	log4stdc/xmlcfg.obj \
	log4stdc/snprintf.obj \
	log4stdc/vsnprntf.obj \


TESTS= \
	examples/redirect.exe \
	tests/log4stdc/xmlcfg/level.exe \
	tests/log4stdc/xmlcfg/bigfile.exe \
	tests/log4stdc/propcfg/level.exe \
	tests/log4stdc/format/currtime.exe \
	tests/log4stdc/format/snprintf.exe \


all: log4stdc.lib
	echo "all is made"

log4stdc.lib: $(OFILES) $(HEADERS)
	lib $@ -+ log4stdc\logprf.obj, nul,
	lib $@ -+ log4stdc\l4sc_vlg.obj, nul,
	lib $@ -+ log4stdc\l4sc_log.obj, nul,
	lib $@ -+ log4stdc\enabled.obj, nul,
	lib $@ -+ log4stdc\fmtmsg.obj, nul,
	lib $@ -+ log4stdc\logger.obj, nul,
	lib $@ -+ log4stdc\appender.obj, nul,
	lib $@ -+ log4stdc\fileappd.obj, nul,
	lib $@ -+ log4stdc\layout.obj, nul,
	lib $@ -+ log4stdc\logmsg.obj, nul,
	lib $@ -+ log4stdc\propcfg.obj, nul,
	lib $@ -+ log4stdc\object.obj, nul,
	lib $@ -+ log4stdc\l4scpool.obj, nul,
	lib $@ -+ log4stdc\pmutex.obj, nul,
	lib $@ -+ log4stdc\wmutex.obj, nul,
	lib $@ -+ log4stdc\xmlcfg.obj, nul,
	lib $@ -+ log4stdc\snprintf.obj, nul,
	lib $@ -+ log4stdc\vsnprntf.obj, nul,

check: $(OFILES) $(HEADERS) $(TESTS)
	examples\redirect.exe
	tests\log4stdc\xmlcfg\level.exe
	tests\log4stdc\xmlcfg\bigfile.exe
	tests\log4stdc\propcfg\level.exe
	tests\log4stdc\format\currtime.exe
	tests\log4stdc\format\snprintf.exe
	echo "check done"

config.h: win32.mak
	echo "#ifndef _L4SC_CONFIG_H_" >  $@
	echo "#define _L4SC_CONFIG_H_" >> $@
	echo "#define WIN32 1" >> $@
	echo "#define _WIN32 1" >> $@
	echo "#define STDC_HEADERS 1" >> $@
	echo "#define HAVE_ALLOCA_H 1" >> $@
	echo "#define HAVE_FCNTL_H 1" >> $@
	echo "#define HAVE_INTTYPES_H 1" >> $@
	echo "#define HAVE_MALLOC_H 1" >> $@
	echo "#define HAVE_MEMORY_H 1" >> $@
	echo "#define HAVE_STDDEF_H 1" >> $@
	echo "#define HAVE_STDINT_H 1" >> $@
	echo "#define HAVE_STDLIB_H 1" >> $@
	echo "#define HAVE_STRING_H 1" >> $@
	echo "#define HAVE__BOOL 1" >> $@
	echo "#define HAVE__LOCALTIME64 1" >> $@
	echo "#define HAVE__LOCALTIME64_S 1" >> $@
	echo "#endif /* _L4SC_CONFIG_H_ */" >> $@

log4stdc/logprf.obj: log4stdc/logprf.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/logprf.c

log4stdc/l4sc_vlg.obj: log4stdc/l4sc_vlg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/l4sc_vlg.c

log4stdc/l4sc_log.obj: log4stdc/l4sc_log.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/l4sc_log.c

log4stdc/enabled.obj: log4stdc/enabled.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/enabled.c

log4stdc/fmtmsg.obj: log4stdc/fmtmsg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/fmtmsg.c

log4stdc/logger.obj: log4stdc/logger.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/logger.c

log4stdc/appender.obj: log4stdc/appender.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/appender.c

log4stdc/fileappd.obj: log4stdc/fileappd.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/fileappd.c

log4stdc/layout.obj: log4stdc/layout.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/layout.c

log4stdc/logmsg.obj: log4stdc/logmsg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/logmsg.c

log4stdc/propcfg.obj: log4stdc/propcfg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/propcfg.c

log4stdc/object.obj: log4stdc/object.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/object.c

log4stdc/l4scpool.obj: log4stdc/l4scpool.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/l4scpool.c

log4stdc/pmutex.obj: log4stdc/pmutex.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/pmutex.c

log4stdc/wmutex.obj: log4stdc/wmutex.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/wmutex.c

log4stdc/xmlcfg.obj: log4stdc/xmlcfg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/xmlcfg.c

log4stdc/snprintf.obj: log4stdc/snprintf.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/snprintf.c

log4stdc/vsnprntf.obj: log4stdc/vsnprntf.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/vsnprntf.c

examples/redirect.exe: \
		examples/redirect.c \
		log4stdc.lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -Feexamples\redirect.exe examples/redirect.c log4stdc.lib

tests/log4stdc/xmlcfg/level.exe: \
		tests/log4stdc/xmlcfg/level.c \
		log4stdc.lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -Fetests\log4stdc\xmlcfg\level.exe tests/log4stdc/xmlcfg/level.c log4stdc.lib

tests/log4stdc/xmlcfg/bigfile.exe: \
		tests/log4stdc/xmlcfg/bigfile.c \
		log4stdc.lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -Fetests\log4stdc\xmlcfg\bigfile.exe tests/log4stdc/xmlcfg/bigfile.c log4stdc.lib

tests/log4stdc/propcfg/level.exe: \
		tests/log4stdc/propcfg/level.c \
		log4stdc.lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -Fetests\log4stdc\propcfg\level.exe tests/log4stdc/propcfg/level.c log4stdc.lib

tests/log4stdc/format/currtime.exe: \
		tests/log4stdc/format/currtime.c \
		log4stdc.lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -Fetests\log4stdc\format\currtime.exe tests/log4stdc/format/currtime.c log4stdc.lib

tests/log4stdc/format/snprintf.exe: \
		tests/log4stdc/format/snprintf.c \
		log4stdc.lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -Fetests\log4stdc\format\snprintf.exe tests/log4stdc/format/snprintf.c log4stdc.lib

