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
    int w, h;
    cin >> w >> h;
    vector dp(h,vector<vector<vector<int>>>(w,vector<vector<int>>(2,vector<int>(2,0))));
    dp[0][0][0][0] = 1, dp[0][0][1][0] = 1;
    dp[0][0][0][1] = -1, dp[0][0][1][1] = -1;
    int mod = 100000;
    rep(i,0,h){
        rep(j,0,w){
            if(i==0 && j==0){
                dp[i][j+1][0][0] += dp[i][j][0][0];
                dp[i+1][j][1][0] += dp[i][j][1][0];
            }else{
                if(j+1<w){
                    dp[i][j+1][0][0] += dp[i][j][0][0];
                    dp[i][j+1][0][0] %= mod;
                    dp[i][j+1][0][1] += dp[i][j][1][0];
                    dp[i][j+1][0][1] %= mod;
                    dp[i][j+1][0][0] += dp[i][j][0][1];
                    dp[i][j+1][0][0] %= mod;
                }
                if(i+1<h){
                    dp[i+1][j][1][0] += dp[i][j][1][0];
                    dp[i+1][j][1][0] %= mod;
                    dp[i+1][j][1][1] += dp[i][j][0][0];
                    dp[i+1][j][1][1] %= mod;
                    dp[i+1][j][1][0] += dp[i][j][1][1];
                    dp[i+1][j][1][0] %= mod;
                }
            }
        }
    }
    int ans = dp[h-1][w-1][0][0] + dp[h-1][w-1][0][1] + dp[h-1][w-1][1][0] + dp[h-1][w-1][1][1];
    ans %= mod;
    cout << ans << endl;
}