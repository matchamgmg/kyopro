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
    int N,D;
    cin >> N >> D;
    const int M = 1000000+5;
    vector<int> dict(M,0);
    rep(i,0,N){
        int a;
        cin >> a;
        dict[a]++;
    }
    if(D==0){
        int X = 0;
        for(int i : dict){
            X += i>=1;
        }
        cout << N - X << endl;
        return 0;
    }
    const int inf = 1e9;
    vector<vector<int>> dp(dict.size()+D,vector<int>(2,inf));
    int ans = 0;
    rep(i,0,D){
        dp[i][0] = 0;   //use
        dp[i][1] = 0;   //unuse
        for(int j = 0;i+j*D<dict.size();j++){
            int p = i+j*D;
            chmin(dp[p+D][0],dp[p][1]);
            chmin(dp[p+D][1],dp[p][0]+dict[p]);
            chmin(dp[p+D][1],dp[p][1]+dict[p]);
        }
    }
    rep(i,0,D){
        ans += min(dp[dict.size()+i][0],dp[dict.size()+i][1]);
    }
    cout << ans << endl;
}