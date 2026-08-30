#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a)    \
    {         \
        pyes; \
    }         \
    else      \
    {         \
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
        rep(i, 0, n)                                       \
        {                                                  \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
        return false;
}
template <class T>
bool chmin(T &a, T b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    else
        return false;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> X(N), Y(N);
    rep(i, 0, N) cin >> X[i] >> Y[i];
    vector<vector<int>> dist(N, vector<int>(N, 0));
    auto calc_d = [&](int i, int j) -> int
    {
        return (X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j]);
    };
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            dist[i][j] = calc_d(i, j);
        }
    }
    const int INF = 1 << 30;
    vector<vector<int>> dp(N, vector<int>((1 << N), INF));
    dp[0][1] = 0;
    rep(bits, 0, (1 << N))
    {
        rep(i, 0, N)
        {
            if (dp[i][bits] == INF)
                continue;
            rep(j, 0, N)
            {
                if (bits >> j & 1)
                    continue;
                int nbit = bits | (1 << j);
                chmin(dp[j][nbit], dp[i][bits] - dist[i][0] + dist[i][j] + dist[j][0]);
            }
        }
    }
    int ans = INF;
    rep(i, 0, N)
    {
        chmin(ans, dp[i][(1 << N) - 1]);
    }
    cout << ans << endl;
}