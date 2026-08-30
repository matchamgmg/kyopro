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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

struct S{
    char lc, rc;
    int ln, rn;
    bool con;
    int ans;
    S() : lc('.'), rc('.'), ln(0), rn(0), con(true), ans(0) {}
    S(char lc_, int ln_, char rc_, int rn_, bool con_, int ans_) : lc(lc_), rc(rc_), ln(ln_), rn(rn_), con(con_), ans(ans_) {}
};

S op(S l, S r){
    if(l.lc == '.') return r;
    if(r.lc == '.') return l;
    S res;
    res.lc = l.lc;
    res.rc = r.rc;
    res.ln = ((l.con && l.rc == r.lc) ? l.ln + r.ln : l.ln);
    res.rn = ((r.con && l.rc == r.lc) ? r.rn + l.rn : r.rn);
    res.con = (l.con && r.con && l.rc == r.lc);
    res.ans = (l.rc == r.lc ? l.rn + r.ln : 0);
    res.ans = max(max(l.ans, r.ans), res.ans);
    return res;
}

S e(){ return S(); }

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<S> V(N);
    rep(i,0,N){
        char c;
        cin >> c;
        V[i] = S(c,1,c,1,true,1);
    }
    segtree<S,op,e> seg(V);
    int t, k, l, r;
    char x;
    for(;Q--;){
        cin >> t;
        if(t == 1){
            cin >> k >> x;
            k--;
            seg.set(k,S(x,1,x,1,true,1));
        }else{
            cin >> l >> r;
            l--;
            cout << seg.prod(l, r).ans << endl;
        }
    }
}