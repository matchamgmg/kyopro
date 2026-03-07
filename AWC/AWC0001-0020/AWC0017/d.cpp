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

    int N, M, K;
    cin >> N >> M >> K;
    vector<ll> A(N);
    rep(i, 0, N) cin >> A[i];
    vector<vector<ll>> B(N, vector<ll>(N, 0));
    rep(i, 0, M)
    {
        ll U, V, b;
        cin >> U >> V >> b;
        U--;
        V--;
        B[U][V] = b;
        B[V][U] = b;
    }
    const ll INF = 1LL << 60;
    ll ans = -INF;
    rep(bits, 0, (1 << N))
    {
        ll score = 0;
        int cnt = 0;
        rep(i, 0, N)
        {
            if (bits >> i & 1)
            {
                cnt++;
                score += A[i];
            }
        }
        if (cnt != K)
            continue;
        rep(i, 0, N - 1)
        {
            rep(j, i + 1, N)
            {
                if ((bits >> i & 1) && (bits >> j & 1))
                    score -= B[i][j];
            }
        }
        chmax(ans, score);
    }
    cout << ans << endl;
}