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
    int N;
    cin >> N;
    string S;
    cin >> S;
    //i回目に出した手と勝った回数を記録
    //r:1,s:2,p:3
    vvi dp(3,vi(N+1,-1));
    int aoki = (S[0]=='R' ? 0 : S[0]=='S' ? 1 : 2);
    dp[aoki][1] = 0;
    dp[(aoki+2)%3][1] = 1;
    rep(i,1,N){
        aoki = (S[i]=='R' ? 0 : S[i]=='S' ? 1 : 2);
        rep(j,0,3){
            if(j==(aoki+1)%3) continue;
            rep(k,0,3){
                if(j==k) continue;
                if(dp[k][i]==-1) continue;
                if(aoki==j) chmax(dp[j][i+1],dp[k][i]);
                if(aoki==(j+1)%3) chmax(dp[j][i+1],dp[k][i]+1);
            }
        }
    }
    cout << max({dp[0][N],dp[1][N],dp[2][N]}) << endl;
}