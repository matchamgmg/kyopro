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

struct S {
    ll n;
    ll res;
    S() : n(0LL), res(0LL) {}
    S(ll n_, ll res_) : n(n_), res(res_) {}
};

S e(){ return S(); }
S op(const S &a, const S &b){ return {a.n+b.n, a.res+b.res}; }

const int siz = 500001;
segtree<S,op,e> seg(siz);

void add(ll x){
    S a = seg.get(x);
    a.n++;
    a.res += x;
    seg.set(x,a);
}

void del(ll x){
    S a = seg.get(x);
    a.n--;
    a.res -= x;
    seg.set(x,a);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, Q;
    cin >> N >> Q;
    vll A(N);
    rep(i,0,N){
        cin >> A[i];
        add(A[i]);
    }
    ll t, x, y, l, r;
    while(Q--){
        cin >> t;
        if(t==1){
            cin >> x >> y;
            x--;
            del(A[x]);
            A[x] = y;
            add(A[x]);
        }else{
            cin >> l >> r;
            ll ans = 0;
            if(l>r){
                ans = l*N;
            }else{
                ans += l*seg.prod(0,l).n;
                ans += r*seg.prod(r+1,siz).n;
                ans += seg.prod(l,r+1).res;
            }
            cout << ans << '\n';
        }
    }
}