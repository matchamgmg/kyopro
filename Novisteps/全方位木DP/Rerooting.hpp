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

template <class T, class V, class E, auto op, auto e, auto apply_vertex, auto apply_edge>
  requires rerooting_concept<T, V, E, op, e, apply_vertex, apply_edge>
class Rerooting {
  struct Edge {
    int to;
    E edge_weight;
  };

  T dfs1(int u = 0, int par = -1) {
    int s = G[u].size();
    dp[u].assign(s, e());
    T res = e();
    for (int i = 0; i < s; ++i) {
      const auto &[v, vu] = G[u][i];
      if (v == par) {
        continue;
      }
      dp[u][i] = apply_edge(dfs1(v, u), vu);
      res = op(res, dp[u][i]);
    }
    return apply_vertex(res, vertex_weight[u]);
  }

  void dfs2(int u = 0, int par = -1, const T &dp_par = e()) {
    int s = G[u].size();
    for (int i = 0; i < s; ++i) {
      const auto &[v, vu] = G[u][i];
      if (v == par) {
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
    ans[u] = apply_vertex(dp_left[s], vertex_weight[u]);

    for (int i = 0; i < s; ++i) {
      const auto &[v, _] = G[u][i];
      if (v == par) {
        continue;
      }
      dfs2(v, u, apply_vertex(op(dp_left[i], dp_right[i + 1]), vertex_weight[u]));
    }
  }

public:
  int n;
  std::vector<V> vertex_weight;
  std::vector<std::vector<Edge>> G;
  std::vector<std::vector<T>> dp;
  std::vector<T> ans;

  Rerooting(int n_)
      : n(n_), vertex_weight(std::vector<V>(n_)), G(std::vector<std::vector<Edge>>(n_)),
        dp(std::vector<std::vector<T>>(n_)), ans(std::vector<T>(n_)) {}
  Rerooting(const std::vector<V> &vec_)
      : n(vec_.size()), vertex_weight(vec_), G(std::vector<std::vector<Edge>>(vec_.size())),
        dp(std::vector<std::vector<T>>(vec_.size())), ans(std::vector<T>(vec_.size())) {}

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
    G[u].emplace_back(v, vu);
    G[v].emplace_back(u, uv);
  }

  void build() {
    dfs1();
    dfs2();
  }

  const std::vector<T> &get_answer() const {
    return ans;
  }

  const T &operator[](int pos) const {
    assert(0 <= pos && pos < n);
    return ans[pos];
  }
};