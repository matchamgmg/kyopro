#include <bits/stdc++.h>
using namespace std;
#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

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

using P = pair<int, int>;
constexpr const int INF = 1e9;

// 入力用クラス
struct Problem {
  int N, M, T;
  vector<string> v, h;
  vector<P> ball, basket;
  Problem() { input(); }

  void input() {
    cin >> N >> M >> T;
    v.resize(N);
    h.resize(N - 1);
    rep(i, 0, N) { cin >> v[i]; }
    rep(i, 0, N - 1) { cin >> h[i]; }
    ball.resize(M);
    basket.resize(M);
    rep(k, 0, M) { cin >> ball[k].first >> ball[k].second >> basket[k].first >> basket[k].second; }
  }
};

// 出力用クラス
struct Answer {
  string a;
  void print() { cout << a << endl; }
  void fprint() {
    string filename = "output.txt";
    ofstream ofs(filename);
    ofs << a << endl;
    ofs.close();
  }
};

struct Solver {
  Problem prob;
  int S;
  Solver(Problem &problem) : prob(problem), S(problem.N * problem.N * 4) {}

  vector<vector<int>> dist;
  vector<vector<int>> prev_state;
  vector<vector<char>> prev_operation;

  vector<vector<int>> cell_dist;
  vector<vector<int>> cell_prev;

  vector<int> highway_cells;
  vector<int> is_highway;

  int id(int i, int j, int d) const { return ((i * prob.N + j) * 4 + d); }

  int cell_id(int i, int j) const { return i * prob.N + j; }

  int cell_id(P p) const { return p.first * prob.N + p.second; }

  tuple<int, int, int> decode(int x) const {
    int d = x % 4;
    x /= 4;
    int j = x % prob.N;
    int i = x / prob.N;
    return {i, j, d};
  }

  bool can_move(int i, int j, int d) const {
    if (d == 0)
      return j + 1 < prob.N && prob.v[i][j] == '0';
    if (d == 1)
      return i + 1 < prob.N && prob.h[i][j] == '0';
    if (d == 2)
      return j - 1 >= 0 && prob.v[i][j - 1] == '0';
    return i - 1 >= 0 && prob.h[i - 1][j] == '0';
  }

  int forward_state(int x) const {
    auto [i, j, d] = decode(x);
    if (!can_move(i, j, d))
      return x;

    if (d == 0)
      j++;
    else if (d == 1)
      i++;
    else if (d == 2)
      j--;
    else
      i--;

    return id(i, j, d);
  }

  vector<int> neighbors_cell(int c) const {
    int i = c / prob.N;
    int j = c % prob.N;
    vector<int> res;

    if (j + 1 < prob.N && prob.v[i][j] == '0')
      res.push_back(cell_id(i, j + 1));
    if (i + 1 < prob.N && prob.h[i][j] == '0')
      res.push_back(cell_id(i + 1, j));
    if (j - 1 >= 0 && prob.v[i][j - 1] == '0')
      res.push_back(cell_id(i, j - 1));
    if (i - 1 >= 0 && prob.h[i - 1][j] == '0')
      res.push_back(cell_id(i - 1, j));

    return res;
  }

  void bfs_state(int s) {
    queue<int> q;
    dist[s][s] = 0;
    prev_state[s][s] = s;
    q.push(s);

    while (!q.empty()) {
      int cur = q.front();
      q.pop();

      auto relax = [&](int nxt, char op) {
        if (dist[s][nxt] != INF)
          return;
        dist[s][nxt] = dist[s][cur] + 1;
        prev_state[s][nxt] = cur;
        prev_operation[s][nxt] = op;
        q.push(nxt);
      };

      auto [i, j, d] = decode(cur);

      if (can_move(i, j, d))
        relax(forward_state(cur), 'F');
      relax(id(i, j, (d + 1) % 4), 'R');
      relax(id(i, j, (d + 3) % 4), 'L');
    }
  }

  void calc_all_state_distance() {
    dist.assign(S, vector<int>(S, INF));
    prev_state.assign(S, vector<int>(S, -1));
    prev_operation.assign(S, vector<char>(S, '?'));

    rep(s, 0, S) bfs_state(s);
  }

