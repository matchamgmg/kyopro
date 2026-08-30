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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, K;
  cin >> N >> K;
  vector<int> A(2 * N);
  rep(i, 0, N) {
    cin >> A[i];
    A[i + N] = A[i];
  }
  ll NK = N * K;
  int M = 2e5 + 1;
  vector<vector<int>> cnt(2e5 + 1);
  rep(i, 0, 2 * N) {
    cnt[A[i]].emplace_back(i);
  }
  vector<ll> next_start(N), next_len(N);
  rep(i, 0, M) {
    rep(j, 0, cnt[i].size()) {
      if (cnt[i][j] >= N) {
        break;
      }
      next_start[cnt[i][j]] = cnt[i][j + 1] + 1;
    }
  }
  rep(i, 0, N) {
    next_len[i] = next_start[i] - i;
  }
  //   v_cout(next_start);
  //   v_cout(next_len);

  vector<vector<ll>> doubling, doubling_len;
  doubling.emplace_back(next_start);
  doubling_len.emplace_back(next_len);
  ll slen = next_len[0];
  ll spos = next_start[0];
  ll nibeki = 1;
  ll d = 0;
  while (nibeki < NK && slen < NK) {
    rep(i, 0, N) {
      next_start[i] = doubling[d][doubling[d][i] % N];
      next_len[i] = doubling_len[d][i] + doubling_len[d][doubling[d][i] % N];
    }
    doubling.emplace_back(next_start);
    doubling_len.emplace_back(next_len);
    nibeki <<= 1;
    d++;
    slen += next_len[spos % N];
    spos = next_start[spos % N];
  }
  ll D = d;

  // Xの末尾の最も近くで，sが空になったときの，開始位置
  ll pos = 0;
  ll len = 0;
  rrep(d, 0, D + 1) {
    ll npos = doubling[d][pos % N];
    ll nlen = len + doubling_len[d][pos % N];
    assert(nlen > 0);
    if (nlen > NK) {
      continue;
    }
    pos = npos;
    len = nlen;
  }
  vector<int> s;
  vector<bool> seen(M, false);
  while (len < NK) {
    if (seen[A[pos % N]]) {
      while (seen[A[pos % N]]) {
        seen[s.back()] = false;
        s.pop_back();
      }
    } else {
      s.emplace_back(A[pos % N]);
      seen[A[pos % N]] = true;
    }
    pos++;
    len++;
  }
  v_cout(s);
}