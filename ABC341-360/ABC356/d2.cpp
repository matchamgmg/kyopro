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
    ll N,M;
    cin >> N >> M;
    ll n = N,m = M;
    int d = 0;
    while(n>0 || m>0){
        n >>= 1;
        m >>= 1;
        d++;
    }
    mint ans(0);
    rep(i,0,d){
        if(M&1){
            ans += (N+1)/mypow(2,i+1)*mypow(2,i);
            ans += max(0LL,(N+1)%mypow(2,i+1)-mypow(2,i));
        }
        M >>= 1;
    }
    cout << ans.val() << endl;
}