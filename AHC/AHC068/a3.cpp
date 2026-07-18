#include <bits/stdc++.h>
using namespace std;

struct Operation {
  char d;
  int r, c, h, w;
};

class Solver {
private:
  int N;
  int M;

  static constexpr int MAX_HEIGHT = 3;

  vector<int> card_at;
  vector<int> position_of;

  vector<string> V;
  vector<string> H;

  vector<vector<int>> graph;
  vector<vector<short>> dist;

  vector<int> priority_order;
  vector<bool> fixed_cell;

  vector<Operation> answer;

public:
  void input() {
    cin >> N;
    M = N * N;

    card_at.resize(M);
    position_of.resize(M);

    for (int r = 0; r < N; ++r) {
      for (int c = 0; c < N; ++c) {
        int x;
        cin >> x;

        int v = id(r, c);
        card_at[v] = x;
        position_of[x] = v;
      }
    }

    V.resize(N);
    H.resize(N - 1);

    for (int r = 0; r < N; ++r) {
      cin >> V[r];
    }

    for (int r = 0; r + 1 < N; ++r) {
      cin >> H[r];
    }

    build_graph();
    build_distance();
  }

  void solve() {
    build_priority_order();

    fixed_cell.assign(M, false);

    /*
     * 最後の1マスは、他のカードがすべて正しく配置されれば
     * 自動的に正しいカードになる。
     */
    for (int i = 0; i + 1 < M; ++i) {
      int goal = priority_order[i];

      move_card_to_goal(goal);

      assert(card_at[goal] == goal);
      fixed_cell[goal] = true;
    }

    for (int v = 0; v < M; ++v) {
      assert(card_at[v] == v);
    }

    assert(answer.size() <= 100000);
  }

  void output() const {
    /*
     * この問題は操作数を先頭に出力せず、
     * 操作列だけを出力する。
     */
    for (const auto &op : answer) {
      cout << op.d << ' ' << op.r << ' ' << op.c << ' ' << op.h << ' ' << op.w << '\n';
    }
  }

  void output_to_file(const string &filename = "output.txt") const {
    ofstream fout(filename);

    for (const auto &op : answer) {
      fout << op.d << ' ' << op.r << ' ' << op.c << ' ' << op.h << ' ' << op.w << '\n';
    }

    fout.close();
  }

private:
  int id(int r, int c) const { return r * N + c; }

  int row(int v) const { return v / N; }

  int col(int v) const { return v % N; }

  void add_edge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  void build_graph() {
    graph.assign(M, {});

    for (int r = 0; r < N; ++r) {
      for (int c = 0; c + 1 < N; ++c) {
        if (V[r][c] == '0') {
          add_edge(id(r, c), id(r, c + 1));
        }
      }
    }

    for (int r = 0; r + 1 < N; ++r) {
      for (int c = 0; c < N; ++c) {
        if (H[r][c] == '0') {
          add_edge(id(r, c), id(r + 1, c));
        }
      }
    }
  }

