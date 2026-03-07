#include <bits/stdc++.h>
using namespace std;

/*
cpp17での書き方
*/

//template<class T> と template<typename T> は機能的に全く同じ
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
    SegmentTree<T>(vector<T> &v, function<T(T, T)> op, function<T()> e) : n(v.size()), op(op), e(e) {
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
    int N,Q;
    cin >> N >> Q;
    vector<int> A(N);
    for(int i=0;i<N;i++){
        cin >> A[i];
    }
    SegmentTree<int> seg(A, [](int a, int b){return max(a,b);}, [](){return -1;});
    for(int i=0;i<Q;i++){
        int t;
        cin >> t;
        if(t==1){
            int x,v;
            cin >> x >> v;
            x--;
            seg.set(x,v);
        }else if(t==2){
            int l,r;
            cin >> l >> r;
            l--;
            cout << seg.prod(l,r) << endl;
        }else if(t==3){
            int x,v;
            cin >> x >> v;
            x--;
            /*
            x <= j < N && v <= A[j]となる最小のj
            区間[x,j-1] ではv > max(A[k])
            区間[x,j+n] ではv <= max(A[k]) (n=0,1,...)
            つまり、単調性あり
            */
           cout << (seg.max_right(x,[&](int val){return val < v;}) + 1) << endl;
        }
    }
}
