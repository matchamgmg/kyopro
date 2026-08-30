#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>;               // 大きい順
template <class T> using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int Q;
  cin >> Q;
  SlopeTrick st;
  rep(q, 0, Q) {
    int t;
    cin >> t;
    if (t == 1) {
      int a, b;
      cin >> a >> b;
      st.add_func_abs(a);
      st.add_func_constant(b);
    } else {
      auto [x, f] = st.get_min();
      cout << x << ' ' << f << endl;
    }
  }
}