  void build_distance() {
    dist.assign(M, vector<short>(M, -1));

    queue<int> q;

    for (int s = 0; s < M; s++) {
      fill(dist[s].begin(), dist[s].end(), -1);

      dist[s][s] = 0;
      q.push(s);

      while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int to : graph[v]) {
          if (dist[s][to] != -1)
            continue;

          dist[s][to] = dist[s][v] + 1;
          q.push(to);
        }
      }
    }
  }
  /*
   * activeな頂点による誘導部分グラフの関節点を求める。
   */
  vector<bool> find_articulation_points(const vector<bool> &active) const {
    vector<int> ord(M, -1);
    vector<int> low(M, -1);
    vector<bool> articulation(M, false);

    int timer = 0;

    auto dfs = [&](auto &&self, int v, int parent) -> void {
      ord[v] = low[v] = timer++;

      int child_count = 0;

      for (int to : graph[v]) {
        if (!active[to]) {
          continue;
        }

        if (ord[to] == -1) {
          ++child_count;
          self(self, to, v);

          low[v] = min(low[v], low[to]);

          if (parent != -1 && ord[v] <= low[to]) {
            articulation[v] = true;
          }
        } else if (to != parent) {
          low[v] = min(low[v], ord[to]);
        }
      }

      if (parent == -1 && child_count >= 2) {
        articulation[v] = true;
      }
    };

    for (int v = 0; v < M; ++v) {
      if (active[v] && ord[v] == -1) {
        dfs(dfs, v, -1);
      }
    }

    return articulation;
  }

  int active_degree(int v, const vector<bool> &active) const {
    int degree = 0;

    for (int to : graph[v]) {
      if (active[to]) {
        ++degree;
      }
    }

    return degree;
  }

  /*
   * 元の盤面で、そのマスの周囲にある壁・盤面外周の総数。
   */
  int surrounding_wall_count(int v) const {
    int r = row(v);
    int c = col(v);

    int count = 0;

    if (r == 0) {
      ++count;
    } else if (H[r - 1][c] == '1') {
      ++count;
    }

    if (r == N - 1) {
      ++count;
    } else if (H[r][c] == '1') {
      ++count;
    }

    if (c == 0) {
      ++count;
    } else if (V[r][c - 1] == '1') {
      ++count;
    }

    if (c == N - 1) {
      ++count;
    } else if (V[r][c] == '1') {
      ++count;
    }

    return count;
  }

  /*
   * 非関節点を1つずつ削除し、削除順を優先順位とする。
   *
   * 優先順位が高いマスを削除しても、残りのマスは連結である。
   */
  void build_priority_order() {
    priority_order.clear();

    vector<bool> active(M, true);
    int remaining = M;

    while (remaining > 1) {
      vector<bool> articulation = find_articulation_points(active);

      int selected = -1;
      tuple<int, int, int> best_score = {-1, -1, -1};

      for (int v = 0; v < M; ++v) {
        if (!active[v]) {
          continue;
        }

        if (articulation[v]) {
          continue;
        }

        int wall_count = surrounding_wall_count(v);
        int degree = active_degree(v, active);

        /*
         * 壁が多いマスを優先し、
         * 現在の次数が小さいマスを優先する。
         */
        tuple<int, int, int> score = {wall_count, -degree, -v};

        if (selected == -1 || score > best_score) {
          selected = v;
          best_score = score;
        }
      }

      assert(selected != -1);

      priority_order.push_back(selected);
      active[selected] = false;
      --remaining;
    }

    for (int v = 0; v < M; ++v) {
      if (active[v]) {
        priority_order.push_back(v);
        break;
      }
    }

    assert(priority_order.size() == M);
  }

  inline int card_distance(int card, int position) const { return dist[position][card]; }

  int moved_position_by_operation(int v, const Operation &op) const {
    int r = row(v);
    int c = col(v);

    if (r < op.r || r >= op.r + op.h || c < op.c || c >= op.c + op.w) {
      return v;
    }

    if (op.d == 'H') {
      int half = op.w / 2;
      int local_c = c - op.c;

      if (local_c < half) {
        return id(r, c + half);
      } else {
        return id(r, c - half);
      }
    } else {
      int half = op.h / 2;
      int local_r = r - op.r;

      if (local_r < half) {
        return id(r + half, c);
      } else {
        return id(r - half, c);
      }
    }
  }

  long long evaluate_operation(const Operation &op, int target_card) const {
    long long score = 0;

    constexpr long long DISTANCE_WEIGHT = 10;
    constexpr long long FIXED_BONUS = 30;
    constexpr long long BREAK_PENALTY = 50;
    constexpr long long TARGET_WEIGHT = 20;

    for (int x = 0; x < op.h; ++x) {
      for (int y = 0; y < op.w; ++y) {
        int before_position = id(op.r + x, op.c + y);

        int after_position = moved_position_by_operation(before_position, op);

        int card = card_at[before_position];

        int before_distance = card_distance(card, before_position);

        int after_distance = card_distance(card, after_position);

        long long weight = (card == target_card) ? TARGET_WEIGHT : DISTANCE_WEIGHT;

        score += weight * (before_distance - after_distance);

        bool before_correct = card == before_position;

        bool after_correct = card == after_position;

        if (!before_correct && after_correct) {
          score += FIXED_BONUS;
        }

        if (before_correct && !after_correct) {
          score -= BREAK_PENALTY;
        }
      }
    }

    /*
     * 同評価なら小さい長方形を優先するため、
     * 面積に小さなペナルティを与える。
     */
    score -= op.h * op.w;

    return score;
  }

  Operation select_best_operation(int current_position, int next_position, int target_card) const {
    vector<pair<int, Operation>> transitions = enumerate_transitions(current_position);

    bool found = false;
    Operation best_operation;
    long long best_score = numeric_limits<long long>::lowest();

    for (const auto &[to, op] : transitions) {
      if (to != next_position) {
        continue;
      }

      long long score = evaluate_operation(op, target_card);

      if (!found || score > best_score) {
        found = true;
        best_score = score;
        best_operation = op;
      }
    }

    assert(found);
    return best_operation;
  }

  bool cell_is_available(int v) const { return !fixed_cell[v]; }

  /*
   * 長方形 (r, c, h, w) が操作可能か判定する。
   *
   * ・盤面内に収まる
   * ・固定済みマスを含まない
   * ・長方形内部に壁がない
   */
  bool valid_rectangle(int r, int c, int h, int w) const {
    if (r < 0 || c < 0) {
      return false;
    }

    if (r + h > N || c + w > N) {
      return false;
    }

    /*
     * 固定済みマスを含んではならない。
     */
    for (int x = 0; x < h; ++x) {
      for (int y = 0; y < w; ++y) {
        int v = id(r + x, c + y);

        if (fixed_cell[v]) {
          return false;
        }
      }
    }

    /*
     * 長方形内部の横方向の壁を確認する。
     *
     * V[i][j] は、
     * (i,j) と (i,j+1) の間の壁を表す。
     */
    for (int x = 0; x < h; ++x) {
      for (int y = 0; y + 1 < w; ++y) {
        if (V[r + x][c + y] == '1') {
          return false;
        }
      }
    }

    /*
     * 長方形内部の縦方向の壁を確認する。
     *
     * H[i][j] は、
     * (i,j) と (i+1,j) の間の壁を表す。
     */
    for (int x = 0; x + 1 < h; ++x) {
      for (int y = 0; y < w; ++y) {
        if (H[r + x][c + y] == '1') {
          return false;
        }
      }
    }

    return true;
  }

  /*
   * 現在位置vにある対象カードを、1回の細長い長方形操作で
   * 移動できる遷移をすべて生成する。
   */
  /*
   * 現在位置 v にある対象カードを、1回の長方形操作で
   * 移動できる遷移をすべて生成する。
   *
   * 横操作:
   *   高さ 1～MAX_HEIGHT
   *
   * 縦操作:
   *   幅 1～MAX_HEIGHT
   */
  vector<pair<int, Operation>> enumerate_transitions(int v) const {
    vector<pair<int, Operation>> transitions;

    int current_r = row(v);
    int current_c = col(v);

    /*
     * 横方向への移動。
     *
     * 長方形:
     *   高さ rect_h
     *   幅   2*d
     *
     * 左半分のカードは右へ d マス、
     * 右半分のカードは左へ d マス移動する。
     */
    for (int rect_h = 1; rect_h <= min(MAX_HEIGHT, N); ++rect_h) {
      /*
       * 対象カードが長方形の何行目に含まれるかを列挙する。
       */
      for (int row_offset = 0; row_offset < rect_h; ++row_offset) {
        int top = current_r - row_offset;

        if (top < 0 || top + rect_h > N) {
          continue;
        }

        for (int d = 1; 2 * d <= N; ++d) {
          int rect_w = 2 * d;

          /*
           * 対象カードを右へ d マス移動させる。
           *
           * 対象カードを長方形の左半分に含める。
           */
          if (current_c + d < N) {
            for (int column_offset = 0; column_offset < d; ++column_offset) {
              int left = current_c - column_offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              Operation op{'H', top, left, rect_h, rect_w};

              transitions.push_back({id(current_r, current_c + d), op});
            }
          }

          /*
           * 対象カードを左へ d マス移動させる。
           *
           * 対象カードを長方形の右半分に含める。
           */
          if (current_c - d >= 0) {
            for (int column_offset = 0; column_offset < d; ++column_offset) {
              int left = current_c - d - column_offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              Operation op{'H', top, left, rect_h, rect_w};

              transitions.push_back({id(current_r, current_c - d), op});
            }
          }
        }
      }
    }

    /*
     * 縦方向への移動。
     *
     * 長方形:
     *   高さ 2*d
     *   幅   rect_w
     *
     * 上半分のカードは下へ d マス、
     * 下半分のカードは上へ d マス移動する。
     */
    for (int rect_w = 1; rect_w <= min(MAX_HEIGHT, N); ++rect_w) {
      /*
       * 対象カードが長方形の何列目に含まれるかを列挙する。
       */
      for (int column_offset = 0; column_offset < rect_w; ++column_offset) {
        int left = current_c - column_offset;

        if (left < 0 || left + rect_w > N) {
          continue;
        }

        for (int d = 1; 2 * d <= N; ++d) {
          int rect_h = 2 * d;

          /*
           * 対象カードを下へ d マス移動させる。
           *
           * 対象カードを長方形の上半分に含める。
           */
          if (current_r + d < N) {
            for (int row_offset = 0; row_offset < d; ++row_offset) {
              int top = current_r - row_offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              Operation op{'V', top, left, rect_h, rect_w};

              transitions.push_back({id(current_r + d, current_c), op});
            }
          }

          /*
           * 対象カードを上へ d マス移動させる。
           *
           * 対象カードを長方形の下半分に含める。
           */
          if (current_r - d >= 0) {
            for (int row_offset = 0; row_offset < d; ++row_offset) {
              int top = current_r - d - row_offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              Operation op{'V', top, left, rect_h, rect_w};

              transitions.push_back({id(current_r - d, current_c), op});
            }
          }
        }
      }
    }

    return transitions;
  }
  /*
   * 操作を盤面に適用する。
   */
  void apply_operation(const Operation &op) {
    if (op.d == 'H') {
      int half = op.w / 2;

      for (int x = 0; x < op.h; ++x) {
        for (int y = 0; y < half; ++y) {
          int u = id(op.r + x, op.c + y);
          int v = id(op.r + x, op.c + half + y);

          swap_cells(u, v);
        }
      }
    } else {
      int half = op.h / 2;

      for (int x = 0; x < half; ++x) {
        for (int y = 0; y < op.w; ++y) {
          int u = id(op.r + x, op.c + y);
          int v = id(op.r + half + x, op.c + y);

          swap_cells(u, v);
        }
      }
    }

    answer.push_back(op);
  }

  void swap_cells(int u, int v) {
    int card_u = card_at[u];
    int card_v = card_at[v];

    swap(card_at[u], card_at[v]);

    position_of[card_u] = v;
    position_of[card_v] = u;
  }

  /*
   * 指定したカードを、細長い長方形操作の最小回数で
   * 対応する目的地まで移動させる。
   */
  void move_card_to_goal(int card) {
    int start = position_of[card];
    int goal = card;

    if (start == goal) {
      return;
    }

    assert(!fixed_cell[start]);
    assert(!fixed_cell[goal]);

    vector<int> dist(M, -1);
    vector<int> previous(M, -1);

    queue<int> que;

    dist[start] = 0;
    previous[start] = start;
    que.push(start);

    while (!que.empty()) {
      int v = que.front();
      que.pop();

      if (v == goal) {
        break;
      }

      auto transitions = enumerate_transitions(v);

      array<bool, 400> used_to{};
      used_to.fill(false);

      for (const auto &[to, op] : transitions) {
        if (used_to[to]) {
          continue;
        }
        used_to[to] = true;

        if (dist[to] != -1) {
          continue;
        }

        dist[to] = dist[v] + 1;
        previous[to] = v;
        que.push(to);
      }

      for (const auto &[to, op] : transitions) {
        if (dist[to] != -1) {
          continue;
        }

        dist[to] = dist[v] + 1;
        previous[to] = v;
        que.push(to);
      }
    }

    assert(dist[goal] != -1);

    /*
     * 対象カードが通る位置列を復元する。
     */
    vector<int> position_path;

    for (int v = goal;; v = previous[v]) {
      position_path.push_back(v);

      if (v == start) {
        break;
      }
    }

    reverse(position_path.begin(), position_path.end());

    /*
     * 各ステップで、同じ移動先を実現する操作の中から、
     * 現在の盤面に対して最も評価の高いものを選ぶ。
     */
    for (int i = 0; i + 1 < static_cast<int>(position_path.size()); ++i) {
      int current_position = position_of[card];
      int next_position = position_path[i + 1];

      assert(current_position == position_path[i]);

      Operation best_operation = select_best_operation(current_position, next_position, card);

      apply_operation(best_operation);

      assert(position_of[card] == next_position);
    }

    assert(position_of[card] == goal);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  solver.input();
  solver.solve();
  solver.output_to_file();

  return 0;
}