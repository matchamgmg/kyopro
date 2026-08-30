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

#define INF 1LL<<60

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
    // 最長のパス(木の直径)は一度だけ通る
    // それ以外は二度通る
    // bfsを2回行う
    auto bfs = [&](int s, vll &dist, vi &prev){
        queue<pair<ll,int>> que;
        dist[s] = 0;
        que.push({dist[s],s});
        while(!que.empty()){
            auto p = que.front();
            que.pop();
            int u = p.second;
            ll d = p.first;
            for(path pp : G[p.second]){
                int v = pp.first;
                if(dist[v]!=-1) continue;
                dist[v] = d + pp.second;
                que.push({dist[v],v});
                prev[v] = u;
            }
        }
    };
    int s = 0;
    int g = -1;
    vll dist_s(N,-1), dist_g(N,-1);
    vi prev_s(N), prev_g(N);
    bfs(s,dist_s,prev_s);
    ll max_dist = -1;
    rep(i,0,N){
        if(dist_s[i]>max_dist){
            max_dist = dist_s[i];
            g = i;
        }
    }
    bfs(g,dist_g,prev_g);
    max_dist = -1;
    rep(i,0,N){
        if(dist_g[i]>max_dist){
            max_dist = dist_g[i];
            s = i;
        }
    }
    set<pair<int,int>> st;
    while(s!=g){
        st.insert({s,prev_g[s]});
        s = prev_g[s];
    }
    ll ans = 0;
    rep(i,0,N){
        for(auto p : G[i]){
            if(st.count({i,p.first})) continue;
            ans += p.second;
        }
    }
    cout << ans << endl;
}