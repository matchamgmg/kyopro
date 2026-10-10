#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_ACTIONS = 100000;
constexpr int TIME_LIMIT_MS = 1900;

constexpr array<int, 4> DX = {-1, 1, 0, 0};
constexpr array<int, 4> DY = {0, 0, -1, 1};
constexpr array<char, 4> DIR_CHAR = {'U', 'D', 'L', 'R'};

struct Problem {
  int N, K;
  vector<string> C;

  Problem() {
    cin >> N >> K;
    C.resize(N);
    for (auto &row : C)
      cin >> row;
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
  // adj[v] の bit d が立っていれば、d 方向に木の辺がある。
  vector<uint8_t> adj;

  // 巣を根とした情報。
  vector<int> parent;
  vector<int> depth;
  vector<int> child_count;
  vector<char> used;

  int edge_count = 0;
};

struct SimulationResult {
  bool success = false;
  vector<Action> actions;
};

struct Solver {
  Problem P;
  int V;

  vector<int> nest;
  vector<int> nest_color;
  vector<vector<int>> terminals;
  vector<Tree> trees;

  mt19937 mt{0};
  chrono::steady_clock::time_point start_time;

  Solver() : V(P.N * P.N), start_time(chrono::steady_clock::now()) {
    nest.assign(P.K, -1);
    nest_color.assign(V, -1);
    terminals.resize(P.K);

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
    return {v / P.N, v % P.N};
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
    if (!passable(nx, ny))
      return -1;
    return cell_id(nx, ny);
  }

  int direction_between(int u, int v) const {
    auto [ux, uy] = cell_pos(u);
    auto [vx, vy] = cell_pos(v);

    for (int d = 0; d < 4; ++d) {
      if (ux + DX[d] == vx && uy + DY[d] == vy)
        return d;
    }
    return -1;
  }

  bool time_over() const {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
    return elapsed >= TIME_LIMIT_MS;
  }

