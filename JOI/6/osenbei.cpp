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

int main()
{
    int R, C;
    cin >> R >> C;
    vector A(R,vector<int>(C));
    rep(i,0,R){
        rep(j,0,C){
            cin >> A[i][j];
            A[i][j]^=1;
        }
    }
    int inf = 1e9;
    auto solve = [&](vector<int> &V) -> int {
        vector dp(C+1, vector<int>(2,-inf));
        dp[0][0] = 0;
        rep(i,0,C){
            chmax(dp[i+1][0], dp[i][0] + V[i]);
            chmax(dp[i+1][1], dp[i][0] + R - V[i]);
            chmax(dp[i+1][0], dp[i][1] + V[i]);
            chmax(dp[i+1][1], dp[i][1] + R - V[i]);
        }
        int res = 0;
        rep(i,0,2){
            chmax(res, dp[C][i]);
        }
        return res;
    };
    vector<int> B(C);
    int ans = 0;
    for(int i=0; i<(1<<R); i++){
        int bit = i;
        rep(j,0,C) B[j] = 0;
        rep(j,0,R){
            if(bit & 1){
                rep(k,0,C){
                    B[k] += A[j][k]^1;
                }
            }else{
                rep(k,0,C){
                    B[k] += A[j][k];
                }
            }
            bit >>= 1;
        }
        chmax(ans, solve(B));
    }
    // rep(i,0,R){
    //     rep(j,i+1,R+1){
    //         rep(k,0,C){
    //             B[k] = 0;
    //             rep(l,0,R){
    //                 B[k] += ((i<=l && l<j) ? A[l][k]^1 : A[l][k]);
    //             }
    //         }
    //         chmax(ans, solve(B));
    //     }
    // }
    cout << ans << endl;
}