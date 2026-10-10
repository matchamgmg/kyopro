#include <bits/stdc++.h>
using namespace std;

namespace integrated {

struct ParsedProblem {
  int N = 0;
  int K = 0;
  vector<string> C;
};

struct ValidationResult {
  bool valid_output = false;
  bool complete = false;
  int action_count = 0;
  int remaining = 0;
  long long score = (1LL << 60);
  string reason;
};

ParsedProblem parse_problem(const string &input) {
  istringstream iss(input);
  ParsedProblem p;
  if (!(iss >> p.N >> p.K))
    return p;
  p.C.resize(p.N);
  for (auto &s : p.C)
    iss >> s;
  return p;
}

ValidationResult validate_output(const string &input, const string &output) {
  ValidationResult res;
  auto p = parse_problem(input);
  if (p.N <= 0 || (int)p.C.size() != p.N) {
    res.reason = "input parse failed";
    return res;
  }

  const int V = p.N * p.N;
  vector<vector<int>> board(V);
  vector<int> nest_color(V, -1);

  auto id = [&](int x, int y) {
    return x * p.N + y;
  };
  auto inside = [&](int x, int y) {
    return 0 <= x && x < p.N && 0 <= y && y < p.N;
  };

  for (int x = 0; x < p.N; ++x) {
    for (int y = 0; y < p.N; ++y) {
      char c = p.C[x][y];
      if ('A' <= c && c <= 'L')
        nest_color[id(x, y)] = c - 'A';
      if ('a' <= c && c <= 'l')
        board[id(x, y)].push_back(c - 'a');
    }
  }

  auto home = [&](int v) {
    int c = nest_color[v];
    if (c < 0)
      return;
    while (!board[v].empty() && board[v].back() == c)
      board[v].pop_back();
  };

  const array<int, 4> dx = {-1, 1, 0, 0};
  const array<int, 4> dy = {0, 0, -1, 1};

  auto dir_id = [&](char d) -> int {
    if (d == 'U')
      return 0;
    if (d == 'D')
      return 1;
    if (d == 'L')
      return 2;
    if (d == 'R')
      return 3;
    return -1;
  };

  istringstream oss(output);
  string line;
  int action_count = 0;

  while (getline(oss, line)) {
    if (line.find_first_not_of(" \t\r\n") == string::npos)
      continue;

    istringstream ls(line);
    int x, y, k, l;
    char dc;
    string extra;

    if (!(ls >> x >> y >> k >> dc >> l) || (ls >> extra)) {
      res.reason = "malformed action line";
      return res;
    }

    ++action_count;
    if (action_count > 100000) {
      res.reason = "more than 100000 actions";
      return res;
    }

    int d = dir_id(dc);
    if (d < 0 || !inside(x, y) || p.C[x][y] == '#') {
      res.reason = "invalid source or direction";
      return res;
    }

    int source = id(x, y);
    int h = (int)board[source].size();
    if (h == 0 || !(0 <= k && k < h) || !(1 <= l && l <= k + 1)) {
      res.reason = "illegal h/k/l";
      return res;
    }

    int nx = x;
    int ny = y;
    for (int step = 0; step < l; ++step) {
      nx += dx[d];
      ny += dy[d];
      if (!inside(nx, ny) || p.C[nx][ny] == '#') {
        res.reason = "jump crosses wall or board edge";
        return res;
      }
    }

    int destination = id(nx, ny);
    int moved = h - k;
    if ((int)board[destination].size() + moved > 8) {
      res.reason = "destination height exceeds 8";
      return res;
    }

    for (int i = 0; i < moved; ++i) {
      int c = board[source].back();
      board[source].pop_back();
      board[destination].push_back(c);
    }

    home(source);
    home(destination);
  }

  int remaining = 0;
  for (const auto &tower : board)
    remaining += (int)tower.size();

  res.valid_output = true;
  res.complete = (remaining == 0);
  res.action_count = action_count;
  res.remaining = remaining;
  res.score = (long long)action_count + 100000LL * remaining;
  res.reason = res.complete ? "ok" : "slimes remain";
  return res;
}

} // namespace integrated

namespace logic1 {

using namespace std;

constexpr int INF = 1e9;

constexpr int MAX_ACTIONS = 100000;

constexpr int MAX_RECURSION = 40;

constexpr array<int, 4> DX = {-1, 1, 0, 0};

constexpr array<int, 4> DY = {0, 0, -1, 1};

constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Problem {

  int N, K;

  vector<string> C;

  Problem() {

    cin >> N >> K;

    C.resize(N);

    for (auto &row : C) {

      cin >> row;
    }
  }
};

struct Action {

  int x;

  int y;

  int k;

  int d;

  int l;

  friend ostream &operator<<(ostream &os, const Action &a) {

    return os << a.x << ' ' << a.y << ' ' << a.k << ' ' << DIR_CHAR[a.d] << ' ' << a.l;
  }
};

struct Tree {

  // adj[v] の bit d が立っていれば d 方向へ木の辺がある

  vector<uint8_t> adj;

  // 巣を根とした木

  vector<int> parent;

  vector<int> depth;

  vector<int> child_count;

  vector<char> used;

  int edge_count = 0;
};

struct ParkingRecord {

  int origin = -1;

  int parking = -1;

  int color = -1;

  int moved_size = 0;

  // parking に元から存在していた高さ

  int parking_base_height = 0;

  // 退避直後の origin の状態

  vector<int> origin_base;
};

struct SimulationResult {

  bool success = false;

  vector<Action> actions;

  int homed = 0;

  int remaining = 0;
};

struct Solver {

  Problem P;

  int V;

  // 固定情報

  vector<int> nest;

  vector<int> nest_color;

  vector<vector<int>> terminals;

  vector<Tree> trees;

  // 何色の木がこのセルを通っているか

  vector<int> tree_usage_count;

  // =========================================================

  // simulation state

  // =========================================================

  // board[v] : bottom -> top

  vector<vector<int>> board;

  // 一時退避中の block が上に載っているセル

  vector<char> parking_lock;

  // parking の復元先として予約されているセル

  vector<int> parking_origin_reserved;

  vector<ParkingRecord> parkings;

  // ensure_capacity の再帰ループ防止

  vector<char> resolving;

  vector<Action> answer;

  int initial_slimes = 0;

  int homed_slimes = 0;

  /**

   * 1回の simulation で

   * 同色blockを最大何匹までまとめて運ぶか。

   *

   * 1回目: 8

   * デッドロックしたら2回目: 4

   */

  int block_limit = 8;

  Solver() : V(P.N * P.N) {

    nest.assign(P.K, -1);

    nest_color.assign(V, -1);

    terminals.resize(P.K);

    trees.resize(P.K);

    tree_usage_count.assign(V, 0);

    initialize_problem();

    build_all_trees();
  }

  // =========================================================

  // Grid

  // =========================================================

  int cell_id(int x, int y) const {

    return x * P.N + y;
  }

  pair<int, int> cell_pos(int v) const {

    return {

        v / P.N,

        v % P.N,

    };
  }

  bool inside(int x, int y) const {

    return 0 <= x && x < P.N && 0 <= y && y < P.N;
  }

  bool passable(int x, int y) const {

    return inside(x, y) && P.C[x][y] != '#';
  }

  int next_cell(int v, int d) const {

    auto [x, y] = cell_pos(v);

    int nx = x + DX[d];

    int ny = y + DY[d];

    if (!passable(nx, ny)) {

      return -1;
    }

    return cell_id(nx, ny);
  }

  int direction_between(int u, int v) const {

    auto [ux, uy] = cell_pos(u);

    auto [vx, vy] = cell_pos(v);

    for (int d = 0; d < 4; ++d) {

      if (ux + DX[d] == vx && uy + DY[d] == vy) {

        return d;
      }
    }

    return -1;
  }

  bool is_nest_cell(int v) const {

    return nest_color[v] != -1;
  }

  // =========================================================

  // Input

  // =========================================================

  void initialize_problem() {

    for (int x = 0; x < P.N; ++x) {

      for (int y = 0; y < P.N; ++y) {

        int v = cell_id(x, y);

        char c = P.C[x][y];

        if ('A' <= c && c <= 'L') {

          int color = c - 'A';

          nest[color] = v;

          nest_color[v] = color;
        }

        if ('a' <= c && c <= 'l') {

          int color = c - 'a';

          terminals[color].push_back(v);

          ++initial_slimes;
        }
      }
    }
  }

  // =========================================================

  // Fixed greedy tree

  // =========================================================

  void add_tree_edge(Tree &tree, int u, int v) const {

    int d = direction_between(u, v);

    assert(d != -1);

    uint8_t bu = uint8_t(1u << d);

    uint8_t bv = uint8_t(1u << (d ^ 1));

    if (!(tree.adj[u] & bu)) {

      ++tree.edge_count;
    }

    tree.adj[u] |= bu;

    tree.adj[v] |= bv;
  }

  bool has_tree_edge(const Tree &tree, int v, int d) const {

    return tree.adj[v] & uint8_t(1u << d);
  }

  /**

   * 現在できている木に一番近い未接続スライムを、

   * multi-source BFS で1体ずつつなぐ。

   */

