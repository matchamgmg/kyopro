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
      return j + 1 < prob.N && prob.v[i][j] == '0'; // right
    if (d == 1)
      return i + 1 < prob.N && prob.h[i][j] == '0'; // down
    if (d == 2)
      return j - 1 >= 0 && prob.v[i][j - 1] == '0'; // left
    return i - 1 >= 0 && prob.h[i - 1][j] == '0';   // up
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

      if (can_move(i, j, d)) {
        relax(forward_state(cur), 'F');
      }

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

  int move_cost_to_cell(int from, P goal) const { return best_state_to_cell(from, goal).first; }

  int calc_order_score(const vector<int> &order) const {
    int cur = id(0, 0, 0);
    int score = 0;

    for (int k : order) {
      auto [c1, s1] = best_state_to_cell(cur, prob.ball[k]);
      auto [c2, s2] = best_state_to_cell(s1, prob.basket[k]);

      score += c1 + 1;
      score += c2 + 1;

      cur = s2;
    }

    return score;
  }

  string build_answer_from_order(const vector<int> &order) const {
    string res;
    int cur = id(0, 0, 0);

    for (int k : order) {
      auto [_, ball_state] = best_state_to_cell(cur, prob.ball[k]);
      res += restore_path(cur, ball_state);
      res.push_back('S');

      auto [__, basket_state] = best_state_to_cell(ball_state, prob.basket[k]);
      res += restore_path(ball_state, basket_state);
      res.push_back('S');

      cur = basket_state;
    }

    return res;
  }

  vector<int> greedy_order() const {
    vector<int> order;
    vector<int> used(prob.M, 0);

    int cur = id(0, 0, 0);

    rep(_, 0, prob.M) {
      int best_k = -1;
      int best_cost = INF;
      int best_after = -1;

      rep(k, 0, prob.M) {
        if (used[k])
          continue;

        auto [c1, s1] = best_state_to_cell(cur, prob.ball[k]);
        auto [c2, s2] = best_state_to_cell(s1, prob.basket[k]);

        int cost = c1 + c2 + 2;

        if (cost < best_cost) {
          best_cost = cost;
          best_k = k;
          best_after = s2;
        }
      }

      used[best_k] = 1;
      order.push_back(best_k);
      cur = best_after;
    }

    return order;
  }

  string compress_macro_once(const string &src) {
    int L = src.size();
    if (L == 0)
      return src;

    const int MIN_LEN = 3;
    const int MAX_LEN = min(100, L);

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

        // 元: cnt * len
        // 圧縮後: len + 2 + (cnt - 1)
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

  string compress_macro_multi_safe(const string &src) {
    const int L = src.size();
    const int MIN_LEN = 3;
    const int MAX_LEN_LIMIT = 100;

    string res;
    int pos = 0;

    while (pos < L) {
      int remain = L - pos;
      int MAX_LEN = min(MAX_LEN_LIMIT, remain);

      string best_pat;
      vector<int> best_pos;
      int best_gain = 0;

      for (int len = MIN_LEN; len <= MAX_LEN; ++len) {
        unordered_map<string, vector<int>> mp;
        mp.reserve(remain * 2);

        for (int i = pos; i + len <= L; ++i) {
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

          // 元: cnt * len
          // 圧縮後: len + 2 + (cnt - 1)
          int gain = cnt * len - (len + 2 + cnt - 1);

          if (gain > best_gain) {
            best_gain = gain;
            best_pat = pat;
            best_pos = non_overlap;
          }
        }
      }

      if (best_gain <= 0) {
        res += src.substr(pos);
        break;
      }

      int len = best_pat.size();

      // best_pos[0] まではそのまま出力
      res += src.substr(pos, best_pos[0] - pos);

      // best_pos の範囲内だけ、1個のマクロを使う
      int cur = best_pos[0];

      for (int idx = 0; idx < (int)best_pos.size(); ++idx) {
        int p = best_pos[idx];

        res += src.substr(cur, p - cur);

        if (idx == 0) {
          res.push_back('M');
          res += best_pat;
          res.push_back('M');
        } else {
          res.push_back('P');
        }

        cur = p + len;
      }

      // 最後の P を使い終わった後から、次のマクロ登録を許す
      pos = cur;
    }

    return res;
  }

  struct BeamNode {
    vector<int> order;
    vector<char> used;
    int cur_state;
    int score;
  };

  vector<int> beam_initial_order(int beam_width = 100, int candidate_k = 8) const {
    vector<BeamNode> beam;

    BeamNode init;
    init.used.assign(prob.M, 0);
    init.cur_state = id(0, 0, 0);
    init.score = 0;
    beam.push_back(init);

    rep(depth, 0, prob.M) {
      vector<BeamNode> nxt;

      for (const auto &node : beam) {
        vector<pair<int, int>> cand;

        rep(k, 0, prob.M) {
          if (node.used[k])
            continue;

          auto [c1, s1] = best_state_to_cell(node.cur_state, prob.ball[k]);
          auto [c2, s2] = best_state_to_cell(s1, prob.basket[k]);

          cand.push_back({c1 + c2 + 2, k});
        }

        sort(all(cand));

        int lim = min(candidate_k, (int)cand.size());

        rep(t, 0, lim) {
          int k = cand[t].second;

          auto [c1, s1] = best_state_to_cell(node.cur_state, prob.ball[k]);
          auto [c2, s2] = best_state_to_cell(s1, prob.basket[k]);

          BeamNode nn = node;
          nn.order.push_back(k);
          nn.used[k] = 1;
          nn.cur_state = s2;
          nn.score += c1 + c2 + 2;

          nxt.push_back(std::move(nn));
        }
      }

      sort(nxt.begin(), nxt.end(), [](const BeamNode &a, const BeamNode &b) { return a.score < b.score; });

      if ((int)nxt.size() > beam_width) {
        nxt.resize(beam_width);
      }

      beam = std::move(nxt);
    }

    return beam.front().order;
  }

  vector<int> random_greedy_order(int candidate_k = 5) {
    vector<int> order;
    vector<int> used(prob.M, 0);

    int cur = id(0, 0, 0);

    rep(_, 0, prob.M) {
      vector<tuple<int, int, int>> cand;
      // cost, k, after_state

      rep(k, 0, prob.M) {
        if (used[k])
          continue;

        auto [c1, s1] = best_state_to_cell(cur, prob.ball[k]);
        auto [c2, s2] = best_state_to_cell(s1, prob.basket[k]);

        cand.push_back({c1 + c2 + 2, k, s2});
      }

      sort(all(cand));

      int lim = min(candidate_k, (int)cand.size());
      int idx = uniform_int_distribution<int>(0, lim - 1)(rng);

      auto [cost, k, after_state] = cand[idx];

      used[k] = 1;
      order.push_back(k);
      cur = after_state;
    }

    return order;
  }

  vector<int> best_of_multiple_initial_orders(int tries = 100) {
    vector<int> best_order = greedy_order();
    int best_score = calc_order_score(best_order);

    {
      vector<int> order = beam_initial_order(150, 8);
      int score = calc_order_score(order);
      if (score < best_score) {
        best_score = score;
        best_order = order;
      }
    }

    rep(t, 0, tries) {
      int k = uniform_int_distribution<int>(2, 10)(rng);
      vector<int> order = random_greedy_order(k);
      int score = calc_order_score(order);

      if (score < best_score) {
        best_score = score;
        best_order = order;
      }
    }

    cerr << "initial_best_score = " << best_score << endl;
    return best_order;
  }

  void solve1() {
    vector<int> order = greedy_order();
    Answer ans;
    ans.a = build_answer_from_order(order);
    ans.a = compress_macro_multi_safe(ans.a);

    if ((int)ans.a.size() > prob.T)
      ans.a.resize(prob.T);
    ans.print();
  }

  void solve2() {
    TimeKeeper timer(1800);

    vector<int> cur_order = greedy_order();
    vector<int> best_order = cur_order;

    int cur_score = calc_order_score(cur_order);
    int best_score = cur_score;

    uniform_real_distribution<double> real_dist(0.0, 1.0);

    const double START_TEMP = 20.0;
    const double END_TEMP = 0.1;

    while (!timer.isTimeOver()) {
      double t = timer.get_time() / 1800.0;
      double temp = pow(START_TEMP, 1.0 - t) * pow(END_TEMP, t);

      vector<int> nxt_order = cur_order;

      int type = uniform_int_distribution<int>(0, 2)(rng);

      if (type == 0) {
        int i = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        int j = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        if (i != j)
          swap(nxt_order[i], nxt_order[j]);
      } else if (type == 1) {
        int i = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        int j = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        if (i != j) {
          int x = nxt_order[i];
          nxt_order.erase(nxt_order.begin() + i);
          nxt_order.insert(nxt_order.begin() + j, x);
        }
      } else {
        int l = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        int r = uniform_int_distribution<int>(0, prob.M - 1)(rng);
        if (l > r)
          swap(l, r);
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

    Answer ans;
    ans.a = build_answer_from_order(best_order);
    ans.a = compress_macro_multi_safe(ans.a);

    if ((int)ans.a.size() > prob.T)
      ans.a.resize(prob.T);
    ans.print();
  }

  void solve3() {
    TimeKeeper timer(1800);

    vector<int> cur_order = best_of_multiple_initial_orders(200);
    vector<int> best_order = cur_order;

    int cur_score = calc_order_score(cur_order);
    int best_score = cur_score;

    uniform_real_distribution<double> real_dist(0.0, 1.0);

    const double START_TEMP = 20.0;
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

    cerr << "loop = " << loop_cnt << endl;
    cerr << "best_score = " << best_score << endl;

    Answer ans;
    ans.a = build_answer_from_order(best_order);
    ans.a = compress_macro_multi_safe(ans.a);

    if ((int)ans.a.size() > prob.T)
      ans.a.resize(prob.T);

    ans.print();
  }

  void solve() {
    calc_all_state_distance();
    solve3();
  }
};

int main() {
  Problem prob;
  Solver sol(prob);
  sol.solve();
}