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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int N,M;
    cin >> N >> M;
    vll X(N);
    rep(i,0,N){
        cin >> X[i];
    }
    vll B(N+1,0);
    rep(i,0,M){
        int C;
        ll Y;
        cin >> C >> Y;
        B[C] = Y;
    }
    vvll dp(N+1,vll(N+1,-1));    //dp[i][j] : i回目のコイントス，カウンタjにおける最大値
    dp[0][0] = 0;
    ll max_i = 0;
    rep(i,0,N){
        rep(j,0,N+1){
            if(j==0){
                dp[i+1][j] = max_i;
                max_i = dp[i+1][j];
            }else{
                if(dp[i][j-1]==-1) continue;
                chmax(dp[i+1][j],dp[i][j-1] + X[i] + B[j]);
                chmax(max_i,dp[i+1][j]);
            }
        }
    }
    ll ans = 0;
    rep(i,0,N+1){
        chmax(ans,dp[N][i]);
    }
    cout << ans << endl;
}