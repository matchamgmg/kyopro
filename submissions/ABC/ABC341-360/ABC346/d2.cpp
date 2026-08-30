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

#define INF 1LL<<60

int main()
{
    int N;
    cin >> N;
    string S;
    cin >> S;
    vll C(N);
    rep(i,0,N){
        cin >> C[i];
    }
    vvvll dp(N,vvll(2,vll(2,INF)));
    dp[0][0][S[0]-'0'] = 0;
    dp[0][0][S[0]-'0'^1] = C[0];
    rep(i,1,N){
        int r = S[i]-'0';
        rep(k,0,2){
            dp[i][0][k] = dp[i-1][0][k^1] + (r==k ? 0 :C[i]);
            dp[i][1][k] = min(dp[i-1][0][k],dp[i-1][1][k^1]) + (r==k ? 0 : C[i]);
        }
    }
    cout << min(dp[N-1][1][0],dp[N-1][1][1]) << endl;
}