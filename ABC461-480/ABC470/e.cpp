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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, L;
  cin >> N >> L;
  vector<int> A(N);
  rep(i, 0, N) { cin >> A[i]; }
  vector<int> B(N, 0); // 場所が既知の枚数
  vector<int> C(3, 0); // 既知の枚数 0, 1, 2
  C[2] = N;
  int unknown = 2 * N;

  map<tuple<int, int, vector<int>>, double> memo;
  auto f = [&](auto f, int now_score, int life) -> double {
    if (life == 0) {
      return memo[{now_score, life, B}] = 0;
    }
    // その行動をした結果将来得られるスコアの期待値
    double e = 0;
    if (C[2] > 0) {
      rep(i, 0, N) {
        if (B[i] == 2) {
          B[i] = -1;
          C[2]--;
          chmax(e, f(f, now_score + A[i], life));
          B[i] = 2;
          C[2]++;
        }
      }
    } else {
      rep(i, 0, N) {
        if (B[i] == 1) {
          double ex = 0;
          // 確率1/unknownでペア成立
          double p = (double)1 / (double)unknown;
          rep(j, 0, N) {
            if (j == i) {
              B[i] = -1;
              C[1]--;
              C[0]--;
              unknown--;
              ex += p * f(f, now_score + A[i], life);
              B[i] = 1;
              C[1]++;
              C[0]++;
              unknown++;
            } else if (B[j] == 0) {
              B[j]++;
              C[0]--;
              C[1]++;
              unknown--;
              ex += p * f(f, now_score, life - 1);
              B[j]--;
              C[0]++;
              C[1]--;
              unknown++;
            } else if (B[j] == 1) {
              B[j]++;
              C[1]--;
              C[2]++;
              unknown--;
              ex += p * f(f, now_score, life - 1);
              B[j]--;
              C[1]++;
              C[2]--;
              unknown++;
            }
          }
        } else {
        }
      }
    }
  };
}