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

void print(vector<vector<bool>> &used, ll score){
    int h = used.size();
    int w = used[0].size();
    rep(i,0,h){
        rep(j,0,w){
            cout << (used[i][j] ? '#' : '.') << " \n"[j==w-1];
        }
    }
    cout << score << endl << endl;
}

int main()
{
    int H,W;
    cin >> H >> W;
    vvll A(H,vll(W));
    ll best = 0;
    rep(i,0,H){
        rep(j,0,W){
            cin >> A[i][j];
            best ^= A[i][j];
        }
    }
    vll B;
    B.push_back(best);
    ll now_score = best;
    vector used(H,vector<bool>(W,false));
    auto dfs = [&](auto f, ll score, int x, int y) -> void {
        if(!grid_check(x,y,H,W)) return;
        
        if(grid_check(x,y,H,W) && grid_check(x,y+1,H,W)){
            if(!used[x][y] && !used[x][y+1]){
                used[x][y] = true;
                used[x][y+1] = true;
                score ^= (A[x][y]^A[x][y+1]);
                B.push_back(score);
                chmax(best,score);
                int nx = x;
                int ny = y+2;
                if(!grid_check(nx,ny,H,W)){
                    nx++;
                    ny = 0;
                }
                // print(used,score);
                f(f,score,nx,ny);
                score ^= (A[x][y]^A[x][y+1]);
                used[x][y] = false;
                used[x][y+1] = false;
            }
        }

        if(grid_check(x,y,H,W) && grid_check(x+1,y,H,W)){
            if(!used[x][y] && !used[x+1][y]){
                used[x][y] = true;
                used[x+1][y] = true;
                score ^= (A[x][y]^A[x+1][y]);
                B.push_back(score);
                chmax(best,score);
                int nx = x;
                int ny = y+1;
                if(!grid_check(nx,ny,H,W)){
                    nx++;
                    ny = 0;
                }
                // print(used,best);
                f(f,score,nx,ny);
                score ^= (A[x][y]^A[x+1][y]);
                used[x][y] = false;
                used[x+1][y] = false;
            }
        }

        int nx = x;
        int ny = y+1;
        if(!grid_check(nx,ny,H,W)){
            nx++;
            ny = 0;
        }
        f(f,score,nx,ny);
        return;
    };
    // print(used,best);
    dfs(dfs,now_score,0,0);
    cout << best << endl;
}