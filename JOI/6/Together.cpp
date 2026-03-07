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
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]

// 各ビーバーについてあり得る行動
// 1. 自身は動かない
// 2. 左隣のビーバーのところへ移動する
// 3. 右隣のビーバーのところへ移動する

// i : 3 -> i+1 : 1
// i : 2 -> i+1 : 1, 3
// i : 1 -> i+1 : 2, 3 (i-1 : 3)

// i-1, i の行動を状態にもつ

int main()
{
    int N;
    cin >> N;
    vector<ll> X(N);
    rep(i,0,N) cin >> X[i];
    sort(all(X));

    const ll INF = 1LL<<60;
    vector dp(N+1,vector<ll>(9,INF));
    rep(i,0,9) dp[0][i] = 0;
    dp[1][0] = dp[1][3] = dp[1][6] = 0;
    dp[1][2] = dp[1][5] = dp[1][8] = X[1] - X[0];
    dp[2][6] = dp[1][2];
    dp[2][1] = X[1] - X[0];
    
    rep(i,2,N){
        rep(j,0,9){
            rep(k,0,9){
                if(k%3 == 0){// k = 0(j = 6), 3(j = 1), 6(j = 2,5) : 
                    if(i == N-1) continue;
                    if(k == 0 && j == 6) chmin(dp[i+1][k], dp[i][j]);
                    else if(k == 3 && j == 1) chmin(dp[i+1][k], dp[i][j]);
                    else if(k == 6 && (j == 2 || j == 5)) chmin(dp[i+1][k], dp[i][j]);
                }else if(k%3 == 1){// k = 1(j = 0,3,6), 4, 7 :
                    if(k != 1) continue;
                    if(j%3 != 0) continue;
                    chmin(dp[i+1][k], dp[i][j] + X[i] - X[i-1]);
                }else{// k = 2(j = 6), 5(j = 1), 8 :
                    if(i+1 == N) continue;
                    if(k == 8) continue;
                    if(k == 2 && j == 6) chmin(dp[i+1][k], dp[i][j] + X[i+1] - X[i]);
                    else if(k == 5 && j == 1) chmin(dp[i+1][k], dp[i][j] + X[i+1] - X[i]);
                }
            }
        }
    }
    cout << *min_element(all(dp[N])) << endl;
}