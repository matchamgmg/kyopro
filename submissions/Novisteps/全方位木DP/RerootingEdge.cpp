#include <bits/stdc++.h>

/**
 * @brief 全方位木DPに必要な演算の制約
 * @note (T, op, e) はモノイドをなす必要がある
 *
 * @tparam T 乗せるデータの型
 * @tparam V 頂点重みの型
 * @tparam E 辺重みの型
 * @tparam op 結合律を満たす二項演算
 * @tparam e 二項演算の単位元を返す関数
 * @tparam apply_vertex 頂点重みを加える演算
 * @tparam apply_edge 辺重みを加える演算
 */
template <class T, class V, class E, auto op, auto e, auto apply_vertex, auto apply_edge>
concept rerooting_concept = requires(const T &a, const T &b, const V &vertex_weight, const E &edge_weight) {
  { op(a, b) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
  { apply_vertex(a, vertex_weight) } -> std::same_as<T>;
  { apply_edge(a, edge_weight) } -> std::same_as<T>;
};

/**
 * @brief 各辺についての答えを計算する全方位木DP
 */
template <class T, class V, class E, auto op, auto e, auto apply_vertex, auto apply_edge>
  requires rerooting_concept<T, V, E, op, e, apply_vertex, apply_edge>
class RerootingEdge {
  struct Edge {
    int edge_idx;
    int to;
    int rev_idx;
    E edge_weight;
  };

  T dfs1(int u = 0, int par_idx = -1) {
    int s = G[u].size();
    dp[u].assign(s, e());
    exclude[u].assign(s, e());
    T res = e();
    for (int i = 0; i < s; ++i) {
      if (i == par_idx) {
        continue;
      }
      const auto &[_, v, rev, vu] = G[u][i];
      dp[u][i] = apply_edge(dfs1(v, rev), vu);
      res = op(res, dp[u][i]);
    }

    res = apply_vertex(res, vertex_weight[u]);
    if (par_idx != -1) {
      exclude[u][par_idx] = res;
    }
    return res;
  }

  void dfs2(int u = 0, int par_idx = -1, const T &dp_par = e()) {
    int s = G[u].size();
    for (int i = 0; i < s; ++i) {
      const auto &[_, v, __, vu] = G[u][i];
      if (i == par_idx) {
        dp[u][i] = apply_edge(dp_par, vu);
      }
    }

    std::vector<T> dp_left(s + 1, e()), dp_right(s + 1, e());
    for (int i = 0; i < s; ++i) {
      dp_left[i + 1] = op(dp_left[i], dp[u][i]);
    }
    for (int i = s - 1; i >= 0; --i) {
      dp_right[i] = op(dp[u][i], dp_right[i + 1]);
    }

    for (int i = 0; i < s; ++i) {
      if (i == par_idx) {
        continue;
      }
      const auto &[e_idx, v, rev, _] = G[u][i];
      T ans_u = apply_vertex(op(dp_left[i], dp_right[i + 1]), vertex_weight[u]);
      T ans_v = exclude[v][rev];
      ans[e_idx] = op(ans_u, ans_v);
      dfs2(v, rev, ans_u);
    }
  }

public:
  int n;
  int edge_num = 0;
  std::vector<V> vertex_weight;
  std::vector<std::vector<Edge>> G;
  std::vector<std::vector<T>> dp;
  std::vector<std::vector<T>> exclude;
  std::vector<T> ans;

  RerootingEdge(int n_)
      : n(n_), vertex_weight(std::vector<V>(n_)), G(std::vector<std::vector<Edge>>(n_)),
        dp(std::vector<std::vector<T>>(n_)), exclude(std::vector<std::vector<T>>(n_)) {}
  RerootingEdge(const std::vector<V> &vec_)
      : n(vec_.size()), vertex_weight(vec_), G(std::vector<std::vector<Edge>>(vec_.size())),
        dp(std::vector<std::vector<T>>(vec_.size())), exclude(std::vector<std::vector<T>>(vec_.size())) {}

  void set_vertex_weight(const std::vector<V> &vec_) {
    assert(n == vec_.size());
    vertex_weight = vec_;
  }
  void set_vertex_weight(int pos, const V &v) {
    assert(0 <= pos && pos < n);
    vertex_weight[pos] = v;
  }
  // 内部的には逆辺の重みをもつ
  void add_edge(int u, int v, const E &uv, const E &vu) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    int u_idx = G[u].size();
    int v_idx = G[v].size();
    G[u].emplace_back(edge_num, v, v_idx, vu);
    G[v].emplace_back(edge_num, u, u_idx, uv);
    edge_num++;
  }

  void build() {
    ans.assign(edge_num, e());
    dfs1();
    dfs2();
  }

  const std::vector<T> &get_answer() const {
    return ans;
  }

  const T &operator[](int pos) const {
    assert(0 <= pos && pos < edge_num);
    return ans[pos];
  }
};

using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;
struct T {
  ll val;
  ll num_edges;
};

vector<vector<ll>> NCK;

T op(const T &a, const T &b) {
  return T{NCK[a.num_edges + b.num_edges][a.num_edges] * a.val % MOD * b.val % MOD, a.num_edges + b.num_edges};
}
T e() {
  return T{1, 0};
}
T apply_vertex(const T &a, ll x) {
  return a;
}
T apply_edge(const T &a, ll x) {
  return T{a.val, a.num_edges + x};
}

#define rep(i, s, n) for (int i = (s); i < (int)(n); i++)

int main() {
  int N;
  cin >> N;

  NCK.assign(N, vector<ll>(N, 0));
  rep(i, 0, N) {
    NCK[i][i] = 1;
  }
  rep(i, 1, N) {
    NCK[i][0] = 1;
    rep(j, 1, i) {
      NCK[i][j] = NCK[i - 1][j - 1] + NCK[i - 1][j];
      NCK[i][j] %= MOD;
    }
  }

  RerootingEdge<T, ll, ll, op, e, apply_vertex, apply_edge> rerooting(N);
  rep(i, 0, N - 1) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    rerooting.add_edge(a, b, 1, 1);
  }
  rep(i, 0, N) {
    rerooting.set_vertex_weight(i, 0);
  }

  rerooting.build();

  vector<T> res = rerooting.get_answer();
  ll ans = 0;
  for (auto &x : res) {
    ans += x.val;
    ans %= MOD;
  }
  cout << ans << endl;
}