  void calc_all_cell_distance() {
    int C = prob.N * prob.N;
    cell_dist.assign(C, vector<int>(C, INF));
    cell_prev.assign(C, vector<int>(C, -1));

    rep(s, 0, C) {
      queue<int> q;
      cell_dist[s][s] = 0;
      cell_prev[s][s] = s;
      q.push(s);

      while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : neighbors_cell(u)) {
          if (cell_dist[s][v] != INF)
            continue;
          cell_dist[s][v] = cell_dist[s][u] + 1;
          cell_prev[s][v] = u;
          q.push(v);
        }
      }
    }
  }

  string restore_path(int from, int to) const {
    string res;
    int cur = to;

    while (cur != from) {
      res.push_back(prev_operation[from][cur]);
      cur = prev_state[from][cur];
    }

    reverse(all(res));
    return res;
  }

  pair<int, int> best_state_to_cell(int from, P goal) const {
    int best_dist = INF;
    int best_state = -1;

    rep(d, 0, 4) {
      int to = id(goal.first, goal.second, d);
      if (dist[from][to] < best_dist) {
        best_dist = dist[from][to];
        best_state = to;
      }
    }

    return {best_dist, best_state};
  }

  vector<int> restore_cell_path_few_turns(int from, int to) const {
    int N = prob.N;
    int C = N * N;

    // state = cell * 4 + dir
    int SS = C * 4;
    vector<int> d(SS, INF), pv(SS, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    auto sid = [&](int c, int dir) { return c * 4 + dir; };

    // 初期向きはどれでもよい
    rep(dir, 0, 4) {
      int s = sid(from, dir);
      d[s] = 0;
      pq.push({0, s});
    }

    while (!pq.empty()) {
      auto [cost, s] = pq.top();
      pq.pop();
      if (cost != d[s])
        continue;

      int c = s / 4;
      int dir = s % 4;

      if (c == to)
        break;

      for (int nc : neighbors_cell(c)) {
        int ci = c / N, cj = c % N;
        int ni = nc / N, nj = nc % N;

        int ndir;
        if (ni == ci && nj == cj + 1)
          ndir = 0;
        else if (ni == ci + 1 && nj == cj)
          ndir = 1;
        else if (ni == ci && nj == cj - 1)
          ndir = 2;
        else
          ndir = 3;

        // 距離も見るが、曲がりをかなり重くする
        int w = 10 + (ndir == dir ? 0 : 7);

        int ns = sid(nc, ndir);
        if (d[ns] > cost + w) {
          d[ns] = cost + w;
          pv[ns] = s;
          pq.push({d[ns], ns});
        }
      }
    }

    int best = -1;
    rep(dir, 0, 4) {
      int s = sid(to, dir);
      if (best == -1 || d[s] < d[best])
        best = s;
    }

    vector<int> path;
    int cur = best;

    while (cur != -1) {
      path.push_back(cur / 4);
      if (cur / 4 == from)
        break;
      cur = pv[cur];
    }

    reverse(all(path));
    return path;
  }

  void add_highway_cell(int c) {
    if (!is_highway[c]) {
      is_highway[c] = 1;
      highway_cells.push_back(c);
    }
  }

  void add_path_to_highway(int from, int to) {
    vector<int> path = restore_cell_path_few_turns(from, to);

    for (int c : path) {
      add_highway_cell(c);
    }
  }

  void print_highway() const {
    vector<string> grid(prob.N, string(prob.N, '.'));

    for (int c : highway_cells) {
      int i = c / prob.N;
      int j = c % prob.N;
      grid[i][j] = '#';
    }

    // 始点
    grid[0][0] = 'S';

    // ボール
    rep(k, 0, prob.M) {
      auto [i, j] = prob.ball[k];
      if (grid[i][j] == '#')
        grid[i][j] = 'B';
      else
        grid[i][j] = 'b';
    }

    // かご
    rep(k, 0, prob.M) {
      auto [i, j] = prob.basket[k];
      if (grid[i][j] == '#')
        grid[i][j] = 'C';
      else
        grid[i][j] = 'c';
    }

    cerr << "highway map:" << endl;
    rep(i, 0, prob.N) { cerr << grid[i] << endl; }
  }

  void add_geometric_rectangle_loop(int margin) {
    int N = prob.N;

    int r1 = margin;
    int r2 = N - 1 - margin;
    int c1 = margin;
    int c2 = N - 1 - margin;

    if (r1 >= r2 || c1 >= c2)
      return;

    vector<int> checkpoints;

    for (int j = c1; j <= c2; ++j)
      checkpoints.push_back(cell_id(r1, j));
    for (int i = r1 + 1; i <= r2; ++i)
      checkpoints.push_back(cell_id(i, c2));
    for (int j = c2 - 1; j >= c1; --j)
      checkpoints.push_back(cell_id(r2, j));
    for (int i = r2 - 1; i > r1; --i)
      checkpoints.push_back(cell_id(i, c1));

    auto add_cell_path = [&](int from, int to) {
      int cur = to;
      while (cur != from) {
        add_highway_cell(cur);
        cur = cell_prev[from][cur];
      }
      add_highway_cell(from);
    };

    int K = checkpoints.size();
    rep(i, 0, K) {
      int from = checkpoints[i];
      int to = checkpoints[(i + 1) % K];
      add_cell_path(from, to);
    }
  }

  void build_double_loop_highway() {
    int C = prob.N * prob.N;
    is_highway.assign(C, 0);
    highway_cells.clear();

    int outer_margin = max(1, prob.N / 6);
    int inner_margin = prob.N / 3;

    add_geometric_rectangle_loop(outer_margin);
    add_geometric_rectangle_loop(inner_margin);

    // 外側ループと内側ループを4方向で接続
    vector<pair<int, int>> bridges;

    int N = prob.N;

    int o1 = outer_margin;
    int o2 = N - 1 - outer_margin;
    int i1 = inner_margin;
    int i2 = N - 1 - inner_margin;

    int mid = N / 2;

    bridges.push_back({cell_id(o1, mid), cell_id(i1, mid)}); // 上
    bridges.push_back({cell_id(o2, mid), cell_id(i2, mid)}); // 下
    bridges.push_back({cell_id(mid, o1), cell_id(mid, i1)}); // 左
    bridges.push_back({cell_id(mid, o2), cell_id(mid, i2)}); // 右

    for (auto [from, to] : bridges) {
      int cur = to;
      while (cur != from) {
        add_highway_cell(cur);
        cur = cell_prev[from][cur];
      }
      add_highway_cell(from);
    }

    cerr << "double loop highway_size = " << highway_cells.size() << endl;
  }

  void build_geometric_loop_highway() {
    int N = prob.N;
    int C = N * N;

    is_highway.assign(C, 0);
    highway_cells.clear();

    int r1 = N / 3;
    int r2 = 2 * N / 3;
    int c1 = N / 3;
    int c2 = 2 * N / 3;

    vector<int> checkpoints;

    // 上辺: 左 -> 右
    for (int j = c1; j <= c2; ++j) {
      checkpoints.push_back(cell_id(r1, j));
    }

    // 右辺: 上 -> 下
    for (int i = r1 + 1; i <= r2; ++i) {
      checkpoints.push_back(cell_id(i, c2));
    }

    // 下辺: 右 -> 左
    for (int j = c2 - 1; j >= c1; --j) {
      checkpoints.push_back(cell_id(r2, j));
    }

    // 左辺: 下 -> 上
    for (int i = r2 - 1; i > r1; --i) {
      checkpoints.push_back(cell_id(i, c1));
    }

    auto add_cell_path = [&](int from, int to) {
      int cur = to;

      while (cur != from) {
        add_highway_cell(cur);
        cur = cell_prev[from][cur];
      }

      add_highway_cell(from);
    };

    int K = checkpoints.size();

    rep(i, 0, K) {
      int from = checkpoints[i];
      int to = checkpoints[(i + 1) % K];
      add_cell_path(from, to);
    }

    cerr << "geometric loop highway_size = " << highway_cells.size() << endl;
  }

  void build_highway_tree(int add_count = 16) {
    int C = prob.N * prob.N;
    is_highway.assign(C, 0);
    highway_cells.clear();

    int start = cell_id(0, 0);
    add_highway_cell(start);

    vector<int> important;
    important.push_back(start);
    rep(k, 0, prob.M) {
      important.push_back(cell_id(prob.ball[k]));
      important.push_back(cell_id(prob.basket[k]));
    }

    rep(_, 0, add_count) {
      int best_p = -1;
      int best_nearest = -1;
      int best_d = -1;

      for (int p : important) {
        int nearest = -1;
        int dmin = INF;

        for (int h : highway_cells) {
          if (cell_dist[p][h] < dmin) {
            dmin = cell_dist[p][h];
            nearest = h;
          }
        }

        if (dmin > best_d) {
          best_d = dmin;
          best_p = p;
          best_nearest = nearest;
        }
      }

      if (best_p == -1 || best_d <= 2)
        break;

      add_path_to_highway(best_nearest, best_p);
    }

    cerr << "highway_size = " << highway_cells.size() << endl;
  }

  vector<int> restore_cell_path_on_highway(int from, int to) const {
    int C = prob.N * prob.N;
    vector<int> par(C, -1);
    queue<int> q;

    par[from] = from;
    q.push(from);

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      if (u == to)
        break;

      for (int v : neighbors_cell(u)) {
        if (!is_highway[v])
          continue;
        if (par[v] != -1)
          continue;
        par[v] = u;
        q.push(v);
      }
    }

    vector<int> path;
    int cur = to;

    if (par[cur] == -1)
      return path;

    while (cur != from) {
      path.push_back(cur);
      cur = par[cur];
    }

    path.push_back(from);
    reverse(all(path));
    return path;
  }

  void append_move_to_cell(string &res, int &cur_state, int target_cell) const {
    int i = target_cell / prob.N;
    int j = target_cell % prob.N;

    auto [cost, nxt] = best_state_to_cell(cur_state, {i, j});
    res += restore_path(cur_state, nxt);
    cur_state = nxt;
  }

  struct Route {
    int cost;
    int end_state;
    string ops;
  };

  Route direct_route_to_cell(int from, P goal) const {
    auto [cost, to] = best_state_to_cell(from, goal);
    return {cost, to, restore_path(from, to)};
  }

  vector<int> nearest_highway_candidates_from_state(int state, int limit = 5) const {
    vector<pair<int, int>> cand;

    for (int h : highway_cells) {
      int i = h / prob.N;
      int j = h % prob.N;
      auto [c, s] = best_state_to_cell(state, {i, j});
      cand.push_back({c, h});
    }

    sort(all(cand));

    vector<int> res;
    rep(i, 0, min(limit, (int)cand.size())) { res.push_back(cand[i].second); }

    return res;
  }

  vector<int> nearest_highway_candidates_to_cell(P goal, int limit = 5) const {
    int g = cell_id(goal);
    vector<pair<int, int>> cand;

    for (int h : highway_cells) {
      cand.push_back({cell_dist[h][g], h});
    }

    sort(all(cand));

    vector<int> res;
    rep(i, 0, min(limit, (int)cand.size())) { res.push_back(cand[i].second); }

    return res;
  }

  Route highway_route_to_cell(int from, P goal) const {
    Route best = direct_route_to_cell(from, goal);

    const int EXTRA_ALLOW = 12;

    auto entries = nearest_highway_candidates_from_state(from, 5);
    auto exits = nearest_highway_candidates_to_cell(goal, 5);

    for (int ent : entries) {
      for (int ex : exits) {
        string ops;
        int cur = from;

        append_move_to_cell(ops, cur, ent);

        vector<int> path = restore_cell_path_on_highway(ent, ex);
        if (path.empty())
          continue;

        rep(i, 1, (int)path.size()) { append_move_to_cell(ops, cur, path[i]); }

        append_move_to_cell(ops, cur, cell_id(goal));

        if ((int)ops.size() <= best.cost + EXTRA_ALLOW) {
          if ((int)ops.size() < (int)best.ops.size()) {
            best = {(int)ops.size(), cur, ops};
          }
        }
      }
    }

    return best;
  }

  int calc_order_score(const vector<int> &order) const {
    int cur = id(0, 0, 0);
    int score = 0;

    for (int k : order) {
      Route r1 = highway_route_to_cell(cur, prob.ball[k]);
      score += r1.cost + 1;
      cur = r1.end_state;

      Route r2 = highway_route_to_cell(cur, prob.basket[k]);
      score += r2.cost + 1;
      cur = r2.end_state;
    }

    return score;
  }

  string build_answer_from_order(const vector<int> &order) const {
    string res;
    int cur = id(0, 0, 0);

    for (int k : order) {
      Route r1 = highway_route_to_cell(cur, prob.ball[k]);
      res += r1.ops;
      res.push_back('S');
      cur = r1.end_state;

      Route r2 = highway_route_to_cell(cur, prob.basket[k]);
      res += r2.ops;
      res.push_back('S');
      cur = r2.end_state;
    }

    return res;
  }

  vector<int> greedy_order() const {
    vector<int> order;
    vector<int> used(prob.M, 0);

    int cur = id(0, 0, 0);

    rep(_, 0, prob.M) {
      int best_k = -1;
      int best_score = INF;
      int best_after = -1;

      rep(k, 0, prob.M) {
        if (used[k])
          continue;

        Route r1 = highway_route_to_cell(cur, prob.ball[k]);
        Route r2 = highway_route_to_cell(r1.end_state, prob.basket[k]);

        int score = r1.cost + r2.cost + 2;

        if (score < best_score) {
          best_score = score;
          best_k = k;
          best_after = r2.end_state;
        }
      }

      used[best_k] = 1;
      order.push_back(best_k);
      cur = best_after;
    }

    return order;
  }

  string compress_macro_once(const string &src) const {
    int L = src.size();
    if (L == 0)
      return src;

    const int MIN_LEN = 3;
    const int MAX_LEN = min(80, L);

    string best_pat;
    vector<int> best_pos;
    int best_gain = 0;

    for (int len = MIN_LEN; len <= MAX_LEN; ++len) {
      unordered_map<string, vector<int>> mp;
      mp.reserve(L * 2);

      for (int i = 0; i + len <= L; ++i) {
        mp[src.substr(i, len)].push_back(i);
      }

      for (auto &[pat, positions] : mp) {
        vector<int> non_overlap;
        int last_end = -1;

        for (int p : positions) {
          if (p >= last_end) {
            non_overlap.push_back(p);
            last_end = p + len;
          }
        }

        int cnt = non_overlap.size();
        if (cnt < 2)
          continue;

        int gain = cnt * len - (len + 2 + cnt - 1);

        if (gain > best_gain) {
          best_gain = gain;
          best_pat = pat;
          best_pos = non_overlap;
        }
      }
    }

    if (best_gain <= 0)
      return src;

    string res;
    int len = best_pat.size();
    int idx = 0;
    int pos_i = 0;
    bool registered = false;

    while (idx < L) {
      if (pos_i < (int)best_pos.size() && idx == best_pos[pos_i]) {
        if (!registered) {
          res.push_back('M');
          res += best_pat;
          res.push_back('M');
          registered = true;
        } else {
          res.push_back('P');
        }

        idx += len;
        pos_i++;
      } else {
        res.push_back(src[idx]);
        idx++;
      }
    }

    return res;
  }

  string make_forward_macro() const {
    string s;
    s.push_back('M');
    rep(_, 0, prob.N) s.push_back('F');
    s.push_back('M');
    return s;
  }

  int apply_op_state(int cur, char op) const {
    auto [i, j, d] = decode(cur);
    if (op == 'R')
      return id(i, j, (d + 1) % 4);
    if (op == 'L')
      return id(i, j, (d + 3) % 4);
    if (op == 'F')
      return forward_state(cur);
    return cur;
  }

  int simulate_ops_state(int cur, const string &ops) const {
    for (char op : ops)
      cur = apply_op_state(cur, op);
    return cur;
  }

  int snap_state_after_P(int cur) const {
    rep(_, 0, prob.N) cur = forward_state(cur);
    return cur;
  }

  string turn_to_dir(int cur, int target_dir) const {
    auto [i, j, d] = decode(cur);
    int diff = (target_dir - d + 4) % 4;

    if (diff == 0)
      return "";
    if (diff == 1)
      return "R";
    if (diff == 2)
      return "RR";
    return "L";
  }

  Route wall_snap_route_to_cell(int from, P goal) const {
    Route best = direct_route_to_cell(from, goal);

    // 1回だけ壁スナップしてから目的地へ向かう
    rep(dir, 0, 4) {
      string ops;
      int cur = from;

      string turn = turn_to_dir(cur, dir);
      ops += turn;
      cur = simulate_ops_state(cur, turn);

      ops.push_back('P');
      cur = snap_state_after_P(cur);

      auto [c2, to] = best_state_to_cell(cur, goal);
      string tail = restore_path(cur, to);

      ops += tail;

      if ((int)ops.size() < (int)best.ops.size()) {
        best = {(int)ops.size(), to, ops};
      }
    }

    // 2回壁スナップする候補も見る
    rep(dir1, 0, 4) {
      rep(dir2, 0, 4) {
        string ops;
        int cur = from;

        string t1 = turn_to_dir(cur, dir1);
        ops += t1;
        cur = simulate_ops_state(cur, t1);

        ops.push_back('P');
        cur = snap_state_after_P(cur);

        string t2 = turn_to_dir(cur, dir2);
        ops += t2;
        cur = simulate_ops_state(cur, t2);

        ops.push_back('P');
        cur = snap_state_after_P(cur);

        auto [c3, to] = best_state_to_cell(cur, goal);
        string tail = restore_path(cur, to);

        ops += tail;

        if ((int)ops.size() < (int)best.ops.size()) {
          best = {(int)ops.size(), to, ops};
        }
      }
    }

    return best;
  }

  int calc_order_score_wall_snap(const vector<int> &order) const {
    int cur = id(0, 0, 0);
    int score = prob.N + 2; // M F...F M で P=F^N を登録

    for (int k : order) {
      Route r1 = wall_snap_route_to_cell(cur, prob.ball[k]);
      score += r1.cost + 1;
      cur = r1.end_state;

      Route r2 = wall_snap_route_to_cell(cur, prob.basket[k]);
      score += r2.cost + 1;
      cur = r2.end_state;
    }

    return score;
  }

  string build_answer_wall_snap(const vector<int> &order) const {
    string res = make_forward_macro();
    int cur = id(0, 0, 0);

    for (int k : order) {
      Route r1 = wall_snap_route_to_cell(cur, prob.ball[k]);
      res += r1.ops;
      res.push_back('S');
      cur = r1.end_state;

      Route r2 = wall_snap_route_to_cell(cur, prob.basket[k]);
      res += r2.ops;
      res.push_back('S');
      cur = r2.end_state;
    }

    return res;
  }

  vector<int> greedy_order_wall_snap() const {
    vector<int> order;
    vector<int> used(prob.M, 0);

    int cur = id(0, 0, 0);

    rep(_, 0, prob.M) {
      int best_k = -1;
      int best_score = INF;
      int best_after = -1;

      rep(k, 0, prob.M) {
        if (used[k])
          continue;

        Route r1 = wall_snap_route_to_cell(cur, prob.ball[k]);
        Route r2 = wall_snap_route_to_cell(r1.end_state, prob.basket[k]);

        int score = r1.cost + r2.cost + 2;

        if (score < best_score) {
          best_score = score;
          best_k = k;
          best_after = r2.end_state;
        }
      }

      used[best_k] = 1;
      order.push_back(best_k);
      cur = best_after;
    }

    return order;
  }

  void solve_wall_snap() {
    calc_all_state_distance();

    vector<int> order = greedy_order_wall_snap();

    Answer ans;
    ans.a = build_answer_wall_snap(order);

    if ((int)ans.a.size() > prob.T) {
      ans.a.resize(prob.T);
    }

    cerr << "wall_snap_len = " << ans.a.size() << endl;
    ans.print();
  }

  void solve1() {
    calc_all_state_distance();
    calc_all_cell_distance();
    build_double_loop_highway();
    print_highway();

    vector<int> order = greedy_order();

    Answer ans;
    ans.a = build_answer_from_order(order);
    ans.a = compress_macro_once(ans.a);

    if ((int)ans.a.size() > prob.T) {
      ans.a.resize(prob.T);
    }

    cerr << "raw_score = " << calc_order_score(order) << endl;
    cerr << "answer_len = " << ans.a.size() << endl;

    ans.print();
  }

  vector<vector<int>> pair_cost;

  void calc_pair_cost() {
    pair_cost.assign(prob.M + 1, vector<int>(prob.M, INF));

    int start_state = id(0, 0, 0);

    rep(k, 0, prob.M) {
      Route r1 = highway_route_to_cell(start_state, prob.ball[k]);
      Route r2 = highway_route_to_cell(r1.end_state, prob.basket[k]);
      pair_cost[prob.M][k] = r1.cost + r2.cost + 2;
    }

    rep(prev, 0, prob.M) {
      auto [_, prev_end] = best_state_to_cell(id(prob.ball[prev].first, prob.ball[prev].second, 0), prob.basket[prev]);

      // より正確には build 時の end_state と合わせたいが、まず高速化優先
      rep(k, 0, prob.M) {
        Route r1 = highway_route_to_cell(prev_end, prob.ball[k]);
        Route r2 = highway_route_to_cell(r1.end_state, prob.basket[k]);
        pair_cost[prev][k] = r1.cost + r2.cost + 2;
      }
    }
  }

  int calc_order_score_fast(const vector<int> &order) const {
    int score = pair_cost[prob.M][order[0]];

    rep(i, 1, prob.M) { score += pair_cost[order[i - 1]][order[i]]; }

    return score;
  }

  void solve2() {
    TimeKeeper timer(1800);

    calc_all_state_distance();
    calc_all_cell_distance();

    build_double_loop_highway();
    print_highway();
    calc_pair_cost();

    vector<int> cur_order = greedy_order();
    vector<int> best_order = cur_order;

    int cur_score = calc_order_score_fast(cur_order);
    int best_score = cur_score;

    mt19937 rng(random_device{}());
    uniform_real_distribution<double> real_dist(0.0, 1.0);

    const double START_TEMP = 30.0;
    const double END_TEMP = 0.1;

    long long loop_cnt = 0;

    while (!timer.isTimeOver()) {
      loop_cnt++;

      double t = timer.get_time() / 1800.0;
      double temp = pow(START_TEMP, 1.0 - t) * pow(END_TEMP, t);

      vector<int> nxt_order = cur_order;

      int type = uniform_int_distribution<int>(0, 2)(rng);

      if (type == 0) {
        int i = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        int j = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        if (i == j)
          continue;

        swap(nxt_order[i], nxt_order[j]);

      } else if (type == 1) {
        int i = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        int j = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        if (i == j)
          continue;

        int x = nxt_order[i];
        nxt_order.erase(nxt_order.begin() + i);
        nxt_order.insert(nxt_order.begin() + j, x);

      } else {
        int l = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        int r = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        if (l > r)
          swap(l, r);
        if (l == r)
          continue;

        reverse(nxt_order.begin() + l, nxt_order.begin() + r + 1);
      }

      int nxt_score = calc_order_score_fast(nxt_order);
      int diff = nxt_score - cur_score;

      if (diff <= 0 || real_dist(rng) < exp(-diff / temp)) {
        cur_order = nxt_order;
        cur_score = nxt_score;

        if (cur_score < best_score) {
          best_score = cur_score;
          best_order = cur_order;
        }
      }
    }

    cerr << "loop = " << loop_cnt << endl;
    cerr << "best_score = " << best_score << endl;

    Answer ans;
    ans.a = build_answer_from_order(best_order);
    ans.a = compress_macro_once(ans.a);

    if ((int)ans.a.size() > prob.T) {
      ans.a.resize(prob.T);
    }

    cerr << "answer_len = " << ans.a.size() << endl;
    ans.print();
  }

  void solve() { solve_wall_snap(); }
};

int main() {
  Problem prob;
  Solver sol(prob);
  sol.solve();
}