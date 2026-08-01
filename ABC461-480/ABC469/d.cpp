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

  int N, M;
  cin >> N >> M;
  vector<int> A(M), B(M);
  rep(i, 0, M) { cin >> A[i] >> B[i]; }
  rep(i, 0, M) {
    A[i]--;
    B[i]--;
  }
  int ans = 0;
  int a1 = A[0];
  vector<int> C1(N, 0);
  rep(i, 0, M) {
    if (A[i] == a1 || B[i] == a1) {
      C1[a1]++;
    } else {
      C1[A[i]]++;
      C1[B[i]]++;
    }
  }
  rep(i, 0, N) {
    if (i == a1) {
      continue;
    }
    if (C1[i] + C1[a1] >= M) {
      ans++;
    }
  }

  int a2 = B[0];
  vector<int> C2(N, 0);
  rep(i, 0, M) {
    if (A[i] == a2 || B[i] == a2) {
      C2[a2]++;
    } else {
      C2[A[i]]++;
      C2[B[i]]++;
    }
  }
  rep(i, 0, N) {
    if (i == a2) {
      continue;
    }
    if (C2[i] + C2[a2] >= M) {
      ans++;
    }
  }
  int cnt = 0;
  rep(i, 0, M) {
    cnt += (A[i] == a1 || A[i] == a2 || B[i] == a1 || B[i] == a2);
  }
  if (cnt >= M) {
    ans--;
  }
  cout << ans << endl;
}