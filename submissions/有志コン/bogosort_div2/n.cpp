/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>;               // 大きい順
template <class T> using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)
#define rrep(i, s, n) for (int i = (n - 1); i >= (int)(s); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
void pyes() {
  cout << "Yes" << endl;
}
void pno() {
  cout << "No" << endl;
}
void YN(bool x) {
  cout << (x ? "Yes" : "No") << endl;
}
template <class T> void v_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) cout << a[i] << " ";
  cout << endl;
}
template <class T> void vv_cout(const vector<T> &a) {
  int n = a.size();
  rep(i, 0, n) {
    rep(j, 0, a[i].size()) cout << a[i][j] << " ";
    cout << endl;
  }
}
bool grid_check(int x, int y, int X, int Y) {
  return (0 <= x && x < X && 0 <= y && y < Y);
}
template <class T> bool chmax(T &a, T b) {
  if (a < b) {
    a = b;
    return true;
  } else {
    return false;
  }
}
template <class T> bool chmin(T &a, T b) {
  if (a > b) {
    a = b;
    return true;
  } else {
    return false;
  }
}

// すべての頂点の出自数が判明すると，辺の追加や削除はできなくなる
// 未探索の辺については，重みの変更，行先の変更ができる
// 未探索の頂点については，辺の追加，削除ができる
// 距離X以上で確定する頂点は未探索のままでもいい

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, Q;
  cin >> N >> M >> Q;
  struct Edge {
    int id;
    int idx;
    int from;
    int to;
    ll weight;
  };

  vector<Edge> edges(M);
  vector<vector<Edge>> G(N);
  vector<int> degree(N, 0);
  rep(i, 0, M) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    u--, v--;
    edges[i] = {i, int(G[u].size()), u, v, w};
    G[u].emplace_back(i, G[u].size(), u, v, w);
    degree[u]++;
  }

  vector<vector<Edge>> AG = G;

  vector<int> discovered_edge(M, false), discovered_vertex(N, false);
  discovered_vertex[0] = true;
  vector<vector<Edge>> JG(N);
  vector<pair<int, int>> queries(Q);
  int cur = 0;
  rep(q, 0, Q) {
    int t, x;
    cin >> t >> x;
    x--;
    queries[q] = {t, x};
    if (t == 1) {
      Edge &e = G[cur][x];
      discovered_edge[e.id] = true;
      discovered_vertex[e.to] = true;
      JG[e.from].push_back(e);
    } else {
      cur = x;
    }
  }

  ll X;
  cin >> X;

  int MM = M;
  rep(i, 0, M) {
    auto &e = edges[i];
    if (!discovered_edge[e.id]) {
      AG[e.from][e.idx].to = N - 1;
      AG[e.from][e.to].weight = 1;
    }
  }
  rep(i, 0, N) {
    if (!discovered_vertex[i]) {
      AG[i].emplace_back(-1, -1, i, N - 1, 1);
      MM++;
    }
  }

  auto dijkstra = [&](const vector<vector<Edge>> &g, vector<ll> &dist) -> void {
    pq_g<pair<ll, int>> que;
    dist[0] = 0;
    que.emplace(0, 0);
    while (!que.empty()) {
      auto p = que.top();
      que.pop();
      ll d = p.first;
      ll u = p.second;
      if (dist[u] < d) {
        continue;
      }
      for (auto &e : g[u]) {
        ll nd = d + e.weight;
        if (nd >= dist[e.to]) {
          continue;
        }
        dist[e.to] = nd;
        que.emplace(nd, e.to);
      }
    }
  };

  const ll INF = 1e18;
  vector<ll> Gdist(N, INF), JGdist(N, INF), AGdist(N, INF);
  dijkstra(G, Gdist);
  dijkstra(JG, JGdist);
  dijkstra(AG, AGdist);
  if (X != AGdist[N - 1]) {
    cout << "WA" << endl;
    cout << MM << endl;
    rep(i, 0, N) {
      for (auto &e : AG[i]) {
        cout << e.from + 1 << ' ' << e.to + 1 << ' ' << e.weight << endl;
      }
    }
  } else {
    vector<bool> important_vertex(N, false), important_edge(M, false);
    rep(i, 0, N) {
      if (Gdist[i] < Gdist[N - 1] - 1) {
        important_vertex[i] = true;
      }
    }
    rep(i, 0, M) {
      auto &e = edges[i];
      if (important_vertex[e.from]) {
        important_edge[i] = true;
      }
    }
    bool ok = true;
    vector<int> edge_cnt(M, 0);
    cur = 0;
    rep(q, 0, Q) {
      int t = queries[q].first;
      int x = queries[q].second;
      if (t == 1) {
        auto &e = G[cur][x];
        if (!important_edge[e.id]) {
          ok = false;
        }
        edge_cnt[e.id]++;
      } else {
        cur = x;
      }
    }
    rep(i, 0, M) {
      ok &= (edge_cnt[i] <= 1);
    }
    if (ok) {
      cout << "AC" << endl;
    } else {
      cout << "TOOMANY" << endl;
    }
  }
}