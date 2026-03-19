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

// 木DPにより，部分木の解を親ノードへ集約する
// 持つべき値 :
// 部分木の根の次数 : 4以上, 3, 2以下
// 閉じたムカデグラフの最大長
// 開いたムカデグラフの最大長

void solve()
{
    int N;
    cin >> N;
    vector<vector<int>> G(N);
    vector<int> Deg(N, 0);
    rep(i, 0, N - 1)
    {
        int A, B;
        cin >> A >> B;
        A--;
        B--;
        G[A].emplace_back(B);
        G[B].emplace_back(A);
        Deg[A]++;
        Deg[B]++;
    }

    vector<int> OpenCenti(N, 0), CloseCenti(N, 0);
    auto DFS = [&](auto f, int u, int p) -> void
    {
        int best = 0, second_best = 0;
        for (int v : G[u])
        {
            if (v == p)
            {
                continue;
            }
            f(f, v, u);
            if (best <= OpenCenti[v])
            {
                second_best = best;
                best = OpenCenti[v];
            }
            else if (second_best <= OpenCenti[v])
            {
                second_best = OpenCenti[v];
            }
        }
        if (Deg[u] >= 4)
        {
            chmax(CloseCenti[u], best + second_best + 1);
            chmax(OpenCenti[u], best + 1);
        }
        else if (Deg[u] == 3)
        {
            chmax(CloseCenti[u], best + 1);
            chmax(OpenCenti[u], 1);
        }
        else if (Deg[u] == 2)
        {
            chmax(CloseCenti[u], 1);
        }
    };

    DFS(DFS, 0, -1);
    cout << *max_element(all(CloseCenti)) << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    cin >> Q;
    for (; Q--;)
        solve();
}