  // =========================================================
  // Input / initial information
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
        } else if ('a' <= c && c <= 'l') {
          int color = c - 'a';
          terminals[color].push_back(v);
        }
      }
    }
  }

  // =========================================================
  // Greedy Steiner tree
  // =========================================================

  void add_tree_edge(Tree &tree, int u, int v) const {
    int d = direction_between(u, v);
    assert(d != -1);

    uint8_t bu = uint8_t(1u << d);
    uint8_t bv = uint8_t(1u << (d ^ 1));

    if (!(tree.adj[u] & bu))
      ++tree.edge_count;

    tree.adj[u] |= bu;
    tree.adj[v] |= bv;
  }

  bool has_tree_edge(const Tree &tree, int v, int d) const {
    return (tree.adj[v] & uint8_t(1u << d)) != 0;
  }

  /**
   * 巣だけから始め、現在の木全体を multi-source BFS の始点として、
   * 木に最も近い未接続スライムを1体ずつ接続する。
   *
   * これを各色について固定木として一度だけ構築する。
   */
  Tree build_greedy_tree(int color) const {
    Tree tree;
    tree.adj.assign(V, 0);

    vector<char> in_tree(V, false);
    vector<char> is_terminal(V, false);

    for (int v : terminals[color])
      is_terminal[v] = true;

    const int root = nest[color];
    in_tree[root] = true;

    while (true) {
      bool finished = true;
      for (int v : terminals[color]) {
        if (!in_tree[v]) {
          finished = false;
          break;
        }
      }
      if (finished)
        break;

      queue<int> que;
      vector<int> prev(V, -2);

      // 現在の木全体を始点にする。
      for (int v = 0; v < V; ++v) {
        if (!in_tree[v])
          continue;
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
          if (nv == -1)
            continue;
          if (prev[nv] != -2)
            continue;

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

  void build_root_info(int color, Tree &tree) const {
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
        if (!has_tree_edge(tree, v, d))
          continue;

        int nv = next_cell(v, d);
        assert(nv != -1);

        if (tree.parent[nv] != -2)
          continue;

        tree.parent[nv] = v;
        tree.depth[nv] = tree.depth[v] + 1;
        tree.used[nv] = true;
        ++tree.child_count[v];
        que.push(nv);
      }
    }
  }

  void build_all_trees() {
    trees.resize(P.K);

    for (int color = 0; color < P.K; ++color) {
      trees[color] = build_greedy_tree(color);
      build_root_info(color, trees[color]);
    }
  }

  // =========================================================
  // Simulation state helpers
  // =========================================================

  vector<vector<int>> initial_board() const {
    vector<vector<int>> board(V);

    for (int color = 0; color < P.K; ++color) {
      for (int v : terminals[color]) {
        board[v].push_back(color);
      }
    }

    return board;
  }

  int top_color(const vector<vector<int>> &board, int v) const {
    if (board[v].empty())
      return -1;
    return board[v].back();
  }

  int top_block_size(const vector<vector<int>> &board, int v) const {
    if (board[v].empty())
      return 0;

    int color = board[v].back();
    int cnt = 0;

    for (int i = (int)board[v].size() - 1; i >= 0 && board[v][i] == color; --i) {
      ++cnt;
    }

    return cnt;
  }

  int count_color(const vector<vector<int>> &board, int color) const {
    int result = 0;
    for (const auto &tower : board) {
      for (int c : tower)
        result += (c == color);
    }
    return result;
  }

  int count_all(const vector<vector<int>> &board) const {
    int result = 0;
    for (const auto &tower : board)
      result += (int)tower.size();
    return result;
  }

  void home_cell(vector<vector<int>> &board, int v) const {
    int color = nest_color[v];
    if (color == -1)
      return;

    while (!board[v].empty() && board[v].back() == color) {
      board[v].pop_back();
    }
  }

  // =========================================================
  // Actions
  // =========================================================

  bool execute_action(vector<vector<int>> &board, vector<Action> &actions, int source, int moved_size,
                      int destination) const {
    if ((int)actions.size() >= MAX_ACTIONS)
      return false;
    if (board[source].empty())
      return false;
    if (!(1 <= moved_size && moved_size <= (int)board[source].size()))
      return false;
    if ((int)board[destination].size() + moved_size > 8)
      return false;

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

    if (d == -1 || l <= 0)
      return false;

    int h = board[source].size();
    int k = h - moved_size;

    if (!(0 <= k && k < h))
      return false;
    if (l > k + 1)
      return false;

    // 通過マスはすべて床でなければならない。
    int v = source;
    for (int step = 0; step < l; ++step) {
      v = next_cell(v, d);
      if (v == -1)
        return false;
    }
    if (v != destination)
      return false;

    actions.push_back({sx, sy, k, d, l});

    // 上から取り出すので移動部分は反転する。
    // 今回は同色blockのみを動かすため、色列としては変化しない。
    for (int i = 0; i < moved_size; ++i) {
      int color = board[source].back();
      board[source].pop_back();
      board[destination].push_back(color);
    }

    home_cell(board, source);
    home_cell(board, destination);

    return true;
  }

  /**
   * source から根方向の「次の集約点」を返す。
   *
   * ・分岐点
   * ・巣
   *
   * のうち最初に現れるもの。
   */
  int next_merge_point(int color, int source) const {
    const auto &tree = trees[color];
    int v = tree.parent[source];

    while (v != -1) {
      if (v == nest[color])
        return v;
      if (tree.child_count[v] >= 2)
        return v;
      v = tree.parent[v];
    }

    return -1;
  }

  /**
   * full blockを動かす場合に合法な範囲で、
   * 固定木上を根方向へできるだけ長くジャンプする。
   *
   * ただし、
   * ・木が曲がる
   * ・次の分岐点/巣に着く
   * ・同色towerを見つける
   * のいずれかで止める。
   *
   * 容量不足なら -1 を返し、1辺移動側の処理に任せる。
   */
  int find_jump_destination(const vector<vector<int>> &board, int color, int source, int moved_size) const {
    const auto &tree = trees[color];

    int first_parent = tree.parent[source];
    if (first_parent < 0)
      return -1;

    int direction = direction_between(source, first_parent);
    if (direction == -1)
      return -1;

    int h = board[source].size();
    int k = h - moved_size;
    int max_jump = k + 1;

    int merge_point = next_merge_point(color, source);

    vector<int> candidates;
    int v = source;

    for (int l = 1; l <= max_jump; ++l) {
      int p = tree.parent[v];
      if (p < 0)
        break;

      if (direction_between(v, p) != direction)
        break;

      candidates.push_back(p);
      v = p;

      if (!board[v].empty() && top_color(board, v) == color)
        break;
      if (v == merge_point)
        break;
    }

    for (int i = (int)candidates.size() - 1; i >= 0; --i) {
      int destination = candidates[i];
      if ((int)board[destination].size() + moved_size <= 8) {
        return destination;
      }
    }

    return -1;
  }

  /**
   * 「行き詰まり解消のために先行させるblock」専用のジャンプ先探索。
   *
   * 同色towerへの着地も許可する。
   * ただし、着地後も先行blockの packet_size は増やさず、
   * 下にある同色blockを取り込まない（論理的にマージしない）。
   *
   * 同色towerを途中で飛び越えることも許可する。
   * また、分岐点でも止まる必要はなく、同一直線上で合法な範囲を
   * できるだけ根方向へ進める。
   */
  int find_forced_jump_destination(const vector<vector<int>> &board, int color, int source, int moved_size) const {
    const auto &tree = trees[color];

    int first_parent = tree.parent[source];
    if (first_parent < 0)
      return -1;

    int direction = direction_between(source, first_parent);
    if (direction == -1)
      return -1;

    int h = board[source].size();
    int k = h - moved_size;
    int max_jump = k + 1;

    vector<int> candidates;
    int v = source;

    for (int l = 1; l <= max_jump; ++l) {
      int p = tree.parent[v];
      if (p < 0)
        break;
      if (direction_between(v, p) != direction)
        break;

      candidates.push_back(p);
      v = p;

      if (v == nest[color])
        break;
    }

    for (int i = (int)candidates.size() - 1; i >= 0; --i) {
      int destination = candidates[i];

      // 同色towerにも着地してよい。
      // ただし移動後も packet_size はそのままなので、
      // 下の同色blockを先行blockへ取り込まない。
      if ((int)board[destination].size() + moved_size <= 8) {
        return destination;
      }
    }

    return -1;
  }

  /**
   * 行き詰まり解消のために先行させるblockを、
   * 他の通常処理へ戻さず、そのまま最後まで巣へ送る。
   *
   * この処理中は同色blockの上への着地自体は許可する。
   * ただし、着地しても packet_size を増やさず、下の同色blockを
   * 先行blockには取り込まない。つまり物理的には連続して見えても、
   * 次の操作では元の先行block分だけを上から動かす。
   *
   * 同色blockのせいで packet 全体を着地できない場合だけ、
   * そのblockを先に同じルールで最後まで帰巣させて容量を作る。
   *
   * 他色1匹などの上にblock全体が載らない場合だけ、容量分に分割する。
   * 分割して先に進めた部分も、その場で最後まで帰巣させてから
   * sourceに残った部分の処理へ戻る。
   */
  bool flush_block_to_nest_no_merge(vector<vector<int>> &board, vector<Action> &actions, int color, int source,
                                    int packet_size) const {
    if (packet_size <= 0)
      return true;
    if ((int)actions.size() >= MAX_ACTIONS)
      return false;

    if (source == nest[color]) {
      home_cell(board, source);
      return true;
    }

    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (top_block_size(board, source) < packet_size)
      return false;
    if (!trees[color].used[source])
      return false;

    // packet全体を長距離移動できるなら、そのまま先へ送る。
    // 同色towerに着地しても packet_size は増やさない。
    int destination = find_forced_jump_destination(board, color, source, packet_size);

    if (destination != -1) {
      if (!execute_action(board, actions, source, packet_size, destination)) {
        return false;
      }

      if (destination == nest[color])
        return true;

      return flush_block_to_nest_no_merge(board, actions, color, destination, packet_size);
    }

    // packet全体を置けるジャンプ先がないので、直後の親を処理する。
    int parent = trees[color].parent[source];
    if (parent < 0)
      return false;

    /**
     * 親が同色で packet 全体を置けない場合。
     *
     * 先に親towerを高さ8まで同色で埋め、
     * その上部7段だけを「先行block」として巣まで送り切る。
     *
     * 例:
     *
     *   source: cccc
     *   parent: cccccc
     *
     *   source -> parent に2匹だけ載せる
     *
     *   source: cc
     *   parent: cccccccc
     *
     *   parent の上7匹を最後まで帰巣
     *
     *   parent: c
     *
     * 先行中は通常の同色towerへ着地しても packet_size は増やさない。
     * ただし、その先で再び同色towerに詰まった場合は、同じように
     * 「高さ8まで埋める -> 上7匹を先行」の処理を再帰的に行う。
     */
    if (!board[parent].empty() && top_color(board, parent) == color) {
      int need = 8 - (int)board[parent].size();

      // parent は full packet を受け入れられなかったので、
      // need < packet_size のはず。
      if (need < 0 || need >= packet_size)
        return false;

      if (need > 0) {
        if (!execute_action(board, actions, source, need, parent)) {
          return false;
        }
      }

      if ((int)board[parent].size() != 8)
        return false;

      // 一色ずつ処理しているため、親towerの下に他色があっても高々1匹。
      // よって高さ8まで埋めた時点で上7段は必ず active color のはず。
      if (top_block_size(board, parent) < 7)
        return false;

      if (!flush_block_to_nest_no_merge(board, actions, color, parent, 7)) {
        return false;
      }

      int remaining_packet = packet_size - need;

      // source に残った元の先行packetも、通常処理へ戻さず
      // そのまま最後まで帰巣させる。
      if (remaining_packet > 0) {
        return flush_block_to_nest_no_merge(board, actions, color, source, remaining_packet);
      }

      return true;
    }

    // 親が空、または他色tower。
    int capacity = 8 - (int)board[parent].size();
    if (capacity <= 0)
      return false;

    int moved_size = min(packet_size, capacity);

    if (!execute_action(board, actions, source, moved_size, parent)) {
      return false;
    }

    // 今動かした部分を、他のblockを処理する前に最後まで帰巣させる。
    if (parent != nest[color]) {
      if (!flush_block_to_nest_no_merge(board, actions, color, parent, moved_size)) {
        return false;
      }
    }

    // 容量不足でsourceに残した部分があれば、それも続けて最後まで送る。
    if (moved_size < packet_size) {
      if (!flush_block_to_nest_no_merge(board, actions, color, source, packet_size - moved_size)) {
        return false;
      }
    }

    return true;
  }

  /**
   * v にある active color の top blockを根方向へ進める。
   *
   * 容量不足で親が同色なら、親側を先に根方向へ押し出して空きを作る。
   * 親が他色なら、そのセルには未処理色1匹しか存在しないので、
   * 入る分だけ current color を載せる。
   *
   * 再帰先は必ず depth が小さくなるため循環しない。
   */
  bool advance_one(vector<vector<int>> &board, vector<Action> &actions, int color, int source) const {
    if (source == nest[color])
      return false;
    if (board[source].empty())
      return false;
    if (top_color(board, source) != color)
      return false;
    if (!trees[color].used[source])
      return false;

    int block_size = top_block_size(board, source);
    if (block_size <= 0)
      return false;

    // まず、full blockのまま可能なら合法な長距離移動を行う。
    int jump_destination = find_jump_destination(board, color, source, block_size);
    if (jump_destination != -1) {
      return execute_action(board, actions, source, block_size, jump_destination);
    }

    // full blockを運べない場合は直後の親で処理する。
    int parent = trees[color].parent[source];
    if (parent < 0)
      return false;

    int capacity = 8 - (int)board[parent].size();

    /**
     * full blockを親へ置けず、親のtopも同色なら行き詰まり。
     *
     * ここでは親blockをそのまま先行させない。
     * まず current block の一部を親へ載せてtower全体を高さ8にし、
     * その上部7段だけを先行blockとして最後まで帰巣させる。
     *
     * 先行blockは途中で同色towerへ着地しても論理的にはマージせず、
     * 7匹の packet_size を維持する。
     */
    if (!board[parent].empty() && top_color(board, parent) == color && capacity < block_size) {
      int need = capacity;

      // 高さ8にするために必要な分だけ current -> parent へマージ。
      // capacity < block_size なので source 側には必ず1匹以上残る。
      if (need > 0) {
        if (!execute_action(board, actions, source, need, parent)) {
          return false;
        }
      }

      if ((int)board[parent].size() != 8)
        return false;
      if (top_block_size(board, parent) < 7)
        return false;

      // 高さ8towerの上7段だけを先行させ、最後まで帰巣させる。
      if (!flush_block_to_nest_no_merge(board, actions, color, parent, 7)) {
        return false;
      }

      // 今回の advance_one では先行処理までで終了する。
      // source に残ったblockは次の通常ループで改めて処理する。
      return true;
    }

    if (capacity <= 0) {
      // 一色ずつ処理している限り、同色以外の高さ8towerは原理上作らない。
      return false;
    }

    // 親が他色なら、その空きに入る分だけ運ぶ。
    // 8匹block + 他色1匹のような場合は7+1に分割できる。
    int moved_size = min(block_size, capacity);
    if (moved_size <= 0)
      return false;

    return execute_action(board, actions, source, moved_size, parent);
  }

  // =========================================================
  // One color
  // =========================================================

  int deepest_active_cell(const vector<vector<int>> &board, int color) const {
    int best = -1;
    int best_depth = -1;
    int best_block_size = -1;

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;
      if (top_color(board, v) != color)
        continue;
      if (!trees[color].used[v])
        continue;
      if (v == nest[color])
        continue;

      int depth = trees[color].depth[v];
      int block_size = top_block_size(board, v);

      // 葉側から処理して、分岐点で自然にマージさせる。
      if (depth > best_depth || (depth == best_depth && block_size < best_block_size) ||
          (depth == best_depth && block_size == best_block_size && v < best)) {
        best = v;
        best_depth = depth;
        best_block_size = block_size;
      }
    }

    return best;
  }

  bool solve_one_color(vector<vector<int>> &board, vector<Action> &actions, int color) const {
    while (count_color(board, color) > 0) {
      if ((int)actions.size() >= MAX_ACTIONS)
        return false;

      int source = deepest_active_cell(board, color);

      // current color が盤面に残っているのにtopに存在しないなら失敗。
      // 一色ずつ処理する設計では原理上起きない。
      if (source == -1)
        return false;

      if (!advance_one(board, actions, color, source))
        return false;
    }

    return true;
  }

  // =========================================================
  // One color order
  // =========================================================

  SimulationResult simulate(const vector<int> &color_order) const {
    vector<vector<int>> board = initial_board();
    vector<Action> actions;
    actions.reserve(4096);

    for (int color : color_order) {
      if (!solve_one_color(board, actions, color)) {
        return {false, {}};
      }
    }

    if (count_all(board) != 0) {
      return {false, {}};
    }

    return {true, std::move(actions)};
  }

  // =========================================================
  // Color order search
  // =========================================================

  void try_order(const vector<int> &order, set<vector<int>> &seen, SimulationResult &best) const {
    if (!seen.insert(order).second)
      return;

    auto result = simulate(order);
    if (!result.success)
      return;

    if (!best.success || result.actions.size() < best.actions.size()) {
      best = std::move(result);
    }
  }

  long long permutation_count_cap(int n, long long cap) const {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
      if (result > cap / i)
        return cap;
      result *= i;
    }
    return result;
  }

  vector<Action> solve() {
    SimulationResult best;
    set<vector<int>> seen;

    vector<int> order(P.K);
    iota(order.begin(), order.end(), 0);

    // 基本順。
    try_order(order, seen, best);

    // 逆順。
    reverse(order.begin(), order.end());
    try_order(order, seen, best);

    // スライム数の多い色から。
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
      if (terminals[a].size() != terminals[b].size()) {
        return terminals[a].size() > terminals[b].size();
      }
      return a < b;
    });
    try_order(order, seen, best);

    // スライム数の少ない色から。
    reverse(order.begin(), order.end());
    try_order(order, seen, best);

    if (P.K >= 2) {
      const long long all_orders = permutation_count_cap(P.K, 1000000000LL);

      // 残り時間は色順探索に使う。
      while (!time_over()) {
        if ((long long)seen.size() == all_orders)
          break;

        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), mt);
        try_order(order, seen, best);
      }
    }

    // この方針では通常必ず完全解が得られる。
    // 念のため、探索中に候補が得られなかった場合は基本順を再実行する。
    if (!best.success) {
      iota(order.begin(), order.end(), 0);
      best = simulate(order);
    }

    return best.actions;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  auto answer = solver.solve();

  for (const auto &action : answer) {
    cout << action << '\n';
  }

  return 0;
}
