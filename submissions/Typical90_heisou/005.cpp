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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// 小課題1を解く
// i桁目まで決めたとき余りを管理
// dp[i][(j*10+c[k])%B] += dp[i][j]
// O(NBK)

// N <= 10e18のとき
// 行列累乗 O(B^3 logN)
// ダブリング O(B^2 logN)
// k桁の数を前半i桁と後半j桁に分ける
// dp[i][p]: i桁の数で余りがpの数の個数
// dp[j][q]: j桁の数で余りがqの数の個数
// dp[i+j][(p*10^j+q)%B] += dp[i][p]*dp[j][q]

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const ll MOD = 1e9+7;

    ll N, B, K;
    cin >> N >> B >> K;
    vector<ll> c(K);
    rep(i,0,K) cin >> c[i];

    auto mul = [&](const vector<ll> &dpi, const vector<ll> &dpj, ll tj) -> vector<ll> {
        vector<ll> res(B, 0);
        rep(i,0,B){
            rep(j,0,B){
                res[(i*tj+j)%B] += dpi[i]*dpj[j];
                res[(i*tj+j)%B] %= MOD;
            }
        }
        return res;
    };

    ll lg = 0;
    ll n = N;
    while(n>0){
        lg++;
        n /= 2;
    }
    vector<ll> Ten(lg+1, 10);
    rep(i,1,lg+1){
        Ten[i] = Ten[i-1]*Ten[i-1]%B;
    }

    vector<vector<ll>> dp(1, vector<ll>(B, 0));
    rep(i,0,K){
        dp[0][c[i]%B]++;
    }
    rep(cnt,0,lg+1){
        dp.emplace_back(mul(dp[cnt],dp[cnt],Ten[cnt]));
    }
    vector<ll> ans(B, 0);
    ans[0] = 1;
    rep(cnt,0,lg+1){
        if((N>>cnt)&1){
            ans = mul(ans, dp[cnt], Ten[cnt]);
        }
    }
    cout << ans[0] << endl;
}