#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ull = unsigned long long;
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

struct Edge{
    int to;
    ll weight;
};

int main()
{
    int N,M;
    cin >> N >> M;
    vector<vector<Edge>> G(N);
    rep(i,0,M){
        int a,b;
        ll w;
        cin >> a >> b >> w;
        a--;
        b--;
        G[a].push_back({b,w});
        G[b].push_back({a,w});
    }
    ull ans = 0 - 1;

    vector<bool> seen(N,false);
    auto dfs = [&](auto f, int u, ull weight) -> void {
        if(u==N-1){
            chmin(ans,weight);
            return;
        }
        for(auto e : G[u]){
            if(seen[e.to]==false){
                seen[e.to] = true;
                f(f,e.to,weight^e.weight);
                seen[e.to] = false;
            }
        }
        return;
    };
    seen[0] = true;
    dfs(dfs,0,0);
    cout << ans << endl;
}