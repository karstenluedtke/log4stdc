#!/bin/sh

M="barefootc.nmk"

HEADERS=`grep 'HEADERS.*=' Makefile.am | sed -e'1,$s/^.*= *//g'`
SOURCES=`grep 'barefootc_a_SOURCES.*=' Makefile.am | sed -e'1,$s/^.*= *//g'`

echo ""	> $M
echo "CPPFLAGS=-I."	>> $M
echo "CFLAGS="		>> $M
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
