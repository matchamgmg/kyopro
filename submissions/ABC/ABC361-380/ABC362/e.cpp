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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

#define MOD 998244353

int main()
{
    int N;
    cin >> N;
    vll A(N);
    rep(i,0,N){
        cin >> A[i];
    }
    //Aiで始まる長さl,公差dの等差数列の個数
    //d = Aj - Aiとすると，Aiで始まる長さl,公差dの等差数列は
    //Ajで始まる長さl-1,公差dの等差数列の先頭にAiを追加したもの
    vector<vector<map<ll,ll>>> dp(N,vector<map<ll,ll>>(N+1,map<ll,ll>()));
    rrep(i,0,N-1){
        rep(j,i+1,N){
            ll d = A[j] - A[i];
            rep(l,2,N+2-j){
                dp[i][l][d] += ((l-1==1 && dp[j][l-1][d]==0LL) ? 1LL : dp[j][l-1][d]);
                dp[i][l][d] %= MOD;
            }
        }
    }
    rep(l,1,N+1){
        if(l==1){
            printf("%d ",N);
        }else{
            ll ans = 0;
            rep(i,0,N-1){
                for(auto p : dp[i][l]){
                    ans += p.second;
                    ans %= MOD;
                }
            }
            printf("%lld ",ans);
        }
    }
}