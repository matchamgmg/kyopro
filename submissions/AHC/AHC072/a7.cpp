#include <bits/stdc++.h>

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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;

  solver.solve();

  return 0;
}