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

struct edge {
    int v;
    ll w;
};

int main()
{
    int N,M;
    cin >> N >> M;
    vector<vector<edge>> G(N);
    rep(i,0,M){
        int u;
        int v;
        ll w;
        cin >> u >> v >> w;
        u--;
        v--;
        G[u].push_back({v,w});
    }
    const ll inf = 1LL<<60;
    vector<ll> dist(N,inf);
    dist[0] = 0;
    pq_g<pair<int,ll>> q;
    q.push({0,0});
    while(!q.empty()){
        auto p = q.top();
        q.pop();
        if(p.second > dist[p.first]) continue;
        for(edge e : G[p.first]){
            if((p.second | e.w) >= dist[e.v]) continue;
            dist[e.v] = (p.second | e.w);
            q.push({e.v,dist[e.v]});
        }
    }
    cout << dist[N-1] << endl;
}