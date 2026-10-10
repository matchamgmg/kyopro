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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;

  solver.solve();

  return 0;
}