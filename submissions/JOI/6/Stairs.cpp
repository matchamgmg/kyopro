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

// 愚直なDP : O(N^2)
// 配るDPをするとき，登れる場所すべてに +1*(そこまでの場合の数) する
// 一足に登れる範囲を求めれば，imosで配る
// 途中までの累積和を計算すれば大丈夫

//-----------------------------
// もらうDPで，DPの累積和を用いて高速化

const int MOD = 1234567;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, P;
    cin >> N >> P;
    vector<ll> H(N);
    rep(i,0,N) cin >> H[i];
    vll SH(N+1,0);
    rep(i,0,N) SH[i+1] = SH[i] + H[i];

    // vector<ll> dp(N+1,0);
    // dp[0] = 1;
    // dp[1] = -1;
    
    // rep(i,0,N){
    //     int k = upper_bound(all(SH), P+SH[i]) - SH.begin();
    //     ll add = dp[i];
    //     dp[i+1] += add;
    //     dp[i+1] %= MOD;
    //     if(k <= N){
    //         dp[k] += MOD - add;
    //         dp[k] %= MOD;
    //     }
    //     dp[i+1] += dp[i];
    //     dp[i+1] %= MOD;
    // }
    // cout << dp[N] << endl;

    vector<ll> dp(N+1, 0);
    dp[0] = 1;
    vector<ll> Sdp(N+2,0);
    Sdp[1] = Sdp[0] + dp[0];

    rep(i,1,N+1){
        int k = lower_bound(all(SH), SH[i]-P) - SH.begin();
        ll add = (Sdp[i] + MOD - Sdp[k]) % MOD;
        dp[i] = add;
        Sdp[i+1] += Sdp[i] + dp[i];
        Sdp[i+1] %= MOD;  
    }
    cout << dp[N] << endl;
}