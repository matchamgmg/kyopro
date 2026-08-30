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

#define INF 1<<30

int main()
{
    int N;
    cin >> N;
    // dpで解く
    // sum{B}が小さいことを利用する
    vi A(N),B(N);
    int sum = 0;
    rep(i,0,N){
        cin >> A[i] >> B[i];
        sum += B[i];
    }
    if(sum%3!=0){
        cout << -1 << endl;
        return 0;
    }
    int s = sum/3;
    
    // 二次元配列でもち，old -> newの遷移を行う
    vvi dp(s+1,vi(s+1,INF));
    dp[0][0] = 0;
    rep(i,0,N){
        vector old(s+1,vi(s+1,INF));
        swap(old,dp);
        int cost1 = (A[i]==1 ? 0 : 1);
        int cost2 = (A[i]==2 ? 0 : 1);
        int cost3 = (A[i]==3 ? 0 : 1);
        rep(j1,0,s+1) rep(j2,0,s+1){
            if(j1+B[i] <= s) chmin(dp[j1+B[i]][j2], old[j1][j2]+cost1);
            if(j2+B[i] <= s) chmin(dp[j1][j2+B[i]], old[j1][j2]+cost2);
            chmin(dp[j1][j2], old[j1][j2]+cost3);
        }
    }
    cout << (dp[s][s]==INF ? -1 : dp[s][s]) << endl;
}