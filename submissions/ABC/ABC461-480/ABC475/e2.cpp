/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

#include <atcoder/all>
#include <bits/stdc++.h>

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

  int N, M, K;
  cin >> N >> M >> K;
  string T;
  cin >> T;
  vector<string> S(N);
  rep(i, 0, N) {
    cin >> S[i];
  }
  rep(i, 0, N) {
    rep(j, 0, K) {
      S[i][j] = (S[i][j] == T[j] ? 'o' : 'x');
    }
  }

  struct Node {
    char c = '.';
    int left = -1;
    int right = -1;
    int ac = 0;
    int child_ac = 0;
  };

  vector<Node> Trie;
  int nodes = 0;
  Trie.emplace_back('.', -1, -1, 0, 0);
  nodes++;

  auto insert_string = [&](auto f, int d, int idx, const string &s) -> void {
    if (d == K) {
      Trie[idx].ac++;
      Trie[idx].child_ac++;
      return;
    }
    char c = s[d];
    if (c == 'o') {
      if (Trie[idx].left == -1) {
        Trie.emplace_back('o', -1, -1, 0, 0);
        Trie[idx].left = nodes;
        nodes++;
      }
      int l = Trie[idx].left;
      f(f, d + 1, l, s);
    } else {
      if (Trie[idx].right == -1) {
        Trie.emplace_back('x', -1, -1, 0, 0);
        Trie[idx].right = nodes;
        nodes++;
      }
      int r = Trie[idx].right;
      f(f, d + 1, r, s);
    }
    Trie[idx].child_ac++;
  };

  auto erase_string = [&](auto f, int d, int idx, const string &s) -> void {
    if (d == K) {
      Trie[idx].ac--;
      Trie[idx].child_ac--;
      return;
    }
    char c = s[d];
    if (c == 'o') {
      f(f, d + 1, Trie[idx].left, s);
    } else {
      f(f, d + 1, Trie[idx].right, s);
    }
    Trie[idx].child_ac--;
  };

  auto is_passed = [&](auto f, int d, int idx, const string &s, int left_ac, bool success) -> bool {
    if (d == K) {
      int rank = left_ac + Trie[idx].ac;
      return (rank <= M && success);
    }
    char c = s[d];
    if (c == 'o') {
      return f(f, d + 1, Trie[idx].left, s, left_ac, success | true);
    } else {
      if (Trie[idx].left != -1) {
        left_ac += Trie[Trie[idx].left].child_ac;
      }
      return f(f, d + 1, Trie[idx].right, s, left_ac, success | false);
    }
  };

  rep(i, 0, N) {
    insert_string(insert_string, 0, 0, S[i]);
  }
  int Q;
  cin >> Q;
  rep(q, 0, Q) {
    int i, j;
    cin >> i >> j;
    i--, j--;
    erase_string(erase_string, 0, 0, S[i]);
    S[i][j] = (S[i][j] == 'o' ? 'x' : 'o');
    insert_string(insert_string, 0, 0, S[i]);
    YN(is_passed(is_passed, 0, 0, S[i], 0, false));
  }
}