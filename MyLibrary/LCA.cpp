#include <vector>
#include <algorithm>
using namespace std;

// 有向木
struct LCA_Directed
{
    // 頂点0が根
    // N-1本の有向辺が与えられる
    LCA_Directed(int n_, const vector<int> &P) : n(n_), max_depth(-1)
    {
        parent.assign(n_, 0);
        for (int i = 0; i < n_ - 1; ++i)
        {
            parent[i + 1] = P[i];
        }
        depth.assign(n_, INF);

        calc_depth();
        calc_doubling_table();
    }

    void calc_depth()
    {
        auto DFS = [this](auto f, int v, int root = 0) -> void
        {
            if (depth[v] < INF)
            {
                return;
            }
            if (v == root)
            {
                return;
            }
            f(f, parent[v]);
            depth[v] = depth[parent[v]] + 1;
            return;
        };

        depth[0] = 0;
        for (int i = 1; i < n; ++i)
        {
            DFS(DFS, i);
        }
        max_depth = *max_element(depth.begin(), depth.end());
    }

    void calc_doubling_table()
    {
        doubling_parent.emplace_back(parent);
        vector<int> next_parent(n);
        int k = 0;
        while ((1 << k) <= max_depth - 1)
        {
            for (int i = 0; i < n; ++i)
            {
                next_parent[i] = doubling_parent[k][doubling_parent[k][i]];
            }
            doubling_parent.emplace_back(next_parent);
            ++k;
        }
        max_k = k;
    }

    int solve(int x, int y)
    {
        if (depth[x] > depth[y])
        {
            swap(x, y);
        }
        int depth_diff = depth[y] - depth[x];
        for (int k = 0; k <= max_k; ++k)
        {
            if (depth_diff & (1 << k))
            {
                y = doubling_parent[k][y];
            }
        }

        if (x == y)
        {
            return x;
        }

        for (int k = max_k; k >= 0; --k)
        {
            if (doubling_parent[k][x] != doubling_parent[k][y])
            {
                x = doubling_parent[k][x];
                y = doubling_parent[k][y];
            }
        }
        return doubling_parent[0][x];
    }

private:
    int n;
    const int INF = 1 << 29;
    int max_depth;
    int max_k;
    vector<int> parent;
    vector<int> depth;
    vector<vector<int>> doubling_parent;
};