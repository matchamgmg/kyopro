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
    string T;
    cin >> T;
    int N;
    cin >> N;
    int t = T.size();
    vector<vector<string>> S(N,vector<string>());
    vi A(N);
    rep(i,0,N){
        cin >> A[i];
        rep(j,0,A[i]){
            string s;
            cin >> s;
            S[i].push_back(s);
        }
    }
    vvi dp(N+1,vi(t+1,INF));// 袋Aiまで見て，Tのj文字目まで一致するときの最小コスト
    dp[0][0] = 0;
    rep(i,0,N){
        chmin(dp[i+1][0],dp[i][0]);
        rep(j,0,t){
            chmin(dp[i+1][j+1],dp[i][j+1]);
            rep(k,0,A[i]){
                int len = S[i][k].size();
                if(j-len+1>=0){
                    string s = T.substr(j-len+1,len);
                    if(s==S[i][k]){
                        chmin(dp[i+1][j+1],dp[i][j+1-len]+1);
                    }
                }
            }
        }
    }
    int ans = INF;
    rep(i,0,N+1){
        chmin(ans,dp[i][t]);
    }
    cout << (ans>=INF ? -1 : ans) << endl;
    //vv_cout(dp);
}