/**
 * Disjoint Sparse Table
 * 任意の半群を乗せれる
 */

#include <bits/stdc++.h>

template <class T, T (*op)(T, T)> struct DisjointSparseTable {
private:
  int n;
  std::vector<T> A;
  std::vector<int> log_table;
  std::vector<std::vector<T>> table;

public:
  DisjointSparseTable(const std::vector<T> &v) : n((int)v.size()), A(v) {
    int K = 0;
    while ((1 << K) < n) {
      ++K;
    }
    table.resize(K);

    for (int k = 0; k < K; ++k) {
      table[k] = v;
      int w = 1 << k;
      for (int a = 0; a + w < n; a += 2 * w) {
        int b = a + 2 * w, c = a + w;
        b = std::min(b, n);
        for (int i = c - 2; i >= a; --i) {
          table[k][i] = op(v[i], table[k][i + 1]);
        }
        for (int i = c + 1; i < b; ++i) {
          table[k][i] = op(table[k][i - 1], v[i]);
        }
      }
    }

    log_table.resize(1 << K);
    for (int k = 0; k < K; ++k) {
      for (int i = (1 << k); i < (1 << (k + 1)); ++i) {
        log_table[i] = k;
      }
    }
  }

  T query(int l, int r) const {
    assert(0 <= l && l < r && r <= n);
    if (r == l + 1) {
      return A[l];
    }

    int k = log_table[l ^ (r - 1)];
    return op(table[k][l], table[k][r - 1]);
  }
};