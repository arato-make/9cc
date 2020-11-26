#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" >tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expexted, but got $actual"
        exit 1
    fi 
}

assert 0 '0;'
assert 42 '42;'
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5;"
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 5 '6 + -1;'
assert 1 '- - 1;'
assert 1 '-5 + 6;'

assert 0 '0==1;'
assert 1 '42==42;'
assert 1 '0!=1;'
assert 0 '42!=42;'

assert 1 '0<1;'
assert 0 '1<1;'
assert 0 '2<1;'
assert 1 '0<=1;'
assert 1 '1<=1;'
assert 0 '2<=1;'

assert 1 '1>0;'
assert 0 '1>1;'
assert 0 '1>2;'
assert 1 '1>=0;'
assert 1 '1>=1;'
assert 0 '1>=2;'
assert 1 '100-50*2 == 10*3-15*2;'
assert 3 'a=3;'
assert 10 'z=2*5;'
assert 13 'z=2*5;a=3;c=a+z;'
assert 13 'foo=2*5;bar=3;foobar=foo+bar;'
assert 13 'return 13;'
assert 3 'foo=2*5;bar=3;return bar;foobar=foo+bar;'
assert 13 'foo=2*5;bar=3;return foo+bar;'
assert 23 'return 3+5*4;'
assert 5 'if(1==1) return 5; return 10;'
assert 10 'if(1==2) return 5; return 10;'
assert 5 'if(1==1) 2+3;else 2*5;'
assert 10 'if(1==2) 2+3;else 2*5;'
assert 5 'a=0;while(a < 5) a=a+1; return a;'
assert 8 'while(a < 8) a=a+1; return a;'
assert 5 'a=0;for(b=0;b<5;b=b+1) a=a+1; return a;'
assert 16 'a=0;for(b=0;b!=8;b=b+1) a=a+2; return a;'
assert 8 'while(a < 8){a=a+1;} return a;'
assert 4 'test=0;a=0;while(a < 8) {a=a+2;test=test+1;} return test;'
assert 5 'if(1==1) {a=2;a=a+3;} else 2*5;'
assert 10 'if(1==2) 2+3; else {a=2;a=a*5;}'
assert 4 'a=0;for(b=0;b!=8;b=b+1) {a=a+1;b=b+1;} return a;'

echo OK