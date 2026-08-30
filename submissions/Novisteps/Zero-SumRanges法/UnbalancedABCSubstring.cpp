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

  ll N;
  cin >> N;
  string S;
  cin >> S;
  vector<vector<int>> ABC(3, vector<int>(N));
  rep(i, 0, N) {
    rep(j, 0, 3) {
      ABC[j][i] = (j == S[i] - 'A');
    }
  }
  vector<ll> AB(2 * N + 1, 0), BC(2 * N + 1, 0), CA(2 * N + 1);
  AB[N]++;
  BC[N]++;
  CA[N]++;
  ll ab_eq = 0, bc_eq = 0, ca_eq = 0;
  ll abc_eq = 0;
  map<pair<int, int>, ll> mp;
  mp[{0, 0}]++;
  ll ab = 0, bc = 0, ca = 0;
  rep(i, 0, N) {
    ab += ABC[1][i] - ABC[0][i];
    bc += ABC[2][i] - ABC[1][i];
    ca += ABC[0][i] - ABC[2][i];
    ab_eq += AB[ab + N];
    bc_eq += BC[bc + N];
    ca_eq += CA[ca + N];
    abc_eq += mp[{ab, bc}];
    AB[ab + N]++;
    BC[bc + N]++;
    CA[ca + N]++;
    mp[{ab, bc}]++;
  }
  cout << N * (N + 1) / 2 - (ab_eq + bc_eq + ca_eq - 2 * abc_eq) << endl;
}