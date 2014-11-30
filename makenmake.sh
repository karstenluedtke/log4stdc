#!/bin/sh

M="barefootc.nmk"

HEADERS=`grep 'HEADERS.*=' Makefile.am | sed -e'1,$s/^.*= *//g'`
SOURCES=`grep 'barefootc_a_SOURCES.*=.*[cp]$' Makefile.am \
					| sed -e'1,$s/^.*= *//g'`

echo ""	> $M
echo 'CPPFLAGS=-I. -Ilog4stdc' "\\"				>> $M
echo '	-DWIN32=1 -D_WIN32=1 -Dinline=""' "\\"			>> $M
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
echo '	-DHAVE_STRUCT_TIMESPEC=1' "\\"				>> $M
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

echo 'barefootc.lib: $(OFILES) $(HEADERS)'	 >> $M
echo '	lib $(OFILES) /OUT:$@'			 >> $M
echo "" >> $M

for i in $SOURCES ; do
	c=`echo $i`
	o=`echo $i | sed -e'1,$s/.cp*$/.obj/g'`
	echo "$o: $c" '$(HEADERS)'		>> $M
	echo '	cl /c $(CPPFLAGS) $(CFLAGS) -Fo$@' "$c" >> $M
	echo "" >> $M
done
