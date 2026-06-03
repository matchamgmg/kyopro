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

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> B(N);
  rep(i, 0, N) {
    cin >> B[i];
    B[i]--;
  }
  vector<int> idx(N);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) { return B[i] < B[j]; });
  // カメラに映る星の集合の中で，最も暗い星を固定(星v)
  // カメラに映る最も暗い星が星vであるような集合の個数
  // = (星vより左にある星vより明るい星の個数+1) * (星vより右にある星vより明るい星の個数+1)

  fenwick_tree<ll> ft(N);
  ll ans = 0;
  ll cnt = 0;
  for (int i : idx) {
    ll left = ft.sum(0, i);
    ll right = cnt - left;
    ans += (left + 1) * (right + 1);
    ft.add(i, 1);
    cnt++;
  }
  cout << ans << endl;
}