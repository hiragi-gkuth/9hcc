#!/bin/bash

try() {
    expected="$1"
    input="$2"

    src/9hcc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
	echo "$input => $actual"
    else
	echo "$input => $expected expected, but got $actual"
	exit 1
    fi
}


try 0 0
try 42 42
try 23 '20+4-1'
try 128 '10 + 114 - 23 + 27'
try 47 "5+6*7"
try 15 "5*(9-6)"
try 4 "(3+5)/2"
try 10 "-(3+3)*5+40"
try 1 "1==1" #mean true
try 0 "1!=1" #mean false
try 1 "1<=1" 
try 0 "1<1"
try 0 "a=1"

echo OK

