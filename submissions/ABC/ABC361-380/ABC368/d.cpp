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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int N,K;
    cin >> N >> K;
    vector<set<int>> G(N);
    rep(i,0,N-1){
        int a,b;
        cin >> a >> b;
        a--; b--;
        G[a].insert(b);
        G[b].insert(a);
    }
    set<int> V;
    rep(i,0,K){
        int v;
        cin >> v;
        v--;
        V.insert(v);
    }
    vi dist1(N,-1), dist2(N,-1);
    auto bfs = [&](int p, vi &dist) -> int {
        queue<int> que;
        que.push(p);
        dist[p] = 0;
        int v;
        while(!que.empty()){
            v = que.front();
            int dis = dist[v];
            que.pop();
            for(int u : G[v]){
                if(dist[u]!=-1) continue;
                dist[u] = dis + 1;
                que.push(u);
            }
        }
        return v;
    };
    int p1, p2;
    p1 = 0;
    p2 = bfs(p1,dist1);
    p1 = bfs(p2,dist2);
    vi idx(N);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return dist2[i] > dist2[j]; });
    int ans = N;
    rep(i,0,N){
        int v = idx[i];
        if(V.count(v)) continue;
        if(G[v].size()>=2) continue;
        int u = *G[v].begin();
        G[v].erase(u);
        G[u].erase(v);
        ans--;
    }
    rrep(i,0,N){
        int v = idx[i];
        if(V.count(v)) continue;
        if(G[v].size()!=1) continue;
        int u = *G[v].begin();
        G[v].erase(u);
        G[u].erase(v);
        ans--;
    }
    cout << ans << endl;
}