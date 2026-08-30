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

    int N, M;
    cin >> N >> M;
    struct Edge {
        int to;
        ll cost;
        Edge() {}
        Edge(int t, ll c) : to(t), cost(c) {}
    };

    vector<vector<Edge>> G(N);
    rep(i, 0, M) {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        a--;
        b--;
        G[a].emplace_back(b, c);
        G[b].emplace_back(a, c);
    }

    auto dijkstra = [&](int s, vector<ll>& dist) -> void {
        pq_g<pair<ll, int>> que;
        dist[s] = 0;
        que.emplace(dist[s], s);
        while (!que.empty()) {
            auto p = que.top();
            que.pop();
            int u = p.second;
            ll d = p.first;
            if (d > dist[u]) continue;
            for (auto e : G[u]) {
                ll nd = d + e.cost;
                if (nd >= dist[e.to]) continue;
                dist[e.to] = nd;
                que.emplace(dist[e.to], e.to);
            }
        }
    };

    const ll INF = 1LL << 60;
    vector<ll> dist1(N, INF), distN(N, INF);
    dijkstra(0, dist1);
    dijkstra(N - 1, distN);
    rep(i, 0, N) { cout << dist1[i] + distN[i] << endl; }
}