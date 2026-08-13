#include <bits/stdc++.h>

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

int main() {}