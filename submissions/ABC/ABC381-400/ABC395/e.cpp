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

#define INF 1LL<<62

int main()
{
    int N,M;
    cin >> N >> M;
    ll X;
    cin >> X;
    vector<vector<pair<int,ll>>> G(N*2);
    rep(i,0,M){
        int u,v;
        cin >> u >> v;
        u--;
        v--;
        G[u].emplace_back(v,1LL);
        G[v+N].emplace_back(u+N,1LL);
    }
    rep(i,0,N){
        G[i].emplace_back(i+N,X);
        G[i+N].emplace_back(i,X);
    }
    int start = 0;
    vll dist(2*N,INF);
    pq_g<pair<ll,int>> que;
    dist[start] = 0;
    que.emplace(dist[start],start);
    while(!que.empty()){
        auto p = que.top();
        ll d = p.first;
        int u = p.second;
        que.pop();
        if(d > dist[u]) continue;
        for(auto n : G[u]){
            int v = n.first;
            ll cost = n.second;
            ll nd = d + cost;
            if(nd >= dist[v]) continue;
            dist[v] = nd;
            que.emplace(dist[v],v);
        }
    }
    cout << min(dist[N-1],dist[2*N-1]) << endl;
}