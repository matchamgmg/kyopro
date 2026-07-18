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

  vector<int> card_at;
  vector<int> position_of;

  vector<string> V;
  vector<string> H;

  vector<vector<int>> graph;

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

  bool cell_is_available(int v) const { return !fixed_cell[v]; }

  /*
   * 横長区間 [left, left + length) が使用可能か判定する。
   *
   * 高さ1なので、確認する壁は横方向の壁のみ。
   */
  bool valid_horizontal_segment(int r, int left, int length) const {
    if (left < 0 || left + length > N) {
      return false;
    }

    for (int c = left; c < left + length; ++c) {
      if (!cell_is_available(id(r, c))) {
        return false;
      }
    }

    for (int c = left; c + 1 < left + length; ++c) {
      if (V[r][c] == '1') {
        return false;
      }
    }

    return true;
  }

  /*
   * 縦長区間 [top, top + length) が使用可能か判定する。
   */
  bool valid_vertical_segment(int top, int c, int length) const {
    if (top < 0 || top + length > N) {
      return false;
    }

    for (int r = top; r < top + length; ++r) {
      if (!cell_is_available(id(r, c))) {
        return false;
      }
    }

    for (int r = top; r + 1 < top + length; ++r) {
      if (H[r][c] == '1') {
        return false;
      }
    }

    return true;
  }

  /*
   * 現在位置vにある対象カードを、1回の細長い長方形操作で
   * 移動できる遷移をすべて生成する。
   */
  vector<pair<int, Operation>> enumerate_transitions(int v) const {
    vector<pair<int, Operation>> transitions;

    int r = row(v);
    int c = col(v);

    /*
     * 横方向への移動。
     *
     * 幅2dの左半分にカードがあれば右へd、
     * 右半分にあれば左へd移動する。
     */
    for (int d = 1; d < N; ++d) {
      int width = 2 * d;

      /*
       * 右へdマス移動。
       * カードは左半分のoffset番目にある。
       */
      for (int offset = 0; offset < d; ++offset) {
        int left = c - offset;

        if (!valid_horizontal_segment(r, left, width)) {
          continue;
        }

        int to_c = c + d;

        if (to_c >= N) {
          continue;
        }

        Operation op{'H', r, left, 1, width};

        transitions.push_back({id(r, to_c), op});
      }

      /*
       * 左へdマス移動。
       * カードは右半分のoffset番目にある。
       */
      for (int offset = 0; offset < d; ++offset) {
        int left = c - d - offset;

        if (!valid_horizontal_segment(r, left, width)) {
          continue;
        }

        int to_c = c - d;

        if (to_c < 0) {
          continue;
        }

        Operation op{'H', r, left, 1, width};

        transitions.push_back({id(r, to_c), op});
      }
    }

    /*
     * 縦方向への移動。
     */
    for (int d = 1; d < N; ++d) {
      int height = 2 * d;

      /*
       * 下へdマス移動。
       */
      for (int offset = 0; offset < d; ++offset) {
        int top = r - offset;

        if (!valid_vertical_segment(top, c, height)) {
          continue;
        }

        int to_r = r + d;

        if (to_r >= N) {
          continue;
        }

        Operation op{'V', top, c, height, 1};

        transitions.push_back({id(to_r, c), op});
      }

      /*
       * 上へdマス移動。
       */
      for (int offset = 0; offset < d; ++offset) {
        int top = r - d - offset;

        if (!valid_vertical_segment(top, c, height)) {
          continue;
        }

        int to_r = r - d;

        if (to_r < 0) {
          continue;
        }

        Operation op{'V', top, c, height, 1};

        transitions.push_back({id(to_r, c), op});
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
    vector<Operation> previous_operation(M);

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

      for (const auto &[to, op] : transitions) {
        if (dist[to] != -1) {
          continue;
        }

        dist[to] = dist[v] + 1;
        previous[to] = v;
        previous_operation[to] = op;
        que.push(to);
      }
    }

    /*
     * 幅2・高さ2の隣接交換を含み、
     * 未固定領域は連結なので必ず到達できる。
     */
    assert(dist[goal] != -1);

    vector<Operation> operations;

    int current = goal;

    while (current != start) {
      operations.push_back(previous_operation[current]);
      current = previous[current];
    }

    reverse(operations.begin(), operations.end());

    for (const auto &op : operations) {
      apply_operation(op);
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
  solver.output();

  return 0;
}