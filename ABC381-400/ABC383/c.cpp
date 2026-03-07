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
    int H,W,D;
    cin >> H >> W >> D;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    vector<vector<int>> dist(H,vector<int>(W,-1));
    vector<pair<int,int>> humidifier;
    rep(i,0,H) rep(j,0,W){
        if(S[i][j]=='H') humidifier.push_back({i,j});
    }
    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};
    int ans = 0;
    queue<pair<int,int>> que;
    for(auto p : humidifier){
        dist[p.first][p.second] = 0;
        que.push({p.first,p.second});
    }
    while(!que.empty()){
        auto p = que.front();
        int dis = dist[p.first][p.second];
        int x = p.first;
        int y = p.second;
        que.pop();
        rep(i,0,4){
            int xx = x + dx[i];
            int yy = y + dy[i];
            if(!(0<=xx && xx<H && 0<=yy && yy<W)) continue;
            if(dist[xx][yy]!=-1) continue;
            if(S[xx][yy]=='#') continue;
            dist[xx][yy] = dis + 1;
            que.push({xx,yy});
        }
    }
    rep(i,0,H) rep(j,0,W){
        if(dist[i][j]<=D && dist[i][j]>=0) ans++;
    }
    cout << ans << endl;
}