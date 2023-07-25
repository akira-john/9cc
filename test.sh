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

# 数値
assert 0 'main() {return 0;}'
assert 42 'main() {return 42;}'
# 式
assert 21 'main() {return 5+20-4;}'
assert 144 'main() {return 12 * (13 - 1) ;}'
assert 10 'main() {return -4 * 10 + 5 * +10 ;}'
# 比較
assert 1 'main() {return 10*-1+11==1 ;}'
assert 1 'main() {return 20+1 <= 21 ;}'
assert 0 'main() {return 15 > 15;}'
# 変数
assert 20 'main() {foo=2; bar=10; return foo*bar;}'
assert 11 'main() {f1 = 30; f2 = 19; return f1-f2;}'
# 条件文
assert 3 'main() {if(1) return 3; return 1;}'
assert 17 'main() {a=1; b=15; if(a>=b) return 0; return 17;}'
assert 16 'main() {a=16; b=15; while(a<b) a=a*2; return a;}'
assert 51 'main() {i=0; j=0; for(i=1; i<=16; i=i+3) j=i+j; return j;}'
assert 1 'main() {for(;;) return 1; return 2;}'
assert 5 'main() {a=1; b=1; while(a<5){a=a+1; b=b+1;} return b;}'
# 関数宣言
assert 4 'main() {return add(1, 3);} add(x, y){return x+y;}'
assert 4 'main() {return dfs(0, -1);} dfs(v, par){if(v==5){return par;} return dfs(v+1, v);}'
# 参照
assert 3 'main() {x=3; y=&x; return *y;}'
assert 8 'main() {x=3; y=&x; *y=x+5; return x;}'

echo ""
echo SUCCESS !!!!!!!!!!
