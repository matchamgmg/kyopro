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
    vector<vector<int>> rows(H,vector<int>(W+1,0)), cols(H+1,vector<int>(W,0));
    rep(i,0,H){
        rep(j,0,W){
            rows[i][j+1] = rows[i][j] + (S[i][j]=='#');
        }
    }
    rep(j,0,W){
        rep(i,0,H){
            cols[i+1][j] = cols[i][j] + (S[i][j]=='#');
        }
    }
    int Aoi = 0, Rin = 0;
    rep(i,0,H){
        rep(j,0,W){
            Aoi += (S[i][j]=='#');
        }
    }
    const int INF = 1<<29;
    Rin = W*H - Aoi;
    int maxAoi = -1, Aoi_row;
    rep(i,0,H){
        int maxRin = -1, Rin_col = -1;
        rep(j,0,W){
            int p = rows[i][j] + rows[i][W] - rows[i][j+1] + cols[i][j] + cols[H][j] - cols[i+1][j];
            int n = (H + W -2) - p;
            int nRin = Rin + p - n;
            if(maxRin<nRin){
                maxRin = nRin;
                Rin_col = j;
            }
        }
        int nAoi = H*W - maxRin;
        if(maxAoi<nAoi){
            maxAoi = nAoi;
            Aoi_row = i;
        }
    }
    cout << maxAoi << ' ' << H*W-maxAoi << endl;
}