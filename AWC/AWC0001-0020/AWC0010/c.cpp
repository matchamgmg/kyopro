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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, K, Q;
    cin >> N >> K >> Q;
    vector<ll> A(N);
    rep(i, 0, N) cin >> A[i];
    dsu uf(N);
    rep(i, 0, N - 1) {
        if (abs(A[i] - A[i + 1]) <= K) uf.merge(i, i + 1);
    }
    for (; Q--;) {
        int l, r;
        cin >> l >> r;
        l--;
        r--;
        YN(uf.same(l, r));
    }
}