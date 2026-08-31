#include <bits/stdc++.h>

template <class T, auto op, auto inv, auto e>
concept BIT_concept = requires(const T &a, const T &b) {
  { op(a, b) } -> std::same_as<T>;
  { inv(a) } -> std::same_as<T>;
  { e() } -> std::same_as<T>;
};

/**
 * @brief 可換群上の fenwick_tree
 */
template <class T, auto op, auto inv, auto e>
  requires BIT_concept<T, op, inv, e>
class BIT {
  int n;
  std::vector<T> data;

public:
  BIT(int n_) : n(n_), data(std::vector<T>(n_ + 1, e())) {}
  BIT(int n_, const T &x) : n(n_), data(std::vector<T>(n_ + 1, e())) {
    for (int i = 1; i <= n; ++i) {
      data[i] = op(data[i], x);
      int j = i + (i & -i);
      if (j <= n) {
        data[j] = op(data[j], data[i]);
      }
    }
  }
  BIT(const std::vector<T> &vec) : n(vec.size()), data(std::vector<T>(vec.size() + 1, e())) {
    for (int i = 1; i <= n; ++i) {
      data[i] = op(data[i], vec[i - 1]);
      int j = i + (i & -i);
      if (j <= n) {
        data[j] = op(data[j], data[i]);
      }
    }
  }

  T prefix_sum(int r) const {
    assert(0 <= r && r <= n);
    T res = e();
    for (; r > 0; r -= (r & -r)) {
      res = op(data[r], res);
    }
    return res;
  }

  T sum(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    return op(inv(prefix_sum(l)), prefix_sum(r));
  }

  T get(int p) const {
    assert(0 <= p && p < n);
    return sum(p, p + 1);
  }

  void add(int p, const T &x) {
    assert(0 <= p && p < n);
    for (++p; p <= n; p += (p & -p)) {
      data[p] = op(data[p], x);
    }
  }

  void set(int p, const T &x) {
    assert(0 <= p && p < n);
    T prev = get(p);
    add(p, op(inv(prev), x));
  }

  /**
   * @brief `f(prefix_sum(r)) == true` となる最大の r (0 <= r <= n) を返す
   * @pre rに関してf(prefix_sum(r))は true -> false の単調性を持つ
   * @pre f(e()) == true
   */
  template <class F> int max_right(F f) const {
    assert(f(e()));
    int res = 0;
    T val = e();
    for (int k = std::bit_floor(static_cast<unsigned>(n)); k > 0; k >>= 1) {
      if (k + res <= n) {
        T nxt = op(val, data[k + res]);
        if (f(nxt)) {
          val = std::move(nxt);
          res += k;
        }
      }
    }
    return res;
  }
};