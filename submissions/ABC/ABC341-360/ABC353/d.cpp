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

#define MOD 998244353

ll TEN(int d){
    ll res = 1;
    while(d--){
        res *= 10;
        //res %= MOD;
    }
    return res;
}

int main()
{
    int N;
    cin >> N;
    vll A(N);
    rep(i,0,N){
        cin >> A[i];
    }
    ll ans = 0;
    vi d(N,0);
    rep(i,0,N){
        ll a = A[i];
        while(a>0){
            a /= 10;
            d[i]++;
        }
    }
    vll ruiseki_d(N+1,0);
    rep(i,0,N){
        ruiseki_d[i+1] = ruiseki_d[i] + TEN(d[i]);
        //ruiseki_d[i+1] %= MOD;
    }
    rep(i,0,N-1){
        ans += A[i]*((ruiseki_d[N] + MOD - ruiseki_d[i+1])%MOD)%MOD;
        ans += (i+1)*A[i+1]%MOD;
    }
    cout << ans%MOD << endl;
}