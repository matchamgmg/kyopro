#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    int N,K;
    cin >> N >> K;
    vvi AB(N,vi(2));
    rep(i,0,N){
        cin >> AB[i][0];
    }
    rep(i,0,N){
        cin >> AB[i][1];
    }
    vector<vector<bool>> dp(N,vector<bool>(2,false));
    //数Xで終わる数列を作れるか
    dp[0][0] = true;
    dp[0][1] = true;
    rep(i,1,N){
        rep(j,0,2){
            if(dp[i-1][j]==false) continue;
            rep(k,0,2){
                if(abs(AB[i][k]-AB[i-1][j])<=K){
                    dp[i][k] = true;
                }
            }
        }
    }
    if(dp[N-1][0]||dp[N-1][1]){
        pyes;
    }else{
        pno;
    }
}