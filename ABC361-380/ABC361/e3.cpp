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

int main()
{
    int N;
    cin >> N;
    using path = pair<int,ll>;
    vector<vector<path>> G(N);
    rep(i,0,N-1){
        int u,v;
        ll c;
        cin >> u >> v >> c;
        u--;
        v--;
        G[u].push_back({v,c});
        G[v].push_back({u,c});
    }

    // dfsで直径を求める
    auto dfs = [&](auto f, int u, ll d=0, int p=-1) -> pair<ll,int> {
        auto res = make_pair(d,u);
        for(auto e: G[u]){
            if(e.first==p) continue;
            res = max(res,f(f,e.first,d+e.second,u));
        }
        return res;
    };

    auto bfs = [&](int s, vll &dist){
        queue<pair<int,ll>> que;
        dist[s] = 0;
        que.push({s,dist[s]});
        while(!que.empty()){
            auto p = que.front();
            que.pop();
            int u = p.first;
            ll d = p.second;
            for(auto pp : G[u]){
                int v = pp.first;
                if(dist[v]!=-1) continue;
                dist[v] = d + pp.second;
                que.push({v,dist[v]});
            }
        }
    };

    // int s = 0, g = -1;
    // vll dist_s(N,-1), dist_g(N,-1);
    // bfs(s,dist_s);
    // ll max_dist = -1;
    // rep(i,0,N){
    //     if(max_dist<dist_s[i]){
    //         g = i;
    //         max_dist = dist_s[i];
    //     }
    // }
    // bfs(g,dist_g);
    ll ans = 0;
    // ll diameter = *max_element(all(dist_g));
    rep(i,0,N){
        for(path p : G[i]){
            ans += p.second;
        }
    }
    int a = dfs(dfs,0).second;
    ll diameter = dfs(dfs,a).first;
    cout << (ans - diameter) << endl;
}