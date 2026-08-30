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

bool less_concat(const string &a, const string &b) {
  string S1 = a + b;
  string S2 = b + a;
  int s = S1.size();
  int i = 0;
  while (i < s && S1[i] == S2[i]) {
    ++i;
  }
  if (i == s) {
    return false;
  }
  return S1[i] < S2[i];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  vector<string> S(N);
  rep(i, 0, N) {
    cin >> S[i];
  }
  vector<ll> value(N);
  rep(i, 0, N) {
    value[i] = stoll(S[i]);
  }

  vector<int> idx(N);
  iota(all(idx), 0);
  sort(all(idx), [&](int i, int j) {
    if (S[i].size() > S[j].size()) {
      return true;
    } else if (S[i].size() < S[j].size()) {
      return false;
    } else {
      return value[i] > value[j];
    }
  });

  string T1 = "";
  if (K < N) {
    vector<int> idx2(K - 1);
    rep(i, 0, K - 1) {
      idx2[i] = idx[i];
    }
    sort(rall(idx2), [&](int i, int j) { return less_concat(S[i], S[j]); });
    sort(idx.begin() + K, idx.end(), [&](int i, int j) { return value[i] > value[j]; });
    T1 = S[idx[K]];
    rep(i, 0, K - 1) {
      T1 += S[idx2[i]];
    }
    reverse(all(T1));
    while (T1.size() > 1 && T1.back() == '0') {
      T1.pop_back();
    }
    reverse(all(T1));
  }

  sort(idx.begin(), idx.begin() + K, [&](int i, int j) { return less_concat(S[i], S[j]); });
  string T2 = S[idx[K - 1]];
  rrep(i, 0, K - 1) {
    T2 += S[idx[i]];
  }
  reverse(all(T2));
  while (T2.size() > 1 && T2.back() == '0') {
    T2.pop_back();
  }
  reverse(all(T2));

  if (T1.size() == T2.size()) {
    if (less_concat(T1, T2)) {
      cout << T2 << endl;
    } else {
      cout << T1 << endl;
    }
  } else if (T1.size() < T2.size()) {
    cout << T2 << endl;
  } else {
    cout << T1 << endl;
  }
}