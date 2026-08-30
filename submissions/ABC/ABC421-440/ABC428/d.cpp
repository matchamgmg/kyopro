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

void solve(){
    ll C,D;
    cin >> C >> D;

    ll ans = 0;
    auto intsqrt = [&](ll x) -> ll {
        ll res = sqrt(x);
        while(res*res>x) res--;
        while((res+1)*(res+1)<=x) res++;
        return res;
    };
    auto fn = [&](ll n, ll m) -> ll {
        ll res1 = intsqrt(n);
        ll res2 = intsqrt(m-1);
        return res1 - res2;
    };
    ll xmin = 1, xmax = 9, cshift = 10;
    // long long のオーバーフローに注意
    while(xmin <= C+D){
        ll l = max(xmin,C+1);
        ll r = min(xmax,C+D);
        if(l<=r){
            ll vl = C * cshift + l;
            ll vr = C * cshift + r;
            ans += fn(vr,vl);
        }
        xmin = xmin * 10;
        xmax = (xmax+1)*10-1;
        cshift *= 10;
    }
    cout << ans << endl;
}

int main()
{
    // fn(n) ... 1 ~ n までの平方数の個数
    // C+xを桁数で分類
    // d桁: 10^(d-1) <= C+x < 10^d
    int T;
    cin >> T;
    while(T--){
        solve();
    }
}