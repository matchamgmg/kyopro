# 遅延評価セグメント木

## 何ができるのか
* 区間変更クエリ
* 区間取得クエリ

## lazy segtree が保持するデータ
* data : segtree と同じ
* lazy : その区間に対して行われた操作のうち，まだ自身より下のノードに作用させていないもの

## テンプレート引数
```C++
(1) lazy_segtree<S, op, e, F, mapping, composition, id> seg(int n)

(2) lazy_segtree<S, op, e, F, mapping, composition, id> seg(vector<T> v)
```

`S`: dataの型．区間取得結果の型

`F`: lazyの型．区間操作演算の型

`S op(S a, S b)`: 区間取得の演算

`S mapping(F f, S x)`: 操作`f`を各ノードが持つdataの値`x`に対して作用させる関数．区間変更クエリと区間取得クエリの組合せによって処理が変わる

`F composition(F f, F g)`: すでにこれまでの操作をためているlazyに対して，さらに新しい操作を追加する関数．`g`がこれまでの操作，`f`が後に追加する操作．`g`と`f`の合成写像を返す

`S e()`: 区間取得演算`op`の単位元

`F id()`: 区間操作演算`mapping`の恒等写像

## 遅延セグ木に乗る条件
* `op`が結合則を満たす`(a op b) op c = a op b op c`

* `mapping`: すべてのノードのdataに対して同じ計算式で作用させることができる

* `composition`: 複数の操作 -> 一回の操作として扱うことができる

## チートシート
### 区間加算・区間最小値取得
```C++
using S = long long;
using F = long long;
const S INF = 8e18;

S op(S a, S b){ return min(a, b); }
S e(){ return INF; }
S mapping(F f, S x){ return f+x; }
F composition(F f, F g){ return f+g; }
F id(){ return 0; }
```
### 区間加算・区間最大値取得
```C++
using S = long long;
using F = long long;
const S INF = 8e18;

S op(S a, S b){ return max(a, b); }
S e(){ return -INF; }
S mapping(F f, S x){ return f+x; }
F composition(F f, F g){ return f+g; }
F id(){ return 0; }
```
### 区間加算・区間和取得
```C++
struct S{
    long long val;
    int siz;
    S(){}
    S(long long v, int s) : val(v), siz(s) {}
};
using F = long long;

S op(S a, S b){ return {a.val + b.val, a.siz + b.siz}; }
S e(){ return {0LL, 0LL}; }
S mapping(F f, S x){ return {x.val + x.siz * f, x.siz}; }
F composition(F f, F g){ return f+g; }
F id(){ return 0; }
```
### 区間に対して一次関数を適用・一点取得・積分値取得（区間和取得）
```C++
template <class T>
struct S
{
    T val;
    int siz;
    S() {}
    S(T v, int s) : val(v), siz(s) {}
};
template <class T>
struct F
{
    T times;
    T plus;
    F() {}
    F(T t, T p) : times(t), plus(p) {}
};
template <class T>
S<T> op(S<T> a, S<T> b) { return {a.val + b.val, a.siz + b.siz}; }
template <class T>
S<T> e() { return {T{0}, 0}; }
template <class T>
S<T> mapping(F<T> f, S<T> x) { return {x.val * f.times + x.siz * f.plus, x.siz}; }
template <class T>
F<T> composition(F<T> f, F<T> g) { return {f.times * g.times, f.times * g.plus + f.plus}; }
template <class T>
F<T> id() { return {T{1}, T{0}}; }

using T = int

vector<S<T>> v(N + 1, {0, 1});
lazy_segtree<S<T>, op<T>, e<T>, F<T>, mapping<T>, composition<T>, id<T>> seg(v);
```
### 区間変更・区間最小値取得
```C++
using S = long long;
using F = long long;

const S INF = 8e18;
const F ID = INF;

S op(S a, S b){ return min(a, b); }
S e(){ return INF; }
S mapping(F f, S x){ return (f == ID ? x : f); }
F composition(F f, F g){ return (f == ID ? g : f); }
F id(){ return ID; }
```
### 区間変更・区間最大値取得
```C++
using S = long long;
using F = long long;

const S INF = 8e18;
const F ID = INF;

S op(S a, S b){ return max(a, b); }
S e(){ return -INF; }
S mapping(F f, S x){ return (f == ID ? x : f); }
F composition(F f, F g){ return (f == ID ? g : f); }
F id(){ return ID; }
```
### 区間変更・区間和取得
```C++
struct S{
    long long val;
    int siz;
    S(){}
    S(long long v, int s) : val(v), siz(s) {}
};
using F = long long;

const F ID = 8e18;

S op(S a, S b){ return {a.val + b.val, a.siz + b.siz}; }
S e(){ return {0LL, 0LL}; }
S mapping(F f, S x){ return (f == ID ? x : S(x.siz * f, x.siz)); }
F composition(F f, F g){ return (f == ID ? g : f); }
F id(){ return ID; }
```
### 区間変更・区間XOR取得
```C++
struct S{
    long long val;
    int siz;
    S(){}
    S(long long v, int s) : val(v), siz(s) {}
};
using F = long long;

const F ID = 8e18;

S op(S a, S b){ return {a.val ^ b.val, a.siz + b.siz}; }
S e(){ return {0LL, 0LL}; }
S mapping(F f, S x){ return (f == ID ? x : x.siz%2 == 0 ? S(0LL, x.siz) : S(f, x.siz)); }
F composition(F f, F g){ return (f == ID ? g : f); }
F id(){ return ID; }
```
### 区間XOR更新・区間XOR取得
```C++
struct S{
    long long val;
    int siz;
    S(){}
    S(long long v, int s) : val(v), siz(s) {}
};
using F = long long;

const F ID = 0LL;

S op(S a, S b){ return {a.val ^ b.val, a.siz + b.siz}; }
S e(){ return {0LL, 0LL}; }
S mapping(F f, S x){ return (f == ID ? x : x.siz%2 == 0 ? x : S(x.val ^ f, x.siz)); }
F composition(F f, F g){ return (f == ID ? g : f ^ g); }
F id(){ return ID; }
```