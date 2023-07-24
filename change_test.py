import sys
def input(): return sys.stdin.readline().strip()
def mapint(): return list(map(int, input().split()))
sys.setrecursionlimit(10**9)

assert_list = """
# 数値
assert 0 'return 0;'
assert 42 'return 42;'
# 式
assert 21 'return 5+20-4;'
assert 144 'return 12 * (13 - 1) ;'
assert 10 'return -4 * 10 + 5 * +10 ;'
# 比較
assert 1 'return 10*-1+11==1 ;'
assert 1 'return 20+1 <= 21 ;'
assert 0 'return 15 > 15;'
# 変数
assert 20 'foo=2; bar=10; return foo*bar;'
assert 11 'f1 = 30; f2 = 19; return f1-f2;'
# 条件文
assert 3 'if(1) return 3; return 1;'
assert 17 'a=1; b=15; if(a>=b) return 0; return 17;'
assert 16 'a=16; b=15; while(a<b) a=a*2; return a;'
assert 51 'i=0; j=0; for(i=1; i<=16; i=i+3) j=i+j; return j;'
assert 1  'for(;;) return 1; return 2;'
assert 5 'a=1; b=1; while(a<5){a=a+1; b=b+1;} return b;'

assert 3 'return ret3();'
assert 5 'return add(6, -1);'
"""

pre = "'main() {"
suf = "}'"
for asserts in assert_list.split("\n"):
    x = asserts.split()
    if(len(x)<2):
        print(*x)
        continue
    if(x[0]!="assert"):
        print(*x)
        continue
    old = " ".join(x[2:])
    old = old[1:-1]
    new = x[:2] + [pre + old + suf]
    print(*new)
    