  Tree build_greedy_tree(int color) const {

    Tree tree;

    tree.adj.assign(V, 0);

    vector<char> in_tree(V, false);

    vector<char> is_terminal(V, false);

    for (int v : terminals[color]) {

      is_terminal[v] = true;
    }

    int root = nest[color];

    in_tree[root] = true;

    while (true) {

      bool finished = true;

      for (int v : terminals[color]) {

        if (!in_tree[v]) {

          finished = false;

          break;
        }
      }

      if (finished) {

        break;
      }

      queue<int> que;

      vector<int> prev(V, -2);

      /**

       * 現在の木全体をBFS始点にする。

       */

      for (int v = 0; v < V; ++v) {

        if (!in_tree[v]) {

          continue;
        }

        prev[v] = -1;

        que.push(v);
      }

      int target = -1;

      while (!que.empty()) {

        int v = que.front();

        que.pop();

        if (is_terminal[v] && !in_tree[v]) {

          target = v;

          break;
        }

        for (int d = 0; d < 4; ++d) {

          int nv = next_cell(v, d);

          if (nv == -1) {

            continue;
          }

          if (prev[nv] != -2) {

            continue;
          }

          prev[nv] = v;

          que.push(nv);
        }
      }

      assert(target != -1);

      int cur = target;

      while (prev[cur] != -1) {

        int p = prev[cur];

        add_tree_edge(tree, cur, p);

        in_tree[cur] = true;

        in_tree[p] = true;

        cur = p;
      }
    }

    return tree;
  }

  void build_root_info(int color, Tree &tree) {

    tree.parent.assign(V, -2);

    tree.depth.assign(V, -1);

    tree.child_count.assign(V, 0);

    tree.used.assign(V, false);

    int root = nest[color];

    queue<int> que;

    tree.parent[root] = -1;

    tree.depth[root] = 0;

    tree.used[root] = true;

    que.push(root);

    while (!que.empty()) {

      int v = que.front();

      que.pop();

      for (int d = 0; d < 4; ++d) {

        if (!has_tree_edge(tree, v, d)) {

          continue;
        }

        int nv = next_cell(v, d);

        if (tree.parent[nv] != -2) {

          continue;
        }

        tree.parent[nv] = v;

        tree.depth[nv] = tree.depth[v] + 1;

        tree.used[nv] = true;

        ++tree.child_count[v];

        que.push(nv);
      }
    }
  }

  void build_all_trees() {

    int total_edges = 0;

    for (int color = 0; color < P.K; ++color) {

      trees[color] = build_greedy_tree(color);

      build_root_info(color, trees[color]);

      total_edges += trees[color].edge_count;

      for (int v = 0; v < V; ++v) {

        if (trees[color].used[v]) {

          ++tree_usage_count[v];
        }
      }

      cerr << "tree " << char('a' + color) << " edges=" << trees[color].edge_count << '\n';
    }

    cerr << "total tree edges=" << total_edges << '\n';
  }

  // =========================================================

  // Simulation initialization

  // =========================================================

  void reset_simulation_state() {

    board.assign(V, {});

    for (int color = 0; color < P.K; ++color) {

      for (int v : terminals[color]) {

        board[v].push_back(color);
      }
    }

    parking_lock.assign(V, false);

    parking_origin_reserved.assign(V, 0);

    parkings.clear();

    resolving.assign(V, false);

    answer.clear();

    homed_slimes = 0;
  }

  // =========================================================

  // Tower / block

  // =========================================================

  int top_color(int v) const {

    if (board[v].empty()) {

      return -1;
    }

    return board[v].back();
  }

  /**

   * 最上部に連続している同色blockの高さ。

   *

   * bottom dddccc top

   *

   * -> 3

   */

  int top_block_size(int v) const {

    if (board[v].empty()) {

      return 0;
    }

    int color = board[v].back();

    int count = 0;

    for (int i = (int)board[v].size() - 1; i >= 0 && board[v][i] == color; --i) {

      ++count;
    }

    return count;
  }

  /**

   * 今回の simulation で一度に運べる

   * 同色blockの最大サイズ。

   */

  int movable_block_size(int v) const {

    return min(top_block_size(v), block_limit);
  }

  int remaining_slimes() const {

    int result = 0;

    for (const auto &tower : board) {

      result += tower.size();
    }

    return result;
  }

  void home_cell(int v) {

    int color = nest_color[v];

    if (color == -1) {

      return;
    }

    while (!board[v].empty() && board[v].back() == color) {

      board[v].pop_back();

      ++homed_slimes;
    }
  }

  // =========================================================

  // Actual operation

  // =========================================================

  bool execute_action(int source, int k, int d, int l) {

    if ((int)answer.size() >= MAX_ACTIONS) {

      return false;
    }

    int h = board[source].size();

    if (h == 0) {

      return false;
    }

    if (!(0 <= k && k < h)) {

      return false;
    }

    if (!(1 <= l && l <= k + 1)) {

      return false;
    }

    int destination = source;

    /**

     * 途中にtowerがあっても問題ない。

     * 壁・盤外だけ禁止。

     */

    for (int step = 0; step < l; ++step) {

      destination = next_cell(destination, d);

      if (destination == -1) {

        return false;
      }
    }

    int moved = h - k;

    if ((int)board[destination].size() + moved > 8) {

      return false;
    }

    auto [x, y] = cell_pos(source);

    answer.push_back({

        x,

        y,

        k,

        d,

        l,

    });

    /**

     * 上から取り出すため移動部分は反転する。

     *

     * 今回動かす部分は同色なので

     * blockとしては問題ない。

     */

    for (int i = 0; i < moved; ++i) {

      int color = board[source].back();

      board[source].pop_back();

      board[destination].push_back(color);
    }

    home_cell(source);

    home_cell(destination);

    return true;
  }

  /**

   * source の最上部同色blockから

   * moved_size匹だけ destination に移動する。

   *

   * l > 1 にも対応。

   */

  bool move_top_same_color_some(int source, int destination, int moved_size) {

    if (board[source].empty()) {

      return false;
    }

    if (!(1 <= moved_size && moved_size <= top_block_size(source))) {

      return false;
    }

    if ((int)board[destination].size() + moved_size > 8) {

      return false;
    }

    auto [sx, sy] = cell_pos(source);

    auto [tx, ty] = cell_pos(destination);

    int d = -1;

    int l = -1;

    if (sx == tx) {

      if (sy < ty) {

        d = 3;

        l = ty - sy;

      } else if (ty < sy) {

        d = 2;

        l = sy - ty;
      }

    } else if (sy == ty) {

      if (sx < tx) {

        d = 1;

        l = tx - sx;

      } else if (tx < sx) {

        d = 0;

        l = sx - tx;
      }
    }

    if (d == -1 || l <= 0) {

      return false;
    }

    int h = board[source].size();

    int k = h - moved_size;

    /**

     * l <= k+1

     */

    if (l > k + 1) {

      return false;
    }

    return execute_action(source, k, d, l);
  }

  // =========================================================

  // Tree utilities

  // =========================================================

  /**

   * v から根方向へ進んで、

   *

   * ・次の分岐点

   * ・巣

   *

   * の最初のものを返す。

   */

  int next_branch(int color, int v) const {

    const auto &tree = trees[color];

    if (!tree.used[v]) {

      return -1;
    }

    int p = tree.parent[v];

    while (p != -1) {

      if (p == nest[color]) {

        return p;
      }

      if (tree.child_count[p] >= 2) {

        return p;
      }

      p = tree.parent[p];
    }

    return -1;
  }

  // =========================================================

  // Temporary parking

  // =========================================================

  int find_parking_cell(int origin, int forbidden1, int forbidden2, int moving_color, int moving_size) const {

    int best = -1;

    int best_score = INF;

    for (int d = 0; d < 4; ++d) {

      int v = next_cell(origin, d);

      if (v == -1 || v == forbidden1 || v == forbidden2) {

        continue;
      }

      /**

       * 帰巣で消えてしまう危険を避ける。

       */

      if (is_nest_cell(v)) {

        continue;
      }

      if (parking_lock[v]) {

        continue;
      }

      if (parking_origin_reserved[v] > 0) {

        continue;
      }

      if ((int)board[v].size() + moving_size > 8) {

        continue;
      }

      int score = 100 * tree_usage_count[v];

      /**

       * 空きセル優先。

       * 次に同色。

       * 異色も高さが許せば使用可能。

       */

      if (board[v].empty()) {

        score += 0;

      } else if (top_color(v) == moving_color) {

        score += 10;

      } else {

        score += 20;
      }

      score += board[v].size();

      if (score < best_score) {

        best_score = score;

        best = v;
      }
    }

    return best;
  }

  /**

   * origin の最上部blockを一時退避。

   *

   * block_limit=4 の simulation では

   * 一度に最大4匹しか退避しない。

   */

  bool park_top_block(int origin, int forbidden1, int forbidden2, int recursion_depth) {

    if (recursion_depth > MAX_RECURSION) {

      return false;
    }

    if (board[origin].empty()) {

      return false;
    }

    if (parking_lock[origin]) {

      return false;
    }

    if (parking_origin_reserved[origin] > 0) {

      return false;
    }

    int color = top_color(origin);

    int moved_size = movable_block_size(origin);

    if (moved_size <= 0) {

      return false;
    }

    int parking = find_parking_cell(origin, forbidden1, forbidden2, color, moved_size);

    if (parking == -1) {

      return false;
    }

    int parking_base_height = board[parking].size();

    if (!move_top_same_color_some(origin, parking, moved_size)) {

      return false;
    }

    /**

     * action後は source側で帰巣が起こる

     * 可能性があるため、その後の状態を保存。

     */

    vector<int> origin_base = board[origin];

    parkings.push_back({

        origin,

        parking,

        color,

        moved_size,

        parking_base_height,

        move(origin_base),

    });

    parking_lock[parking] = true;

    ++parking_origin_reserved[origin];

    return true;
  }

  /**

   * 戻せる一時退避を復元。

   */

