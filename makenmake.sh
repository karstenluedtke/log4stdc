#!/bin/sh

M="Nmakefile"

HEADERS=`grep 'HEADERS.*=' Makefile.am | sed -e'1,$s/^.*= *//g'`
SOURCES=`grep 'barefootc_a_SOURCES.*=.*c$' Makefile.am | sed -e'1,$s/^.*= *//g'`
LOG4STDC=`grep 'log4stdc_a_SOURCES.*=.*c$' Makefile.am | sed -e'1,$s/^.*= *//g'`
TESTS=`grep '^TESTS.*=' Makefile.am | sed -e'1,$s/^.*= *//g'`

echo ""	> $M
echo 'CC=cl'							>> $M
echo 'CPPFLAGS=-I. -Ilog4stdc' "\\"				>> $M
echo '	-DWIN32=1 -D_WIN32=1' "\\"				>> $M
echo '	-DSTDC_HEADERS=1' "\\"					>> $M
echo '	-DHAVE_ALLOCA_H=1' "\\"					>> $M
echo '	-DHAVE_FCNTL_H=1' "\\"					>> $M
echo '	-DHAVE_INTTYPES_H=1' "\\"				>> $M
echo '	-DHAVE_MALLOC_H=1' "\\"					>> $M
echo '	-DHAVE_MEMORY_H=1' "\\"					>> $M
echo '	-DHAVE_STDDEF_H=1' "\\"					>> $M
echo '	-DHAVE_STDINT_H=1' "\\"					>> $M
echo '	-DHAVE_STDLIB_H=1' "\\"					>> $M
echo '	-DHAVE_STRING_H=1' "\\"					>> $M
echo '	-DHAVE__BOOL=1' "\\"					>> $M
echo '	-DHAVE__LOCALTIME64=1' "\\"				>> $M
echo '	-DHAVE__LOCALTIME64_S=1' "\\"				>> $M
echo '	-DPACKAGE="barefootc"' "\\"				>> $M
echo '	-DPACKAGE_BUGREPORT="bug-barefootc@example.org"' "\\"	>> $M
echo '	-DPACKAGE_NAME="barefootc"' "\\"			>> $M
echo '	-DPACKAGE_STRING="barefootc 0.6.1"' "\\"		>> $M
echo '	-DPACKAGE_TARNAME="barefootc"' "\\"			>> $M
echo '	-DPACKAGE_URL=""' "\\"					>> $M
echo '	-DPACKAGE_VERSION="0.6.1"' "\\"				>> $M
echo '	-DVERSION="0.6.1"'					>> $M
echo 'CFLAGS='							>> $M
echo 'CXXFLAGS=-EHsc'						>> $M
echo "" >> $M

echo "HEADERS= \\" >> $M
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

echo "OFL4SC= \\" >> $M
for i in $LOG4STDC ; do
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

echo 'all: barefootc.lib log4stdc.lib'		 >> $M
echo '	echo "all is made"'			 >> $M
echo "" >> $M

echo 'barefootc.lib: $(OFILES) $(HEADERS)'	 >> $M
echo '	lib $(OFILES) /OUT:$@'			 >> $M
echo "" >> $M

echo 'log4stdc.lib: $(OFL4SC) $(HEADERS)'	 >> $M
echo '	lib $(OFL4SC) /OUT:$@'			 >> $M
echo "" >> $M

echo 'check: $(OFILES) $(HEADERS) $(TESTS)'	 >> $M
for i in $TESTS ; do
	echo "	${i}.exe" | sed -e'1,$s@/@\\@'g  >> $M
done
echo '	echo "check done"'			 >> $M
echo "" >> $M

for i in $SOURCES $LOG4STDC ; do
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
	o=`echo $s | sed -e'1,$s/.cp*$/.obj/g'`
	echo "$x: \\" 					>> $M
	for c in $s ; do
		o=`echo $c | sed -e'1,$s/.cp*$/.obj/g'`
		echo "		$o \\"			>> $M
	done
	echo '		barefootc.lib log4stdc.lib' 	>> $M
	echo '	$(CC) $(CFLAGS) $(CXXFLAGS) -Fe$@' "$o barefootc.lib log4stdc.lib" >> $M
	echo "" >> $M
	for c in $s ; do
		o=`echo $c | sed -e'1,$s/.cp*$/.obj/g'`
		echo "$o: $c" '$(HEADERS)'		>> $M
		echo '	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Fo$@' "$c" >> $M
		echo "" >> $M
	done
	echo "$x"
done

