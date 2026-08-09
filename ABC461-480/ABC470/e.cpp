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
void pyes() { cout << "Yes" << endl; }
void pno() { cout << "No" << endl; }
void YN(bool x) { cout << (x ? "Yes" : "No") << endl; }
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
bool grid_check(int x, int y, int X, int Y) { return (0 <= x && x < X && 0 <= y && y < Y); }
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

// 1枚目のカードを選ぶ前に，既知のカードでペアができるなら，そのペアを取ればよい
// 2枚目のカードを選ぶ前に，1枚目のカードと既知のカードでペアができるなら，そのペアを取ればよい
// 既知のカードでペアを作ることができないなら，未知のカードを選ぶとしてよい
// f(L, C0, C1) := ライフがL, 未知のカードが2C0 + C1枚，そのうち相方が既知のカードがC1枚
//                今後獲得できるペアの個数の期待値
// 初期値 f(L, N, 0) -> 終了 f(0, C0, C1), f(l, 0, c1)
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, L;
  cin >> N >> L;
  vector<int> A(N);
  rep(i, 0, N) { cin >> A[i]; }

  map<tuple<int, int, int>, double> memo;

  auto dp = [&](auto f, int life, int aikata_unknown, int aikata_known) -> double {
    tuple<int, int, int> state(life, aikata_unknown, aikata_known);
    if (memo.contains(state)) {
      return memo[state];
    }
    if (life == 0) {
      return memo[state] = 0;
    }
    if (life > 0 && aikata_unknown == 0) {
      return memo[state] = aikata_known;
    }
    int remain = 2 * aikata_unknown + aikata_known;
    // 1枚目は未知のカードを選ぶ
    // 相方が既知のとき
    double p = (double)aikata_known / remain;
    // 相方が未知のとき
    // 二枚目にその相方を選ぶ
    double q = (1 - p) / (remain - 1);
    // 二枚目に相方が未知のカードを選ぶ
    double r = (1 - p) * 2 * (aikata_unknown - 1) / (remain - 1);
    // 二枚目に相方が既知のカードを選ぶ．このときライフが1以上なら次のターンにそのペアを選ぶ
    double s = (1 - p) * aikata_known / (remain - 1);

    double ex = 0;
    if (aikata_known > 0 && p > 0) {
      ex += p * (1 + f(f, life, aikata_unknown, aikata_known - 1));
    }
    if (aikata_unknown > 1 && r > 0) {
      ex += r * f(f, life - 1, aikata_unknown - 2, aikata_known + 2);
    }
    if (aikata_unknown > 0) {
      if (q > 0) {
        ex += q * (1 + f(f, life, aikata_unknown - 1, aikata_known));
      }
      if (s > 0) {
        if (life > 1) {
          ex += s * (1 + f(f, life - 1, aikata_unknown - 1, aikata_known));
        } else {
          ex += s * f(f, life - 1, aikata_unknown - 1, aikata_known);
        }
      }
    }
    return memo[state] = ex;
  };

  dp(dp, L, N, 0);

  double ex = memo[{L, N, 0}];
  int sum = 0;
  rep(i, 0, N) { sum += A[i]; }
  cout << fixed << setprecision(10) << (double)sum * ex / N << endl;
}