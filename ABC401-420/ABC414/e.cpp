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

vector<ll> hurui(ll n){
    vector<ll> res;
    vector<bool> is_prime(n+1,true);
    is_prime[0] = false;
    is_prime[1] = false;
    rep(i,0,n+1){
        if(!is_prime[i]) continue;
        res.push_back(i);
        for(int j=2*i;j<n+1;j+=i){
            is_prime[j] = false;
        }
    }
    return res;
}

int main()
{
    ll N;
    cin >> N;
    vector<ll> prime = hurui((ll)(sqrt(N+1)));
    // a <= b -> 不適
    // a > b
    mint ans = mint(N-1)*mint(N-2)*mint(2).inv();
    vector<mint> zeta(N+1,mint(0));
    rep(i,0,prime.size()){
        
    }
    cout << ans.val() << endl;
}