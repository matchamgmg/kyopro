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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x)
{
    cout << (x ? "Yes" : "No") << endl;
}
template <class T>
void v_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n) cout << a[i] << " ";
    cout << endl;
}
template <class T>
void vv_cout(const vector<T> &a)
{
    int n = a.size();
    rep(i, 0, n)
    {
        rep(j, 0, a[i].size()) cout << a[i][j] << " ";
        cout << endl;
    }
}
bool grid_check(int x, int y, int X, int Y)
{
    return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T>
bool chmax(T &a, T b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    else
    {
        return false;
    }
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
    {
        return false;
    }
}

void solve()
{
    int N, M;
    cin >> N >> M;
    vector<vector<int>> G(N);
    rep(i, 0, M)
    {
        int U, V;
        cin >> U >> V;
        U--;
        V--;
        G[U].emplace_back(V);
        G[V].emplace_back(U);
    }
    int W;
    cin >> W;
    vector<string> S(N);
    rep(i, 0, N)
    {
        cin >> S[i];
    }

    vector<vector<int>> G2(N * W);
    rep(u, 0, N)
    {
        rep(i, 0, W)
        {
            if (S[u][i] == 'o' && S[u][(i + 1) % W] == 'o')
            {
                G2[u * W + i].emplace_back(u * W + (i + 1) % W);
            }
        }
        for (int v : G[u])
        {
            rep(i, 0, W)
            {
                if (S[u][i] == 'o' && S[v][(i + 1) % W] == 'o')
                {
                    G2[u * W + i].emplace_back(v * W + (i + 1) % W);
                }
            }
        }
    }

    vector<bool> visited(N * W, false);
    vector<bool> finised(N * W, false);

    auto DFS = [&](auto f, int U) -> bool
    {
        visited[U] = true;
        for (int V : G2[U])
        {
            if (finised[V])
            {
                continue;
            }
            if (visited[V] && !finised[V])
            {
                return true;
            }
            if (f(f, V))
            {
                return true;
            }
        }
        finised[U] = true;
        return false;
    };

    bool ans = false;
    rep(V, 0, N * W)
    {
        if (finised[V])
        {
            continue;
        }
        ans |= DFS(DFS, V);
    }
    YN(ans);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (; T--;)
        solve();
}