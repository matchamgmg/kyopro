#include <bits/stdc++.h>

/**
 * @brief (x - a)_+ := max(0, x - a), (a - x)_+ := max(0, a - x) として，
 * f(x) := \sum (x - a_i)_+ + \sum (a_j - x)_+ で表される関数を管理する
 */
template <long long INF = (long long)1e18> class SlopeTrick {
  long long min_f = 0;
  std::priority_queue<long long> L;
  std::priority_queue<long long, std::vector<long long>, std::greater<long long>> R;

public:
  SlopeTrick() {
    L.push(-INF);
    R.push(INF);
  }
  SlopeTrick(long long x) : min_f(x) {
    L.push(-INF);
    R.push(INF);
  }

  /**
   * @brief 関数fの最小値を与えるxのうち，最小のものx_0とその時の値f(x_0)を返す
   * @return {x_0, f(x_0)}
   */
  std::tuple<long long, long long> get_min() {
    return {L.top(), min_f};
  }

  /**
   * @brief 定数関数aを加算
   */
  void add_func_constant(long long a) {
    min_f += a;
  }

  /**
   * @brief (x - a)_+ を加算
   */
  void add_func_plus(long long a) {
    long long l0 = L.top();
    min_f += std::max(0LL, l0 - a);
    L.push(a);
    R.push(L.top());
    L.pop();
  }

  /**
   * @brief (a - x)_+ を加算
   */
  void add_func_minus(long long a) {
    long long r0 = R.top();
    min_f += std::max(0LL, a - r0);
    R.push(a);
    L.push(R.top());
    R.pop();
  }

  /**
   * @brief |x - a| = (x - a)_+ + (a - x)_+ を加算
   */
  void add_func_abs(long long a) {
    add_func_plus(a);
    add_func_minus(a);
  }
};

int main() {}