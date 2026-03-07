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

#define INF 1<<29

int main()
{
    int N,X;
    cin >> N >> X;
    vi V(N),A(N),C(N);
    rep(i,0,N){
        cin >> V[i] >> A[i] >> C[i];
    }
    int dp[X+1][3];
    rep(i,0,X+1){
        rep(j,0,3){
            dp[i][j] = -(INF);
        }
    }
    rep(i,0,3){
        dp[0][i] = 0;
    }
    // vector dp(N+1,vvi(X+1,vi(3,0)));
    rep(i,0,N){
        // oldを使わない
        // 配列のコピーがなくなる
        rrep(j,0,X+1){
            if(j-C[i]>=0 && dp[j-C[i]][V[i]-1]!=-(INF)){
                chmax(dp[j][V[i]-1],dp[j-C[i]][V[i]-1]+A[i]);
            }
        }
        rep(j,1,X+1){
            rep(k,0,3){
                chmax(dp[j][k],dp[j-1][k]);
            }
        }
    }
    
    int ans = 0;
    rep(i,0,X+1){
        rep(j,i,X+1){
            int min_v = min({dp[i][0],dp[j-i][1],dp[X-j][2]});
            chmax(ans,min_v);
        }
    }

    cout << ans << endl;

    // rep(i,0,X+1){
    //     cout << dp[i][0] << " \n"[i==X];
    // }
}