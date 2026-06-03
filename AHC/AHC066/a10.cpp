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

  mt19937 rng{random_device{}()};

  int id(int i, int j, int d) const { return ((i * prob.N + j) * 4 + d); }

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

  int apply_op(int st, char op) const {
    auto [i, j, d] = decode(st);
    if (op == 'R')
      return id(i, j, (d + 1) % 4);
    if (op == 'L')
      return id(i, j, (d + 3) % 4);
    if (op == 'F')
      return forward_state(st);
    return st;
  }

  int wall_distance(int st) const {
    int cur = st;
    int cnt = 0;
    while (true) {
      int nxt = forward_state(cur);
      if (nxt == cur)
        break;
      cur = nxt;
      cnt++;
    }
    return cnt;
  }

  int snap_state(int st) const {
    while (true) {
      int nxt = forward_state(st);
      if (nxt == st)
        return st;
      st = nxt;
    }
  }

  string turn_to_dir(int st, int td) const {
    auto [i, j, d] = decode(st);
    int diff = (td - d + 4) % 4;
    if (diff == 0)
      return "";
    if (diff == 1)
      return "R";
    if (diff == 2)
      return "RR";
    return "L";
  }

  int simulate_virtual(int st, const string &ops) const {
    for (char c : ops) {
      if (c == 'X')
        st = snap_state(st);
      else
        st = apply_op(st, c);
    }
    return st;
  }

  void bfs(int s) {
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

    rep(s, 0, S) bfs(s);
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

  struct Route {
    int cost;
    int end_state;
    string ops; // F,R,L,S,X の仮想操作列
  };

  Route direct_route_to_cell(int from, P goal) const {
    auto [cost, to] = best_state_to_cell(from, goal);
    return {cost, to, restore_path(from, to)};
  }

  int count_walls() const {
    int w = 0;

    rep(i, 0, prob.N) {
      rep(j, 0, prob.N - 1) {
        if (prob.v[i][j] == '1')
          w++;
      }
    }

    rep(i, 0, prob.N - 1) {
      rep(j, 0, prob.N) {
        if (prob.h[i][j] == '1')
          w++;
      }
    }

    return w;
  }

  Route x_route_to_cell(int from, P goal) const {
    Route best = direct_route_to_cell(from, goal);

    // Xを1回使う
    rep(d1, 0, 4) {
      string ops;
      int cur = from;

      string t1 = turn_to_dir(cur, d1);
      ops += t1;
      cur = simulate_virtual(cur, t1);

      ops.push_back('X');
      cur = snap_state(cur);

      auto [c2, to] = best_state_to_cell(cur, goal);
      ops += restore_path(cur, to);

      int cost = (int)ops.size();

      // あまり損しないならX経由を優先
      if (cost <= best.cost + 6) {
        if (cost < best.cost || count(all(ops), 'X') > count(all(best.ops), 'X')) {
          best = {cost, to, ops};
        }
      }
    }

    // Xを2回使う
    rep(d1, 0, 4) {
      rep(d2, 0, 4) {
        string ops;
        int cur = from;

        string t1 = turn_to_dir(cur, d1);
        ops += t1;
        cur = simulate_virtual(cur, t1);

        ops.push_back('X');
        cur = snap_state(cur);

        string t2 = turn_to_dir(cur, d2);
        ops += t2;
        cur = simulate_virtual(cur, t2);

        ops.push_back('X');
        cur = snap_state(cur);

        auto [c3, to] = best_state_to_cell(cur, goal);
        ops += restore_path(cur, to);

        int cost = (int)ops.size();

        if (cost <= best.cost + 10) {
          if (cost < best.cost || count(all(ops), 'X') > count(all(best.ops), 'X')) {
            best = {cost, to, ops};
          }
        }
      }
    }

    return best;
  }

  // Route x_route_to_cell(int from, P goal) const {
  //   Route direct = direct_route_to_cell(from, goal);
  //   Route best = direct;

  //   const int EXTRA_ALLOW_1 = 12;
  //   const int EXTRA_ALLOW_2 = 20;

  //   auto better = [&](const Route &cand, int extra_allow) {
  //     int cand_x = count(all(cand.ops), 'X');
  //     int best_x = count(all(best.ops), 'X');

  //     if (cand.cost > direct.cost + extra_allow)
  //       return false;

  //     // 幹線道路(X)を使う経路を優先
  //     if (cand_x > best_x)
  //       return true;

  //     // Xの数が同じなら短い方
  //     if (cand_x == best_x && cand.cost < best.cost)
  //       return true;

  //     return false;
  //   };

  //   // Xを1回使う
  //   rep(d1, 0, 4) {
  //     string ops;
  //     int cur = from;

  //     string t1 = turn_to_dir(cur, d1);
  //     ops += t1;
  //     cur = simulate_virtual(cur, t1);

  //     ops.push_back('X');
  //     cur = snap_state(cur);

  //     auto [c2, to] = best_state_to_cell(cur, goal);
  //     ops += restore_path(cur, to);

  //     Route cand{(int)ops.size(), to, ops};

  //     if (better(cand, EXTRA_ALLOW_1)) {
  //       best = cand;
  //     }
  //   }

  //   // Xを2回使う
  //   rep(d1, 0, 4) {
  //     rep(d2, 0, 4) {
  //       string ops;
  //       int cur = from;

  //       string t1 = turn_to_dir(cur, d1);
  //       ops += t1;
  //       cur = simulate_virtual(cur, t1);

  //       ops.push_back('X');
  //       cur = snap_state(cur);

  //       string t2 = turn_to_dir(cur, d2);
  //       ops += t2;
  //       cur = simulate_virtual(cur, t2);

  //       ops.push_back('X');
  //       cur = snap_state(cur);

  //       auto [c3, to] = best_state_to_cell(cur, goal);
  //       ops += restore_path(cur, to);

  //       Route cand{(int)ops.size(), to, ops};

  //       if (better(cand, EXTRA_ALLOW_2)) {
  //         best = cand;
  //       }
  //     }
  //   }

  //   return best;
  // }

  int calc_order_score(const vector<int> &order) const {
    int cur = id(0, 0, 0);
    int score = 0;

    for (int k : order) {
      Route r1 = x_route_to_cell(cur, prob.ball[k]);
      score += r1.cost + 1;
      cur = r1.end_state;

      Route r2 = x_route_to_cell(cur, prob.basket[k]);
      score += r2.cost + 1;
      cur = r2.end_state;
    }

    return score;
  }

  string build_virtual_answer(const vector<int> &order) const {
    string res;
    int cur = id(0, 0, 0);

    for (int k : order) {
      Route r1 = x_route_to_cell(cur, prob.ball[k]);
      res += r1.ops;
      res.push_back('S');
      cur = r1.end_state;

      Route r2 = x_route_to_cell(cur, prob.basket[k]);
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

        Route r1 = x_route_to_cell(cur, prob.ball[k]);
        Route r2 = x_route_to_cell(r1.end_state, prob.basket[k]);

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

  string expand_without_macro(const string &vops) const {
    string res;
    int cur = id(0, 0, 0);

    for (char c : vops) {
      if (c == 'X') {
        int d = wall_distance(cur);
        rep(_, 0, d) {
          res.push_back('F');
          cur = forward_state(cur);
        }
      } else {
        res.push_back(c);
        cur = apply_op(cur, c);
      }
    }

    return res;
  }

  string expand_with_f_macro(const string &vops, int K) const {
    string res;
    int cur = id(0, 0, 0);
    bool registered = false;

    auto emit_P = [&]() {
      if (!registered) {
        res.push_back('M');
        rep(_, 0, K) res.push_back('F');
        res.push_back('M');
        registered = true;
      } else {
        res.push_back('P');
      }

      rep(_, 0, K) cur = forward_state(cur);
    };

    for (int i = 0; i < (int)vops.size();) {
      // F...FX をまとめて P で吸収する
      if (vops[i] == 'F') {
        int j = i;
        while (j < (int)vops.size() && vops[j] == 'F')
          j++;

        if (j < (int)vops.size() && vops[j] == 'X') {
          int need = wall_distance(cur);
          int q = (need + K - 1) / K;

          rep(_, 0, q) emit_P();

          i = j + 1;
          continue;
        }

        // 通常のF列
        while (i < j) {
          // ぴったりK個進むならPを使う
          if (registered && i + K <= j) {
            emit_P();
            i += K;
          } else {
            res.push_back('F');
            cur = forward_state(cur);
            i++;
          }
        }

        continue;
      }

      if (vops[i] == 'X') {
        int need = wall_distance(cur);
        int q = (need + K - 1) / K;

        rep(_, 0, q) emit_P();

        i++;
        continue;
      }

      res.push_back(vops[i]);
      cur = apply_op(cur, vops[i]);
      i++;
    }

    return res;
  }

  string convert_virtual_to_answer(const string &vops) const {
    string best = expand_without_macro(vops);

    for (int K = 2; K <= prob.N; ++K) {
      string s = expand_with_f_macro(vops, K);
      if ((int)s.size() < (int)best.size()) {
        best = s;
      }
    }

    return best;
  }

  void solve1() {
    calc_all_state_distance();

    TimeKeeper timer(1600);

    vector<int> cur_order = greedy_order();
    vector<int> best_order = cur_order;

    int cur_score = calc_order_score(cur_order);
    int best_score = cur_score;

    uniform_real_distribution<double> real_dist(0.0, 1.0);

    const double START_TEMP = 25.0;
    const double END_TEMP = 0.1;

    while (!timer.isTimeOver()) {
      double t = timer.get_time() / 1600.0;
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

      int nxt_score = calc_order_score(nxt_order);
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

    string vops = build_virtual_answer(best_order);
    string ans = convert_virtual_to_answer(vops);

    if ((int)ans.size() > prob.T) {
      ans.resize(prob.T);
    }

    cout << ans << endl;
  }

  vector<int> greedy_order_shortest() const {
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

        Route r1 = direct_route_to_cell(cur, prob.ball[k]);
        Route r2 = direct_route_to_cell(r1.end_state, prob.basket[k]);

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

  string build_answer_shortest(const vector<int> &order) const {
    string res;
    int cur = id(0, 0, 0);

    for (int k : order) {
      Route r1 = direct_route_to_cell(cur, prob.ball[k]);
      res += r1.ops;
      res.push_back('S');
      cur = r1.end_state;

      Route r2 = direct_route_to_cell(cur, prob.basket[k]);
      res += r2.ops;
      res.push_back('S');
      cur = r2.end_state;
    }

    return res;
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

  Route shortest_route_to_cell(int from, P goal) const { return direct_route_to_cell(from, goal); }

  int calc_order_score_shortest(const vector<int> &order) const {
    int cur = id(0, 0, 0);
    int score = 0;

    for (int k : order) {
      Route r1 = direct_route_to_cell(cur, prob.ball[k]);
      score += r1.cost + 1;
      cur = r1.end_state;

      Route r2 = direct_route_to_cell(cur, prob.basket[k]);
      score += r2.cost + 1;
      cur = r2.end_state;
    }

    return score;
  }

  void solve_shortest_path() {
    calc_all_state_distance();

    TimeKeeper timer(1600);

    vector<int> cur_order = greedy_order_shortest();
    vector<int> best_order = cur_order;

    int cur_score = calc_order_score_shortest(cur_order);
    int best_score = cur_score;

    uniform_real_distribution<double> real_dist(0.0, 1.0);

    const double START_TEMP = 25.0;
    const double END_TEMP = 0.1;

    while (!timer.isTimeOver()) {
      double t = timer.get_time() / 1600.0;
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

      int nxt_score = calc_order_score_shortest(nxt_order);
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

    Answer ans;
    ans.a = build_answer_shortest(best_order);
    ans.a = compress_macro_once(ans.a);

    if ((int)ans.a.size() > prob.T) {
      ans.a.resize(prob.T);
    }

    ans.print();
  }

  void solve() {
    int w = count_walls();

    // 目安。まずは N/3〜N/2 あたりで調整
    if (w <= prob.N / 3) {
      solve_shortest_path();
    } else {
      solve1(); // X・壁スナップ幹線方針
    }
  }
};

int main() {
  Problem prob;
  Solver sol(prob);
  sol.solve();
}