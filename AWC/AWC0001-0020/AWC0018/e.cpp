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

// dp[i][j] : i番目の山をj個目に登頂するときの、その時点までの入山料の最小値

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K, B;
    cin >> N >> K >> B;
    vector<ll> C(N), S(N);
    rep(i, 0, N) cin >> C[i] >> S[i];

    const int INF = 1e9;
    vector<vector<ll>> dp(N, vector<ll>(N + 1, INF));
    rep(i, 0, N)
    {
        dp[i][1] = C[i];
        rep(j, 2, N + 1)
        {
            ll best = INF;
            rep(pre, 0, i)
            {
                if (S[pre] < S[i])
                {
                    chmin(best, dp[pre][j - 1] + C[i]);
                }
            }
            dp[i][j] = best;
        }
    }
    // vv_cout(dp);
    rrep(x, 0, min(N, K) + 1)
    {
        ll best = INF;
        rep(i, 0, N)
        {
            chmin(best, dp[i][x]);
        }
        if (best <= B)
        {
            cout << x << endl;
            return 0;
        }
    }
}