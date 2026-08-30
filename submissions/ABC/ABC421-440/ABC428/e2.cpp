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

template<class T, auto e, auto merge, auto add_root>
struct Rerooting {
    vector<vector<T>> dp;
    vector<T> ans;
    vector<vector<int>> G;

    Rerooting() {}
    Rerooting(int N) : G(N) {
        dp.resize(N);
        ans.assign(N, e(-1));
    }
    Rerooting(vector<vector<int>> F) : G(F){
        int N = F.size();
        dp.resize(N);
        ans.assign(N, e());
    }

    void add_edge(int a, int b){
        G[a].push_back(b);
    }
    void build(){   // 全方位木DP
        dfs1(0);
        dfs2(0,e(-1));
    }

    T dfs1(int u, int p = -1){
        T dp_cum = e(u);
        int siz = G[u].size();
        dp[u].assign(siz, e(-1));
        for(int i=0; i<siz; ++i){
            int v = G[u][i];
            if(v == p) continue;
            dp[u][i] = dfs1(v, u);
            dp_cum = merge(dp_cum, dp[u][i]);
        }
        return add_root(dp_cum);
    }
    void dfs2(int u, const T &dp_p, int p = -1){
        int siz = G[u].size();
        for(int i=0; i<siz; ++i){
            if(G[u][i] == p) dp[u][i] = dp_p;
        }
        vector<T> dp_l(siz+1, e(-1)), dp_r(siz+1, e(-1));
        dp_l[0] = e(u);
        dp_r[siz] = e(u);
        for(int i=0; i<siz; ++i){
            dp_l[i+1] = merge(dp_l[i], dp[u][i]);
        }
        for(int i=siz-1; i>=0; --i){
            dp_r[i] = merge(dp_r[i+1], dp[u][i]);
        }
        ans[u] = add_root(dp_l[siz]);
        for(int i=0; i<siz; ++i){
            int v = G[u][i];
            if(v == p) continue;
            dfs2(v, add_root(merge(dp_l[i], dp_r[i+1])), u);
        }
    }
};

using pii = pair<int,int>;

pii e(int i = -1){ return make_pair(0, i); }
// mergeはstd::で使用されている
pii merge_(pii a, pii b){
    return (a.first == b.first ? a.second > b.second ? a : b : a.first > b.first ? a : b);
}
pii add_root(pii a){
    return make_pair(a.first+1,a.second);
}

int main()
{
    // はじめの木DPの初期値と累積mergeの初期値に注意
    // というよりも，今回の問題に関しては，単位元の選び方に注意が必要
    // もっと言えば，葉ノードについての値を適切に設定する必要がある
    int N;
    cin >> N;
    Rerooting<pii, e, merge_, add_root> reroot(N);
    rep(i,0,N-1){
        int a, b;
        cin >> a >> b;
        a--, b--;
        reroot.add_edge(a,b);
        reroot.add_edge(b,a);
    }
    reroot.build();
    rep(i,0,N){
        cout << reroot.ans[i].second+1 << endl;
    }
}