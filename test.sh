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

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "
assert 144 " 12 * (13 - 1) "
assert 7 " 1 + 2 * 3 "
assert 10 " -4 * 10 + 5 * +10 "
assert 1 " 10*-1+11==1 "
assert 1 " 20+1 <= 21 "
assert 0 "15 > 15"

echo OK
