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

// N <= 2e5 の場合の解法
// 頂点vから2^dだけ進んだ頂点
// 頂点vから2^dだけ進んだスコアの総和
// 頂点vから2^dまで進んだ中でのスコアの最大値

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, K;
  cin >> N >> K;
  vector<ll> P(N), C(N);
  rep(i, 0, N) {
    cin >> P[i];
    P[i]--;
  }
  rep(i, 0, N) {
    cin >> C[i];
  }

  vector<vector<ll>> next, score, max_score;
  next.emplace_back(P);
  score.emplace_back(C);
  max_score.emplace_back(C);

  ll nibeki = 1;
  ll d = 0;
  vector<ll> next_next(N), next_score(N), next_max_score(N);
  while (nibeki < K) {
    rep(i, 0, N) {
      next_next[i] = next[d][next[d][i]];
      next_score[i] = score[d][i] + score[d][next[d][i]];
      next_max_score[i] =
          max(max_score[d][i], score[d][i] + max_score[d][next[d][i]]);
    }
    next.emplace_back(next_next);
    score.emplace_back(next_score);
    max_score.emplace_back(next_max_score);
    nibeki <<= 1;
    d++;
  }

  const ll INF = 9e18;
  ll ans = -INF;
  rep(i, 0, N) {
    ll k = K;
    d = 0;
    ll u = i;
    ll sc = 0;
    ll msc = -INF;
    while (k) {
      if (k & 1) {
        msc = max(msc, sc + max_score[d][u]);
        sc += score[d][u];
        u = next[d][u];
      }
      k >>= 1;
      d++;
    }
    chmax(ans, msc);
  }
  cout << ans << endl;
}