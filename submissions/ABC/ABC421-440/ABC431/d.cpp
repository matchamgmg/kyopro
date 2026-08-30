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
    int N;
    cin >> N;
    vll W(N), H(N), B(N);
    ll weight = 0;
    rep(i,0,N){
        cin >> W[i] >> H[i] >> B[i];
        weight += W[i];
    }
    // k番目の部品を頭に取り付けた時の頭の重さがWのときのうれしさの最大値
    // k番目の部品を体に取り付けた時のうれしさの最大値
    // k番目まで取り付けて，頭の重さがWのとき，体の重さはsum_k W[k]
    vector<vector<ll>> dp(N+1,vector<ll>(N*500+1,0));
    rep(i,0,N){
        rep(j,0,N*500+1){
            // 頭につけるとき
            if(j-W[i]>=0) dp[i+1][j] = max(dp[i+1][j],dp[i][j-W[i]] + H[i]);
            // 体につけるとき
            dp[i+1][j] = max(dp[i+1][j],dp[i][j] + B[i]);
        }
    }
    ll ans = 0;
    rep(i,0,N*500+1){
        if(i<=weight-i){
            chmax(ans,dp[N][i]);
        }
    }
    cout << ans << endl;
}