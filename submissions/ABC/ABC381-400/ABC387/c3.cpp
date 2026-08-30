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

int main(){
    auto snake = [&](ll N) -> ll {
        ll n = N;
        vector<int> A;
        int d = 0;
        while(n>0){
            A.push_back(n%10);
            n /= 10;
            d++;
        }
        reverse(all(A));
        // 桁DP
        vector dp(d,vector(2,vector(2,vector<ll>(10,0))));
        // digit
        // smaller
        // leading zero
        // number
        rep(i,1,A[0]){
            dp[0][1][0][i] = 1;
        }
        dp[0][1][1][0] = 1;
        dp[0][0][0][A[0]] = 1;
        rep(i,1,d){
            rep(s,0,2){
                rep(l,0,2){
                    rep(j,0,10){// 先頭の桁の数字
                        rep(x,0,10){// i桁目の数字
                            // snake?
                            if(l==0 && j<=x) continue;
                            // bigger
                            if(s==0 && x>A[i]) continue;
                            int nj = (l==0 ? j : x);
                            int ns = (s==1 ? 1 : x<A[i] ? 1 : 0);
                            int nl = (l==0 ? 0 : x==0 ? 1 : 0);
                            dp[i][ns][nl][nj] += dp[i-1][s][l][j];
                        }
                    }
                }
            }
        }
        ll res = 0;
        rep(s,0,2){
            rep(j,0,10){
                res += dp[d-1][s][0][j];
            }
        }
        return res;
    };
    ll L,R;
    cin >> L >> R;
    ll ans = snake(R) - snake(L-1);
    cout << ans << endl;
}