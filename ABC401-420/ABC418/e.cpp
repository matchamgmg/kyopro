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

ll gcd(ll a, ll b){
    while(b != 0){
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main()
{
    int N;
    cin >> N;
    vector<ll> X(N),Y(N);
    rep(i,0,N){
        cin >> X[i] >> Y[i];
    }
    map<pair<ll,ll>,ll> m;
    map<pair<ll,ll>,ll> n;
    ll inf = 0;
    rep(i,0,N-1)rep(j,i+1,N){
        if(X[j]-X[i]==0){
            inf++;
        }else{
            ll g = gcd(abs(X[j]-X[i]),abs(Y[j]-Y[i]));
            ll x = (X[j]-X[i])/g;
            ll y = (Y[j]-Y[i])/g;
            if(x*y<0 && x<0){
                x = -x;
                y = -y;
            }
            m[{x,y}]++;
        }
        n[{X[i]+X[j],Y[i]+Y[j]}]++;
    }
    ll ans = 0;
    if(inf>1){
        ans += inf*(inf-1)/2;
    }
    for(auto p : m){
        p.first;
        if(p.second>1){
            ans += p.second*(p.second-1)/2;
        }
    }
    for(auto p : n){
        if(p.second>1){
            ans -= p.second*(p.second-1)/2;
        }
    }
    cout << ans << endl;
}