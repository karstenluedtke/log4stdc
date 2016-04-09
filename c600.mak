
CC=cl
CPPFLAGS=-I. -Ilog4stdc -DHAVE_CONFIG_H=1
LFLAGS=-link /STACK:4000

HEADERS=config.h \
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
	log4stdc/sockappd.obj \
	log4stdc/layout.obj \
	log4stdc/logmsg.obj \
	log4stdc/propcfg.obj \
	log4stdc/object.obj \
	log4stdc/l4scpool.obj \
	log4stdc/pmutex.obj \
	log4stdc/wmutex.obj \
	log4stdc/xmlcfg.obj \
	log4stdc/strcfg.obj \
	log4stdc/filecfg.obj \
	log4stdc/snprintf.obj \
	log4stdc/vsnprntf.obj \
	log4stdc/sockappd.obj \
	log4stdc/jsonstrm.obj \
	log4stdc/l4jstrm.obj \


TESTS= \
	examples/redirect.exe \
	tests/xmlcfg/level.exe \
	tests/xmlcfg/bigfile.exe \
	tests/propcfg/level.exe \
	tests/format/snprintf.exe \
	tests/format/currtime.exe \
	tests/appender/l4jsock.exe \
	tests/appender/l4j2sock.exe \
	tests/appender/jsonsock.exe \


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
	lib $@ -+ log4stdc\sockappd.obj, nul,
	lib $@ -+ log4stdc\layout.obj, nul,
	lib $@ -+ log4stdc\logmsg.obj, nul,
	lib $@ -+ log4stdc\propcfg.obj, nul,
	lib $@ -+ log4stdc\object.obj, nul,
	lib $@ -+ log4stdc\l4scpool.obj, nul,
	lib $@ -+ log4stdc\pmutex.obj, nul,
	lib $@ -+ log4stdc\wmutex.obj, nul,
	lib $@ -+ log4stdc\xmlcfg.obj, nul,
	lib $@ -+ log4stdc\strcfg.obj, nul,
	lib $@ -+ log4stdc\filecfg.obj, nul,
	lib $@ -+ log4stdc\snprintf.obj, nul,
	lib $@ -+ log4stdc\vsnprntf.obj, nul,
	lib $@ -+ log4stdc\sockappd.obj, nul,
	lib $@ -+ log4stdc\jsonstrm.obj, nul,
	lib $@ -+ log4stdc\l4jstrm.obj, nul,

check: $(OFILES) $(HEADERS) $(TESTS)
	examples\redirect.exe
	tests\xmlcfg\level.exe
	tests\xmlcfg\bigfile.exe
	tests\propcfg\level.exe
	tests\format\snprintf.exe
	tests\format\currtime.exe
	tests\appender\l4jsock.exe
	tests\appender\l4j2sock.exe
	tests\appender\jsonsock.exe
	echo "check done"

config.h: c600.mak
	echo #ifndef _L4SC_CONFIG_H_ >  $@
	echo #define _L4SC_CONFIG_H_ >> $@
	echo #endif /* _L4SC_CONFIG_H_ */ >> $@

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

log4stdc/sockappd.obj: log4stdc/sockappd.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/sockappd.c

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

log4stdc/strcfg.obj: log4stdc/strcfg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/strcfg.c

log4stdc/filecfg.obj: log4stdc/filecfg.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/filecfg.c

log4stdc/snprintf.obj: log4stdc/snprintf.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/snprintf.c

log4stdc/vsnprntf.obj: log4stdc/vsnprntf.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/vsnprntf.c

log4stdc/sockappd.obj: log4stdc/sockappd.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/sockappd.c

log4stdc/jsonstrm.obj: log4stdc/jsonstrm.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/jsonstrm.c

log4stdc/l4jstrm.obj: log4stdc/l4jstrm.c $(HEADERS)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@ log4stdc/l4jstrm.c

examples/redirect.exe: \
		examples/redirect.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Foexamples\redirect.obj examples/redirect.c 
	$(CC) $(CFLAGS) -Feexamples\redirect.exe examples\redirect.obj log4stdc.lib $(LFLAGS)

tests/xmlcfg/level.exe: \
		tests/xmlcfg/level.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\xmlcfg\level.obj tests/xmlcfg/level.c 
	$(CC) $(CFLAGS) -Fetests\xmlcfg\level.exe tests\xmlcfg\level.obj log4stdc.lib $(LFLAGS)

tests/xmlcfg/bigfile.exe: \
		tests/xmlcfg/bigfile.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\xmlcfg\bigfile.obj tests/xmlcfg/bigfile.c 
	$(CC) $(CFLAGS) -Fetests\xmlcfg\bigfile.exe tests\xmlcfg\bigfile.obj log4stdc.lib $(LFLAGS)

tests/propcfg/level.exe: \
		tests/propcfg/level.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\propcfg\level.obj tests/propcfg/level.c 
	$(CC) $(CFLAGS) -Fetests\propcfg\level.exe tests\propcfg\level.obj log4stdc.lib $(LFLAGS)

tests/format/snprintf.exe: \
		tests/format/snprintf.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\format\snprintf.obj tests/format/snprintf.c 
	$(CC) $(CFLAGS) -Fetests\format\snprintf.exe tests\format\snprintf.obj log4stdc.lib $(LFLAGS)

tests/format/currtime.exe: \
		tests/format/currtime.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\format\currtime.obj tests/format/currtime.c 
	$(CC) $(CFLAGS) -Fetests\format\currtime.exe tests\format\currtime.obj log4stdc.lib $(LFLAGS)

tests/appender/l4jsock.exe: \
		tests/appender/l4jsock.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\appender\l4jsock.obj tests/appender/l4jsock.c 
	$(CC) $(CFLAGS) -Fetests\appender\l4jsock.exe tests\appender\l4jsock.obj log4stdc.lib $(LFLAGS)

tests/appender/l4j2sock.exe: \
		tests/appender/l4j2sock.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\appender\l4j2sock.obj tests/appender/l4j2sock.c 
	$(CC) $(CFLAGS) -Fetests\appender\l4j2sock.exe tests\appender\l4j2sock.obj log4stdc.lib $(LFLAGS)

tests/appender/jsonsock.exe: \
		tests/appender/jsonsock.c \
		log4stdc.lib
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fotests\appender\jsonsock.obj tests/appender/jsonsock.c 
	$(CC) $(CFLAGS) -Fetests\appender\jsonsock.exe tests\appender\jsonsock.obj log4stdc.lib $(LFLAGS)

