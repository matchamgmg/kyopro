#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

/*
Fenwick Tree (BIT tree)
*/

ll op(ll a, ll b){ return a+b; }
ll e(){ return 0LL; }

int main()
{
    int N,M;
    cin >> N >> M;
    vll A(N);
    rep(i,0,N){
        cin >> A[i];
    }
    vll sAmodM(N+1,0);
    rep(i,0,N){
        sAmodM[i+1] = sAmodM[i] + A[i];
        sAmodM[i+1] %= M;
    }
    // (S[r] - S[l-1]) mod M = S[r] - S[l-1] + (S[r]>=S[l-1] ? 0 : M);
    // sum{1<=l,r<=N} = sum{1<=l<=N}sum{l<=r<=N}
    // sum{1<=l<=N}(-S[l-1]*(N-l+1) + sum{l<=r<=N}S[r] + M*X[l-1])
    // sum{1<=r<=N}(S[r]*r - sum{1<=l<=r}S[l-1] + M*X[r])
    vll ruisekiS(N+2,0);
    rep(i,0,N+1){
        ruisekiS[i+1] = ruisekiS[i] + sAmodM[i];
    }
    // X[r] : 各rについて (S[r]<S[l-1]) を満たす lの個数
    // 転倒数
    vll X(N+1,0);
    segtree<ll,op,e> BIT(M);
    rep(i,0,N+1){
        BIT.set(sAmodM[i],1);
        X[i] = i - BIT.prod(0,sAmodM[i]);
    }
    ll ans = 0;
    rep(i,1,N+1){
        ans += sAmodM[i] * i;
        ans -= ruisekiS[i];
        ans += X[i];
    }
    cout << ans << endl;
}