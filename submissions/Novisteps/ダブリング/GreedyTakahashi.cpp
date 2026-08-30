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

// あるバスに乗ったとき，次に乗るバスは一意に定まる
// バスについてのグラフを考える
// 時刻Zの直前にどのバスに乗っていたか -> 二分探索（ダブリング）

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, Q;
  cin >> N >> M >> Q;
  vector<int> A(M), B(M), S(M), T(M);
  rep(i, 0, M) {
    cin >> A[i] >> B[i] >> S[i] >> T[i];
    A[i]--;
    B[i]--;
  }
  // 都市ごとの時刻表
  vector<vector<pair<int, int>>> jikokuhyou(N);
  rep(i, 0, M) {
    jikokuhyou[A[i]].emplace_back(S[i], i);
  }
  rep(i, 0, N) {
    sort(all(jikokuhyou[i]));
  }
  // バスiに乗っているとき，次に乗るバス
  // 乗れるバスがないときはM
  vector<int> next_bus(M + 1);
  rep(i, 0, M) {
    int u = B[i];
    auto itr = lower_bound(all(jikokuhyou[u]), make_pair(T[i], -1));
    if (itr != jikokuhyou[u].end()) {
      next_bus[i] = (*itr).second;
    } else {
      next_bus[i] = M;
    }
  }
  next_bus[M] = M;
  // ダブリング
  vector<vector<int>> doubling;
  doubling.emplace_back(next_bus);
  ll nibeki = 1;
  ll d = 0;
  vector<int> n_bus(M + 1);
  while (nibeki < M) {
    rep(i, 0, M + 1) {
      n_bus[i] = doubling[d][doubling[d][i]];
    }
    doubling.emplace_back(n_bus);
    nibeki <<= 1;
    d++;
  }
  ll D = d;
  rep(q, 0, Q) {
    int X, Y, Z;
    cin >> X >> Y >> Z;
    Y--;
    // バスに一回でも乗るか
    auto itr = lower_bound(all(jikokuhyou[Y]), make_pair(X, -1));
    if (itr == jikokuhyou[Y].end()) {
      cout << Y + 1 << endl;
    } else {
      if ((*itr).first >= Z) {
        cout << Y + 1 << endl;
      } else {
        int u = (*itr).second;
        if (Z <= T[u]) {
          cout << A[u] + 1 << ' ' << B[u] + 1 << endl;
          continue;
        }
        rrep(d, 0, D + 1) {
          int v = doubling[d][u];
          if (v == M) {
            continue;
          } else if (T[v] >= Z) {
            continue;
          } else {
            u = v;
          }
        }
        int v = next_bus[u];
        if (v == M) {
          cout << B[u] + 1 << endl;
        } else if (Z <= S[v]) {
          cout << A[v] + 1 << endl;
        } else {
          cout << A[v] + 1 << ' ' << B[v] + 1 << endl;
        }
      }
    }
  }
}