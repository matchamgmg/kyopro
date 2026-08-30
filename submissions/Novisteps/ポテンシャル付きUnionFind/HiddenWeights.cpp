#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

/**
 * @brief 加法群をポテンシャルとして持つ，ポテンシャル付きUnionFind
 */
template <class T> class PotentializedUnionFind {
  int n_;
  int group_num_;
  std::vector<int> parent_;
  std::vector<T> potential_; ///< 各グループでの代表元との関係

public:
  PotentializedUnionFind(int n)
      : n_(n), group_num_(n), parent_(std::vector<int>(n, -1)),
        potential_(std::vector<T>(n, T{})) {}

  int groups() {
    return group_num_;
  }

  int size(int x) {
    return -parent_[root(x)];
  }

  int root(int x) {
    if (parent_[x] < 0) {
      return x;
    }
    int r = root(parent_[x]);
    potential_[x] += potential_[parent_[x]];
    return parent_[x] = r;
  }

  T get_potential(int x) {
    root(x);
    return potential_[x];
  }

  /**
   * @brief x -> y へのポテンシャル
   * @pre is_same(x, y) == true
   */
  T diff(int x, int y) {
    return get_potential(y) - get_potential(x);
  }

  bool is_same(int x, int y) {
    return root(x) == root(y);
  }

  /**
   * @brief 頂点xから頂点yに向かって重さwの辺を張る
   * @return true: マージ成功，false: 矛盾発生
   */
  bool merge(int x, int y, T w) {
    if (is_same(x, y)) {
      if (diff(x, y) == w) {
        return true;
      } else {
        return false;
      }
    } else {
      w -= get_potential(y), w += get_potential(x);
      x = root(x), y = root(y);
      if (size(x) > size(y)) {
        std::swap(x, y);
        w = -w;
      }
      parent_[y] += parent_[x];
      parent_[x] = y;
      potential_[x] = -w;
      group_num_--;
      return true;
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  //   PotentializedUnionFind<ll> puf(N);
  //   rep(i, 0, M) {
  //     int u, v;
  //     ll w;
  //     cin >> u >> v >> w;
  //     u--;
  //     v--;
  //     puf.merge(u, v, w);
  //   }
  //   rep(i, 0, N) {
  //     cout << puf.get_potential(i) << ' ';
  //   }
  //   cout << endl;

  struct Edge {
    int to;
    ll weight;
    Edge() {}
    Edge(int t, ll w) : to(t), weight(w) {}
  };

  vector<vector<Edge>> G(N);
  rep(i, 0, M) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    u--;
    v--;
    G[u].emplace_back(v, w);
    G[v].emplace_back(u, -w);
  }

  const ll INF = 2e18;
  vector<ll> potential(N, INF);
  vector<bool> seen(N, false);
  auto dfs = [&](auto f, int u) -> void {
    seen[u] = true;
    for (auto [t, w] : G[u]) {
      if (seen[t]) {
        continue;
      }
      potential[t] = potential[u] + w;
      f(f, t);
    }
  };

  rep(i, 0, N) {
    if (seen[i]) {
      continue;
    }
    potential[i] = 0;
    dfs(dfs, i);
  }
  rep(i, 0, N) {
    cout << potential[i] << ' ';
  }
  cout << endl;
}