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
    vll X(N),Y(N),Z(N);
    rep(i,0,N){
        cin >> X[i] >> Y[i] >> Z[i];
    }
    vll cost(N);
    auto f = [&](ll m, int i){
        return X[i] + m > Y[i] - m;
    };
    rep(i,0,N){
        ll L = -1, R = Y[i];
        while(R-L>1){
            ll mid = (L+R)/2;
            (f(mid,i) ? R : L) = mid;
        }
        cost[i] = R;
    }
    ll Z_sum = 0;
    rep(i,0,N){
        Z_sum += Z[i];
    }
    vvll dp(N+1,vll(Z_sum+1,INF));
    dp[0][0] = 0;
    rep(i,0,N){
        rep(j,0,Z_sum+1){
            chmin(dp[i+1][j],dp[i][j]);
            if(j-Z[i]>=0){
                chmin(dp[i+1][j],dp[i][j-Z[i]] + cost[i]);
            }else{
                chmin(dp[i+1][j],cost[i]);
            }
        }
    }
    cout << dp[N][(Z_sum+1)/2] << endl;
}