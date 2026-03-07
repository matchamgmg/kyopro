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

struct point {
    int x;
    int y;
};

int main()
{
    int H,W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    const int inf = 1e9;
    vvi dist(H,vi(W,inf));
    queue<pair<point,int>> que;
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='E'){
                dist[i][j] = 0;
                que.push({{i,j},0});
            }
        }
    }
    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};
    vector<char> c = {'<','^','>','v'};
    while(!que.empty()){
        auto p = que.front();
        que.pop();
        int px = p.first.x;
        int py = p.first.y;
        rep(i,0,4){
            int nx = px + dx[i];
            int ny = py + dy[i];
            if(!grid_check(nx,ny,H,W)) continue;
            if(S[nx][ny]=='#') continue;
            if(dist[nx][ny]<inf) continue;
            dist[nx][ny] = p.second + 1;
            S[nx][ny] = c[i];
            que.push({{nx,ny},dist[nx][ny]});
        }
    }
    rep(i,0,H){
        cout << S[i] << '\n';
    }
    cout << flush;
}