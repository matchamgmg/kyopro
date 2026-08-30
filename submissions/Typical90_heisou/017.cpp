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

    ll N, M;
    cin >> N >> M;
    vector<ll> L(M), R(M);
    rep(i, 0, M) { cin >> L[i] >> R[i]; }
    rep(i, 0, M) {
        L[i]--;
        R[i]--;
    }
    vector<int> idx(M);
    iota(all(idx), 0);
    sort(all(idx), [&](int i, int j) {
        return R[i] == R[j] ? L[i] < L[j] : R[i] < R[j];
    });
    // 1. 包含せず，接しない
    // 2. 接する
    // 3. 接せず，包含する
    ll ans = 0;
    vector<ll> A(N + 1, 0);
    rep(i, 0, M) { A[R[i]]++; }
    rep(i, 0, N) { A[i + 1] += A[i]; }
    rep(i, 0, M) {
        if (L[i] == 0) continue;
        ans += A[L[i] - 1];
    }
    vector<ll> cnt(N, 0);
    rep(i, 0, M) {
        cnt[L[i]]++;
        cnt[R[i]]++;
    }
    rep(i, 0, N) { ans += cnt[i] * (cnt[i] - 1) / 2; }
    fenwick_tree<ll> ft(N);
    rep(i, 0, M) {
        ans += ft.sum(L[idx[i]] + 1, N);
        ft.add(L[idx[i]], 1);
    }
    cout << M * (M - 1) / 2 - ans << endl;
}