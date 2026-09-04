/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll N, K;
  cin >> N >> K;
  vector<ll> A(N);
  rep(i, 0, N) {
    cin >> A[i];
  }
  sort(all(A));
  if ((K + 1) % 2 == 0) {
    ll M = (K + 1) / 2;
    vector<ll> H(M), L(M);
    rep(i, 0, M) {
      L[i] = A[i];
    }
    rep(i, 0, M) {
      H[M - i - 1] = A[N - i - 1];
    }
    ll ans = 0;
    rep(i, 0, M) {
      ans += abs(H[i] - L[i]);
    }
    {
      ll res1 = 0;
      rep(i, 0, M - 1) {
        res1 += abs(H[i] - L[i + 1]);
      }
      ll res2 = 0;
      rep(i, 0, M - 1) {
        res2 += abs(L[i] - H[i + 1]);
      }
      ans += max(res1, res2);
    }
    cout << ans << endl;
  } else {
    ll M = K / 2;
    ll ans = 0;
    {
      vector<ll> H(M + 1), L(M);
      rep(i, 0, M) {
        L[i] = A[i];
      }
      rep(i, 0, M + 1) {
        H[M - i] = A[N - i - 1];
      }
      swap(H[1], H[M]);
      ll res = 0;
      rep(i, 0, M) {
        res += abs(H[i] - L[i]);
        res += abs(H[i + 1] - L[i]);
      }
      chmax(ans, res);
    }
    {
      vector<ll> H(M), L(M + 1);
      rep(i, 0, M + 1) {
        L[i] = A[i];
      }
      rep(i, 0, M) {
        H[M - i - 1] = A[N - i - 1];
      }
      swap(L[0], L[M - 1]);
      ll res = 0;
      rep(i, 0, M) {
        res += abs(H[i] - L[i]);
        res += abs(H[i] - L[i + 1]);
      }
      chmax(ans, res);
    }
    cout << ans << endl;
  }
}