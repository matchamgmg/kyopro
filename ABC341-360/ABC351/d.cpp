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
#define next_p(a) next_permutation(all(a))
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
    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='#'){
                rep(k,0,4){
                    int x = i + dx[k];
                    int y = j + dy[k];
                    if(!grid_check(x,y,H,W)) continue;
                    if(S[x][y]=='#') continue;
                    S[x][y] = '@';
                }
            }
        }
    }
    int ans = 0;
    vvi seen(H,vi(W,-1));
    auto bfs = [&](int x,int y) -> int {
        if(S[x][y]=='@') return 1;
        int res = 0;
        int mark = x*W+y;
        seen[x][y] = mark;
        queue<pair<int,int>> que;
        que.push({x,y});
        res++;
        while(!que.empty()){
            auto p = que.front();
            que.pop();
            rep(i,0,4){
                int xx = p.first + dx[i];
                int yy = p.second + dy[i];
                if(!grid_check(xx,yy,H,W)) continue;
                if(seen[xx][yy]==mark) continue;
                if(S[xx][yy]=='#') continue;
                res++;
                seen[xx][yy] = mark;
                if(S[xx][yy]=='@') continue;
                que.push({xx,yy});
            }
        }
        return res;
    };
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='#') continue;
            if(seen[i][j]!=-1) continue;
            chmax(ans,bfs(i,j));
        }
    }
    cout << ans << endl;
}