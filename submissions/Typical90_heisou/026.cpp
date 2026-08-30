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
    vector<vector<int>> G(N);
    rep(i, 0, N - 1)
    {
        int A, B;
        cin >> A >> B;
        A--;
        B--;
        G[A].emplace_back(B);
        G[B].emplace_back(A);
    }
    vector<int> A, B;
    auto DFS = [&](auto f, int s, int p, int t) -> void
    {
        if (t & 1)
        {
            A.emplace_back(s + 1);
        }
        else
        {
            B.emplace_back(s + 1);
        }
        for (int v : G[s])
        {
            if (v == p)
                continue;
            f(f, v, s, t ^ 1);
        }
        return;
    };
    DFS(DFS, 0, -1, 0);
    if (A.size() < B.size())
        swap(A, B);
    sort(all(A));
    rep(i, 0, N / 2)
    {
        cout << A[i] << ' ';
    }
    cout << endl;
}