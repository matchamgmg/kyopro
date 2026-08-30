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

// A[n] = LCM(1,2,...,n)
// A[i] は単調増加
// A[i] <= A[i+1]
// A[i] < A[i+1] となるとき：
//  A[i] の素因数分解とA[i+1] の素因数分解について，指数の和が +1
//  A[i] の素因数分解における，ある素数pについて，A[i] は p^k で割り切れる
//  かつ，i+1 = p^(k+1) とあらわされる
// A[i] == A[i+1] となるとき：
//  i+1 = p^(k+1) となるp, k が存在しない
// i+1 = p^(k+1) とあらわされるとき
// 1,...,i < p^(k+1) であるからA[i] はp^(k+1)で割り切れない

// 求めるもの 1 + (for i = L+1::R : += (i は素数冪である))

// 区間篩 : [l,r] をすべて素因数分解する
// M = max(R-L, root(R)), O(MloglogM)

vector<ll> prime_enumerate(ll N){
    vector<bool> is_prime(N+1,true);
    vector<ll> primes;
    if(N<2) return primes;
    is_prime[0] = is_prime[1] = false;
    for(ll i=2; i*i<=N; ++i){
        if(is_prime[i]){
            for(ll j = i*i; j<=N; j+=i) is_prime[j] = false;
        }
    }
    for(ll i=2; i<=N; ++i){
        if(is_prime[i]) primes.push_back(i);
    }
    return primes;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll L, R;
    cin >> L >> R;

    vector<ll> divs(R-L);

    ll ans = 1;

    // root(R) 以下の素数に関して
    for(ll p : prime_enumerate(sqrt(R) + 100)){
        // [L+1, R] のうち，pの倍数であるものに関して...
        // [L+1, R] において，pの倍数であるもののうち最小のものは...
        for(ll x = p*(L/p + 1); x<=R; x+=p){
            // xが素数冪か
            // つまり割れる素数がただ一種類
            if(divs[x - (L+1)] >= 1) continue;
            divs[x - (L+1)] = 1;
            ll y = x;
            while(y % p == 0){
                y /= p;
            }
            if(y == 1) ans++;
        }
    }

    // 篩で見つけられない素数冪に関して
    // つまりroot(R)より大きい素数に関して
    for(ll v : divs) ans += (v == 0);

    cout << ans << endl;

}