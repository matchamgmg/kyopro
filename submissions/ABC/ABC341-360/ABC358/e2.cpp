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

// k個のものに，k個挿入する
// o o o | |
// (k+n)!/(k!n!)

int main()
{
    int K;
    cin >> K;
    vi C(26);
    rep(i,0,26) cin >> C[i];

    vector<mint> A(K+1,0), invA(K+1,0);
    A[0] = mint(1);
    rep(i,0,K) A[i+1] = A[i]*mint(i+1);
    rep(i,0,K+1) invA[i] = A[i].inv();

    auto fn = [&](int a, int b) -> mint {
        return A[a+b]*invA[a]*invA[b];
    };

    // 文字iをC_i個使いたいとき，長さk-C_iの文字列であって，
    // 長さK-C_iの文字列に，文字iを挿入する
    // はじめ，空文字列
    vector<mint> dp(K+1,0);
    dp[0] = 1;
    rep(i,0,26){
        vector<mint> next_dp(K+1,0);
        rep(j,0,K+1){
            rep(k,0,1001){
                // 文字iをk文字使う
                if(k>C[i]) continue;
                // 長さj-C[i]の文字列に文字iを挿入
                if(j-k<0) continue;
                next_dp[j] += dp[j-k]*fn(j-k,k);
            }
        }
        // rep(j,0,K+1){
        //     cout << next_dp[j].val() << ' ';
        // }
        // cout << endl;
        dp = next_dp;
    }
    mint ans = 0;
    rep(i,1,K+1){
        ans += dp[i];
    }
    cout << ans.val() << endl;
}