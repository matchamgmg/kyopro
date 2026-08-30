#include <bits/stdc++.h>

template <class T, auto op, auto inv, auto e, auto eq>
concept puf_group = requires(T a, T b) {
  { op(a, b) } -> std::same_as<T>;
  { inv(a) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
  { eq(a, b) } -> std::same_as<bool>;
};

/**
 * @brief 群をポテンシャルとして持つポテンシャル付きUnionFind
 * @tparam T ポテンシャルの型
 * @tparam op ポテンシャル同士の演算
 * @tparam inv 逆元を返す関数
 * @tparam e 単位元を返す関数
 * @tparam eq 等価判定用関数
 */
template <class T, auto op, auto inv, auto e, auto eq>
  requires puf_group<T, op, inv, e, eq>
class PotentializedUnionFind {
  int n_;
  int group_num_;
  std::vector<int> parent_;
  std::vector<T> potential_; ///< 各頂点から親へのポテンシャル

public:
  PotentializedUnionFind(int n)
      : n_(n), group_num_(n), parent_(std::vector<int>(n, -1)),
        potential_(std::vector<T>(n, e())) {}

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
    potential_[x] = op(potential_[x], potential_[parent_[x]]);
    return parent_[x] = r;
  }

  /**
   * @brief 頂点xから代表元へのポテンシャルを返す
   */
  T get_potential(int x) {
    root(x);
    return potential_[x];
  }

  /**
   * @brief x -> y へのポテンシャル
   * @pre is_same(x, y) == true
   */
  T diff(int x, int y) {
    return op(get_potential(x), inv(get_potential(y)));
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
      if (eq(diff(x, y), w)) {
        return true;
      } else {
        return false;
      }
    } else {
      T px = get_potential(x);
      T py = get_potential(y);
      w = op(inv(px), op(w, py));
      x = root(x), y = root(y);
      if (size(x) > size(y)) {
        std::swap(x, y);
        w = inv(w);
      }
      parent_[y] += parent_[x];
      parent_[x] = y;
      potential_[x] = w;
      group_num_--;
      return true;
    }
  }
};

using ll = long long;
struct S {
  ll x, y;
  S() : x(0), y(0) {}
  S(ll x, ll y) : x(x), y(y) {}
};

S op(S a, S b) {
  return {a.x + b.x, a.y + b.y};
}
S inv(S a) {
  return {-a.x, -a.y};
}
bool eq(S a, S b) {
  return a.x == b.x && a.y == b.y;
}
S e() {
  return S{};
}

int main() {
  int N = 10;
  PotentializedUnionFind<S, op, inv, e, eq> puf(N);
}