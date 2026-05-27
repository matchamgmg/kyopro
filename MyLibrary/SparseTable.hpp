/**
 * Sparse Table
 * 静的な列に対する区間クエリについて
 * 事前計算O(NlogN)、クエリごとO(1)
 *
 * 冪等モノイド
 * op(a \in S, a \in S) -> a
 * に対してのみ使える
 */

#include <bits/stdc++.h>

template <class T, auto op> struct SparseTable {
private:
  int n;
  std::vector<int> log_table;
  std::vector<std::vector<T>> table;

public:
  SparseTable(const std::vector<T> &v) : n((int)v.size()) {
    log_table.assign(n + 1, 0);

    for (int i = 2; i <= n; ++i) {
      log_table[i] = log_table[i / 2] + 1;
    }

    int K = log_table[n] + 1;
    table.assign(K, std::vector<T>(n));

    table[0] = v;

    for (int k = 1; k < K; ++k) {
      for (int i = 0; i + (1 << k) <= n; ++i) {
        table[k][i] = op(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  T query(int l, int r) const {
    int len = r - l;
    int k = log_table[len];

    return op(table[k][l], table[k][r - (1 << k)]);
  }
};