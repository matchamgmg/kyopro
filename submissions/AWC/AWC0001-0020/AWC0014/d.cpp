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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, T;
    cin >> N >> M >> T;
    struct Edge {
        int to;
        ll cost;
        Edge() {}
        Edge(int t, ll c) : to(t), cost(c) {}
    };
    vector<vector<Edge>> G(N);
    rep(i, 0, M) {
        int u, v;
        ll c;
        cin >> u >> v >> c;
        u--;
        v--;
        G[u].emplace_back(v, c);
        G[v].emplace_back(u, c);
    }

    const ll INF = 1LL << 60;
    vector<ll> dist(N, INF);
    pq_g<pair<ll, int>> que;
    dist[0] = 0;
    que.emplace(dist[0], 0);
    while (!que.empty()) {
        auto p = que.top();
        que.pop();
        int u = p.second;
        ll d = p.first;
        if (dist[u] < d) continue;
        for (auto e : G[u]) {
            int v = e.to;
            ll nd = d + e.cost;
            if (dist[v] <= nd) continue;
            dist[v] = nd;
            que.emplace(nd, v);
        }
    }

    cout << (dist[T - 1] == INF ? -1LL : dist[T - 1] * 2) << endl;
}