  bool try_restore_parkings() {

    bool any = false;

    bool changed = true;

    while (changed) {

      changed = false;

      for (int i = (int)parkings.size() - 1; i >= 0; --i) {

        const auto rec = parkings[i];

        /**

         * origin が退避直後と同じ状態に

         * 戻っている場合のみ復元。

         */

        if (board[rec.origin] != rec.origin_base) {

          continue;
        }

        if ((int)board[rec.origin].size() + rec.moved_size > 8) {

          continue;
        }

        int expected_height = rec.parking_base_height + rec.moved_size;

        if ((int)board[rec.parking].size() != expected_height) {

          continue;
        }

        bool correct_block = true;

        for (int j = rec.parking_base_height; j < expected_height; ++j) {

          if (board[rec.parking][j] != rec.color) {

            correct_block = false;

            break;
          }
        }

        if (!correct_block) {

          continue;
        }

        int d = direction_between(rec.parking, rec.origin);

        if (d == -1) {

          continue;
        }

        /**

         * parking に元からあった部分を残し、

         * 上の退避blockだけ戻す。

         */

        if (!execute_action(rec.parking, rec.parking_base_height, d, 1)) {

          continue;
        }

        parking_lock[rec.parking] = false;

        --parking_origin_reserved[rec.origin];

        parkings.erase(parkings.begin() + i);

        changed = true;

        any = true;
      }
    }

    return any;
  }

  // =========================================================

  // Capacity resolution

  // =========================================================

  /**

   * destination に incoming_size 匹置けるようにする。

   *

   * destination の色は問わない。

   *

   * 高さ8を超える場合だけ、

   * destination のtop blockを先に進める。

   */

  bool ensure_capacity(int destination, int incoming_color, int incoming_size, int incoming_source,

                       int incoming_stop_target, int recursion_depth) {

    if (recursion_depth > MAX_RECURSION) {

      return false;
    }

    if (!parking_lock[destination] && (int)board[destination].size() + incoming_size <= 8) {

      return true;
    }

    if (parking_lock[destination]) {

      return false;
    }

    if (resolving[destination]) {

      return false;
    }

    resolving[destination] = true;

    while ((int)board[destination].size() + incoming_size > 8) {

      if (board[destination].empty()) {

        break;
      }

      int before_height = board[destination].size();

      /**

       * まず destination のtop blockを

       * 自分自身の木に沿って進める。

       */

      advance_top_block(destination, recursion_depth + 1, incoming_source);

      if ((int)board[destination].size() + incoming_size <= 8) {

        resolving[destination] = false;

        return true;
      }

      /**

       * 高さが減ったなら、

       * 必要ならさらに次のblockも処理。

       */

      if ((int)board[destination].size() < before_height) {

        continue;
      }

      /**

       * 木に沿って進められなかった場合のみ

       * 一時退避。

       */

      int forbidden2 = -1;

      if (destination != incoming_stop_target && trees[incoming_color].used[destination]) {

        forbidden2 = trees[incoming_color].parent[destination];
      }

      if (!park_top_block(destination, incoming_source, forbidden2, recursion_depth + 1)) {

        resolving[destination] = false;

        return false;
      }
    }

    bool ok = !parking_lock[destination] && (int)board[destination].size() + incoming_size <= 8;

    resolving[destination] = false;

    return ok;
  }

  // =========================================================

  // Main movement

  // =========================================================

  /**

   * start のtop blockを、

   * その色の固定木上で

   * 次の分岐点または巣まで進める。

   *

   * l > 1 を利用可能。

   *

   * 同色blockが途中に存在した場合は、

   * そこで止まってマージする。

   */

  bool advance_top_block(int start, int recursion_depth, int forbidden) {

    if (recursion_depth > MAX_RECURSION) {

      return false;
    }

    if (board[start].empty()) {

      return false;
    }

    if (parking_lock[start]) {

      return false;
    }

    int color = top_color(start);

    if (color == -1) {

      return false;
    }

    if (!trees[color].used[start]) {

      return false;
    }

    int target = next_branch(color, start);

    if (target == -1) {

      return false;
    }

    int current = start;

    /**

     * block_limit=8なら最大8匹。

     * block_limit=4なら最大4匹。

     */

    int packet_size = movable_block_size(start);

    int before_actions = answer.size();

    while (current != target) {

      if (board[current].empty()) {

        break;
      }

      if (top_color(current) != color) {

        break;
      }

      /**

       * 再帰処理で盤面が変化している可能性もあるため

       * 再確認。

       */

      packet_size = min(packet_size, movable_block_size(current));

      if (packet_size <= 0) {

        break;
      }

      int first_parent = trees[color].parent[current];

      if (first_parent < 0 || first_parent == forbidden) {

        break;
      }

      int h = board[current].size();

      /**

       * packet_size匹動かす場合、

       *

       * k = h - packet_size

       * l <= k+1

       */

      int k = h - packet_size;

      int max_jump = k + 1;

      int direction = direction_between(current, first_parent);

      if (direction == -1) {

        break;
      }

      /**

       * 同一直線上で、

       * 今回の操作で到達可能な祖先を列挙。

       */

      vector<int> path;

      int v = current;

      for (int l = 1; l <= max_jump; ++l) {

        int p = trees[color].parent[v];

        if (p < 0) {

          break;
        }

        /**

         * 木が曲がったら

         * 1回のジャンプでは進めない。

         */

        if (direction_between(v, p) != direction) {

          break;
        }

        if (p == forbidden) {

          break;
        }

        path.push_back(p);

        v = p;

        /**

         * 同色blockがあれば、

         * マージするためそこを終点にする。

         *

         * 同色を飛び越えない。

         */

        if (!board[v].empty() && top_color(v) == color) {

          break;
        }

        if (v == target) {

          break;
        }
      }

      bool moved = false;

      /**

       * ----------------------------------------------------

       * まずは盤面を動かさず、

       * 現状で着地可能な最遠地点を探す。

       * ----------------------------------------------------

       */

      for (int i = (int)path.size() - 1; i >= 0; --i) {

        int destination = path[i];

        if (parking_lock[destination]) {

          continue;
        }

        if ((int)board[destination].size() + packet_size > 8) {

          continue;
        }

        if (!move_top_same_color_some(current, destination, packet_size)) {

          continue;
        }

        current = destination;

        moved = true;

        break;
      }

      /**

       * ----------------------------------------------------

       * 直接着地可能な祖先がない場合だけ、

       * 直後の親を再帰的に空ける。

       * ----------------------------------------------------

       */

      if (!moved) {

        if (!ensure_capacity(first_parent, color, packet_size, current, target, recursion_depth + 1)) {

          break;
        }

        int capacity = 8 - (int)board[first_parent].size();

        int moved_size = min(packet_size, capacity);

        if (moved_size <= 0) {

          break;
        }

        if (!move_top_same_color_some(current, first_parent, moved_size)) {

          break;
        }

        current = first_parent;

        moved = true;
      }

      if (!moved) {

        break;
      }

      /**

       * 自身の巣なら帰巣済み。

       */

      if (current == nest[color]) {

        break;
      }

      if (board[current].empty()) {

        break;
      }

      if (top_color(current) != color) {

        break;
      }

      /**

       * 同色に着地した場合はマージ。

       *

       * ただし次に一度に運ぶ量は

       * block_limitまで。

       */

      packet_size = movable_block_size(current);
    }

    return (int)answer.size() > before_actions;
  }

  // =========================================================

  // Candidate selection

  // =========================================================

  struct Candidate {

    int depth;

    int block_size;

    int cell;
  };

  /**

   * 現在topに露出しているblockについて、

   * 自身の木上で深いものから処理。

   */

  vector<Candidate> build_candidates() const {

    vector<Candidate> result;

    for (int v = 0; v < V; ++v) {

      if (board[v].empty()) {

        continue;
      }

      if (parking_lock[v]) {

        continue;
      }

      int color = top_color(v);

      if (color == -1) {

        continue;
      }

      if (!trees[color].used[v]) {

        continue;
      }

      if (v == nest[color]) {

        continue;
      }

      int depth = trees[color].depth[v];

      if (depth < 0) {

        continue;
      }

      result.push_back({

          depth,

          movable_block_size(v),

          v,

      });
    }

    sort(result.begin(), result.end(), [](const Candidate &a, const Candidate &b) {
      if (a.depth != b.depth) {

        return a.depth > b.depth;
      }

      if (a.block_size != b.block_size) {

        return a.block_size > b.block_size;
      }

      return a.cell < b.cell;
    });

    return result;
  }

  // =========================================================

  // One complete simulation

  // =========================================================

  SimulationResult run_simulation(int new_block_limit) {

    block_limit = new_block_limit;

    /**

     * 盤面・parking・answer等をすべて

     * 初期状態へ戻す。

     *

     * trees は固定なので作り直さない。

     */

    reset_simulation_state();

    cerr << "\n=== simulation"

         << " block_limit=" << block_limit << " ===\n";

    bool deadlock = false;

    while (remaining_slimes() > 0 && (int)answer.size() < MAX_ACTIONS) {

      /**

       * 戻せる一時退避を先に復元。

       */

      try_restore_parkings();

      auto candidates = build_candidates();

      bool progressed = false;

      for (const auto &cand : candidates) {

        int before = answer.size();

        advance_top_block(cand.cell, 0, -1);

        if ((int)answer.size() > before) {

          progressed = true;

          break;
        }
      }

      if (progressed) {

        continue;
      }

      /**

       * 通常処理が進まなくても、

       * parking復元だけで進展する場合がある。

       */

      if (try_restore_parkings()) {

        continue;
      }

      /**

       * スライムが残っているのに

       * 1手も生成できない。

       *

       * これをデッドロックとする。

       */

      deadlock = true;

      cerr << "deadlock"

           << " block_limit=" << block_limit << " actions=" << answer.size() << " remaining=" << remaining_slimes()

           << " parking=" << parkings.size() << '\n';

      break;
    }

    SimulationResult result;

    result.success = !deadlock && remaining_slimes() == 0;

    result.actions = answer;

    result.homed = homed_slimes;

    result.remaining = remaining_slimes();

    cerr << "result"

         << " block_limit=" << block_limit << " success=" << result.success << " actions=" << result.actions.size()

         << " homed=" << result.homed << "/" << initial_slimes << " remaining=" << result.remaining << '\n';

    return result;
  }

  // =========================================================

  // Solve

  // =========================================================

