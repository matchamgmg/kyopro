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

// 前計算O(N^3)
// O(N!)

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<int> P(N);
    rep(i, 0, N) cin >> P[i];
    int S, T;
    cin >> S >> T;
    S--;
    T--;
    struct Edge
    {
        int from, to, cost;
        Edge() {}
        Edge(const int f, const int t, const int c) : from(f), to(t), cost(c) {}
    };

    vector<Edge> edges;
    rep(i, 0, M)
    {
        int U, V, W;
        cin >> U >> V >> W;
        U--;
        V--;
        edges.emplace_back(U, V, W);
        edges.emplace_back(V, U, W);
        // cout << U << ' ' << V << endl;
    }

    const int INF = 1 << 29;
    vector<vector<int>> dist(N, vector<int>(N, INF));
    rep(i, 0, N) dist[i][i] = 0;
    for (auto e : edges)
    {
        chmin(dist[e.from][e.to], e.cost);
    }
    rep(k, 0, N)
    {
        rep(i, 0, N)
        {
            rep(j, 0, N)
            {
                chmin(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    // bit DP
    // 部分集合・最終地点
    vector<vector<int>> dp(N, vector<int>((1 << N), INF));
    dp[S][1 << S] = 0;
    rep(bit, 0, (1 << N))
    {
        rep(i, 0, N)
        {
            if (dp[i][bit] == INF)
                continue;
            rep(j, 0, N)
            {
                if (bit >> j & 1)
                    continue;
                int nbit = bit | (1 << j);
                chmin(dp[j][nbit], dp[i][bit] + dist[i][j]);
            }
        }
    }
    int ans = -INF;
    rep(bit, 0, (1 << N))
    {
        if (bit >> T & 1)
        {
            int gain = 0;
            rep(i, 0, N)
            {
                if (bit >> i & 1)
                {
                    gain += P[i];
                }
            }
            chmax(ans, gain - dp[T][bit]);
        }
    }
    cout << ans << endl;
}