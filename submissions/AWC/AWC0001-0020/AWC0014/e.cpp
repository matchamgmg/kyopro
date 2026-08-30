#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>;  // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>;  // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a) {  \
        pyes; \
    } else {  \
        pno;  \
    }
#define v_cout(a)                         \
    {                                     \
        int n = a.size();                 \
        rep(i, 0, n) cout << a[i] << " "; \
        cout << endl;                     \
    }
#define vv_cout(a)                                         \
    {                                                      \
        int n = a.size();                                  \
        rep(i, 0, n) {                                     \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T& a, T b) {
    if (a < b) {
        a = b;
        return true;
    } else
        return false;
}
template <class T>
bool chmin(T& a, T b) {
    if (a > b) {
        a = b;
        return true;
    } else
        return false;
}

struct S {
    long long val;
    int siz;
    S() {}
    S(long long v, int s) : val(v), siz(s) {}
};
using F = long long;

S op(S a, S b) { return {a.val + b.val, a.siz + b.siz}; }
S e() { return {0LL, 0LL}; }
S mapping(F f, S x) { return {x.val + x.siz * f, x.siz}; }
F composition(F f, F g) { return f + g; }
F id() { return 0; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<ll> C(N);
    rep(i, 0, N) cin >> C[i];
    vector<S> V(N);
    rep(i, 0, N) { V[i] = S(C[i], 1); }
    lazy_segtree<S, op, e, F, mapping, composition, id> lseg(V);
    ll t, l, r, v;
    rep(i, 0, Q) {
        cin >> t;
        if (t == 1) {
            cin >> l >> r >> v;
            l--;
            lseg.apply(l, r, v);
        } else {
            cin >> l >> r;
            l--;
            cout << lseg.prod(l, r).val << endl;
        }
    }
}