  void solve() {

    /**

     * ================================================

     * 1回目

     *

     * 同色blockを最大8匹までまとめる。

     * ================================================

     */

    auto result8 = run_simulation(8);

    if (result8.success) {

      cerr << "use block_limit=8\n";

      for (const auto &action : result8.actions) {

        cout << action << '\n';
      }

      return;
    }

    /**

     * ================================================

     * デッドロックしたら、

     * 盤面を完全に初期状態へ戻して

     * block_limit=4 で最初から再試行。

     * ================================================

     */

    cerr << "\nretry from initial state"

         << " with block_limit=4\n";

    auto result4 = run_simulation(4);

    if (result4.success) {

      cerr << "use block_limit=4\n";

      for (const auto &action : result4.actions) {

        cout << action << '\n';
      }

      return;
    }

    /**

     * 両方失敗した場合。

     *

     * デバッグ用として、

     * remainingが少ない方を出力する。

     */

    cerr << "both simulations failed\n"

         << "limit8:"

         << " actions=" << result8.actions.size() << " remaining=" << result8.remaining << '\n'

         << "limit4:"

         << " actions=" << result4.actions.size() << " remaining=" << result4.remaining << '\n';

    const auto &best = (result4.remaining < result8.remaining ||

                        (result4.remaining == result8.remaining && result4.actions.size() < result8.actions.size()))

                           ? result4

                           : result8;

    for (const auto &action : best.actions) {

      cout << action << '\n';
    }
  }
};

string run(const string &input) {
  istringstream iss(input);
  ostringstream oss;
  ostringstream err;

  auto *old_in = cin.rdbuf(iss.rdbuf());
  auto *old_out = cout.rdbuf(oss.rdbuf());
  auto *old_err = cerr.rdbuf(err.rdbuf());
  cin.clear();

  Solver solver;
  solver.solve();

  cin.clear();
  cin.rdbuf(old_in);
  cout.rdbuf(old_out);
  cerr.rdbuf(old_err);
  return oss.str();
}

} // namespace logic1

namespace logic2 {

using namespace std;

constexpr int INF = 1e9;

constexpr int MAX_ACTIONS = 100000;

constexpr int MAX_RECURSION = 40;

constexpr array<int, 4> DX = {-1, 1, 0, 0};

constexpr array<int, 4> DY = {0, 0, -1, 1};

constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Problem {

  int N, K;

  vector<string> C;

  Problem() {

    cin >> N >> K;

    C.resize(N);

    for (auto &row : C) {

      cin >> row;
    }
  }
};

struct Action {

  int x;

  int y;

  int k;

  int d;

  int l;

  friend ostream &operator<<(ostream &os, const Action &a) {

    return os << a.x << ' ' << a.y << ' ' << a.k << ' ' << DIR_CHAR[a.d] << ' ' << a.l;
  }
};

struct Tree {

  vector<uint8_t> adj;

  vector<int> parent;

  vector<int> depth;

  vector<int> child_count;

  vector<char> used;

  int edge_count = 0;
};

struct ParkingRecord {

  int origin = -1;

  int parking = -1;

  int color = -1;

  int moved_size = 0;

  int parking_base_height = 0;

  vector<int> origin_base;
};

struct SimulationResult {

  bool success = false;

  vector<Action> actions;

  int homed = 0;

  int remaining = 0;
};

struct Solver {

  Problem P;

  int V;

  // =========================================================

  // Fixed information

  // =========================================================

  vector<int> nest;

  vector<int> nest_color;

  vector<vector<int>> terminals;

  vector<Tree> trees;

  vector<int> tree_usage_count;

  int initial_slimes = 0;

  // =========================================================

  // Simulation state

  // =========================================================

  // bottom -> top

  vector<vector<int>> board;

  vector<char> parking_lock;

  vector<int> parking_origin_reserved;

  vector<ParkingRecord> parkings;

  vector<char> resolving;

  /**

   * deferred[v]:

   *

   * 現在のtop blockを通常処理では動かさない。

   *

   * ・高さ4で集積待ち

   * ・待機blockに進路を妨害されて待機

   *

   * の両方に使用。

   */

  vector<char> deferred;

  /**

   * 他色blockの上で高さ4が形成されたblock。

   *

   * このblockが初めて空きマスへ移動した瞬間に

   * deferredへ移行する。

   */

  vector<char> pending_wait;

  /**

   * true:

   *   高さ4による待機を行うフェーズ。

   *

   * 全ての通常blockがなくなったらfalseにし、

   * 以後二度と待機状態は作らない。

   */

  bool waiting_phase = true;

  vector<Action> answer;

  int homed_slimes = 0;

  /**

   * 1回に運ぶ同色blockの最大高さ。

   *

   * 1回目: 8

   * deadlock時: 4

   */

  int block_limit = 8;

  Solver() : V(P.N * P.N) {

    nest.assign(P.K, -1);

    nest_color.assign(V, -1);

    terminals.resize(P.K);

    trees.resize(P.K);

    tree_usage_count.assign(V, 0);

    initialize_problem();

    build_all_trees();
  }

  // =========================================================

  // Grid

  // =========================================================

  int cell_id(int x, int y) const {

    return x * P.N + y;
  }

  pair<int, int> cell_pos(int v) const {

    return {

        v / P.N,

        v % P.N,

    };
  }

  bool inside(int x, int y) const {

    return 0 <= x && x < P.N && 0 <= y && y < P.N;
  }

  bool passable(int x, int y) const {

    return inside(x, y) && P.C[x][y] != '#';
  }

  int next_cell(int v, int d) const {

    auto [x, y] = cell_pos(v);

    int nx = x + DX[d];

    int ny = y + DY[d];

    if (!passable(nx, ny)) {

      return -1;
    }

    return cell_id(nx, ny);
  }

  int direction_between(int u, int v) const {

    auto [ux, uy] = cell_pos(u);

    auto [vx, vy] = cell_pos(v);

    for (int d = 0; d < 4; ++d) {

      if (ux + DX[d] == vx && uy + DY[d] == vy) {

        return d;
      }
    }

    return -1;
  }

  bool is_nest_cell(int v) const {

    return nest_color[v] != -1;
  }

  // =========================================================

  // Input

  // =========================================================

  void initialize_problem() {

    for (int x = 0; x < P.N; ++x) {

      for (int y = 0; y < P.N; ++y) {

        int v = cell_id(x, y);

        char c = P.C[x][y];

        if ('A' <= c && c <= 'L') {

          int color = c - 'A';

          nest[color] = v;

          nest_color[v] = color;
        }

        if ('a' <= c && c <= 'l') {

          int color = c - 'a';

          terminals[color].push_back(v);

          ++initial_slimes;
        }
      }
    }
  }

  // =========================================================

  // Greedy tree

  // =========================================================

  void add_tree_edge(Tree &tree, int u, int v) const {

    int d = direction_between(u, v);

    assert(d != -1);

    uint8_t bu = uint8_t(1u << d);

    uint8_t bv = uint8_t(1u << (d ^ 1));

    if (!(tree.adj[u] & bu)) {

      ++tree.edge_count;
    }

    tree.adj[u] |= bu;

    tree.adj[v] |= bv;
  }

  bool has_tree_edge(const Tree &tree, int v, int d) const {

    return tree.adj[v] & uint8_t(1u << d);
  }

  Tree build_greedy_tree(int color) const {

    Tree tree;

    tree.adj.assign(V, 0);

    vector<char> in_tree(V, false);

    vector<char> is_terminal(V, false);

    for (int v : terminals[color]) {

      is_terminal[v] = true;
    }

    int root = nest[color];

    in_tree[root] = true;

    while (true) {

      bool finished = true;

      for (int v : terminals[color]) {

        if (!in_tree[v]) {

          finished = false;

          break;
        }
      }

      if (finished) {

        break;
      }

      queue<int> que;

      vector<int> prev(V, -2);

      /**

       * 現在の木全体からmulti-source BFS。

       */

      for (int v = 0; v < V; ++v) {

        if (!in_tree[v]) {

          continue;
        }

        prev[v] = -1;

        que.push(v);
      }

      int target = -1;

      while (!que.empty()) {

        int v = que.front();

        que.pop();

        if (is_terminal[v] && !in_tree[v]) {

          target = v;

          break;
        }

        for (int d = 0; d < 4; ++d) {

          int nv = next_cell(v, d);

          if (nv == -1) {

            continue;
          }

          if (prev[nv] != -2) {

            continue;
          }

          prev[nv] = v;

          que.push(nv);
        }
      }

      assert(target != -1);

      int cur = target;

      while (prev[cur] != -1) {

        int p = prev[cur];

        add_tree_edge(tree, cur, p);

        in_tree[cur] = true;

        in_tree[p] = true;

        cur = p;
      }
    }

    return tree;
  }

  void build_root_info(int color, Tree &tree) {

    tree.parent.assign(V, -2);

    tree.depth.assign(V, -1);

    tree.child_count.assign(V, 0);

    tree.used.assign(V, false);

    int root = nest[color];

    queue<int> que;

    tree.parent[root] = -1;

    tree.depth[root] = 0;

    tree.used[root] = true;

    que.push(root);

    while (!que.empty()) {

      int v = que.front();

      que.pop();

      for (int d = 0; d < 4; ++d) {

        if (!has_tree_edge(tree, v, d)) {

          continue;
        }

        int nv = next_cell(v, d);

        if (tree.parent[nv] != -2) {

          continue;
        }

        tree.parent[nv] = v;

        tree.depth[nv] = tree.depth[v] + 1;

        tree.used[nv] = true;

        ++tree.child_count[v];

        que.push(nv);
      }
    }
  }

