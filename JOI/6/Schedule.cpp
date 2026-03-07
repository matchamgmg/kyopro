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
    int N;
    string S;
    cin >> N >> S;
    int inf = 1e9;
    int mod = 10007;
    vector dp(N+1,vector<int>(8,0));
    dp[0][1] = 1;
    map<char,int> mp;
    mp['J'] = 1;
    mp['O'] = 2;
    mp['I'] = 4;
    int pa, na;
    pa = 1; na = 1;
    rep(i,0,N){
        pa = na;
        na = mp[S[i]];
        rep(j0,0,2){
            rep(j1,0,2){
                rep(j2,0,2){
                    if((pa & (j0+j1*2+j2*4)) == 0) continue;
                    if(dp[i][j0+j1*2+j2*4] == -inf) continue;
                    rep(k0,0,2){
                        rep(k1,0,2){
                            rep(k2,0,2){
                                if((na & (k0+k1*2+k2*4)) == 0) continue;
                                int psiz = j0+j1+j2;
                                int nsiz = k0+k1+k2;
                                if(psiz == 1){
                                    if((pa & (k0+k1*2+k2*4)) == 0) continue;
                                }else if(psiz == 2){
                                    if((na & (j0+j1*2+j2*4)) == 0){
                                        if(((j0+j1*2+j2*4) & (k0+k1*2+k2*4)) == 0) continue;
                                    }
                                }
                                dp[i+1][k0+k1*2+k2*4] += dp[i][j0+j1*2+j2*4];
                                dp[i+1][k0+k1*2+k2*4] %= mod;
                            }
                        }
                    }
                }
            }
        }
    }
    // vv_cout(dp);
    int ans = 0;
    rep(i,0,8){
        ans += dp[N][i];
    }
    ans %= mod;
    cout << ans << endl;
}