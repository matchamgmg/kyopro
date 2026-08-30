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

  int N, Q;
  cin >> N >> Q;
  vector<int> Pigeon_to_box(N), Box_to_label(N), Label_to_box(N);
  rep(i, 0, N) { Pigeon_to_box[i] = Box_to_label[i] = Label_to_box[i] = i; }
  int t, a, b;
  rep(q, 0, Q) {
    cin >> t;
    if (t == 1) {
      cin >> a >> b;
      a--;
      b--;
      Pigeon_to_box[a] = Label_to_box[b];
    } else if (t == 2) {
      cin >> a >> b;
      a--;
      b--;
      swap(Box_to_label[Label_to_box[a]], Box_to_label[Label_to_box[b]]);
      swap(Label_to_box[a], Label_to_box[b]);
    } else {
      cin >> a;
      a--;
      cout << Box_to_label[Pigeon_to_box[a]] + 1 << endl;
    }
  }
}