  void build_all_trees() {

    int total_edges = 0;

    for (int color = 0; color < P.K; ++color) {

      trees[color] = build_greedy_tree(color);

      build_root_info(color, trees[color]);

      total_edges += trees[color].edge_count;

      for (int v = 0; v < V; ++v) {

        if (trees[color].used[v]) {

          ++tree_usage_count[v];
        }
      }

      cerr << "tree " << char('a' + color) << " edges=" << trees[color].edge_count << '\n';
    }

    cerr << "total tree edges=" << total_edges << '\n';
  }

  // =========================================================

  // Simulation initialization

  // =========================================================

  void reset_simulation_state() {

    board.assign(V, {});

    for (int color = 0; color < P.K; ++color) {

      for (int v : terminals[color]) {

        board[v].push_back(color);
      }
    }

    parking_lock.assign(V, false);

    parking_origin_reserved.assign(V, 0);

    parkings.clear();

    resolving.assign(V, false);

    deferred.assign(V, false);

    pending_wait.assign(V, false);

    waiting_phase = true;

    answer.clear();

    homed_slimes = 0;
  }

  // =========================================================

  // Tower

  // =========================================================

  int top_color(int v) const {

    if (board[v].empty()) {

      return -1;
    }

    return board[v].back();
  }

  int top_block_size(int v) const {

    if (board[v].empty()) {

      return 0;
    }

    int color = board[v].back();

    int result = 0;

    for (int i = (int)board[v].size() - 1; i >= 0 && board[v][i] == color; --i) {

      ++result;
    }

    return result;
  }

  int movable_block_size(int v) const {

    return min(top_block_size(v), block_limit);
  }

  int remaining_slimes() const {

    int result = 0;

    for (const auto &tower : board) {

      result += tower.size();
    }

    return result;
  }

  int deferred_count() const {

    int result = 0;

    for (int v = 0; v < V; ++v) {

      if (deferred[v] && !board[v].empty()) {

        ++result;
      }
    }

    return result;
  }

  void home_cell(int v) {

    int color = nest_color[v];

    if (color == -1) {

      return;
    }

    while (!board[v].empty() && board[v].back() == color) {

      board[v].pop_back();

      ++homed_slimes;
    }

    if (board[v].empty()) {

      deferred[v] = false;

      pending_wait[v] = false;
    }
  }

  // =========================================================

  // Waiting state

  // =========================================================

  /**

   * 高さ4が新しく形成されたセルについて、

   * 待機状態を設定する。

   *

   * tower全体が4:

   *   空きマス上に4匹block

   *   -> 即deferred

   *

   * tower全体 > 4:

   *   他色blockの上に形成

   *   -> pending_wait

   */

  void register_new_height4_block(int v) {

    if (!waiting_phase) {

      return;
    }

    if (is_nest_cell(v)) {

      return;
    }

    if (board[v].empty()) {

      return;
    }

    if (top_block_size(v) != 4) {

      return;
    }

    if ((int)board[v].size() == 4) {

      deferred[v] = true;

      pending_wait[v] = false;

    } else {

      pending_wait[v] = true;
    }
  }

  /**

   * 全待機を解除し、

   * 以後は待機を一切発生させない。

   */

  void finish_waiting_phase() {

    fill(deferred.begin(), deferred.end(), false);

    fill(pending_wait.begin(), pending_wait.end(), false);

    waiting_phase = false;

    cerr << "finish waiting phase"

         << " actions=" << answer.size() << " remaining=" << remaining_slimes() << '\n';
  }

  // =========================================================

  // Actual action

  // =========================================================

  bool execute_action(int source, int k, int d, int l) {

    if ((int)answer.size() >= MAX_ACTIONS) {

      return false;
    }

    int h = board[source].size();

    if (h == 0) {

      return false;
    }

    if (!(0 <= k && k < h)) {

      return false;
    }

    if (!(1 <= l && l <= k + 1)) {

      return false;
    }

    int destination = source;

    for (int step = 0; step < l; ++step) {

      destination = next_cell(destination, d);

      if (destination == -1) {

        return false;
      }
    }

    int moved = h - k;

    if ((int)board[destination].size() + moved > 8) {

      return false;
    }

    auto [x, y] = cell_pos(source);

    answer.push_back({

        x,

        y,

        k,

        d,

        l,

    });

    for (int i = 0; i < moved; ++i) {

      int color = board[source].back();

      board[source].pop_back();

      board[destination].push_back(color);
    }

    home_cell(source);

    home_cell(destination);

    return true;
  }

  /**

   * sourceの最上部同色blockから

   * moved_size匹だけdestinationへ移動。

   *

   * track_waiting=true の場合は、

   * 高さ4の生成イベントも追跡する。

   */

  bool move_top_same_color_some(int source, int destination, int moved_size, bool track_waiting = true) {

    if (board[source].empty()) {

      return false;
    }

    int moving_color = top_color(source);

    int source_block_before = top_block_size(source);

    int destination_color_before = top_color(destination);

    int destination_block_before = top_block_size(destination);

    bool destination_was_empty = board[destination].empty();

    bool source_pending_before = waiting_phase && pending_wait[source];

    bool destination_pending_before = waiting_phase && pending_wait[destination];

    bool destination_deferred_before = waiting_phase && deferred[destination];

    if (!(1 <= moved_size && moved_size <= source_block_before)) {

      return false;
    }

    if ((int)board[destination].size() + moved_size > 8) {

      return false;
    }

    auto [sx, sy] = cell_pos(source);

    auto [tx, ty] = cell_pos(destination);

    int d = -1;

    int l = -1;

    if (sx == tx) {

      if (sy < ty) {

        d = 3;

        l = ty - sy;

      } else if (ty < sy) {

        d = 2;

        l = sy - ty;
      }

    } else if (sy == ty) {

      if (sx < tx) {

        d = 1;

        l = tx - sx;

      } else if (tx < sx) {

        d = 0;

        l = sx - tx;
      }
    }

    if (d == -1 || l <= 0) {

      return false;
    }

    int h = board[source].size();

    int k = h - moved_size;

    if (l > k + 1) {

      return false;
    }

    if (!execute_action(source, k, d, l)) {

      return false;
    }

    if (!track_waiting || !waiting_phase) {

      return true;
    }

    /**

     * sourceのtop blockを動かしたため、

     * source側に付いていたpendingは

     * 原則として移動packet側へ移す。

     */

    pending_wait[source] = false;

    /**

     * deferred block自身はwaiting phase中には

     * 通常動かさない想定。

     *

     * 念のためsourceのフラグは消す。

     */

    deferred[source] = false;

    bool carried_pending = source_pending_before;

    /**

     * destinationに元から待機状態があれば、

     * マージ後もその場所は待機状態を維持。

     */

    if (destination_deferred_before) {

      deferred[destination] = true;

      pending_wait[destination] = false;

    } else {

      pending_wait[destination] = destination_pending_before || carried_pending;
    }

    /**

     * 他色の上で高さ4になっていたblockが

     * 初めて空きマスへ着地。

     */

    if (carried_pending && destination_was_empty && !is_nest_cell(destination) && !board[destination].empty()) {

      deferred[destination] = true;

      pending_wait[destination] = false;
    }

    /**

     * source側で新しく高さ4が形成されたか。

     *

     * 例:

     *

     * cccccccc

     * 上4匹を移動

     *

     * -> sourceにccccが新しく残る

     */

    if (!board[source].empty() && top_block_size(source) == 4 && source_block_before != 4) {

      register_new_height4_block(source);
    }

    /**

     * destination側で新しく高さ4が形成されたか。

     *

     * cc + cc -> cccc

     *

     * など。

     */

    if (!board[destination].empty() && top_block_size(destination) == 4 &&

        (destination_color_before != moving_color || destination_block_before != 4)) {

      if (!deferred[destination]) {

        register_new_height4_block(destination);
      }
    }

    if (board[source].empty()) {

      deferred[source] = false;

      pending_wait[source] = false;
    }

    if (board[destination].empty()) {

      deferred[destination] = false;

      pending_wait[destination] = false;
    }

    return true;
  }

  // =========================================================

  // Tree utilities

  // =========================================================

  int next_branch(int color, int v) const {

    const auto &tree = trees[color];

    if (!tree.used[v]) {

      return -1;
    }

    int p = tree.parent[v];

    while (p != -1) {

      if (p == nest[color]) {

        return p;
      }

      if (tree.child_count[p] >= 2) {

        return p;
      }

      p = tree.parent[p];
    }

    return -1;
  }

  // =========================================================

  // Parking

  // =========================================================

  int find_parking_cell(int origin, int forbidden1, int forbidden2, int moving_color, int moving_size) const {

    int best = -1;

    int best_score = INF;

    for (int d = 0; d < 4; ++d) {

      int v = next_cell(origin, d);

      if (v == -1 || v == forbidden1 || v == forbidden2) {

        continue;
      }

      if (is_nest_cell(v)) {

        continue;
      }

      if (parking_lock[v]) {

        continue;
      }

      if (parking_origin_reserved[v] > 0) {

        continue;
      }

      if ((int)board[v].size() + moving_size > 8) {

        continue;
      }

      int score = 100 * tree_usage_count[v];

      /**

       * 待機地点も使用可能だが、

       * parking先としては優先度を下げる。

       */

      if (waiting_phase && deferred[v]) {

        score += 1000;
      }

      if (board[v].empty()) {

        score += 0;

      } else if (top_color(v) == moving_color) {

        score += 10;

      } else {

        score += 20;
      }

      score += board[v].size();

      if (score < best_score) {

        best_score = score;

        best = v;
      }
    }

    return best;
  }

