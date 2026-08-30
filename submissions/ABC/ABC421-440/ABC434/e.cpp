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
#define input(A) rep(i,0,A.size()) cin >> A[i]

struct UnionFind {
    int n;
    int group_num;
    vector<int> parent;
    vector<int> siz;
    vector<int> edges_num;
    UnionFind(int n_) : n(n_) {
        group_num = n_;
        parent.assign(n_,-1);
        siz.assign(n_,1);
        edges_num.assign(n_,0);
    }

    int root(int u){
        assert(u<n);
        if(parent[u]==-1) return u;
        return parent[u] = root(parent[u]);
    }

    bool same(int u, int v){
        assert(u<n&&v<n);
        return root(u) == root(v);
    }

    bool merge(int u, int v){
        assert(u<n&&v<n);
        if(same(u,v)){
            edges_num[root(u)]++;
            return false;
        }
        u = root(u), v = root(v);
        if(siz[u]<siz[v]) swap(u,v);
        siz[u] += siz[v];
        edges_num[u] += edges_num[v];
        edges_num[u]++;
        parent[v] = u;
        group_num--;
        return true;
    }

    int groups() {
        return group_num;
    }
};

int main()
{
    int N;
    cin >> N;
    vll X(N), R(N);
    rep(i,0,N){
        cin >> X[i] >> R[i];
    }
    // ウサギのとび先を頂点，ウサギを辺とする
    // 各連結成分は，木または木でない
    // 木のとき，頂点数-1
    // 葉の方から使う
    // 木でないとき，頂点数
    // 全域木+余っている辺1つについて，頂点の片方を根とする
    // 最後に根を選択する
    set<ll> st;
    rep(i,0,N){
        st.insert(X[i]+R[i]);
        st.insert(X[i]-R[i]);
    }
    int M = st.size();
    map<ll,int> mp;
    int m = 0;
    for(ll a : st){
        mp[a] = m;
        m++;
    }
    UnionFind uf(M);
    rep(i,0,N){
        uf.merge(mp[X[i]+R[i]],mp[X[i]-R[i]]);
    }
    ll ans = 0;
    rep(i,0,M){
        if(uf.root(i)==i){
            if(uf.edges_num[i]==uf.siz[i]-1){
                ans += uf.edges_num[i];
            }else{
                ans += uf.siz[i];
            }
        }
    }
    cout << ans << endl;
}