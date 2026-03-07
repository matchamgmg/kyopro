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

struct T{
    ll val;
    int siz;
};
struct F{
    ll ten;
    ll suu;
};
T op(T a, T b){ return {a.val+b.val, a.siz+b.siz}; }
T e(){ return {0,0}; }
T mapping(F f, T x){ return {x.val*f.ten+x.siz*f.suu, x.siz}; }
F composition(F f, F g){ return {g.ten*f.ten, g.suu*f.ten+f.suu}; }
F id(){ return {1,0}; }


int main()
{
    int N;
    cin >> N;
    string S;
    cin >> S;
    vector<T> v(N,{0,1});
    lazy_segtree<T,op,e,F,mapping,composition,id> seg(v);
    ll ans = 0;
    rep(i,0,N){
        ll x = S[i] - '0';
        seg.apply(0,i+1,{10,x});
        ans += seg.prod(0,i+1).val;
    }
    cout << ans << endl;
}