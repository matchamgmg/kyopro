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

    int N, M;
    cin >> N >> M;
    vector<ll> L(M), D(M), K(M), C(M);
    vector<int> A(M), B(M);
    rep(i, 0, M) cin >> L[i] >> D[i] >> K[i] >> C[i] >> A[i] >> B[i];
    rep(i, 0, M) {
        A[i]--;
        B[i]--;
    }
    vector<vector<int>> G(N);
    rep(i, 0, M) { G[B[i]].emplace_back(i); }
    const ll INF = 1LL << 60;
    vector<ll> f(N, -INF);
    pq<pair<ll, int>> que;
    f[N - 1] = INF;
    que.emplace(INF, N - 1);
    while (!que.empty()) {
        auto p = que.top();
        que.pop();
        if (p.first < f[p.second]) continue;
        for (int i : G[p.second]) {
            // T[i] + D[i]*x <= f[v]-C[i]
            ll t = f[p.second] - C[i] - L[i];
            if (t < 0) continue;
            t = min(t / D[i], K[i] - 1) * D[i] + L[i];
            if (f[A[i]] >= t) continue;
            f[A[i]] = t;
            que.emplace(t, A[i]);
        }
    }
    rep(i, 0, N - 1) {
        if (f[i] == -INF)
            cout << "Unreachable" << endl;
        else
            cout << f[i] << endl;
    }
}