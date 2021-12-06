#!/bin/bash

clear

mkdir tmp
mkdir log

flex ../work/src/lang/scanner.l
mv lex.yy.c ./tmp/scanner.c

cp ../work/src/lang/tokenUtils.c ./tmp/tokenUtils.c
cp ../work/src/lang/tokenUtils.h ./tmp/tokenUtils.h
cp ../work/src/pre/cDefs.h ./tmp/cDefs.h

yacc --verbose --debug -d ../work/src/lang/syntax.ypp -o tmp/syntax.cpp

mv ./tmp/syntax.output ./log/syntaxLog.txt

cc -c ./tmp/scanner.c -o ./tmp/scanner.o -I ../work/src/lang
cc -c ./tmp/tokenUtils.c -o ./tmp/tokenUtils.o