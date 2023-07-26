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
assert 0 'int main() {return 0;}'
assert 42 'int main() {return 42;}'
# 式
assert 21 'int main() {return 5+20-4;}'
assert 144 'int main() {return 12 * (13 - 1) ;}'
assert 10 'int main() {return -4 * 10 + 5 * +10 ;}'
# 比較
assert 1 'int main() {return 10*-1+11==1 ;}'
assert 1 'int main() {return 20+1 <= 21 ;}'
assert 0 'int main() {return 15 > 15;}'
# 変数
assert 20 'int main() {int foo=2; int bar=10; return foo*bar;}'
assert 11 'int main() {int f1 = 30; int f2 = 19; return f1-f2;}'
# 条件文
assert 3 'int main() {if(1) return 3; return 1;}'
assert 17 'int main() {int a=1; int b=15; if(a>=b) return 0; return 17;}'
assert 16 'int main() {int a=16; int b=15; while(a<b) a=a*2; return a;}'
assert 51 'int main() {int i=0; int j=0; for(i=1; i<=16; i=i+3) j=i+j; return j;}'
assert 1 'int main() {for(;;) return 1; return 2;}'
assert 5 'int main() {int a=1; int b=1; while(a<5){a=a+1; b=b+1;} return b;}'
# 関数宣言
assert 4 'int main() {return add(1, 3);} int add(int x, int y){return x+y;}'
assert 4 'int main() {return dfs(0, -1);} int dfs(int v, int par){if(v==5){return par;} return dfs(v+1, v);}'
# 参照
assert 8 'int main() {int x=3; int *y=&x; *y=x+5; return x;}'
assert 5 'int main() {int x=3; int y=5; return *(&x+1);}'
assert 7 'int main() {int x=3; int y=5; *(&y-1)=7; return x;}'
assert 2 'int main() {int x=3; return (&x+2)-&x;}'
assert 8 'int main() { int x=3; int y=5; return foo(&x, y); } int foo(int *x, int y) { return *x + y; }'
# 一次元配列
assert 4 'int main() { int x[2]; int *y=&x; *y=4; return *x;}'
assert 3 'int main() { int x[3]; *x=1; *(x+1)=2; x[2]=3; return x[2];}'
# 多次元配列
assert 0 'int main() { int x[2][3]; int *y=x; *y=0; return **x;}'
assert 1 'int main() { int x[2][3]; int *y=x; *(y+1)=1; return *(*x+1); }'
assert 2 'int main() { int x[2][3]; int *y=x; *(y+2)=2; return *(*x+2); }'
assert 3 'int main() { int x[2][3]; int *y=x; *(y+3)=3; return **(x+1); }'
assert 5 'int main() { int x[2][3]; x[1][2]=5; return *(*(x+1)+2); }'
assert 6 'int main() { int x[2][3]; int *y=x; y[6]=6; return x[2][0]; }'
# sizeof
assert 8 'int main() { int x; return sizeof(x);}'
assert 96 'int main() { int x[3][4]; return sizeof x;}'
assert 32 'int main() { int x[3][4]; return sizeof *x;}'

echo ""
echo SUCCESS !!!!!!!!!!
