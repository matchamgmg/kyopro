/*
セグメント木

関連用語 :
    モノイド(結合律を満たし、単位元が存在するような代数構造)
        二項演算op(a,b)に対して
            結合律 : op(op(a,b), c) = op(a, op(b,c))    for all a,b,c in S
            単位元の存在 : op(a, e) = op(e, a) = a      for all a in S

完全二分木
数列{a_i}について
{a_i | i = s~t}について、クエリをO(log N)で処理する
a_iの値をO(log N)で更新する

木の高さ : O(log N)
全体のセグメント数 : O(N)

それぞれのセグメントの値を求める :
    下から順に計算していけば全体でO(N)
    直下のセグメントの値が分かっていると仮定して、一つあたりO(1)

_______________________________
_______________ _______________
_______ ======= _______ _______     [2,11)に対するクエリ
___ === ___ ___ === ___ ___ ___
_ _ _ _ _ _ _ _ _ _ = _ _ _ _ _
0 1 2 3 4 5 6 7 8 9 A B C D E F

===============================
=============== _______________
_______ ======= _______ _______     a_6の値を更新
___ ___ ___ === ___ ___ ___ ___
_ _ _ _ _ _ = _ _ _ _ _ _ _ _ _
0 1 2 3 4 5 6 7 8 9 A B C D E F

セグ木に乗る演算の条件
数列の各要素と、区間に対して行う演算が結合法則を満たす
(交換法則は満たさなくてよい)

型 S
二項演算 S op(S a, S b)
単位元 S e()
を定義する必要がある

Range Min Queryの場合
e() = INF
    min(a,INF) = min(INF,a) = a

コンストラクタ
SegmentTree<S> seg(int n, function<S(S, S)> op, function<S()> e)
    長さnの数列aを作る。初期値はすべてe()
SegmentTree<S> seg(vector<S> v, function<S(S, S)> op, function<S()> e)
    長さn=v.size()の数列aをつくる。vの内容が初期値となる

関数
set(int p, S x) : a[p]にxを代入する O(log N)
get(int p) : a[p]を返す O(1)
prod(int l, int r) : op(a[l],...,a[r-1])を計算する
    l = rのときはe()を返す  O(log N)
all_prod() : op(a[0],...,a[n-1])を計算する
    n = 0のときはe()を返す  O(1)

(関数オブジェクト : ラムダ式とかgreater<int>{}とか)

セグメント木上の二分探索    O(log N)
max_right<f>(int l) : bool f(S x)を定義する必要がある
max_right<F>(int l, F f) : Sを引数にとりboolを返す関数オブジェクトを渡す
    気持ち :
        f(op(a[l],...,a[r-1])) = true
        && f(op(a[l],...,a[r])) = false
        となるrをいずれか一つ返す
        fが単調なとき、f(op(a[l],...,a[r-1])) = true となる最大のrを返す
    実際 :
        コーナーケースr=l, r=nを考慮する
        r=n : 全部true (f(op(a[l],...,a[r])) = false となる区間が存在しないとき)
        r=l : 全部false (f(op(a[l],...,a[r-1])) = true となる区間が存在しないとき)
    まとめると :
        (r=l || f(op(a[l],...,a[r-1])) = true)
        && (r=n || f(op(a[l],...,a[r])) = false)
        を満たすrをいずれか一つ返す
    f(e()) = true
min_left<f>(int r)
min_left<F>(int r, F f)
    気持ち :
        f(op(a[l],...,a[r-1])) = true
        && f(op(a[l-1],...,a[r-1])) = false
        となるlをいずれか一つ返す
        fが単調なとき、f(op(a[l],...,a[r-1])) = true となる最小のlを返す
    実際 :
        コーナーケースl=r, l=0を考慮する
        l=0 : 全部true (f(op(a[l-1],...,a[r-1])) = false となる区間が存在しないとき)
        l=r : 全部false (f(op(a[l],...,a[r-1])) = true となる区間が存在しないとき)
    まとめると :
        (l=r || f(op(a[l],...,a[r-1])) = true)
        && (l=0 || f(op(a[l-1],...,a[r])) = false)
        を満たすlをいずれか一つ返す
    f(e()) = true

セグ木内の配列について、1-indexedの方が実装しやすい
a[0]は使わない方針
ノード数 : 2*n - 1 (n >= v.size())
要素数2*nの配列を用意すればOK

op(T l, T r) の引数の順番に注意
    交換律が成り立たなくてもよいため
*/

#include <bits/stdc++.h>
using namespace std;

