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

using namespace std;

int op(int a, int b) { return min(a, b); }

int main() {

  /* https://judge.yosupo.jp/problem/staticrmq */

  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    cin >> A[i];
  }
  SparseTable<int, op> sp(A);

  for (int q = 0; q < Q; ++q) {
    int l, r;
    cin >> l >> r;
    cout << sp.query(l, r) << endl;
  }
}