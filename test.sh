#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 'return 0;'
assert 42 'return 42;'

assert 21 'return 5+20-4;'
assert 41 'return 12 + 34 - 5 ;'
assert 144 'return 12 * (13 - 1) ;'
assert 7 'return 1 + 2 * 3 ;'
assert 10 'return -4 * 10 + 5 * +10 ;'

assert 1 'return 10*-1+11==1 ;'
assert 1 'return 20+1 <= 21 ;'
assert 0 'return 15 > 15;'
assert 2 '2; return 1+1;'

assert 20 'foo=2; bar=10; return foo*bar;'
assert 11 'f1 = 30; f2 = 19; return f1-f2;'

assert 3 'if(1) return 3; return 1;'
assert 17 'a=1; b=15; if(a>=b) return 0; return 17;'

assert 16 'a=16; b=15; while(a<b) a=a*2; return a;'

assert 51 'i=0; j=0; for(i=1; i<=16; i=i+3) j=i+j; return j;'
assert 1  'for(;;) return 1; return 2;'

echo ""
echo SUCCESS !!!!!!!!!!
