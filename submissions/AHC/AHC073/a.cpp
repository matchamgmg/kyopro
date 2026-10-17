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

class TimeKeeper {
private:
  std::chrono::high_resolution_clock::time_point start_time_;
  int64_t time_threshold_;

public:
  TimeKeeper() {};

  TimeKeeper(const int64_t &time_threshold)
      : start_time_(std::chrono::high_resolution_clock::now()), time_threshold_(time_threshold) {}

  bool isTimeOver() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;

    return duration_cast<milliseconds>(diff).count() >= time_threshold_;
  }

  auto get_time() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
    return duration_cast<milliseconds>(diff).count();
  }
};

struct Point {
  int x, y;
  bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  bool inner_grid() const {
    return 0 <= x && x < 128 && 0 <= y && y < 128;
  }
};

struct Problem {
  int N, M, K, T;
  Point s;
  array<Point, 150> G;
  Problem() {
    input();
  }
  void input() {
    cin >> N >> M >> K >> T;
    cin >> s.x >> s.y;
    for (int i = 0; i < 150; ++i) {
      cin >> G[i].x >> G[i].y;
    }
  }
};

array<char, 4> UDLR = {'U', 'D', 'L', 'R'};
array<int, 4> dx = {-1, 1, 0, 0};
array<int, 4> dy = {0, 0, -1, 1};

struct Action0 {
  char c;
};

struct Action1 {
  Point p;
};

struct Answer {
  int A;
  array<Point, 3> Circles;
  vector<int> Action_type;
  vector<Action0> action0;
  vector<Action1> action1;

  void print() {
    for (Point &p : Circles) {
      cout << p.x << ' ' << p.y << '\n';
    }
    int cnt0 = 0;
    int cnt1 = 0;
    for (int t : Action_type) {
      if (t == 0) {
        cout << action0[cnt0++].c << '\n';
      } else {
        cout << 'J' << ' ' << action1[cnt1].p.x << ' ' << action1[cnt1].p.y << '\n';
        ++cnt1;
      }
    }
    cout << flush;
  }
};

struct Compressed_answer {
  int A;
  array<Point, 3> Circles;
  vector<int> Action_type;
  vector<Point> next_goal;
};

const int INF = 1e9;

struct Solver {
  Problem P;
  mt19937 mt = mt19937{0};
  TimeKeeper tk = TimeKeeper{1950};

  void set_available_area(array<array<bool, 128>, 128> &available_area) {
    for (int i = 0; i < 42; ++i) {
      for (int j = 0; j < 42; ++j) {
        available_area[i + 43][j + 43] = true;
      }
    }
  }

  void set_random_circles(Compressed_answer &cans) {
    do {
      for (Point &p : cans.Circles) {
        p.x = mt() % 42 + 43;
        p.y = mt() % 42 + 43;
      }
    } while (cans.Circles[0] == cans.Circles[1] || cans.Circles[1] == cans.Circles[2] ||
             cans.Circles[2] == cans.Circles[0]);
  }

  void set_dist(array<array<array<int, 128>, 128>, 150> &dist) {
    queue<Point> que;
    Point u, v;
    for (int m = 0; m < 150; ++m) {
      dist[m][P.G[m].x][P.G[m].y] = 0;
      que.push(P.G[m]);
      while (!que.empty()) {
        u = que.front();
        que.pop();
        int ud = dist[m][u.x][u.y];
        for (int d = 0; d < 4; ++d) {
          v.x = u.x + dx[d];
          v.y = u.y + dy[d];
          if (!v.inner_grid()) {
            continue;
          }
          if (dist[m][v.x][v.y] != INF) {
            continue;
          }
          dist[m][v.x][v.y] = ud + 1;
          que.push(v);
        }
      }
    }
  }

  Point warp(Point &p1, Point &cir) {
    return Point{cir.x * 2 - p1.x, cir.y * 2 - p1.y};
  }

  void encode_moving(Answer &ans, Point &p1, Point &p2) {
    Point p = p1;
    while (p.x < p2.x) {
      ans.Action_type.push_back(0);
      ans.action0.emplace_back('D');
      p.x++;
    }
    while (p.x > p2.x) {
      ans.Action_type.push_back(0);
      ans.action0.emplace_back('U');
      p.x--;
    }
    while (p.y < p2.y) {
      ans.Action_type.push_back(0);
      ans.action0.emplace_back('R');
      p.y++;
    }
    while (p.y > p2.y) {
      ans.Action_type.push_back(0);
      ans.action0.emplace_back('L');
      p.y--;
    }
  }

  void encode_jumping(Answer &ans, Point &cir) {
    ans.Action_type.push_back(1);
    ans.action1.emplace_back(cir);
  }

  void encode_cans(Answer &ans, Compressed_answer &cans) {
    ans.Circles = cans.Circles;
    Point current = P.s;
    int siz = cans.Action_type.size();
    for (int i = 0; i < siz; ++i) {
      if (cans.Action_type[i] == 0) {
        encode_moving(ans, current, cans.next_goal[i]);
        current = cans.next_goal[i];
      } else {
        encode_jumping(ans, cans.next_goal[i]);
        current = warp(current, cans.next_goal[i]);
      }
    }
    ans.A = ans.Action_type.size();
  }

  Answer solve_inner_area_greedy() {
    Answer ans;
    Compressed_answer best_cans;
    best_cans.A = INF;
    int A;
    // array<array<bool, 128>, 128> available_area{false};
    // set_available_area(available_area);
    // array<array<array<int, 128>, 128>, 150> dist{INF};
    // set_dist(dist);
    while (!tk.isTimeOver()) {
      Compressed_answer cans;
      A = 0;
      set_random_circles(cans);
      array<Point, 3> Circles = cans.Circles;
      Point current = P.s;
      for (int m = 0; m < 150; ++m) {
        Point goal = P.G[m];
        // 直接向かう
        int direct_approarch = abs(current.x - goal.x) + abs(current.y - goal.y);
        // 魔法陣を一回経由
        int warp_approarch = INF;
        int best_circle = -1;
        Point best_point = {-1, -1};
        int score;
        auto calc_score = [&](Point &p1, Point &p, Point &cir, Point &p2) -> int {
          int res = 0;
          res += abs(p1.x - p.x) + abs(p1.y - p.y);
          Point pp = warp(p, cir);
          res += abs(pp.x - p2.x) + abs(pp.y - p2.y);
          return res + 1;
        };
        for (int k = 0; k < 3; ++k) {
          for (int i = 0; i < 42; ++i) {
            for (int j = 0; j < 42; ++j) {
              Point p = Point{i + 43, j + 43};
              score = calc_score(current, p, Circles[k], goal);
              if (score <= warp_approarch) {
                best_circle = k;
                best_point = p;
                warp_approarch = score;
              }
            }
          }
        }

        if (direct_approarch <= warp_approarch) {
          A += direct_approarch;
          cans.Action_type.push_back(0);
          cans.next_goal.push_back(goal);
        } else {
          A += warp_approarch;
          cans.Action_type.push_back(0);
          cans.next_goal.push_back(best_point);
          cans.Action_type.push_back(1);
          cans.next_goal.push_back(Circles[best_circle]);
          cans.Action_type.push_back(0);
          cans.next_goal.push_back(goal);
          Circles[best_circle] = best_point;
        }

        current = goal;
      }
      cans.A = A;
      if (cans.A <= best_cans.A) {
        swap(best_cans, cans);
      }
    }
    encode_cans(ans, best_cans);
    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  Answer ans = sol.solve_inner_area_greedy();
  ans.print();
}