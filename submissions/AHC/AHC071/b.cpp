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

struct Problem {
  const int W = 60;
  const int H = 40;
  const int K = 120;
  array<int, 5> C;
  array<pair<int, int>, 120> Hole;
  void input() {
    int x;
    cin >> x >> x >> x;
    for (int &c : C) {
      cin >> c;
    }
    for (auto &p : Hole) {
      cin >> p.first >> p.second;
    }
  }
};

struct Answer {
  int cost;
  int bricks;
  vector<tuple<int, int, int>> actions;

  void print() {
    cout << bricks << '\n';
    for (auto &[x, y, l] : actions) {
      cout << x << ' ' << y << ' ' << l << '\n';
    }
    cout << flush;
  }
};

struct Frontier {
  int y;
  vector<int> Holes;
};

struct Leagal_action {
  int cost;
  vector<pair<int, int>> x_l;
};

struct Beam_state {
  int cost;
  vector<pair<int, int>> first_action;
  vector<int> holes;
  bool operator>(const Beam_state &rhs) const {
    if (cost == rhs.cost) {
      return holes.size() > rhs.holes.size();
    } else {
      return cost > rhs.cost;
    }
  }
  bool operator<(const Beam_state &rhs) const {
    if (cost == rhs.cost) {
      return holes.size() < rhs.holes.size();
    } else {
      return cost < rhs.cost;
    }
  }
};

struct Solver {
  Problem P;
  mt19937 mt;
  const int INF = 1e9;
  Solver() : mt(mt19937(0)) {
    P.input();
  }
  // f.Holes は昇順
  Leagal_action get_leagal_action(const Frontier &f, int Loop = 100) {
    int best_cost = INF;
    vector<pair<int, int>> best_action;
    for (int t = 0; t < Loop; ++t) {
      int cost = 0;
      vector<pair<int, int>> action;
      int r = -1;
      for (int hole_pos : f.Holes) {
        if (r >= P.W - 1) {
          continue;
        }
        if (hole_pos <= r) {
          continue;
        }
        int brick_len_half = mt() % min(5, (P.W - r - 2) / 2 + 1);
        int brick_len = brick_len_half * 2 + 1;
        int brick_pos = min(max(r + 1, hole_pos - (int)(mt() % (brick_len_half + 1))), P.W - brick_len);
        r = brick_pos + brick_len - 1;
        cost += P.C[brick_len_half];
        action.emplace_back(brick_pos, brick_len);
      }
      if (cost <= best_cost) {
        best_cost = cost;
        swap(best_action, action);
      }
    }
    return Leagal_action(best_cost, std::move(best_action));
  }

  // 元々の穴を列挙
  Frontier get_frontier(int y) {
    vector<int> frontier;
    for (auto &[x_, y_] : P.Hole) {
      if (y_ == y) {
        frontier.push_back(x_);
      }
    }
    sort(all(frontier));
    return Frontier(y, std::move(frontier));
  }

  // 元々の穴と新しい穴
  Frontier get_frontier(const Leagal_action &action, int y) {
    vector<int> frontier;
    for (auto &[x_, y_] : P.Hole) {
      if (y_ == y) {
        frontier.push_back(x_);
      }
    }
    for (auto &[pos, len] : action.x_l) {
      int hole_pos = pos + len / 2;
      frontier.push_back(hole_pos);
    }
    sort(all(frontier));
    frontier.erase(unique(all(frontier)), frontier.end());
    return Frontier(y, std::move(frontier));
  }

  // 乱択貪欲
  Answer greedy() {
    Answer ans;
    ans.cost = 0;
    ans.bricks = 0;
    int y = 39;
    Frontier f = get_frontier(y);
    Leagal_action a;
    for (; y > 0; --y) {
      a = get_leagal_action(f);
      for (auto &[x, l] : a.x_l) {
        ans.actions.emplace_back(x, y, l);
      }
      ans.cost += a.cost;
      ans.bricks += a.x_l.size();
      f = get_frontier(a, y - 1);
    }
    a = get_leagal_action(f);
    for (auto &[x, l] : a.x_l) {
      ans.actions.emplace_back(x, y, l);
    }
    ans.cost += a.cost;
    ans.bricks += a.x_l.size();
    return ans;
  }

  Leagal_action get_random_action(const Frontier &f) {
    int cost = 0;
    vector<pair<int, int>> action;
    int r = -1;
    for (int hole_pos : f.Holes) {
      if (r >= P.W - 1) {
        continue;
      }
      if (hole_pos <= r) {
        continue;
      }
      int brick_len_half = mt() % min(5, (P.W - r - 2) / 2 + 1);
      int brick_len = brick_len_half * 2 + 1;
      int brick_pos = min(max(r + 1, hole_pos - (int)(mt() % (brick_len_half + 1))), P.W - brick_len);
      r = brick_pos + brick_len - 1;
      cost += P.C[brick_len_half];
      action.emplace_back(brick_pos, brick_len);
    }
    return Leagal_action(cost, std::move(action));
  }

  Leagal_action get_leagal_beam_action(const Frontier &f, int y, int Loop = 300, int width = 50, int depth = 10) {
    depth = min(depth, y + 1);
    pq<Beam_state> que;
    int dep = 0;
    Beam_state state;
    Leagal_action action;
    Frontier frontier;
    for (int t = 0; t < Loop; ++t) {
      action = get_random_action(f);
      frontier = get_frontier(action, y - 1);
      que.emplace(action.cost, action.x_l, frontier.Holes);
      if (que.size() > width) {
        que.pop();
      }
    }
    ++dep;
    for (; dep < depth; ++dep) {
      pq<Beam_state> next_que;
      for (int w = 0; w < width; ++w) {
        if (que.empty()) {
          break;
        }
        auto [c, x_l, fron] = que.top();
        que.pop();
        for (int t = 0; t < Loop; ++t) {
          action = get_random_action(Frontier(y - dep, fron));
          frontier = get_frontier(action, y - 1 - dep);
          next_que.emplace(c + action.cost, x_l, frontier.Holes);
          if (next_que.size() > width) {
            next_que.pop();
          }
        }
      }
      swap(que, next_que);
    }
    assert(!que.empty());
    while (que.size() > 1) {
      que.pop();
    }
    auto [c, x_l, fron] = que.top();
    int cost = 0;
    for (auto &[_, l] : x_l) {
      cost += P.C[l / 2];
    }
    return Leagal_action(cost, std::move(x_l));
  }

  // ビームサーチ
  Answer beam_search() {
    Answer ans;
    ans.cost = 0;
    ans.bricks = 0;
    int y = 39;
    Frontier f = get_frontier(y);
    Leagal_action a;
    for (; y > 0; --y) {
      a = get_leagal_beam_action(f, y);
      for (auto &[x, l] : a.x_l) {
        ans.actions.emplace_back(x, y, l);
      }
      ans.cost += a.cost;
      ans.bricks += a.x_l.size();
      f = get_frontier(a, y - 1);
    }
    a = get_leagal_beam_action(f, y);
    for (auto &[x, l] : a.x_l) {
      ans.actions.emplace_back(x, y, l);
    }
    ans.cost += a.cost;
    ans.bricks += a.x_l.size();
    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  Answer ans = sol.beam_search();
  ans.print();
}