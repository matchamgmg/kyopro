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

// 区間DP ?

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> A(2 * N);
    rep(i, 0, 2 * N) cin >> A[i];
    const int INF = 1e9;
    // dp[l][r] : [l,r)をすべて取り除くための最小コスト
    // vector<vector<int>> dp(2 * N + 1, vector<int>(2 * N + 1, INF));
    // auto DFS = [&](auto f, int l, int r) -> int {
    //     if (dp[l][r] != INF) return dp[l][r];
    //     if (l > r) {
    //         return INF;
    //     }
    //     if (l == r) {
    //         return dp[l][r] = 0;
    //     }
    //     if (r - l == 1) {
    //         return INF;
    //     }
    //     if (r - l == 2) {
    //         return dp[l][r] = abs(A[r - 1] - A[l]);
    //     }
    //     int res = INF;
    //     chmin(res, f(f, l + 1, r - 1) + abs(A[r - 1] - A[l]));
    //     for (int k = l + 2; k + 1 < r; k += 2) {
    //         chmin(res, f(f, l, k) + f(f, k, r));
    //     }
    //     return dp[l][r] = res;
    // };

    // DFS(DFS, 0, 2 * N);

    vector<vector<int>> dp(2 * N, vector<int>(2 * N, INF));
    rep(i, 0, 2 * N - 1) dp[i][i + 1] = abs(A[i + 1] - A[i]);
    for (int i = 3; i < 2 * N; i += 2) {
        for (int l = 0; l + i < 2 * N; l++) {
            chmin(dp[l][l + i], dp[l + 1][l + i - 1] + abs(A[l] - A[l + i]));
            for (int k = l + 1; k + 1 < l + i; ++k) {
                chmin(dp[l][l + i], dp[l][k] + dp[k + 1][l + i]);
            }
        }
    }
    cout << dp[0][2 * N - 1] << endl;
}