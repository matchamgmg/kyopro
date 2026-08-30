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

    int N, Q;
    cin >> N >> Q;
    vector<int> P(N);
    rep(i, 0, N) cin >> P[i];
    rep(i, 0, N) P[i]--;
    vector<int> L(Q), R(Q);
    rep(i, 0, Q) cin >> L[i] >> R[i];
    rep(i, 0, Q) L[i]--;
    vector<int> qidx(Q);
    iota(all(qidx), 0);

    int maxN = 1;
    while (maxN < N) {
        maxN <<= 1;
    }

    vector<long long> Hilbert_val(Q);
    auto Hilbert_order = [&](int x, int y) -> long long {
        long long rx, ry, d;
        d = 0;
        for (long long s = maxN >> 1; s > 0; s >>= 1) {
            rx = (x & s) > 0, ry = (y & s) > 0;
            d += s * s * ((rx * 3) ^ ry);
            if (ry > 0) continue;
            if (rx > 0) {
                x = maxN - 1 - x;
                y = maxN - 1 - y;
            }
            swap(x, y);
        }
        return d;
    };

    for (int i = 0; i < Q; ++i) Hilbert_val[i] = Hilbert_order(L[i], R[i]);
    sort(all(qidx),
         [&](int i, int j) { return Hilbert_val[i] < Hilbert_val[j]; });

    vector<int> cnt(N, 0);
    int variety = 0;
    auto add = [&](int x) -> void {
        cnt[P[x]]++;
        if (cnt[P[x]] == 1) variety++;
        return;
    };

    auto del = [&](int x) -> void {
        cnt[P[x]]--;
        if (cnt[P[x]] == 0) variety--;
        return;
    };

    vector<int> ans(Q);

    int nl = 0, nr = 0;
    for (int i : qidx) {
        while (nl > L[i]) --nl, add(nl);
        while (nr < R[i]) add(nr), ++nr;
        while (nl < L[i]) del(nl), ++nl;
        while (nr > R[i]) --nr, del(nr);
        /* クエリの計算結果 */
        ans[i] = variety;
    }
    rep(i, 0, Q) cout << ans[i] << endl;
}