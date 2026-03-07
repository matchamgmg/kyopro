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

// 町iにいるとき，次に行ける町はi+1~i+K

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M, K;
    cin >> N >> M >> K;
    vector<ll> A(N), B(N);
    rep(i,0,N) cin >> A[i] >> B[i];
    const ll INF = 1LL<<60;
    vector dp(N+1, vector<ll>(M+1, -INF));
    dp[0][0] = 0;
    rep(i,0,N){
        // もらうdp
        // 初めて訪れる町
        rep(k,0,M+1){
            // chmax(dp[i+1][k], dp[0][k]);
            if(k-B[i]>=0) chmax(dp[i+1][k], dp[0][k-B[i]]+A[i]);
        }
        // 前の町からの遷移
        rep(j,0,i){
            if(i-j<=K){
                rep(k,0,M+1){
                    // chmax(dp[i+1][k], dp[j+1][k]);
                    if(k-B[i]>=0) chmax(dp[i+1][k], dp[j+1][k-B[i]]+A[i]);
                }
            }
        }
        // rep(k,0,M+1){
        //     cout << (dp[i+1][k] < 0 ? -1 : dp[i+1][k]) << ' ';
        // }
        // cout << endl;
    }
    ll ans = 0;
    rep(i,0,N+1){
        rep(j,0,M+1){
            chmax(ans, dp[i][j]);
        }
    }
    cout << ans << endl;
}