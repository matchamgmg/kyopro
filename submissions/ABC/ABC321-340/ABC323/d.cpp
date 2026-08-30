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

ll mypow(ll a,ll b){
    ll res = 1;
    while(b>0){
        if(b&1) res *= a;
        a *= a;
        b >>= 1;
    }
    return res;
}

int main()
{
    int N;
    cin >> N;
    vll S(N),C(N);
    rep(i,0,N){
        cin >> S[i] >> C[i];
    }
    map<ll,ll> type_val;
    ll ans = 0;
    rep(i,0,N){
        ll s = S[i];
        ll d = 1;
        while(s%2==0){
            s >>= 1;
            d <<= 1;
        }
        type_val[s] += C[i]*d;
    }
    for(auto p : type_val){
        ll x = p.second;
        while(x>0){
            if(x&1) ans++;
            x >>= 1;
        }
    }
    cout << ans << endl;
}