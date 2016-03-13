#!/bin/sh

if [ "x$1" != "x" ] ; then
	M="$1"
else
	M="win32.mak"
fi

HEADERS=`grep 'HEADERS.*=' Makefile.am | sed -e'1,$s/^.*= *//g'`
SOURCES=`grep 'log4stdc_a_SOURCES.*=.*c$' Makefile.am | sed -e'1,$s/^.*= *//g'`
TESTS=`grep '^TESTS.*=' Makefile.am | grep -v testlink | sed -e'1,$s/^.*= *//g'`

V=`grep AC_INIT configure.ac | sed -e'1,$s/^.*4stdc.,..//' -e'1,$s/.,..bug.*//'`

case "$M" in
*win32*)
	CONFIGFLAGS="WIN32 _WIN32 STDC_HEADERS"
	CONFIGFLAGS="$CONFIGFLAGS HAVE_ALLOCA_H HAVE_FCNTL_H HAVE_INTTYPES_H"
	CONFIGFLAGS="$CONFIGFLAGS HAVE_MALLOC_H HAVE_MEMORY_H HAVE_STDDEF_H"
	CONFIGFLAGS="$CONFIGFLAGS HAVE_STDINT_H HAVE_STDLIB_H HAVE_STRING_H"
	CONFIGFLAGS="$CONFIGFLAGS HAVE__BOOL"
	CONFIGFLAGS="$CONFIGFLAGS HAVE__LOCALTIME64 HAVE__LOCALTIME64_S"
	;;
*)
	CONFIGFLAGS=""
	;;
esac

echo ""	> $M
echo 'CC=cl'							>> $M
echo 'CPPFLAGS=-I. -Ilog4stdc' -DHAVE_CONFIG_H=1		>> $M
case "$M" in
*win32*)
	echo 'CXXFLAGS=-EHsc'					>> $M
	;;
*c600*)
	echo 'LFLAGS=-link /STACK:4000'				>> $M
	;;
esac
echo "" >> $M

echo "HEADERS=config.h \\" >> $M
for i in $HEADERS ; do
	h=`echo $i`
	echo "	$h \\" >> $M
done
echo "" >> $M
echo "" >> $M

echo "OFILES= \\" >> $M
for i in $SOURCES ; do
	o=`echo $i | sed -e'1,$s/.cp*$/.obj/g'`
	echo "	$o \\" >> $M
done
echo "" >> $M
echo "" >> $M

echo "TESTS= \\" >> $M
for i in $TESTS ; do
	x=$i.exe
	echo "	$x \\" >> $M
done
echo "" >> $M
echo "" >> $M

echo 'all: log4stdc.lib'			 >> $M
echo '	echo "all is made"'			 >> $M
echo "" >> $M

echo 'log4stdc.lib: $(OFILES) $(HEADERS)'	 >> $M
for i in $SOURCES ; do
	echo -n '	lib $@ -+ '		 >> $M
	echo -n "$i" | sed -e'1,$s/.cp*$/.obj/g' -e'1,$s@/@\\@g' >> $M
	echo    ', nul,'			 >> $M
done
echo "" >> $M

echo 'check: $(OFILES) $(HEADERS) $(TESTS)'	 >> $M
for i in $TESTS ; do
	echo "	${i}.exe" | sed -e'1,$s@/@\\@'g  >> $M
done
echo '	echo "check done"'			 >> $M
echo "" >> $M

echo "config.h: $M"						>> $M
echo '	echo #ifndef _L4SC_CONFIG_H_ >  $@'			>> $M
echo '	echo #define _L4SC_CONFIG_H_ >> $@'			>> $M
for flag in $CONFIGFLAGS ; do
	echo '	echo #define '"$flag"' 1 >> $@'			>> $M
done
echo '	echo #endif /* _L4SC_CONFIG_H_ */ >> $@'		>> $M
echo "" >> $M

for i in $SOURCES ; do
	c=`echo $i`
	o=`echo $i | sed -e'1,$s/.cp*$/.obj/g'`
	echo "$o: $c" '$(HEADERS)'		>> $M
	echo '	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@' "$c" >> $M
	echo "" >> $M
done

for i in $TESTS ; do
	x=$i.exe
	t=`echo $i | sed -e'1,$s/[^a-zA-Z0-9_]/_/g'`
	s=`grep "^${t}_SOURCES.*=.*cp*$" Makefile.am | sed -e'1,$s/^.*= *//g'`
	o=$i.obj
	echo "$x: \\" 					>> $M
	for c in $s ; do
		echo "		$c \\"			>> $M
	done
	echo '		log4stdc.lib'		 	>> $M
	echo -n '	$(CC) -c $(CPPFLAGS) $(CFLAGS) ' >> $M
	case "$s" in
	*.cpp)
		echo -n '$(CXXFLAGS) '			>> $M
		;;
	esac
	echo -n "-Fo$o " | sed -e'1,$s@/@\\@g'		>> $M
	for c in $s ; do
		echo -n "$c "				>> $M
	done
	echo '' >> $M
	echo -n '	$(CC) $(CFLAGS) '		>> $M
	case "$s" in
	*.cpp)
		echo -n '$(CXXFLAGS) '			>> $M
		;;
	esac
	echo -n "-Fe$x " | sed -e'1,$s@/@\\@g'		>> $M
	echo -n "$o " | sed -e'1,$s@/@\\@g'		>> $M
	echo 'log4stdc.lib $(LFLAGS)' 			>> $M
	echo "" >> $M
	echo "$x"
done

