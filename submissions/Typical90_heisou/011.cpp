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

    int N;
    cin >> N;
    vector<ll> D(N), C(N), S(N);
    rep(i, 0, N) cin >> D[i] >> C[i] >> S[i];
    vector<int> idx(N);
    iota(all(idx), 0);
    sort(all(idx), [&](int i, int j) { return D[i] < D[j]; });
    ll M = D[idx[N - 1]];
    vector<ll> dp(M + 1, 0);
    rep(i, 0, N) {
        int k = idx[i];
        vector<ll> next_dp = dp;
        rep(j, 0, min(D[k] + 1, M + 1)) {
            if (j - C[k] >= 0) chmax(next_dp[j], dp[j - C[k]] + S[k]);
        }
        rep(j, 0, M) chmax(next_dp[j + 1], next_dp[j]);
        swap(dp, next_dp);
        // v_cout(dp);
    }
    cout << dp[M] << endl;
}