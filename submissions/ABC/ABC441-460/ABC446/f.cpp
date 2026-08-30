#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T>
using pq = priority_queue<T, vector<T>>;  // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>;  // 小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) \
    if (a) {  \
        pyes; \
    } else {  \
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
        rep(i, 0, n) {                                     \
            rep(j, 0, a[i].size()) cout << a[i][j] << " "; \
            cout << endl;                                  \
        }                                                  \
        cout << endl;                                      \
    }
#define grid_check(x, y, X, Y) (0 <= x && x < X && 0 <= y && y < Y)
template <class T>
bool chmax(T& a, T b) {
    if (a < b) {
        a = b;
        return true;
    } else
        return false;
}
template <class T>
bool chmin(T& a, T b) {
    if (a > b) {
        a = b;
        return true;
    } else
        return false;
}

// 頂点1..kを始点としてDFS,BFS
// 頂点k+1..Nにたどり着いたとき，その頂点をキューに保持

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<vector<int>> G(N);
    rep(i, 0, M) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        G[u].emplace_back(v);
    }
    set<int> next_v;
    vector<bool> seen(N, false);

    auto DFS = [&](auto f, int s, int k) -> void {
        seen[s] = true;
        for (int v : G[s]) {
            if (seen[v]) continue;
            if (v > k) {
                next_v.emplace(v);
                continue;
            }
            f(f, v, k);
        }
    };

    next_v.emplace(0);
    rep(i, 0, N) {
        if (next_v.count(i)) {
            next_v.erase(i);
            DFS(DFS, i, i);
            if (seen[i])
                cout << next_v.size() << endl;
            else
                cout << -1 << endl;
        } else {
            cout << -1 << endl;
        }
    }
}