//template<class T> と template<typename T> は機能的に全く同じ
//C++17の書き方
template<class T>
struct SegmentTree {
    private:
    int n;  //与える配列の要素数
    int siz;  //葉ノードの数
    int dep;    //木の深さ
    vector<T> a;
    void update(int k){
        a[k] = op(a[2*k], a[2*k+1]);
    }

    public:
    function<T(T, T)> op;
    function<T()> e;
    

    //コンストラクタ
    SegmentTree<T>(int N, function<T(T, T)> op, function<T()> e) : n(N), op(op), e(e) {
        siz = 1;
        dep = 0;
        while(siz < n){
            siz *= 2;
            dep++;
        }
        a = vector<T>(2*siz, e());
    }
    SegmentTree<T>(vector<T> v, function<T(T, T)> op, function<T()> e) : n(v.size()), op(op), e(e) {
        siz = 1;
        dep = 0;
        while(siz < n){
            siz *= 2;
            dep++;
        }
        a = vector<T>(2*siz, e());
        for(int i=0 ; i<n ; i++){
            a[siz+i] = v[i];
        }
        for(int i=siz-1 ; i>=1 ; i--){
            update(i);
        }
    }

    //メソッド
    void set(int p, T x){
        assert(0<=p && p<n);
        p += siz;
        a[p] = x;
        for(int i=1 ; i<=dep ; i++){
            update(p >> i);
        }
    }

    T get(int p){
        assert(0<=p && p<n);
        return a[p+siz];
    }

    T prod(int l, int r){//葉から順番に評価
        assert(0<=l && l<=r && r<=n);
        T l_val = e(), r_val = e();
        l += siz;
        r += siz;
        while(l<r){
            if(l&1){
                l_val = op(l_val, a[l]);
                l++;
            }
            if(r&1){
                r_val = op(a[r-1], r_val);
                r--;
            }
            l >>= 1;
            r >>= 1;
        }
        return op(l_val, r_val);
    }

    T all_prod(){
        return a[1];
    }

    //二分探索
    template<class F>
    int max_right(int l, F f){
        assert(0<=l && l<=n);
        assert(f(e()));
        if(l==n) return n;
        l += siz;
        T seg_val = e();
        do{
            while(l%2==0) l >>= 1;  //左端のセグメントa[l]から判定する
            if(!f(op(seg_val, a[l]))){
                while(l<siz){   //ここで二分探索
                    l = (2*l);
                    if(f(op(seg_val, a[l]))){
                        seg_val = op(seg_val, a[l]);
                        l++;
                    }
                }
                return l - siz;
            }
            seg_val = op(seg_val, a[l]);    //はじめはここに飛ぶ
            l++;
        } while((l&-l)!=l); //(l&1) : 2^{lのLSBから数えた連続する0の個数}, l=2^kのときだけ(l&-l)==lになる
        //はじめのループでf(op(seg_val, d[l])) = true となったとき
        return n;
    }

    template<class F>
    int min_left(int r, F f){
        assert(0<=r && r<=n);
        assert(f(e()));
        if(r==0) return 0;
        r += siz;
        T seg_val = e();
        do{
            r--;
            while(r>1 && (r%2)) r >>= 1;  //右端のセグメントa[r]から判定する
            if(!f(op(a[r], seg_val))){
                while(r<siz){   //ここで二分探索
                    r = (2*r + 1);
                    if(f(op(a[r], seg_val))){
                        seg_val = op(a[r], seg_val);
                        r--;
                    }
                }
                return r + 1 - siz;
            }
            seg_val = op(a[r], seg_val);    //はじめはここに飛ぶ
        } while((r&-r)!=r); //(l&1) : 2^{lのLSBから数えた連続する0の個数}, l=2^kのときだけ(l&-l)==lになる
        //はじめのループでf(op(seg_val, d[l])) = true となったとき
        return 0;
    }
};

int main(){
    SegmentTree<int> seg1(3, [](int a, int b){return min(a,b);}, [](){return 1<<29;});
    seg1.set(0,1);
    seg1.set(2,3);
    cout << seg1.prod(0,3) << endl;

    auto op = [](int a, int b){
        return a+b;
    };
    auto e = [](){
        return 0;
    };
    SegmentTree<int> seg2({1,2,3,4}, op, e);
    cout << seg2.all_prod() << endl;
    
    cout << seg2.max_right(2, [](int val){return val<=7;}) << endl;
    cout << seg2.min_left(4, [](int val){return val<=7;}) << endl;
    auto q = [](int val){
        return val <= 6;
    };
    cout << seg2.max_right(0, q) << endl;
    cout << seg2.min_left(4, q) << endl;
}