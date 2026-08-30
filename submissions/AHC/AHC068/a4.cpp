#include <bits/stdc++.h>
using namespace std;

struct Operation {
  char d;
  int r;
  int c;
  int h;
  int w;
};

class Solver {
private:
  static constexpr int MAX_HEIGHT = 3;
  static constexpr int INF = 1 << 30;

  int N;
  int M;

  // card_at[v] : マス v に置かれているカード番号
  vector<int> card_at;

  // position_of[x] : カード x が置かれているマス
  vector<int> position_of;

  // V[r][c] : (r,c) と (r,c+1) の間の壁
  vector<string> V;

  // H[r][c] : (r,c) と (r+1,c) の間の壁
  vector<string> H;

  // 壁のない隣接マスからなるグラフ
  vector<vector<int>> graph;

  // wall_dist[v][goal]
  // 元の壁だけを考慮した、v から goal への最短距離
  vector<vector<short>> wall_dist;

  // next_step[v][goal]
  // v から goal への最短路上で次に進むマス
  vector<vector<short>> next_step;

  // 優先度の高い順に目的地のマス番号を格納
  vector<int> priority_order;

  // すでに確定して、今後動かしてはいけないマス
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
        int card;
        cin >> card;

        int v = id(r, c);
        card_at[v] = card;
        position_of[card] = v;
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
    build_all_pairs_shortest_paths();
  }

  void solve() {
    build_priority_order();

    fixed_cell.assign(M, false);

    /*
     * 最後の1マスは、残り399枚が正しく配置されれば
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

  void output(ostream &os) const {
    for (const auto &op : answer) {
      os << op.d << ' ' << op.r << ' ' << op.c << ' ' << op.h << ' ' << op.w << '\n';
    }
  }

  void output_stdout() const { output(cout); }

  bool output_to_file(const string &filename = "answer.txt") const {
    ofstream fout(filename);

    if (!fout) {
      cerr << "Failed to open output file: " << filename << '\n';
      return false;
    }

    output(fout);
    return true;
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

  /*
   * 元の壁だけを考慮した全点対最短距離を計算する。
   *
   * wall_dist[position][goal]
   * next_step[position][goal]
   */
  void build_all_pairs_shortest_paths() {
    wall_dist.assign(M, vector<short>(M, -1));

    next_step.assign(M, vector<short>(M, -1));

    /*
     * 各目的地 goal を始点としてBFSする。
     * グラフは無向なので、
     * goalからの距離 = goalへの距離。
     */
    for (int goal = 0; goal < M; ++goal) {
      queue<int> que;

      wall_dist[goal][goal] = 0;
      que.push(goal);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int to : graph[v]) {
          if (wall_dist[to][goal] != -1) {
            continue;
          }

          wall_dist[to][goal] = wall_dist[v][goal] + 1;

          que.push(to);
        }
      }
    }

    /*
     * vからgoalへ向かう最短路上の次マスを求める。
     */
    for (int v = 0; v < M; ++v) {
      for (int goal = 0; goal < M; ++goal) {
        if (v == goal) {
          next_step[v][goal] = v;
          continue;
        }

        int best = -1;

        for (int to : graph[v]) {
          if (wall_dist[to][goal] + 1 == wall_dist[v][goal]) {
            best = to;
            break;
          }
        }

        assert(best != -1);
        next_step[v][goal] = static_cast<short>(best);
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

  int surrounding_wall_count(int v) const {
    int r = row(v);
    int c = col(v);

    int count = 0;

    // 上
    if (r == 0) {
      ++count;
    } else if (H[r - 1][c] == '1') {
      ++count;
    }

    // 下
    if (r == N - 1) {
      ++count;
    } else if (H[r][c] == '1') {
      ++count;
    }

    // 左
    if (c == 0) {
      ++count;
    } else if (V[r][c - 1] == '1') {
      ++count;
    }

    // 右
    if (c == N - 1) {
      ++count;
    } else if (V[r][c] == '1') {
      ++count;
    }

    return count;
  }

  /*
   * 非関節点を順に削除することで、
   * 固定後も未固定領域が連結となる順序を作る。
   */
  void build_priority_order() {
    priority_order.clear();

    vector<bool> active(M, true);
    int remaining = M;

    while (remaining > 1) {
      vector<bool> articulation = find_articulation_points(active);

      int selected = -1;

      /*
       * 比較順:
       * 1. 周囲の壁が多い
       * 2. active部分グラフでの次数が小さい
       * 3. マス番号が小さい
       */
      tuple<int, int, int> best_score{-1, numeric_limits<int>::min(), numeric_limits<int>::min()};

      for (int v = 0; v < M; ++v) {
        if (!active[v]) {
          continue;
        }

        if (articulation[v]) {
          continue;
        }

        int wall_count = surrounding_wall_count(v);

        int degree = active_degree(v, active);

        tuple<int, int, int> score{wall_count, -degree, -v};

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

    assert(static_cast<int>(priority_order.size()) == M);
  }

  /*
   * 長方形が合法か判定する。
   *
   * ・盤面内
   * ・固定済みマスを含まない
   * ・内部に壁がない
   */
  bool valid_rectangle(int r, int c, int h, int w) const {
    if (r < 0 || c < 0) {
      return false;
    }

    if (r + h > N || c + w > N) {
      return false;
    }

    for (int x = 0; x < h; ++x) {
      for (int y = 0; y < w; ++y) {
        if (fixed_cell[id(r + x, c + y)]) {
          return false;
        }
      }
    }

    /*
     * 横方向の隣接マス間の壁。
     */
    for (int x = 0; x < h; ++x) {
      for (int y = 0; y + 1 < w; ++y) {
        if (V[r + x][c + y] == '1') {
          return false;
        }
      }
    }

    /*
     * 縦方向の隣接マス間の壁。
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
   * 現在位置 v の対象カードを1回で移動できる
   * 全ての遷移を列挙する。
   *
   * 横操作の高さ、縦操作の幅を
   * 1～MAX_HEIGHTに制限する。
   */
  vector<pair<int, Operation>> enumerate_transitions(int v) const {
    vector<pair<int, Operation>> transitions;

    int current_r = row(v);
    int current_c = col(v);

    /*
     * 横操作。
     */
    for (int rect_h = 1; rect_h <= MAX_HEIGHT; ++rect_h) {
      for (int row_offset = 0; row_offset < rect_h; ++row_offset) {
        int top = current_r - row_offset;

        if (top < 0 || top + rect_h > N) {
          continue;
        }

        for (int d = 1; 2 * d <= N; ++d) {
          int rect_w = 2 * d;

          /*
           * 右へdマス。
           */
          if (current_c + d < N) {
            for (int offset = 0; offset < d; ++offset) {
              int left = current_c - offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              transitions.push_back({id(current_r, current_c + d), Operation{'H', top, left, rect_h, rect_w}});
            }
          }

          /*
           * 左へdマス。
           */
          if (current_c - d >= 0) {
            for (int offset = 0; offset < d; ++offset) {
              int left = current_c - d - offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              transitions.push_back({id(current_r, current_c - d), Operation{'H', top, left, rect_h, rect_w}});
            }
          }
        }
      }
    }

    /*
     * 縦操作。
     */
    for (int rect_w = 1; rect_w <= MAX_HEIGHT; ++rect_w) {
      for (int column_offset = 0; column_offset < rect_w; ++column_offset) {
        int left = current_c - column_offset;

        if (left < 0 || left + rect_w > N) {
          continue;
        }

        for (int d = 1; 2 * d <= N; ++d) {
          int rect_h = 2 * d;

          /*
           * 下へdマス。
           */
          if (current_r + d < N) {
            for (int offset = 0; offset < d; ++offset) {
              int top = current_r - offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              transitions.push_back({id(current_r + d, current_c), Operation{'V', top, left, rect_h, rect_w}});
            }
          }

          /*
           * 上へdマス。
           */
          if (current_r - d >= 0) {
            for (int offset = 0; offset < d; ++offset) {
              int top = current_r - d - offset;

              if (!valid_rectangle(top, left, rect_h, rect_w)) {
                continue;
              }

              transitions.push_back({id(current_r - d, current_c), Operation{'V', top, left, rect_h, rect_w}});
            }
          }
        }
      }
    }

    return transitions;
  }

  /*
   * あるマスが操作によって移動する先を返す。
   * 長方形外なら元の位置を返す。
   */
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
      }

      return id(r, c - half);
    }

    int half = op.h / 2;
    int local_r = r - op.r;

    if (local_r < half) {
      return id(r + half, c);
    }

    return id(r - half, c);
  }

  inline int card_distance(int card, int position) const { return wall_dist[position][card]; }

  /*
   * 実際の移動方向が、壁込み最短路の最初の方向と
   * おおむね一致しているか判定する。
   */
  bool follows_next_direction(int card, int before_position, int after_position) const {
    if (before_position == card) {
      return false;
    }

    int next = next_step[before_position][card];

    int expected_dr = row(next) - row(before_position);

    int expected_dc = col(next) - col(before_position);

    int actual_dr = row(after_position) - row(before_position);

    int actual_dc = col(after_position) - col(before_position);

    if (expected_dr != 0) {
      return (actual_dc == 0 && actual_dr * expected_dr > 0);
    }

    return (actual_dr == 0 && actual_dc * expected_dc > 0);
  }

  /*
   * 同じ移動先を実現する操作の比較用評価。
   */
  long long evaluate_operation(const Operation &op, int target_card) const {
    constexpr long long DISTANCE_WEIGHT = 10;
    constexpr long long TARGET_WEIGHT = 20;
    constexpr long long FIXED_BONUS = 30;
    constexpr long long BREAK_PENALTY = 50;
    constexpr long long DIRECTION_BONUS = 3;
    constexpr long long AREA_PENALTY = 1;

    long long score = 0;

    for (int x = 0; x < op.h; ++x) {
      for (int y = 0; y < op.w; ++y) {
        int before_position = id(op.r + x, op.c + y);

        int after_position = moved_position_by_operation(before_position, op);

        int card = card_at[before_position];

        int before_distance = card_distance(card, before_position);

        int after_distance = card_distance(card, after_position);

        long long weight = card == target_card ? TARGET_WEIGHT : DISTANCE_WEIGHT;

        score += weight * (before_distance - after_distance);

        bool before_correct = card == before_position;

        bool after_correct = card == after_position;

        if (!before_correct && after_correct) {
          score += FIXED_BONUS;
        }

        if (before_correct && !after_correct) {
          score -= BREAK_PENALTY;
        }

        if (follows_next_direction(card, before_position, after_position)) {
          score += DIRECTION_BONUS;
        }
      }
    }

    score -= AREA_PENALTY * op.h * op.w;

    return score;
  }

  /*
   * 同じ next_position へ対象カードを移動させる操作のうち、
   * 現在の盤面に対して最も評価が高いものを選択する。
   */
  Operation select_best_operation(int current_position, int next_position, int target_card) const {
    vector<pair<int, Operation>> transitions = enumerate_transitions(current_position);

    bool found = false;
    Operation best_operation{};
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

  void swap_cells(int u, int v) {
    int card_u = card_at[u];
    int card_v = card_at[v];

    swap(card_at[u], card_at[v]);

    position_of[card_u] = v;
    position_of[card_v] = u;
  }

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

      vector<pair<int, Operation>> transitions = enumerate_transitions(v);

      /*
       * 同じ移動先への複数操作は、
       * BFS上では同じ辺として扱う。
       */
      array<bool, 400> used_to{};

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
    }

    assert(dist[goal] != -1);

    /*
     * 対象カードが通る位置列を復元。
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
     * 同じ位置遷移を実現する操作の中から、
     * 現在盤面に対して最良のものを毎回選ぶ。
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

  /*
   * AtCoder提出時はこちら。
   */
  // solver.output_stdout();

  /*
   * ローカルでファイル出力したい場合は、
   * 上の output_stdout() をコメントアウトして使用する。
   */
  solver.output_to_file("output.txt");

  return 0;
}