#include <bits/stdc++.h>

#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
// using mint = modint1000000007;
template <class T> using pq = priority_queue<T, vector<T>>; // 大きい順
template <class T>
using pq_g = priority_queue<T, vector<T>, greater<T>>; // 小さい順

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

const int INF = 1e9;
int op(int a, int b) {
  return min(a, b);
}
int e() {
  return INF;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, Q;
  cin >> N >> M >> Q;
  N++;
  vector<int> A(M), B(M), idx(M);
  rep(i, 0, M) {
    cin >> A[i] >> B[i];
    B[i]++;
    idx[i] = i;
  }
  sort(all(idx),
       [&](int i, int j) { return A[i] == A[j] ? B[i] < B[j] : A[i] < A[j]; });
  segtree<int, op, e> seg(N + 1);
  for (int i : idx) {
    int v = seg.get(A[i]);
    seg.set(A[i], min(v, B[i]));
  }
  // 開始日から貪欲にイベントを選んだ時の，そのイベントの終了日
  vector<int> shuryobi(N + 1);
  rep(i, 0, N + 1) {
    shuryobi[i] = seg.prod(i, N + 1);
  }
  // v_cout(shuryobi);
  vector<vector<int>> doubling;
  doubling.emplace_back(shuryobi);
  ll nibeki = 1;
  ll d = 0;
  vector<int> next_dp(N + 1);
  while (nibeki < M) {
    rep(i, 0, N + 1) {
      int v = doubling[d][i];
      if (v == INF) {
        v = N;
      }
      next_dp[i] = doubling[d][v];
    }
    doubling.emplace_back(next_dp);
    nibeki <<= 1;
    d++;
  }
  ll D = d;

  rep(q, 0, Q) {
    int L, R;
    cin >> L >> R;
    R++;
    int ans = 0;
    int day = L;
    rrep(d, 0, D + 1) {
      int nday = doubling[d][day];
      int nans = ans + (1 << d);
      if (nday > R) {
        continue;
      } else {
        day = nday;
        ans = nans;
      }
    }
    cout << ans << endl;
  }
}