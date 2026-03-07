#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

struct UnionFind
{
    vector<int> par, siz, black, isblack;
    /*
    par[i] : iを含むグループの根
    siz[i] : iを根とする木の頂点数
    */

    //初期化
    UnionFind(int n) : par(n, -1), siz(n, 1), black(n, 0), isblack(n, 0) {}

    //根を求める
    int root(int x){
        if(par[x]==-1) return x;
        else return par[x] = root(par[x]);
    }

    //xとyが同じグループに属するかどうか
    bool issame(int x, int y){
        return root(x) == root(y);
    }

    // query
    bool query(int x){
        return black[root(x)] >= 1;
    }

    void reverse(int x){
        if(isblack[x]==0){
            isblack[x] = 1;
            black[root(x)] += 1;
        }else{
            isblack[x] = 0;
            black[root(x)] -= 1;
        }
    }

    //xを含むグループとyを含むグループとを併合する
    bool unite(int x, int y){
        x = root(x); y = root(y);
        if(x==y) return false;
        if(siz[x] < siz[y]) swap(x,y);  //分岐を書くのを省略しているだけ
        par[y] = x;
        siz[x] += siz[y];
        black[x] += black[y];
        return true;
    }

    //連結成分の個数を求める
    int groups(){
        int res = 0;
        for(int i=0;i<(int)par.size();i++){
            if(root(i)==i) res++;
        }
        return res;
    }

    //xを含むグループのサイズを返す
    int size(int x){
        return siz[root(x)];
    }
};

int main()
{
    int N,Q;
    cin >> N >> Q;
    UnionFind uf(N);
    while(Q--){
        int t;
        cin >> t;
        if(t==1){
            int x,y;
            cin >> x >> y;
            x--;
            y--;
            uf.unite(x,y);
        }else if(t==2){
            int x;
            cin >> x;
            x--;
            uf.reverse(x);
        }else{
            int x;
            cin >> x;
            x--;
            YN(uf.query(x));
        }
    }
}