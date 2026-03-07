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
    int A,B,C,D;
    cin >> A >> B >> C >> D;
    A--; B--; C--; D--;
    vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};

    auto bfs = [&](int x, int y, vector<vector<int>> &area_, int num){
        queue<pair<int,int>> q;
        q.push({x,y});
        area_[x][y] = num;
        while(!q.empty()){
            auto [px,py] = q.front();
            q.pop();
            rep(i,0,4){
                int nx = px + dx[i];
                int ny = py + dy[i];
                if(!grid_check(nx,ny,H,W)) continue;
                if(S[nx][ny]=='#') continue;
                if(area_[nx][ny]!=-1) continue;
                q.push({nx,ny});
                area_[nx][ny] = num;
            }
        }
    };
    vector<vector<int>> area(H,vector<int>(W,-1));
    int num = 0;
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='#') continue;
            if(area[i][j]==-1){
                bfs(i,j,area,num);
                num++;
            }
        }
    }
    vector<vector<int>> g(num);
    set<pair<int,int>> edges;
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='#') continue;
            rep(k,0,4){
                int x = i + dx[k];
                int y = j + dy[k];
                int xx = x + dx[k];
                int yy = y + dy[k];
                if(grid_check(x,y,H,W)){
                    if(S[x][y]!='#') continue;
                    rep(k2,0,4){
                        int nx = x + dx[k2];
                        int ny = y + dy[k2];
                        if(!grid_check(nx,ny,H,W)) continue;
                        if(S[nx][ny]=='.'){
                            int a = area[i][j];
                            int b = area[nx][ny];
                            if(a==b) continue;
                            if(a>b) swap(a,b);
                            if(edges.count({a,b})) continue;
                            edges.insert({a,b});
                            g[a].push_back(b);
                            g[b].push_back(a);
                        }
                    }
                }

                if(grid_check(x,y,H,W) && grid_check(xx,yy,H,W)){
                    if(S[x][y]!='#' || S[xx][yy]!='#') continue;
                    rep(k2,0,4){
                        int nx = xx + dx[k2];
                        int ny = yy + dy[k2];
                        if(!grid_check(nx,ny,H,W)) continue;
                        if(S[nx][ny]=='.'){
                            int a = area[i][j];
                            int b = area[nx][ny];
                            if(a==b) continue;
                            if(a>b) swap(a,b);
                            if(edges.count({a,b})) continue;
                            edges.insert({a,b});
                            g[a].push_back(b);
                            g[b].push_back(a);
                        }
                    }

                }
            }
        }
    }

    queue<int> que;
    vector<int> dist(num,(1<<29));
    int s = area[A][B];
    int ss = area[C][D];
    que.push(s);
    dist[s] = 0;
    while(!que.empty()){
        int u = que.front();
        que.pop();
        for(int v : g[u]){
            if(dist[v]<(1<<29)) continue;
            que.push(v);
            dist[v] = dist[u] + 1;
        }
    }
    cout << dist[ss] << endl;
}