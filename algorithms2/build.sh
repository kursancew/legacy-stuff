#!/bin/sh
rm -rf bout
mkdir bout
for f in *.cpp; do
	g++ -Wall -obout/$(basename $f).x $f
done
