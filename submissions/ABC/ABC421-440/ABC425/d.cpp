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
    int H, W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H) cin >> S[i];
    vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};
    const int INF = 1<<29;
    vector<vector<int>> dist(H,vector<int>(W,INF));
    queue<pair<int,int>> que;
    int ans = 0;
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='#'){
                dist[i][j] = 0;
                ans++;
                que.push(make_pair(i,j));
            }
        }
    }
    queue<pair<int,int>> que2;
    int mode = 0;
    while(!que.empty()){
        while(!que.empty() && dist[que.front().first][que.front().second]==mode){
            auto p = que.front();
            que.pop();
            rep(d,0,4){
                int nx = p.first + dx[d];
                int ny = p.second + dy[d];
                if(!grid_check(nx,ny,H,W)) continue;
                if(dist[nx][ny] < INF) continue;
                int cnt = 0;
                rep(i,0,4){
                    int nnx = nx + dx[i];
                    int nny = ny + dy[i];
                    if(!grid_check(nnx,nny,H,W)) continue;
                    if(S[nnx][nny] == '#') cnt++;
                }
                if(cnt == 1){
                    ans++;
                    dist[nx][ny] = dist[p.first][p.second] + 1;
                    que2.push(make_pair(nx,ny));
                    que.push(make_pair(nx,ny));
                }
            }
        }
        while(!que2.empty()){
            auto p = que2.front();
            que2.pop();
            S[p.first][p.second] = '#';
        }
        mode++;
    }
    cout << ans << endl;
}