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

struct UnionFind
{
    UnionFind(int n_) : n(n_), group_num(n_) { par.assign(n_, -1); }
    int root(int x)
    {
        if (par[x] < 0)
            return x;
        return par[x] = root(par[x]);
    }
    int size(int x) { return -par[root(x)]; }
    bool same(int x, int y) { return root(x) == root(y); }
    bool merge(int x, int y)
    {
        x = root(x);
        y = root(y);
        if (x == y)
            return false;
        if (size(x) > size(y))
            swap(x, y);
        par[y] += par[x];
        par[x] = y;
        group_num--;
        return true;
    }
    int groups() { return group_num; }

private:
    int n;
    int group_num;
    vector<int> par;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<vector<int>> A(N, vector<int>(N, -1));
    struct Edge
    {
        int from, to;
        int w;
        Edge() {}
        Edge(int f, int t, int w) : from(f), to(t), w(w) {}
        const bool operator<(const Edge &rhs) const
        {
            return w == rhs.w ? from == rhs.from ? to < rhs.to : from < rhs.from : w < rhs.w;
        }
        const bool operator>(const Edge &rhs) const
        {
            return w == rhs.w ? from == rhs.from ? to > rhs.to : from > rhs.from : w > rhs.w;
        }
    };
    pq_g<Edge> que;
    rep(i, 0, N - 1)
    {
        rep(j, i + 1, N)
        {
            cin >> A[i][j];
            que.emplace(i, j, A[i][j]);
        }
    }
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            A[i][j] = max(A[i][j], A[j][i]);
        }
    }
    vector<vector<pair<int, int>>> G(N);
    UnionFind uf(N);
    bool ans = true;
    while (!que.empty())
    {
        Edge edge = que.top();
        que.pop();
        if (!uf.same(edge.from, edge.to))
        {
            uf.merge(edge.from, edge.to);
            G[edge.from].emplace_back(edge.to, edge.w);
            G[edge.to].emplace_back(edge.from, edge.w);
        }
    }

    const int INF = 1e9;
    vector<vector<int>> dist(N, vector<int>(N, INF));

    auto BFS = [&](int s, vector<int> &vec) -> void
    {
        vec[s] = 0;
        queue<int> que;
        que.push(s);
        while (!que.empty())
        {
            int u = que.front();
            que.pop();
            for (auto [v, w] : G[u])
            {
                if (vec[v] != INF)
                    continue;
                vec[v] = vec[u] + w;
                que.push(v);
            }
        }
    };
    rep(i, 0, N)
    {
        BFS(i, dist[i]);
    }
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            if (A[i][j] == -1)
                continue;
            if (A[i][j] != dist[i][j])
            {
                ans = false;
            }
        }
    }
    YN(ans);
}