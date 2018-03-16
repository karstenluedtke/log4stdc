#!/bin/sh

SRC1=`find log4stdc \( -name \*.c -or -name \*.cpp -or -name \*.h \) -print`
SRC2=`find test     \( -name \*.c -or -name \*.cpp -or -name \*.h \) -print`
SRC3=`find examples \( -name \*.c -or -name \*.cpp -or -name \*.h \) -print`
SRCFILES="$SRC1 $SRC2 $SRC3"

for i in $SRCFILES ; do
	clang-format $i | sed -f ./macroargs.sed > $i.formatted
	cmp $i.formatted $i || cp $i.formatted $i
	rm -f $i.formatted
done
