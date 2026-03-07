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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    cin >> H >> W;
    vector<vector<ll>> A(H, vector<ll>(W));
    rep(i,0,H){
        rep(j,0,W){
            cin >> A[i][j];
        }
    }
    vector<ll> P(H+W-1);
    rep(i,0,H+W-1) cin >> P[i];

    // iターン目にいる場所の候補 min(H,W) 通り
    // 後ろから考える
    // はじめ0枚 -> + P[H+W-1]枚 - A[H+W-1]枚 max(0,)
    // i+j = k: 
    // 普通にi, jの降順に更新可能

    const ll INF = 1LL<<60;
    vector dp(H, vector<ll>(W,INF));
    dp[H-1][W-1] = 0;
    rrep(i,0,H+W-1){
        int c = max(0, i-H+1);
        rep(j,0,min(H,W)){
            if(0<=i-j-c && i-j-c<H && j+c<W){
                if(i-j-c+1<H) chmin(dp[i-j-c][j+c], dp[i-j-c+1][j+c]);
                if(j+c+1<W) chmin(dp[i-j-c][j+c], dp[i-j-c][j+c+1]);
                dp[i-j-c][j+c] += P[i];
                dp[i-j-c][j+c] -= A[i-j-c][j+c];
                chmax(dp[i-j-c][j+c], 0LL);
            }
        }
    }

    cout << dp[0][0] << endl;
}