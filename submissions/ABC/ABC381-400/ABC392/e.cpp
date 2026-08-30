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
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

struct UnionFind
{
    vector<int> par, siz;
    /*
    par[i] : iを含むグループの根
    siz[i] : iを根とする木の頂点数
    */

    //初期化
    UnionFind(int n) : par(n, -1), siz(n, 1) {}

    //根を求める
    int root(int x){
        if(par[x]==-1) return x;
        else return par[x] = root(par[x]);
    }

    //xとyが同じグループに属するかどうか
    bool issame(int x, int y){
        return root(x) == root(y);
    }

    //xを含むグループとyを含むグループとを併合する
    bool unite(int x, int y){
        x = root(x); y = root(y);
        if(x==y) return false;
        if(siz[x] < siz[y]) swap(x,y);  //分岐を書くのを省略しているだけ
        par[y] = x;
        siz[x] += siz[y];
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
    int N,M;
    cin >> N >> M;
    vi A(M),B(M);
    vi edge_idx;
    UnionFind uf(N);
    vector<pair<int,int>> edges;
    rep(i,0,M){
        cin >> A[i] >> B[i];
        A[i]--;
        B[i]--;
        if(uf.issame(A[i],B[i])){
            edges.push_back({A[i],B[i]});
            edge_idx.push_back(i+1);
        }
        uf.unite(A[i],B[i]);
    }
    int j = 0;
    int cnt = 0;
    stringstream ss;
    int siz = edges.size();
    rep(i,0,siz){
        while(j<N && (uf.issame(j,edges[i].first) || uf.issame(j,edges[i].second))){
            j++;
        }
        if(j<N && (!uf.issame(j,edges[i].first) && !uf.issame(j,edges[i].second))){
            cnt++;
            ss << edge_idx[i] << ' ';
            if(!uf.issame(j,edges[i].first)){
                ss << edges[i].first+1 << ' ';
                uf.unite(j,edges[i].first);
            }else{
                ss << edges[i].second+1 << ' ';
                uf.unite(j,edges[i].second);
            }
            ss << j+1 << '\n';
        }
    }
    cout << cnt << endl;
    cout << ss.str() << endl;
}