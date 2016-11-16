#!/bin/sh
rm -rf bout
mkdir bout
for f in $(find . -name *.cpp); do
	g++ -Wall -obout/$(basename $f).x $f
done
find . -name *.in -exec cp \{\} bout \;
