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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  struct Edge {
    int from;
    int to;
    ll weight;
  };
  vector<Edge> edges;
  vector<vector<Edge>> G(N);
  rep(i, 0, Q) {
    int t, u, v;
    cin >> t >> u >> v;
    u--;
    v--;
    G[u].emplace_back(u, v, t);
    G[v].emplace_back(v, u, -t);
  }

  atcoder::scc_graph graph(N);
  rep(i, 0, N) {
    for (Edge &e : G[i]) {
      graph.add_edge(i, e.to);
    }
  }

  vector<vector<int>> SCC = graph.scc();
  int M = SCC.size();
  vector<set<int>> st(M);
  vector<int> idx(N);
  rep(i, 0, M) {
    for (int j : SCC[i]) {
      st[i].insert(j);
      idx[j] = i;
    }
  }

  const int INF = 1e9;
  vector<ll> ans(N, -INF);
  deque<pair<int, int>> que;
  rep(i, 0, M) {
    for (int s : SCC[i]) {
      if (ans[s] == -INF) {
        ans[SCC[0][0]] = 0;
        que.push_back({SCC[0][0], 0});
        while (!que.empty()) {
          auto p = que.front();
          que.pop_front();
          int u = p.first;
          int d = p.second;
          if (ans[u] > d) {
            continue;
          }
          for (auto &e : G[u]) {
            int nd = d + e.weight;
            if (ans[e.to] != -INF) {
              if (st[idx[u]].contains(e.to)) {
                if (ans[u] != nd) {
                  cout << "No" << endl;
                  return 0;
                } else {
                  continue;
                }
              } else {
                if (ans[e.to] >= nd) {
                  continue;
                }
              }
            }
            ans[e.to] = nd;
            if (e.weight == 0) {
              que.push_front({e.to, nd});
            } else {
              que.push_back({e.to, nd});
            }
          }
        }
      }
    }
  }

  bool ok = true;
  rep(i, 0, M) {
    bool same = true;
    if (SCC[i].size() > 1) {
      rep(j, 1, SCC[i].size()) {
        same &= (ans[SCC[i][0]] == ans[SCC[i][j]]);
      }
    }
    ok &= same;
  }
  if (!ok) {
    cout << "No" << endl;
  } else {
    cout << "Yes" << endl;
    v_cout(ans);
  }
}