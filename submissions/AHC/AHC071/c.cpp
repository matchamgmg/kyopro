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
  double score;
  int y;
  int cost;
  int bricks;
  vector<pair<int, int>> first_action;
  vector<int> holes;
  Beam_state() {}
  Beam_state(int y_, int cost_, int bricks_, const vector<pair<int, int>> &fa_, const vector<int> &holes_)
      : y(y_), cost(cost_), bricks(bricks_), first_action(fa_), holes(holes_) {
    score = bricks * ((double)(y + 1) / 80) + 2 * cost * ((double)(40 - y + 1) / 40);
  }
  //   bool operator<(const Beam_state &rhs) const {
  //     if (bricks == rhs.bricks) {
  //       return cost < rhs.cost;
  //     } else {
  //       return bricks < rhs.bricks;
  //     }
  //   }
  //   bool operator<(const Beam_state &rhs) const {
  //     if (cost == rhs.cost) {
  //       return bricks < rhs.bricks;
  //     } else {
  //       return cost < rhs.cost;
  //     }
  //   }
  bool operator<(const Beam_state &rhs) const {
    return score < rhs.score;
  }
};

pair<int, int> operator+(const pair<int, int> &a, const pair<int, int> &b) {
  return {a.first + b.first, a.second + b.second};
}

