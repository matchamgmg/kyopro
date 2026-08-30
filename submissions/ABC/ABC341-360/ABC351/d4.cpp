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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

/*
 * 乱択でやってみる
 * 
*/

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    cin >> H >> W;

    vector<string> S(H);
    rep(i,0,H) cin >> S[i];

    vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};

    auto check = [&](int x, int y) -> bool {
        bool res = true;
        rep(i,0,4){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(!grid_check(nx,ny,H,W)) continue;
            res &= (S[nx][ny] != '#');
        }
        return res;
    };

    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j] == '#') continue;
            if(!check(i,j)) S[i][j] = 'x';
        }
    }

    vector seen(H, vector<int>(W, -1));

    auto DFS = [&](auto f, int x, int y, int p) -> int {
        seen[x][y] = p;
        if(S[x][y] == 'x') return 1;
        int res = 0;
        rep(i,0,4){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(!grid_check(nx,ny,H,W)) continue;
            if(seen[nx][ny] == p) continue;
            if(S[nx][ny] == '#') continue;
            res += f(f, nx, ny, p);
        }
        return res + 1;
    };

    int ans = 0;

    mt19937 mt(0);
    int cnt = 0;
    int i, j;
    do{
        i = mt() % H;
        j = mt() % W;
        if(S[i][j] == '#') continue;
        if(seen[i][j] >= 0) continue;
        chmax(ans, DFS(DFS, i, j, i*W+j));
    }while(++cnt<10000000);

    cout << ans << endl;
}