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

#define INF 1<<30

int main()
{
    int H,W;
    cin >> H >> W;
    vector<string> S(H);
    pair<int,int> start,goal;
    rep(i,0,H){
        cin >> S[i];
        rep(j,0,W){
            if(S[i][j]=='S'){
                start = {i,j};
            }
            if(S[i][j]=='G'){
                goal = {i,j};
            }
        }
    }
    vvi dist1(H,vi(W,INF));
    vvi dist2(H,vi(W,INF));

    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};

    // muki 0:tate,1:yoko

    auto bfs = [&](pair<int,int> s, int parity, vvi &dist) -> int {
        queue<pair<pair<int,int>,pair<int,int>>> que;
        dist[s.first][s.second] = 0;
        que.push({{0,parity},s});
        while(!que.empty()){
            auto p = que.front();
            que.pop();
            int dis = p.first.first;
            int muki = p.first.second;
            int x = p.second.first;
            int y = p.second.second;
            rep(i,0,4){
                if(i%2==muki) continue;
                int dmuki = 1 - muki;
                int xx = x + dx[i];
                int yy = y + dy[i];
                if(!grid_check(xx,yy,H,W)) continue;
                if(dist[xx][yy]!=INF) continue;
                if(S[xx][yy]=='#') continue;
                que.push({{dis+1,dmuki},{xx,yy}});
                dist[xx][yy] = dis+1;
                if(S[xx][yy]=='G'){
                    return dis+1;
                }
            }
        }
        return dist[goal.first][goal.second];
    };

    int ans = INF;
    chmin(ans,bfs(start,0,dist1));
    chmin(ans,bfs(start,1,dist2));
    if(ans==INF){
        ans = -1;
    }
    cout << ans << endl;
}