struct Solver {
  Problem P;
  mt19937 mt;
  const int INF = 1e9;
  Solver() : mt(mt19937(0)) {
    P.input();
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

  // ブロック数最小かつその中でコスト最小のブロック配置を求める
  Leagal_action get_dp3_action(const Frontier &f) {
    vector<int> holes(P.W + 1, 0);
    for (int hole_pos : f.Holes) {
      holes[hole_pos + 1]++;
    }
    for (int i = 0; i < P.W; ++i) {
      holes[i + 1] += holes[i];
    }
    vector<pair<pair<int, int>, pair<int, int>>> dp(P.W + 1, {{INF, INF}, pair<int, int>(-1, -1)});
    dp[0].first = {0, 0};
    for (int i = 0; i < P.W; ++i) {
      if (holes[i + 1] - holes[i] == 0) {
        dp[i + 1].first = min(dp[i + 1].first, dp[i].first);
      }
      for (int half = 0; half < 5; ++half) {
        int len = half * 2 + 1;
        if (i - len + 1 >= 0) {
          if (holes[i + 1] - holes[i + 1 - len] > 0 &&
              dp[i + 1].first >= dp[i - len + 1].first + make_pair(1, P.C[half])) {
            dp[i + 1].first = dp[i - len + 1].first + make_pair(1, P.C[half]);
            dp[i + 1].second = {i - len + 2, len};
          }
        }
      }
    }
    int cost = 0;
    vector<pair<int, int>> action;
    int i = P.W;
    while (i > 0) {
      if (dp[i].second.first == -1) {
        --i;
      } else {
        action.emplace_back(dp[i].second.first - 1, dp[i].second.second);
        cost += P.C[dp[i].second.second / 2];
        i = dp[i].second.first - 1;
      }
    }
    return Leagal_action(cost, std::move(action));
  }

  Answer dp() {
    Answer ans;
    ans.cost = 0;
    ans.bricks = 0;
    int y = 39;
    Frontier f = get_frontier(y);
    Leagal_action a;
    for (; y > 0; --y) {
      a = get_dp3_action(f);
      for (auto &[x, l] : a.x_l) {
        ans.actions.emplace_back(x, y, l);
      }
      ans.cost += a.cost;
      ans.bricks += a.x_l.size();
      f = get_frontier(a, y - 1);
    }
    a = get_dp3_action(f);
    for (auto &[x, l] : a.x_l) {
      ans.actions.emplace_back(x, y, l);
    }
    ans.cost += a.cost;
    ans.bricks += a.x_l.size();
    return ans;
  }

  Leagal_action get_near_pos_action(const Leagal_action &prev_action, const Frontier &f) {
    int n = prev_action.x_l.size();
    if (n == 0) {
      return prev_action;
    }
    Leagal_action action = prev_action;
    vector<int> a(P.W + 1, 0);
    int r = mt() % n;
    int d = mt() % 2;
    int l = action.x_l[r].second;
    action.x_l[r].first = min(max(0, action.x_l[r].first + (1 - 2 * d)), P.W - l);
    for (auto &[x_, l_] : action.x_l) {
      a[x_]++;
      a[x_ + l_]--;
    }
    bool ok = true;
    for (int i = 0; i < P.W; ++i) {
      a[i + 1] += a[i];
    }
    for (int i = 0; i < P.W + 1; ++i) {
      ok &= (a[i] <= 1);
    }
    for (int hole_pos : f.Holes) {
      ok &= (a[hole_pos] >= 1);
    }
    if (ok) {
      return action;
    } else {
      return prev_action;
    }
  }

  Leagal_action get_near_len_action(const Leagal_action &prev_action, const Frontier &f) {
    int n = prev_action.x_l.size();
    if (n == 0) {
      return prev_action;
    }
    Leagal_action action = prev_action;
    vector<int> a(P.W + 1, 0);
    int r = mt() % n;
    int lr = mt() % 2;
    int l = action.x_l[r].second + (2 - 4 * lr);
    if (!(1 <= l && l <= 9)) {
      return prev_action;
    }
    if (action.x_l[r].first - (1 - 2 * lr) + l > P.W) {
      return prev_action;
    }
    action.x_l[r].first -= (1 - 2 * lr);
    if (action.x_l[r].first < 0) {
      return prev_action;
    }
    action.x_l[r].second = l;

    for (auto &[x_, l_] : action.x_l) {
      a[x_]++;
      a[x_ + l_]--;
    }
    bool ok = true;
    for (int i = 0; i < P.W; ++i) {
      a[i + 1] += a[i];
    }
    for (int i = 0; i < P.W + 1; ++i) {
      ok &= (a[i] <= 1);
    }
    for (int hole_pos : f.Holes) {
      ok &= (a[hole_pos] >= 1);
    }
    if (ok) {
      return action;
    } else {
      return prev_action;
    }
  }

  Leagal_action get_leagal_beam_action(const Frontier &f, int y, int Loop = 9, int width = 32, int depth = 10) {
    depth = min(depth, y + 1);
    pq<Beam_state> que;
    int dep = 0;
    Beam_state state;
    Leagal_action action;
    Frontier frontier;
    vector<Leagal_action> action_list;
    int actions = 0;
    map<vector<int>, double> mp;

    action = get_dp3_action(f);
    frontier = get_frontier(action, y - dep - 1);
    state = Beam_state(y - dep, action.cost, action.x_l.size(), action.x_l, frontier.Holes);
    mp[frontier.Holes] = state.score;
    action_list.emplace_back(action);
    actions++;
    que.emplace(state);
    for (int t = 0; t < Loop; ++t) {
      int r = mt() % actions;
      action = get_near_len_action(action_list[r], f);
      frontier = get_frontier(action, y - dep - 1);
      state = Beam_state(y - dep, action.cost, action.x_l.size(), action.x_l, frontier.Holes);
      if (mp.contains(frontier.Holes)) {
        if (mp[frontier.Holes] <= state.score) {
          continue;
        }
      }
      mp[frontier.Holes] = state.score;
      action_list.emplace_back(action);
      actions++;
      que.emplace(state);
      if (que.size() > width) {
        que.pop();
      }
    }
    for (int t = 0; t < 9 * Loop; ++t) {
      int r = mt() % actions;
      action = get_near_pos_action(action_list[r], f);
      frontier = get_frontier(action, y - dep - 1);
      state = Beam_state(y - dep, action.cost, action.x_l.size(), action.x_l, frontier.Holes);
      if (mp.contains(frontier.Holes)) {
        if (mp[frontier.Holes] <= state.score) {
          continue;
        }
      }
      mp[frontier.Holes] = state.score;
      action_list.emplace_back(action);
      actions++;
      que.emplace(state);
      if (que.size() > width) {
        que.pop();
      }
    }
    ++dep;
    for (; dep < depth; ++dep) {
      mp.clear();
      pq<Beam_state> next_que;
      while (!que.empty()) {
        action_list.clear();
        actions = 0;
        auto [__, _, c, b, x_l, fron] = que.top();
        que.pop();

        action = get_dp3_action(Frontier(y - dep, fron));
        frontier = get_frontier(action, y - dep - 1);
        state = Beam_state(y - dep, c + action.cost, b + action.x_l.size(), x_l, frontier.Holes);
        mp[frontier.Holes] = state.score;
        action_list.emplace_back(action);
        actions++;
        next_que.emplace(state);
        for (int t = 0; t < Loop; ++t) {
          int r = mt() % actions;
          action = get_near_len_action(action_list[r], Frontier(y - dep, fron));
          frontier = get_frontier(action, y - 1 - dep);
          state = Beam_state(y - dep, c + action.cost, b + action.x_l.size(), x_l, frontier.Holes);
          if (mp.contains(frontier.Holes)) {
            if (mp[frontier.Holes] <= state.score) {
              continue;
            }
          }
          mp[frontier.Holes] = state.score;
          action_list.emplace_back(action);
          actions++;
          next_que.emplace(state);
          if (next_que.size() > width) {
            next_que.pop();
          }
        }
        for (int t = 0; t < 9 * Loop; ++t) {
          int r = mt() % actions;
          action = get_near_pos_action(action_list[r], Frontier(y - dep, fron));
          frontier = get_frontier(action, y - 1 - dep);
          state = Beam_state(y - dep, c + action.cost, b + action.x_l.size(), x_l, frontier.Holes);
          if (mp.contains(frontier.Holes)) {
            if (mp[frontier.Holes] <= state.score) {
              continue;
            }
          }
          mp[frontier.Holes] = state.score;
          action_list.emplace_back(action);
          actions++;
          next_que.emplace(state);
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
    auto [__, _, c, b, x_l, fron] = que.top();
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