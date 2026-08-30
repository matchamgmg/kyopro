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

// template <class T>
// struct PotentializedUnionFind
// {
//     vector<int> par, siz;
//     vector<T> diff_weight;

//     PotentializedUnionFind(int n, T AbelSumUnity = 0) : siz(n, 1), diff_weight(n, AbelSumUnity)
//     {
//         par.resize(n);
//         iota(par.begin(), par.end(), 0);
//     }

//     int root(int x)
//     {
//         if (par[x] == x)
//             return x;
//         else
//         {
//             int r = root(par[x]);
//             diff_weight[x] += diff_weight[par[x]];
//             return par[x] = r;
//         }
//     }

//     bool issame(int x, int y)
//     {
//         return root(x) == root(y);
//     }

//     T weight(int x)
//     {
//         root(x);
//         return diff_weight[x];
//     }

//     int diff(int x, int y)
//     {
//         // issame(x, y) = false;となる場合の動作は不定
//         return weight(y) - weight(x);
//     }

//     bool unite(int x, int y, T w)
//     {
//         w += weight(x);
//         w -= weight(y);
//         x = root(x);
//         y = root(y);
//         if (x == y)
//             return false;
//         if (siz[x] < siz[y])
//         {
//             swap(x, y);
//             w = -w;
//         }
//         par[y] = x;
//         siz[x] += siz[y];
//         diff_weight[y] = w;
//         return true;
//     }

//     int size(int x)
//     {
//         return siz[par[x]];
//     }

//     int groups()
//     {
//         int res = 0;
//         for (int i = 0; i < par.size(); i++)
//         {
//             if (root(i) == i)
//                 res++;
//         }
//         return res;
//     }
// };

template <class Abel>
struct UnionFind
{
    vector<int> par;
    vector<int> rank;
    vector<Abel> diff_weight;

    UnionFind(int n = 1, Abel SUM_UNITY = 0)
    {
        init(n, SUM_UNITY);
    }

    void init(int n = 1, Abel SUM_UNITY = 0)
    {
        par.resize(n);
        rank.resize(n);
        diff_weight.resize(n);
        for (int i = 0; i < n; ++i)
            par[i] = i, rank[i] = 0, diff_weight[i] = SUM_UNITY;
    }

    int root(int x)
    {
        if (par[x] == x)
        {
            return x;
        }
        else
        {
            int r = root(par[x]);
            diff_weight[x] += diff_weight[par[x]];
            return par[x] = r;
        }
    }

    Abel weight(int x)
    {
        root(x);
        return diff_weight[x];
    }

    bool issame(int x, int y)
    {
        return root(x) == root(y);
    }

    bool merge(int x, int y, Abel w)
    {
        w += weight(x);
        w -= weight(y);
        x = root(x);
        y = root(y);
        if (x == y)
            return false;
        if (rank[x] < rank[y])
            swap(x, y), w = -w;
        if (rank[x] == rank[y])
            ++rank[x];
        par[y] = x;
        diff_weight[y] = w;
        return true;
    }

    Abel diff(int x, int y)
    {
        return weight(y) - weight(x);
    }
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
    UnionFind<int> uf(N);
    bool ans = true;
    while (!que.empty())
    {
        Edge edge = que.top();
        que.pop();
        if (uf.issame(edge.from, edge.to))
        {
            if (abs(uf.diff(edge.from, edge.to)) != edge.w)
            {
                ans = false;
            }
        }
        else
        {
            uf.merge(edge.from, edge.to, edge.w);
            G[edge.from].emplace_back(edge.to, edge.w);
            G[edge.to].emplace_back(edge.from, edge.w);
        }
    }
    if (!ans)
    {
        YN(ans);
        return 0;
    }
    rep(i, 0, N)
    {
        rep(j, 0, N)
        {
            if (A[i][j] == -1)
                continue;
            if (A[i][j] != abs(uf.diff(i, j)))
            {
                ans = false;
            }
        }
    }
    YN(ans);
}