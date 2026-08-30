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
    int H,W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    int a,b,c,d;
    cin >> a >> b >> c >> d;
    a--, b--, c--, d--;
    vector<vector<pair<int,int>>> g(H*W);
    vi dx = {0,1,0,-1}, dy ={1,0,-1,0};
    rep(i,0,H)rep(j,0,W){
        rep(k,0,4){
            int x = i + dx[k];
            int y = j + dy[k];
            if(!grid_check(x,y,H,W)) continue;
            if(S[x][y]=='.'){
                g[i*W+j].push_back({x*W+y,0});
            }else{
                g[i*W+j].push_back({x*W+y,1});
            }
            int xx = x + dx[k];
            int yy = y + dy[k];
            if(!grid_check(xx,yy,H,W)) continue;
            if(S[x][y]=='.' && S[xx][yy]=='.'){
                g[i*W+j].push_back({xx*W+yy,0});
            }else{
                g[i*W+j].push_back({xx*W+yy,1});
            }
        }
    }
    deque<pair<int,int>> q;
    int inf = 1<<29;
    vector<int> dist(H*W,inf);
    dist[a*W+b] = 0;
    q.push_back({a*W+b,dist[a*W+b]});
    while(!q.empty()){
        auto [u,d1] = q.front();
        q.pop_front();
        if(dist[u]<d1) continue;
        for(auto [v,dd] : g[u]){
            if(dist[v]<=dist[u]+dd) continue;
            chmin(dist[v],dist[u]+dd);
            if(dd==0) q.push_front({v,dist[v]});
            else q.push_back({v,dist[v]});
        }
    }
    cout << dist[c*W+d] << endl;
}