  bool park_top_block(int origin, int forbidden1, int forbidden2, int recursion_depth) {

    if (recursion_depth > MAX_RECURSION) {

      return false;
    }

    if (board[origin].empty()) {

      return false;
    }

    if (parking_lock[origin]) {

      return false;
    }

    /**

     * 待機blockはparkingのために押し出さない。

     */

    if (waiting_phase && deferred[origin]) {

      return false;
    }

    if (parking_origin_reserved[origin] > 0) {

      return false;
    }

    int color = top_color(origin);

    int moved_size = movable_block_size(origin);

    if (moved_size <= 0) {

      return false;
    }

    int parking = find_parking_cell(origin, forbidden1, forbidden2, color, moved_size);

    if (parking == -1) {

      return false;
    }

    int parking_base_height = board[parking].size();

    /**

     * parkingは補助操作なので、

     * 高さ4待機イベントにはしない。

     */

    if (!move_top_same_color_some(origin, parking, moved_size, false)) {

      return false;
    }

    vector<int> origin_base = board[origin];

    parkings.push_back({

        origin,

        parking,

        color,

        moved_size,

        parking_base_height,

        std::move(origin_base),

    });

    parking_lock[parking] = true;

    ++parking_origin_reserved[origin];

    return true;
  }

  bool try_restore_parkings() {

    bool any = false;

    bool changed = true;

    while (changed) {

      changed = false;

      for (int i = (int)parkings.size() - 1; i >= 0; --i) {

        const auto rec = parkings[i];

        if (board[rec.origin] != rec.origin_base) {

          continue;
        }

        if ((int)board[rec.origin].size() + rec.moved_size > 8) {

          continue;
        }

        int expected_height = rec.parking_base_height + rec.moved_size;

        if ((int)board[rec.parking].size() != expected_height) {

          continue;
        }

        bool correct_block = true;

        for (int j = rec.parking_base_height; j < expected_height; ++j) {

          if (board[rec.parking][j] != rec.color) {

            correct_block = false;

            break;
          }
        }

        if (!correct_block) {

          continue;
        }

        int d = direction_between(rec.parking, rec.origin);

        if (d == -1) {

          continue;
        }

        if (!execute_action(rec.parking, rec.parking_base_height, d, 1)) {

          continue;
        }

        parking_lock[rec.parking] = false;

        --parking_origin_reserved[rec.origin];

        parkings.erase(parkings.begin() + i);

        changed = true;

        any = true;
      }
    }

    return any;
  }

  // =========================================================

  // Capacity resolution

  // =========================================================

  bool ensure_capacity(int destination, int incoming_color, int incoming_size, int incoming_source,

                       int incoming_stop_target, int recursion_depth) {

    if (recursion_depth > MAX_RECURSION) {

      return false;
    }

    if (!parking_lock[destination] && (int)board[destination].size() + incoming_size <= 8) {

      return true;
    }

    if (parking_lock[destination]) {

      return false;
    }

    /**

     * 待機blockが容量不足を起こしている場合、

     * 待機block自身は押し出さない。

     *

     * 呼び出し側がincoming側を待機状態にする。

     */

    if (waiting_phase && deferred[destination]) {

      return false;
    }

    if (resolving[destination]) {

      return false;
    }

    resolving[destination] = true;

    while ((int)board[destination].size() + incoming_size > 8) {

      if (board[destination].empty()) {

        break;
      }

      int before_height = board[destination].size();

      advance_top_block(destination, recursion_depth + 1, incoming_source);

      if ((int)board[destination].size() + incoming_size <= 8) {

        resolving[destination] = false;

        return true;
      }

      if ((int)board[destination].size() < before_height) {

        continue;
      }

      int forbidden2 = -1;

      if (destination != incoming_stop_target && trees[incoming_color].used[destination]) {

        forbidden2 = trees[incoming_color].parent[destination];
      }

      if (!park_top_block(destination, incoming_source, forbidden2, recursion_depth + 1)) {

        resolving[destination] = false;

        return false;
      }
    }

    bool ok = !parking_lock[destination] && (int)board[destination].size() + incoming_size <= 8;

    resolving[destination] = false;

    return ok;
  }

  // =========================================================

  // Advance

  // =========================================================

  bool advance_top_block(int start, int recursion_depth, int forbidden) {

    if (recursion_depth > MAX_RECURSION) {

      return false;
    }

    if (board[start].empty()) {

      return false;
    }

    if (parking_lock[start]) {

      return false;
    }

    /**

     * waiting phase中のdeferred block自身は

     * 通常前進させない。

     */

    if (waiting_phase && deferred[start]) {

      return false;
    }

    int color = top_color(start);

    if (color == -1) {

      return false;
    }

    if (!trees[color].used[start]) {

      return false;
    }

    int target = next_branch(color, start);

    if (target == -1) {

      return false;
    }

    int current = start;

    int packet_size = movable_block_size(start);

    int before_actions = answer.size();

    while (current != target) {

      if (board[current].empty()) {

        break;
      }

      if (top_color(current) != color) {

        break;
      }

      if (waiting_phase && deferred[current]) {

        break;
      }

      packet_size = min(packet_size, movable_block_size(current));

      if (packet_size <= 0) {

        break;
      }

      int first_parent = trees[color].parent[current];

      if (first_parent < 0 || first_parent == forbidden) {

        break;
      }

      int h = board[current].size();

      int k = h - packet_size;

      /**

       * l <= k+1

       */

      int max_jump = k + 1;

      int direction = direction_between(current, first_parent);

      if (direction == -1) {

        break;
      }

      vector<int> path;

      int v = current;

      for (int l = 1; l <= max_jump; ++l) {

        int p = trees[color].parent[v];

        if (p < 0) {

          break;
        }

        /**

         * 1操作なので一直線のみ。

         */

        if (direction_between(v, p) != direction) {

          break;
        }

        if (p == forbidden) {

          break;
        }

        path.push_back(p);

        v = p;

        /**

         * 同色blockがあれば、

         * そこでマージする。

         */

        if (!board[v].empty() && top_color(v) == color) {

          break;
        }

        if (v == target) {

          break;
        }
      }

      bool moved = false;

      /**

       * 現状の盤面で着地可能な

       * 最遠地点から試す。

       */

      for (int i = (int)path.size() - 1; i >= 0; --i) {

        int destination = path[i];

        if (parking_lock[destination]) {

          continue;
        }

        if ((int)board[destination].size() + packet_size > 8) {

          continue;
        }

        if (!move_top_same_color_some(current, destination, packet_size, true)) {

          continue;
        }

        current = destination;

        moved = true;

        break;
      }

      if (!moved) {

        /**

         * ------------------------------------------------

         * 直後のparentが待機blockで塞がれている場合。

         *

         * 待機blockを動かすのではなく、

         * incoming側も現在位置で待機する。

         * ------------------------------------------------

         */

        if (waiting_phase && deferred[first_parent]) {

          deferred[current] = true;

          break;
        }

        /**

         * 待機blockが原因でないなら、

         * 従来どおり容量確保を試す。

         */

        if (!ensure_capacity(first_parent, color, packet_size, current, target, recursion_depth + 1)) {

          /**

           * ensure_capacityの途中で

           * first_parentが待機状態になった場合も、

           * incoming側を待機。

           */

          if (waiting_phase && deferred[first_parent]) {

            deferred[current] = true;
          }

          break;
        }

        int capacity = 8 - (int)board[first_parent].size();

        int moved_size = min(packet_size, capacity);

        if (moved_size <= 0) {

          break;
        }

        if (!move_top_same_color_some(current, first_parent, moved_size, true)) {

          break;
        }

        current = first_parent;

        moved = true;
      }

      if (!moved) {

        break;
      }

      if (current == nest[color]) {

        break;
      }

      if (board[current].empty()) {

        break;
      }

      /**

       * 高さ4形成、または妨害伝播によって

       * 現在地点が待機状態になったら

       * ここで止める。

       */

      if (waiting_phase && deferred[current]) {

        break;
      }

      if (top_color(current) != color) {

        break;
      }

      /**

       * 同色とマージした場合、

       * 次はマージ後のblockを扱う。

       */

      packet_size = movable_block_size(current);
    }

    return (int)answer.size() > before_actions;
  }

  // =========================================================

  // Candidate selection

  // =========================================================

  struct Candidate {

    int depth;

    int block_size;

    int cell;
  };

  vector<Candidate> build_candidates(bool include_deferred) const {

    vector<Candidate> result;

    for (int v = 0; v < V; ++v) {

      if (board[v].empty()) {

        continue;
      }

      if (parking_lock[v]) {

        continue;
      }

      if (!include_deferred && waiting_phase && deferred[v]) {

        continue;
      }

      int color = top_color(v);

      if (color == -1) {

        continue;
      }

      if (!trees[color].used[v]) {

        continue;
      }

      if (v == nest[color]) {

        continue;
      }

      int depth = trees[color].depth[v];

      if (depth < 0) {

        continue;
      }

      result.push_back({

          depth,

          movable_block_size(v),

          v,

      });
    }

    sort(result.begin(), result.end(), [](const Candidate &a, const Candidate &b) {
      if (a.depth != b.depth) {

        return a.depth > b.depth;
      }

      if (a.block_size != b.block_size) {

        return a.block_size > b.block_size;
      }

      return a.cell < b.cell;
    });

    return result;
  }

  // =========================================================

  // Simulation

  // =========================================================

