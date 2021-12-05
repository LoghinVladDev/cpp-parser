#!/bin/bash

clear

mkdir tmp
mkdir log

flex ../work/src/lang/scanner.l
mv lex.yy.c ./tmp/scanner.c

yacc --verbose --debug -d ../work/src/lang/syntax.ypp -o tmp/syntax.cpp

mv ./tmp/syntax.output ./log/syntaxLog.txt

cc -c ./tmp/scanner.c -o ./tmp/scanner.o