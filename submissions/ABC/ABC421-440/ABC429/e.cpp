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

int main()
{
    int N,M;
    cin >> N >> M;
    vector G(N,vector<int>());
    rep(i,0,M){
        int a, b;
        cin >> a >> b;
        a--, b--;
        G[a].emplace_back(b);
        G[b].emplace_back(a);
    }
    string S;
    cin >> S;
    // for each D, min(d(S1-D)+d(S2-D))
    // 多始点BFS
    // 記録するもの
    // 各頂点について，
    // 1. 始点のなかで最も近い頂点からの距離
    // 2. 始点の中で2番目に近い頂点からの距離
    vector<set<int>> visited(N);
    vector<map<int,int>> dist(N);
    queue<pair<int,int>> que; //u, from
    rep(i,0,N){
        if(S[i]=='S'){
            que.push(make_pair(i,i));
            visited[i].insert(i);
            dist[i][i] = 0;
        }
    }
    while(!que.empty()){
        auto u = que.front();
        que.pop();
        for(int v : G[u.first]){
            if(visited[v].count(u.second)) continue;
            if(visited[v].size()>=2) continue;
            que.push(make_pair(v,u.second));
            visited[v].insert(u.second);
            dist[v][u.second] = dist[u.first][u.second] + 1;
        }
    }
    int ans = 0;
    rep(i,0,N){
        if(S[i]=='D'){
            ans = 0;
            for(auto p : dist[i]){
                ans += p.second;
            }
            cout << ans << endl;
        }
    }
}