  SimulationResult run_simulation(int new_block_limit) {

    block_limit = new_block_limit;

    reset_simulation_state();

    cerr << "\n=== simulation"

         << " block_limit=" << block_limit << " ===\n";

    bool deadlock = false;

    while (remaining_slimes() > 0 && (int)answer.size() < MAX_ACTIONS) {

      try_restore_parkings();

      bool progressed = false;

      /**

       * ===============================================

       * 通常blockを処理。

       *

       * waiting phase中はdeferredを除外。

       * ===============================================

       */

      {

        auto candidates = build_candidates(false);

        for (const auto &cand : candidates) {

          int before = answer.size();

          advance_top_block(cand.cell, 0, -1);

          /**

           * 操作が発生した場合。

           */

          if ((int)answer.size() > before) {

            progressed = true;

            break;
          }

          /**

           * 操作は発生しなくても、

           * 妨害によってdeferredへ移行したなら

           * 状態は進展している。

           */

          if (waiting_phase && deferred[cand.cell]) {

            progressed = true;

            break;
          }
        }
      }

      if (progressed) {

        continue;
      }

      if (try_restore_parkings()) {

        continue;
      }

      /**

       * ===============================================

       * 待機フェーズ終了判定

       *

       * deferredを除いた通常候補が0になったら、

       * 全待機blockを一斉解除。

       *

       * 以後は待機状態を二度と作らない。

       * ===============================================

       */

      if (waiting_phase) {

        auto active = build_candidates(false);

        if (active.empty() && deferred_count() > 0) {

          finish_waiting_phase();

          continue;
        }
      }

      /**

       * 待機解除後も誰も動けないなら

       * 本当のdeadlock。

       */

      deadlock = true;

      cerr << "deadlock"

           << " block_limit=" << block_limit << " actions=" << answer.size() << " remaining=" << remaining_slimes()

           << " parking=" << parkings.size() << " waiting_phase=" << waiting_phase << '\n';

      break;
    }

    SimulationResult result;

    result.success = !deadlock && remaining_slimes() == 0;

    result.actions = answer;

    result.homed = homed_slimes;

    result.remaining = remaining_slimes();

    cerr << "result"

         << " block_limit=" << block_limit << " success=" << result.success << " actions=" << result.actions.size()

         << " homed=" << result.homed << "/" << initial_slimes << " remaining=" << result.remaining << '\n';

    return result;
  }

  // =========================================================

  // Solve

  // =========================================================

  void solve() {

    /**

     * まずblock_limit=8で

     * 初期盤面から最後までシミュレーション。

     */

    auto result8 = run_simulation(8);

    if (result8.success) {

      cerr << "use block_limit=8\n";

      for (const auto &action : result8.actions) {

        cout << action << '\n';
      }

      return;
    }

    /**

     * deadlockした場合のみ、

     * 全状態を初期化してblock_limit=4で

     * 最初からやり直す。

     */

    cerr << "\nretry from initial state"

         << " with block_limit=4\n";

    auto result4 = run_simulation(4);

    if (result4.success) {

      cerr << "use block_limit=4\n";

      for (const auto &action : result4.actions) {

        cout << action << '\n';
      }

      return;
    }

    cerr << "both simulations failed\n"

         << "limit8:"

         << " actions=" << result8.actions.size() << " remaining=" << result8.remaining << '\n'

         << "limit4:"

         << " actions=" << result4.actions.size() << " remaining=" << result4.remaining << '\n';

    /**

     * デバッグ中なので、

     * 両方失敗した場合は残り匹数が少ない方。

     */

    const auto &best = (result4.remaining < result8.remaining ||

                        (result4.remaining == result8.remaining && result4.actions.size() < result8.actions.size()))

                           ? result4

                           : result8;

    for (const auto &action : best.actions) {

      cout << action << '\n';
    }
  }
};

string run(const string &input) {
  istringstream iss(input);
  ostringstream oss;
  ostringstream err;

  auto *old_in = cin.rdbuf(iss.rdbuf());
  auto *old_out = cout.rdbuf(oss.rdbuf());
  auto *old_err = cerr.rdbuf(err.rdbuf());
  cin.clear();

  Solver solver;
  solver.solve();

  cin.clear();
  cin.rdbuf(old_in);
  cout.rdbuf(old_out);
  cerr.rdbuf(old_err);
  return oss.str();
}

} // namespace logic2

namespace logic3 {
/**
 * https://github.com/matchamgmg/kyopro/tree/main
 */

using namespace std;
using ll = long long;
using ld = long double;
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

const int INF = 1e9;
array<char, 4> UDLR = {'U', 'D', 'L', 'R'};
array<int, 4> dx = {-1, 1, 0, 0}, dy = {0, 0, -1, 1};

struct Problem {
  int N;
  int K;
  vector<string> C;
  vector<vector<deque<int>>> Slimes; // 0 ~ 11: color(slime)
  vector<vector<int>> Nests;         // 0 ~ 11: color(nest), -1: floor, -2: wall
  Problem() {
    input();
  }
  void input() {
    cin >> N >> K;
    C.resize(N);
    for (string &s : C) {
      cin >> s;
    }
  }
};

struct Legal_action {
  int x = -1;
  int y = -1;
  int k = -1;
  int d = -1;
  int l = -1;

  bool valid() const {
    return x >= 0 && y >= 0 && k >= 0 && 0 <= d && d < 4 && l >= 1;
  }

  friend ostream &operator<<(ostream &os, const Legal_action &ac) {
    cout << ac.x << ' ' << ac.y << ' ' << ac.k << ' ' << UDLR[ac.d] << ' ' << ac.l;
    return os;
  }
};

struct Answer {
  vector<Legal_action> actions;
  void print() {
    for (Legal_action &action : actions) {
      cout << action << '\n';
    }
  }
};

struct Slime {
  int color;
  int x;
  int y;
};

struct Solver {
  Problem P;
  mt19937 mt = mt19937{0};
  TimeKeeper tk = TimeKeeper{1900};

