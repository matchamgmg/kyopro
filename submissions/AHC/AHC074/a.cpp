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

constexpr const int N = 64, K = 192, P = 256, R = 1024;

struct Input {
  array<array<char, N>, P> A;
  Input() {
    input_init();
  }
  void input_init() {
    int x;
    cin >> x >> x >> x >> x;
    rep(i, 0, P) {
      rep(j, 0, N) {
        cin >> A[i][j];
      }
    }
  }
};

struct Output_barcode {
  array<array<char, N>, K> S;
  void output() {
    rep(i, 0, K) {
      rep(j, 0, N) {
        cout << S[i][j];
      }
      cout << '\n';
    }
    cout << flush;
  }
};

struct Action {
  int k, d;
};

struct Output_query {
  int m = 0;
  vector<Action> actions;
  void output() {
    cout << m << '\n';
    rep(i, 0, m) {
      cout << actions[i].k << ' ' << actions[i].d << '\n';
    }
    cout << flush;
  }
};

struct Solver {
  Input I;
  Solver() {}

  void set_simple_barcode(Output_barcode &barcode) {
    rep(i, 0, N) {
      rep(j, 0, N) {
        barcode.S[i][j] = '.';
      }
    }
    rep(i, 0, N) {
      barcode.S[i][i] = '1';
    }
    rep(i, N, K) {
      rep(j, 0, N) {
        barcode.S[i][j] = '0';
      }
    }
  }

  void solve_simple_barcode() {
    Output_barcode barcode;
    set_simple_barcode(barcode);
    barcode.output();
    struct Paper_stack {
      struct Paper {
        int a = -1;
        bool b = false;
        int l = -1;
        int r = -1;
      };
      array<Paper, N + 2> paper_list; // sentinel [N+1]
      int sentinel = N + 1;
      int white = N;
      Paper_stack() {
        rep(i, 0, N + 2) {
          paper_list[i].a = i;
          if (i > 0) {
            paper_list[i].l = i - 1;
          } else {
            paper_list[i].l = N + 1;
          }
          if (i < N + 1) {
            paper_list[i].r = i + 1;
          } else {
            paper_list[i].r = 0;
          }
        }
      }

      void check(const array<char, N> &target) {
        rep(i, 0, N) {
          if (target[i] == '1') {
            paper_list[i].b = true;
          } else {
            paper_list[i].b = false;
          }
        }
      }

      // d = N
      int move_black(int k) {
        int current = paper_list[sentinel].r;
        while (current != sentinel && paper_list[current].a != k) {
          current = paper_list[current].r;
        }
        int l = paper_list[current].l;
        int r = paper_list[current].r;
        paper_list[l].r = r;
        paper_list[r].l = l;
        paper_list[current].l = paper_list[sentinel].l;
        paper_list[current].r = sentinel;
        paper_list[paper_list[sentinel].l].r = current;
        paper_list[sentinel].l = current;

        white--;
        return N;
      }

      // k = N
      int move_white() {
        int d = N;
        int current = paper_list[sentinel].l;
        while (current != sentinel && paper_list[current].a != N && paper_list[current].b == false) {
          d--;
          current = paper_list[current].l;
        }
        if (white >= d) {
          return -1;
        }
        int l = paper_list[N].l;
        int r = paper_list[N].r;
        paper_list[l].r = r;
        paper_list[r].l = l;
        paper_list[N].l = current;
        paper_list[N].r = paper_list[current].r;
        paper_list[paper_list[current].r].l = N;
        paper_list[current].r = N;

        white = d;
        return d;
      }
    };

    Paper_stack PS;

    int a;
    array<char, N> target;
    rep(q, 0, R) {
      Output_query query;
      cin >> a;
      rep(i, 0, N) {
        target[i] = I.A[a][i];
      }
      PS.check(target);
      int d = PS.move_white();
      if (d != -1) {
        query.actions.emplace_back(N, d);
        query.m++;
      }
      while (true) {
        int current = PS.paper_list[PS.sentinel].r;
        while (current != PS.sentinel && PS.paper_list[current].a != N && PS.paper_list[current].b == true) {
          current = PS.paper_list[current].r;
        }
        int k = PS.paper_list[current].a;
        if (k >= N) {
          break;
        }
        int d = PS.move_black(k);
        query.actions.emplace_back(k, d);
        query.m++;
      }
      query.output();
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  sol.solve_simple_barcode();
}