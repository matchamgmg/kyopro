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
    int W, H;
    cin >> W >> H;
    vector<vector<int>> Field(H,vector<int>(W));
    rep(i,0,H){
        rep(j,0,W){
            cin >> Field[i][j];
        }
    }
    vector<vector<int>> dx = {{-1,0,1,1,1,0},{-1,-1,0,1,0,-1}}; // d[0] : when y even
    vector<vector<int>> dy = {{0,-1,-1,0,1,1},{0,-1,-1,0,1,1}};
    
    // 空洞か外かを判定する　空洞は建物で埋める
    // BFS : 地図の外に当たれば外，最後まで当たらなければたどったマスすべてを建物に変える
    vector<vector<bool>> visited(H,vector<bool>(W,false));
    auto bfs = [&](int x, int y){
        int nx, ny;
        if(visited[y][x]) return;
        if(Field[y][x] == 1) return;
        queue<pair<int,int>> que1, que2;
        bool is_out = false;
        visited[y][x] = true;
        que1.push(make_pair(x,y));
        while(!que1.empty()){
            auto p = que1.front();
            que1.pop();
            que2.push(p);
            rep(i,0,6){
                nx = p.first + dx[p.second%2][i];
                ny = p.second + dy[p.second%2][i];
                if(!grid_check(nx,ny,W,H)){
                    is_out = true;
                    continue;
                }
                if(visited[ny][nx]) continue;
                if(Field[ny][nx] == 1) continue;
                visited[ny][nx] = true;
                que1.push(make_pair(nx,ny));
            }
        }
        while(!que2.empty()){
            auto p = que2.front();
            que2.pop();
            Field[p.second][p.first] = (!is_out);
        }
        return;
    };

    rep(i,0,H){
        rep(j,0,W){
            bfs(j,i);
        }
    }
    rep(i,0,H){
        rep(j,0,W){
            visited[i][j] = false;
        }
    }
    // 建物について左手法
    int ans = 0;
    auto is_inner = [&](int x, int y) -> bool {
        int xx, yy;
        bool res = true;
        rep(d,0,6){
            xx = x + dx[y%2][d];
            yy = y + dy[y%2][d];
            if(!grid_check(xx,yy,W,H)){
                res = false;
                continue;
            }
            res &= (Field[yy][xx] == 1);
        }
        return res;
    };

    auto bfs2 = [&](int x, int y){
        queue<pair<int,int>> que;
        visited[y][x] = true;
        que.push(make_pair(x,y));
        int nx, ny;
        while(!que.empty()){
            auto p = que.front();
            que.pop();
            rep(d,0,6){
                nx = p.first + dx[p.second%2][d];
                ny = p.second + dy[p.second%2][d];
                if(!grid_check(nx,ny,W,H)){
                    ans++;
                    continue;
                }
                if(Field[ny][nx] == 0) ans++;
            }
            rep(d,0,6){
                nx = p.first + dx[p.second%2][d];
                ny = p.second + dy[p.second%2][d];
                if(!grid_check(nx,ny,W,H)) continue;
                if(visited[ny][nx]) continue;
                if(Field[ny][nx] == 0) continue;
                if(is_inner(nx,ny)) continue;
                visited[ny][nx] = true;
                que.push(make_pair(nx,ny));
            }
        }
        return;
    };

    // cout << endl;
    // vv_cout(Field);

    rep(i,0,H){
        rep(j,0,W){
            if(visited[i][j]) continue;
            if(Field[i][j] == 0) continue;
            if(is_inner(j,i)) continue;
            bfs2(j,i);
        }
    }
    cout << ans << endl;
}