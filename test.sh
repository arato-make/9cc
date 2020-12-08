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

assert 0 'int main(){ 0; }'
assert 42 'int main(){ 42; }'
assert 21 'int main(){ 5+20-4; }'
assert 41 'int main(){  12 + 34 - 5; }'
assert 47 'int main(){ 5+6*7; }'
assert 15 'int main(){ 5*(9-6); }'
assert 4 'int main(){ (3+5)/2; }'
assert 5 'int main(){ 6 + -1; }'
assert 1 'int main(){ - - 1; }'
assert 1 'int main(){ -5 + 6; }'

assert 0 'int main(){ 0==1; }'
assert 1 'int main(){ 42==42; }'
assert 1 'int main(){ 0!=1; }'
assert 0 'int main(){ 42!=42; }'

assert 1 'int main(){ 0<1; }'
assert 0 'int main(){ 1<1; }'
assert 0 'int main(){ 2<1; }'
assert 1 'int main(){ 0<=1; }'
assert 1 'int main(){ 1<=1; }'
assert 0 'int main(){ 2<=1; }'

assert 1 'int main(){ 1>0; }'
assert 0 'int main(){ 1>1; }'
assert 0 'int main(){ 1>2; }'
assert 1 'int main(){ 1>=0; }'
assert 1 'int main(){ 1>=1; }'
assert 0 'int main(){ 1>=2; }'
assert 1 'int main(){ 100-50*2 == 10*3-15*2; }'
assert 3 'int main(){ int a=3; }'
assert 10 'int main(){ int z=2*5; }'
assert 13 'int main(){ int z=2*5;int a=3; int c=a+z; }'
assert 13 'int main(){ int foo=2*5;int bar=3;int foobar=foo+bar; }'
assert 13 'int main(){ return 13; }'
assert 3 'int main(){ int foo=2*5;int bar=3;return bar;int foobar=foo+bar; }'
assert 13 'int main(){ int foo=2*5;int bar=3;return foo+bar; }'
assert 23 'int main(){ return 3+5*4; }'
assert 5 'int main(){ if(1==1) return 5; return 10; }'
assert 10 'int main(){ if(1==2) return 5; return 10; }'
assert 5 'int main(){ if(1==1) 2+3;else 2*5; }'
assert 10 'int main(){ if(1==2) 2+3;else 2*5; }'
assert 5 'int main(){ int a=0;while(a < 5) a=a+1; return a; }'
assert 8 'int main(){ int a=0;while(a < 8) a=a+1; return a; }'
assert 5 'int main(){ int a=0;for(int b=0;b<5;b=b+1) a=a+1; return a; }'
assert 16 'int main(){ int a=0;for(int b=0;b!=8;b=b+1) a=a+2; return a; }'
assert 8 'int main(){ int a=0;while(a < 8){a=a+1;} return a; }'
assert 4 'int main(){ int test=0;int a=0;while(a < 8) {a=a+2;test=test+1;} return test; }'
assert 5 'int main(){ if(1==1) {int a=2;a=a+3;} else 2*5; }'
assert 10 'int main(){ if(1==2) 2+3; else {int a=2;a=a*5;} }'
assert 4 'int main(){ int a=0;for(int b=0;b!=8;b=b+1) {a=a+1;b=b+1;} return a; }'
assert 12 'int test(){return 3;} int main(){ int a=0;while(a < 12) a=a+test(); return a; }'
assert 8 'int test(int a, int b){return a*b;} int main(){ test(2,4); }'
assert 3 'int main(){int x=3;int y=&x;return *y;}'
assert 3 'int main(){int x=3;int y=5;int z=&y+8;return *z;}'

echo OK