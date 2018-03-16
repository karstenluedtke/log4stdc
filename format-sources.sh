#!/bin/sh

SRCFILES=`find . \( -name \*.c -or -name \*.cpp -or -name \*.h \) -print`

for i in $SRCFILES ; do
	clang-format $i | sed -f ./macroargs.sed > $i.formatted
	cmp $i.formatted $i || cp $i.formatted $i
	rm -f $i.formatted
done
