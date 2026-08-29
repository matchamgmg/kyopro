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

int gcd(int a, int b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}

struct Enemy {
  int a;
  int b;
  int level;
};

struct Road {
  int i;
  int j;
};

struct Coord {
  int i;
  int j;
};

struct Answer {
  array<Road, 3> road;
  array<int, 10000> mt;
  ll danger = 100000000000;
};

vector<int> dx = {0, 1, 0, -1}, dy{1, 0, -1, 0};

struct Solver {
  const int N = 100;
  const int M = 3;
  const ll INF = 1000000;
  array<Road, 3> road;
  array<Enemy, 10000> enemy;

  void input() {
    int a, b;
    rep(i, 0, N * N) {
      cin >> a >> b;
      enemy[i] = {a, b, (int)floor(sqrt(i + 1 + 1e-12))};
    }
  }

  void decide_road(int x, int y, int z) {
    road[0] = {3 * z % 100, z};
    road[1] = {(y + 3 * z) % 100, (2 * y + z) % 100};
    road[2] = {0, x};
  }

  void decide_road_transpose(int x, int y, int z) {
    road[0] = {z, 3 * z % 100};
    road[1] = {(2 * y + z) % 100, (y + 3 * z) % 100};
    road[2] = {x, 0};
  }

  Solver() {
    input();
  }

  ll calc_dist(const Enemy &e, const Coord &f) {
    return ll(abs(e.a - f.i) + abs(e.b - f.j));
  }

  Answer solve(int x, int y, int z, bool transpose) {
    Answer ans;
    if (transpose) {
      decide_road_transpose(x, y, z);
    } else {
      decide_road(x, y, z);
    }
    ans.road = road;
    ll danger = 0;
    Coord current{0, 0};
    array<Coord, 10000> fuda;
    array<bool, 10000> is_exist{false};
    rep(t, 0, N * N) {
      if (t % 20 == 0) {
        if (t % 2000 == 0) {
          current.i += road[2].i;
          current.j += road[2].j;
          ans.mt[t] = 2;
        } else {
          current.i += road[1].i;
          current.j += road[1].j;
          ans.mt[t] = 1;
        }
      } else {
        current.i += road[0].i;
        current.j += road[0].j;
        ans.mt[t] = 0;
      }

      if (current.i >= N) {
        current.i -= N;
      }
      if (current.j >= N) {
        current.j -= N;
      }
      fuda[t] = current;
      is_exist[current.i * 100 + current.j] = true;

      ll d = INF;
      if (t >= 4000) {
        if (!is_exist[enemy[t].a * 100 + enemy[t].b]) {
          danger += enemy[t].level;
        }
      } else if (t >= 2000) {
        if (!is_exist[enemy[t].a * 100 + enemy[t].b]) {
          if (current.i == 0 || current.i == 99 || current.j == 0 || current.j == 99) {
            bool ok = false;
            rep(d, 0, 4) {
              int nx = current.i + dx[d];
              int ny = current.j + dy[d];
              if (!grid_check(nx, ny, 100, 100)) {
                continue;
              }
              ok |= is_exist[enemy[t].a * 100 + enemy[t].b];
            }
            danger += (2 - (int)ok) * enemy[t].level;
          } else {
            danger += enemy[t].level;
          }
        }
      } else {
        rep(k, 0, t + 1) {
          chmin(d, calc_dist(enemy[t], fuda[k]));
        }
        danger += d * enemy[t].level;
      }
    }

    ans.danger = danger;
    return ans;
  }

  Answer road_search() {
    Answer best_answer;
    Answer ans;
    vector<int> Z = {3, 7, 9, 11, 13, 17, 19, 21, 23, 27, 29, 31, 33, 37};
    for (int z : Z) {
      rep(y, 1, 100) {
        if (gcd(y, 100) != 1) {
          continue;
        }
        if (gcd(y, z) != 1) {
          continue;
        }
        rep(x, 1, 5) {
          ans = solve(x, y, z, false);
          if (best_answer.danger > ans.danger) {
            best_answer = ans;
          }
          ans = solve(x, y, z, true);
          if (best_answer.danger > ans.danger) {
            best_answer = ans;
          }
        }
      }
    }

    return best_answer;
  }
};

void print_answer(const Answer &ans) {
  rep(i, 0, 3) {
    cout << ans.road[i].i << ' ' << ans.road[i].j << '\n';
  }
  rep(i, 0, 10000) {
    cout << ans.mt[i] << '\n';
  }
  cout << flush;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  Solver solver;
  Answer ans = solver.road_search();
  print_answer(ans);
  cerr << ans.danger << endl;
}