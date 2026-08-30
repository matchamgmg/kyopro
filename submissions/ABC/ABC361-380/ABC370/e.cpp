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
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

int main()
{
    int N;
    cin >> N;
    ll K;
    cin >> K;
    vll A(N);
    rep(i,0,N) cin >> A[i];
    vll B(N+1,0);
    rep(i,0,N) B[i+1] = B[i] + A[i];
    vector<mint> dp(N+1,0);
    dp[0] = 1;

    /* dp[n] = sum_{0<=m<n} dp[m] (B[n]-B[m]!=K)
     *       = sum_{0<=m<n, B[m]!=B[n]-K} dp[m]
     * m[x] = sum_{B[m]==x} dp[m]
     * all = sum_{0<=m<n} dp[m]
     * dp[n] = all - m[B[n]-K]
    */

    map<ll,mint> m;
    m[B[0]] = dp[0];
    mint all = dp[0];
    rep(i,1,N+1){
        dp[i] = all - m[B[i]-K];
        all += dp[i];
        m[B[i]] += dp[i];
    }
    cout << dp[N].val() << endl; 
}