  void initialize_field(vector<vector<vector<int>>> &Slimes, vector<vector<int>> &Nests) {
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        char &c = P.C[i][j];
        if (c == '.') {
          Nests[i][j] = -1;
        } else if (c == '#') {
          Nests[i][j] = -2;
        } else if (c >= 'a') {
          Slimes[i][j].push_back(c - 'a');
          Nests[i][j] = -1;
        } else {
          Nests[i][j] = (c - 'A');
        }
      }
    }
  }
  void calc_all_color_distance(vector<vector<vector<int>>> &Dist, const vector<vector<vector<int>>> &Slimes,
                               const vector<vector<int>> &Nests) {
    vector<pair<int, int>> nest(P.K);
    for (int i = 0; i < P.N; ++i) {
      for (int j = 0; j < P.N; ++j) {
        if (Nests[i][j] >= 0) {
          nest[Nests[i][j]] = {i, j};
        }
      }
    }
    queue<pair<int, int>> que;
    for (int i = 0; i < P.K; ++i) {
      auto p = nest[i];
      Dist[i][p.first][p.second] = 0;
      que.push(p);
      while (!que.empty()) {
        p = que.front();
        int pd = Dist[i][p.first][p.second];
        que.pop();
        for (int d = 0; d < 4; ++d) {
          int nx = p.first + dx[d];
          int ny = p.second + dy[d];
          if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
            continue;
          }
          if (Nests[nx][ny] == -2) {
            continue;
          }
          if (Dist[i][nx][ny] != INF) {
            continue;
          }
          Dist[i][nx][ny] = pd + 1;
          que.emplace(nx, ny);
        }
      }
    }
  }

  int calc_slime_rest(int color, const vector<vector<vector<int>>> &Slimes) {
    int res = 0;
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        for (int c : Slimes[x][y]) {
          res += (c == color);
        }
      }
    }
    return res;
  }

  int calc_rocal_rest(int x, int y, const vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests,
                      const vector<vector<vector<int>>> &Dist) {
    int res = 0;
    int col = Nests[x][y];
    int n = Slimes[x][y].size();
    int s = n - 1;
    while (s >= 0 && Slimes[x][y][s] == col) {
      res += Dist[col][x][y];
      res--;
      --s;
    }
    bool under = false;
    while (s >= 0) {
      int c = Slimes[x][y][s];
      res += Dist[c][x][y];
      under |= (c == col);
      --s;
    }
    res += under * 2;
    int bonus = 0;
    int last = -1;
    for (int c : Slimes[x][y]) {
      if (last != c) {
        last = c;
      } else {
        bonus++;
      }
    }
    return res - bonus;
  }

  void jump_slimes(int x, int y, int nx, int ny, int n, vector<vector<vector<int>>> &Slimes) {
    for (int i = 0; i < n; ++i) {
      Slimes[nx][ny].push_back(Slimes[x][y].back());
      Slimes[x][y].pop_back();
    }
  }

  void resolve_slime(int x, int y, int nx, int ny, vector<Slime> &resolved_slime, vector<vector<vector<int>>> &Slimes,
                     const vector<vector<int>> &Nests) {
    int h = Slimes[x][y].size();
    int i = h - 1;
    while (i >= 0 && Slimes[x][y][i] == Nests[x][y]) {
      resolved_slime.emplace_back(Slimes[x][y][i], x, y);
      Slimes[x][y].pop_back();
      i--;
    }
    h = Slimes[nx][ny].size();
    i = h - 1;
    while (i >= 0 && Slimes[nx][ny][i] == Nests[nx][ny]) {
      resolved_slime.emplace_back(Slimes[nx][ny][i], nx, ny);
      Slimes[nx][ny].pop_back();
      i--;
    }
  }

  void unresolve_slime(vector<Slime> &resolved_slime, vector<vector<vector<int>>> &Slimes) {
    std::reverse(resolved_slime.begin(), resolved_slime.end());
    for (Slime &s : resolved_slime) {
      Slimes[s.x][s.y].push_back(s.color);
    }
    resolved_slime.clear();
  }

  Legal_action pre_action(int color, int x, int y, vector<vector<vector<int>>> &Slimes,
                          const vector<vector<int>> &Nests, const vector<vector<vector<int>>> &Dist,
                          int recursion_depth = 0) {
    if (recursion_depth > P.N * P.N || !(0 <= x && x < P.N && 0 <= y && y < P.N) || Slimes[x][y].empty()) {
      return {};
    }

    int pdist = Dist[color][x][y];
    int h = Slimes[x][y].size();
    int i = h - 1;
    int siz = 0;
    while (i >= 0 && Slimes[x][y][i] == color) {
      siz++;
      i--;
    }
    int min_k = h - siz;
    int best_dist = 1e9;
    int best_d = -1, best_l = -1;
    for (int d = 0; d < 4; ++d) {
      for (int l = 1; l <= min_k + 1; ++l) {
        int nx = x + dx[d] * l;
        int ny = y + dy[d] * l;
        if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
          break;
        }
        if (Nests[nx][ny] == -2) {
          break;
        }
        int dist = Dist[color][nx][ny];
        if (pdist <= dist) {
          continue;
        }
        if (!Slimes[nx][ny].empty() && Slimes[nx][ny].back() == color && Slimes[nx][ny].size() + siz <= 8) {
          dist--;
        }
        if (dist <= best_dist) {
          best_dist = dist;
          best_d = d;
          best_l = l;
        }
      }
    }
    if (best_d == -1 || best_l == -1) {
      return {};
    }

    int nx = x + dx[best_d] * best_l;
    int ny = y + dy[best_d] * best_l;
    if (Slimes[nx][ny].size() + siz <= 8) {
      return Legal_action{x, y, min_k, best_d, best_l};
    } else if (Slimes[nx][ny].back() != color) {
      int k = h - (8 - Slimes[nx][ny].size());
      return Legal_action{x, y, k, best_d, best_l};
    } else {
      return pre_action(color, nx, ny, Slimes, Nests, Dist, recursion_depth + 1);
    }
  }

  Legal_action get_color_fixed_action(int color, vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests,
                                      const vector<vector<vector<int>>> &Dist) {
    vector<Slime> candidate;
    int fartherest = -1;
    int min_tower_size = 1e9;
    for (int x = 0; x < P.N; ++x) {
      for (int y = 0; y < P.N; ++y) {
        if (Slimes[x][y].empty()) {
          continue;
        }
        if (Slimes[x][y].back() != color) {
          continue;
        }
        if (fartherest < Dist[color][x][y]) {
          candidate.clear();
          candidate.emplace_back(color, x, y);
          fartherest = Dist[color][x][y];
          int siz = 0;
          int i = Slimes[x][y].size() - 1;
          while (i >= 0 && Slimes[x][y][i] == color) {
            ++siz;
            --i;
          }
          min_tower_size = siz;
        } else if (fartherest == Dist[color][x][y]) {
          int siz = 0;
          int i = Slimes[x][y].size() - 1;
          while (i >= 0 && Slimes[x][y][i] == color) {
            ++siz;
            --i;
          }
          if (min_tower_size == siz) {
            candidate.emplace_back(color, x, y);
          } else if (min_tower_size > siz) {
            candidate.clear();
            candidate.emplace_back(color, x, y);
            min_tower_size = siz;
          }
        }
      }
    }
    if (candidate.empty()) {
      return {};
    }

    int r = mt() % (candidate.size());
    int x = candidate[r].x, y = candidate[r].y;
    int h = Slimes[x][y].size();
    int min_k = h - min_tower_size;
    int best_dist = 1e9;
    int best_d = -1, best_l = -1;
    for (int d = 0; d < 4; ++d) {
      for (int l = 1; l <= min_k + 1; ++l) {
        int nx = x + dx[d] * l;
        int ny = y + dy[d] * l;
        if (!(0 <= nx && nx < P.N && 0 <= ny && ny < P.N)) {
          break;
        }
        if (Nests[nx][ny] == -2) {
          break;
        }
        int dist = Dist[color][nx][ny];
        if (fartherest <= dist) {
          continue;
        }
        if (!Slimes[nx][ny].empty() && Slimes[nx][ny].back() == color && Slimes[nx][ny].size() + min_tower_size <= 8) {
          dist--;
        }
        if (dist <= best_dist) {
          best_dist = dist;
          best_d = d;
          best_l = l;
        }
      }
    }
    if (best_d == -1 || best_l == -1) {
      return {};
    }

    int nx = x + dx[best_d] * best_l;
    int ny = y + dy[best_d] * best_l;
    if (Slimes[nx][ny].size() + min_tower_size <= 8) {
      return Legal_action{x, y, min_k, best_d, best_l};
    } else if (Slimes[nx][ny].back() != color) {
      int k = h - (8 - Slimes[nx][ny].size());
      return Legal_action{x, y, k, best_d, best_l};
    } else {
      return pre_action(color, nx, ny, Slimes, Nests, Dist);
    }
  }

  int apply_action(const Legal_action &action, vector<vector<vector<int>>> &Slimes, const vector<vector<int>> &Nests) {
    if (!action.valid()) {
      return -1;
    }

    int res = 0;
    int x = action.x;
    int y = action.y;
    int nx = x + dx[action.d] * action.l;
    int ny = y + dy[action.d] * action.l;
    int h = Slimes[x][y].size();
    jump_slimes(x, y, nx, ny, h - action.k, Slimes);
    h = Slimes[x][y].size();
    int i = h - 1;
    while (i >= 0 && Slimes[x][y][i] == Nests[x][y]) {
      Slimes[x][y].pop_back();
      res++;
      i--;
    }
    h = Slimes[nx][ny].size();
    i = h - 1;
    while (i >= 0 && Slimes[nx][ny][i] == Nests[nx][ny]) {
      Slimes[nx][ny].pop_back();
      res++;
      i--;
    }
    return res;
  }

  Answer solve_color_fixed() {
    Answer best_ans, ans;
    int best_score = INF;
    Legal_action action;

    vector<vector<vector<int>>> Slimes_original(P.N, vector<vector<int>>(P.N));
    vector<vector<vector<int>>> Slimes;
    vector<vector<int>> Nests(P.N, vector<int>(P.N));
    initialize_field(Slimes_original, Nests);

    vector<vector<vector<int>>> Dist(P.K, vector<vector<int>>(P.N, vector<int>(P.N, INF)));
    calc_all_color_distance(Dist, Slimes_original, Nests);

    vector<int> Colors(P.K);
    iota(Colors.begin(), Colors.end(), 0);
    set<vector<int>> st;

    do {
      if (st.contains(Colors)) {
        // 元コードの探索順を保ちつつ、既出順列で停止し続けないように次へ進める。
        if (P.K >= 2) {
          int ri = mt() % P.K;
          int rj;
          do {
            rj = mt() % P.K;
          } while (rj == ri);
          swap(Colors[ri], Colors[rj]);
        }
        continue;
      }
      st.insert(Colors);

      ans.actions.clear();
      Slimes = Slimes_original;
      bool ok = true;

      for (int col : Colors) {
        int col_slimes = calc_slime_rest(col, Slimes);

        while (col_slimes > 0) {
          if ((int)ans.actions.size() >= 100000) {
            ok = false;
            break;
          }

          action = get_color_fixed_action(col, Slimes, Nests, Dist);
          if (!action.valid()) {
            ok = false;
            break;
          }

          int resolved = apply_action(action, Slimes, Nests);
          if (resolved < 0) {
            ok = false;
            break;
          }

          col_slimes -= resolved;
          ans.actions.push_back(action);
        }

        if (!ok) {
          break;
        }
      }

      if (ok) {
        int remaining = 0;
        for (const auto &row : Slimes) {
          for (const auto &tower : row) {
            remaining += (int)tower.size();
          }
        }

        if (remaining == 0 && (int)ans.actions.size() < best_score) {
          best_score = (int)ans.actions.size();
          best_ans = ans;
        }
      }

      if (P.K >= 2) {
        int ri = mt() % P.K;
        int rj;
        do {
          rj = mt() % P.K;
        } while (rj == ri);
        swap(Colors[ri], Colors[rj]);
      }
    } while (!tk.isTimeOver());

    return best_ans;
  }
};

string run(const string &input, int time_limit_ms) {
  istringstream iss(input);
  ostringstream oss;
  ostringstream err;

  auto *old_in = cin.rdbuf(iss.rdbuf());
  auto *old_out = cout.rdbuf(oss.rdbuf());
  auto *old_err = cerr.rdbuf(err.rdbuf());
  cin.clear();

  Solver sol;
  sol.tk = TimeKeeper{max(1, time_limit_ms)};
  Answer ans = sol.solve_color_fixed();
  ans.print();

  cin.clear();
  cin.rdbuf(old_in);
  cout.rdbuf(old_out);
  cerr.rdbuf(old_err);
  return oss.str();
}

} // namespace logic3

#undef rep
#undef rrep
#undef all
#undef rall

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const auto global_start = chrono::steady_clock::now();
  constexpr int TOTAL_BUDGET_MS = 1850;

  string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

  struct Candidate {
    string name;
    string output;
    integrated::ValidationResult validation;
  };

  vector<Candidate> candidates;

  {
    string out = logic1::run(input);
    candidates.push_back({"logic1", out, integrated::validate_output(input, out)});
  }

  {
    string out = logic2::run(input);
    candidates.push_back({"logic2", out, integrated::validate_output(input, out)});
  }

  int elapsed_ms = (int)chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - global_start).count();
  int logic3_budget = max(1, TOTAL_BUDGET_MS - elapsed_ms);

  {
    string out = logic3::run(input, logic3_budget);
    candidates.push_back({"logic3", out, integrated::validate_output(input, out)});
  }

  int best = -1;
  for (int i = 0; i < (int)candidates.size(); ++i) {
    const auto &c = candidates[i];

    cerr << '[' << c.name << "] valid=" << c.validation.valid_output << " complete=" << c.validation.complete
         << " actions=" << c.validation.action_count << " remaining=" << c.validation.remaining
         << " score=" << c.validation.score << " reason=" << c.validation.reason << '\n';

    // デッドロック・不正出力・盤面にスライムが残る解は候補から除外。
    if (!c.validation.valid_output || !c.validation.complete)
      continue;

    // 完全解では E=0 なので score = T = action_count。
    if (best == -1 || c.validation.score < candidates[best].validation.score) {
      best = i;
    }
  }

  if (best == -1) {
    cerr << "no complete valid solution\n";
    return 0;
  }

  cerr << "selected=" << candidates[best].name << " score=" << candidates[best].validation.score << '\n';

  cout << candidates[best].output;
  return 0;
}
