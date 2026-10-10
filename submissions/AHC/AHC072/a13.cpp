#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_ACTIONS = 100000;
constexpr int TIME_LIMIT_MS = 1900;

// fill8 / flushX 用の局所探索。
// X = 7, 6, 5, 4 をすべて試し、完全解のうち最小手数を採用する。
constexpr int LOCAL_ROUTE_HORIZON = 24;
constexpr int LOCAL_SEARCH_DEPTH = 5;
constexpr int LOCAL_BEAM_WIDTH = 64;
constexpr int LOCAL_SETUP_CANDIDATE_LIMIT = 24;

// 再利用可能なジャンプ台の将来価値。
// 現在のflushだけでなく、後続flushが同じ台を使える場合に
// 局所探索で少し余分なsetupコストを許容する。
constexpr int REUSE_CREDIT_PER_LEVEL = 1;

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

// 盤面は最大20x20、各towerは最大8段なので固定長で保持する。
// vector<vector<int>> と違い、盤面コピー時にセルごとのheap allocationが発生しない。
constexpr int MAX_CELLS = 20 * 20;

struct Tower {
  array<uint8_t, 8> slime{};
  uint8_t height = 0;

  bool empty() const {
    return height == 0;
  }
  int size() const {
    return (int)height;
  }
  int back() const {
    return (int)slime[height - 1];
  }
  int operator[](int i) const {
    return (int)slime[i];
  }

  void push_back(int color) {
    assert(height < 8);
    slime[height++] = (uint8_t)color;
  }

  void pop_back() {
    assert(height > 0);
    --height;
  }
};

using Board = array<Tower, MAX_CELLS>;
static_assert(is_trivially_copyable_v<Board>);

struct Solver {
  Problem P;
  int V;

  vector<int> nest;
  vector<int> nest_color;
  vector<vector<int>> terminals;
  vector<Tree> trees;

  // dist_to_nest[color][v]: v から color の巣までの最短距離。
  vector<vector<int>> dist_to_nest;

  // floor_dist[u][v]: 壁を避けた盤面上の全点対最短距離。
  // flush経路上判定とジャンプ台再利用価値の推定に使う。
  vector<vector<int>> floor_dist;

  // shortest_path_demand[color][v]:
  // 初期terminalのうち、v を通る最短路を持つものの個数。
  // flush size ごとの再利用価値は、この需要を packet size で割って評価する。
  vector<vector<int>> shortest_path_demand;

  // flushの最短路領域は盤面状態に依存しないため、
  // (active color, flush source) ごとに一度だけ構築して再利用する。
  vector<vector<vector<int>>> flush_route_cache;
  vector<vector<array<uint8_t, MAX_CELLS>>> flush_route_mask_cache;

  // 現在試している fill8 / flushX の X。
  // solve() から 7, 6, 5, 4 の順に切り替える。
  int strategy_flush_size = 4;

  mt19937 mt{0};
  chrono::steady_clock::time_point start_time;

  Solver() : V(P.N * P.N), start_time(chrono::steady_clock::now()) {
    nest.assign(P.K, -1);
    nest_color.assign(V, -1);
    terminals.resize(P.K);

    initialize_problem();
    build_distances();
    build_flush_route_cache();
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
  // Shortest path distances
  // =========================================================

  void build_distances() {
    dist_to_nest.assign(P.K, vector<int>(V, INT_MAX));

    for (int color = 0; color < P.K; ++color) {
      queue<int> que;
      int root = nest[color];
      dist_to_nest[color][root] = 0;
      que.push(root);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (dist_to_nest[color][nv] != INT_MAX)
            continue;

          dist_to_nest[color][nv] = dist_to_nest[color][v] + 1;
          que.push(nv);
        }
      }
    }

    floor_dist.assign(V, vector<int>(V, INT_MAX));

    for (int source = 0; source < V; ++source) {
      auto [sx, sy] = cell_pos(source);
      if (!passable(sx, sy))
        continue;

      queue<int> que;
      floor_dist[source][source] = 0;
      que.push(source);

      while (!que.empty()) {
        int v = que.front();
        que.pop();

        for (int d = 0; d < 4; ++d) {
          int nv = next_cell(v, d);
          if (nv == -1)
            continue;
          if (floor_dist[source][nv] != INT_MAX)
            continue;

          floor_dist[source][nv] = floor_dist[source][v] + 1;
          que.push(nv);
        }
      }
    }

    // 各セルが、その色の何匹の初期terminalから巣への最短路に
    // 含まれ得るかを数える。
    // flush size は戦略ごとに変わるので、ここでは需要そのものだけ保存する。
    shortest_path_demand.assign(P.K, vector<int>(V, 0));

    for (int color = 0; color < P.K; ++color) {
      for (int v = 0; v < V; ++v) {
        if (dist_to_nest[color][v] == INT_MAX)
          continue;

        int demand = 0;
        for (int u : terminals[color]) {
          if (floor_dist[u][v] == INT_MAX)
            continue;
          if (floor_dist[u][v] + dist_to_nest[color][v] == dist_to_nest[color][u]) {
            ++demand;
          }
        }

        shortest_path_demand[color][v] = demand;
      }
    }
  }

  bool is_shortest_step(int color, int from, int to) const {
    if (from < 0 || to < 0)
      return false;
    if (dist_to_nest[color][from] == INT_MAX)
      return false;
    return dist_to_nest[color][to] + 1 == dist_to_nest[color][from];
  }

  /**
   * source -> v -> nest が最短路になるか。
   */
  bool on_some_shortest_path(int color, int source, int v) const {
    if (source < 0 || v < 0)
      return false;
    if (floor_dist[source][v] == INT_MAX)
      return false;
    if (dist_to_nest[color][source] == INT_MAX || dist_to_nest[color][v] == INT_MAX) {
      return false;
    }

    return floor_dist[source][v] + dist_to_nest[color][v] == dist_to_nest[color][source];
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

  Board initial_board() const {
    Board board{};

    for (int color = 0; color < P.K; ++color) {
      for (int v : terminals[color]) {
        board[v].push_back(color);
      }
    }

    return board;
  }

  int top_color(const Board &board, int v) const {
    if (board[v].empty())
      return -1;
    return board[v].back();
  }

  int top_block_size(const Board &board, int v) const {
    if (board[v].empty())
      return 0;

    int color = board[v].back();
    int cnt = 0;

    for (int i = (int)board[v].size() - 1; i >= 0 && board[v][i] == color; --i) {
      ++cnt;
    }

    return cnt;
  }

  int count_color(const Board &board, int color) const {
    int result = 0;
    for (int v = 0; v < V; ++v) {
      const auto &tower = board[v];
      for (int i = 0; i < tower.size(); ++i) {
        result += (tower[i] == color);
      }
    }
    return result;
  }

  int count_all(const Board &board) const {
    int result = 0;
    for (int v = 0; v < V; ++v)
      result += board[v].size();
    return result;
  }

  void home_cell(Board &board, int v) const {
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

  bool execute_action(Board &board, vector<Action> &actions, int source, int moved_size, int destination) const {
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
  int find_jump_destination(const Board &board, int color, int source, int moved_size) const {
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
   * flush packet 専用のジャンプ先探索。
   *
   * 通常輸送はGreedy Steiner treeに従うが、flush中だけは木を外れ、
   * 巣への最短距離が1マスごとに必ず減少する直線だけを使う。
   * したがって選ばれたジャンプ全体も必ず最短路上にある。
   *
   * packet_sizeは論理的に固定し、同色towerへ着地しても増やさない。
   */
  int find_flush_jump_destination(const Board &board, int color, int source, int packet_size) const {
    if (dist_to_nest[color][source] == INT_MAX)
      return -1;

    int h = board[source].size();
    int k = h - packet_size;
    int max_jump = k + 1;

    int best = -1;
    int best_length = -1;
    int best_base_height = -1;

    for (int d = 0; d < 4; ++d) {
      int v = source;

      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;

        // 途中の各1マスが最短路方向でなければ、この方向は終了。
        if (!is_shortest_step(color, v, nv))
          break;

        v = nv;

        if ((int)board[v].size() + packet_size > 8)
          continue;

        int base_height = board[v].size();

        // まず長いジャンプを優先。
        // 同じ長さなら、次のジャンプ力を得やすい高い台を優先。
        if (l > best_length || (l == best_length && base_height > best_base_height) ||
            (l == best_length && base_height == best_base_height && v < best)) {
          best = v;
          best_length = l;
          best_base_height = base_height;
        }
      }
    }

    return best;
  }

  vector<int> shortest_next_cells(const Board &board, int color, int source, int packet_size) const {
    vector<int> result;

    for (int d = 0; d < 4; ++d) {
      int v = next_cell(source, d);
      if (v == -1)
        continue;
      if (!is_shortest_step(color, source, v))
        continue;
      result.push_back(v);
    }

    sort(result.begin(), result.end(), [&](int a, int b) {
      bool fit_a = (int)board[a].size() + packet_size <= 8;
      bool fit_b = (int)board[b].size() + packet_size <= 8;
      if (fit_a != fit_b)
        return fit_a > fit_b;

      int ha = board[a].size();
      int hb = board[b].size();
      if (ha != hb)
        return ha > hb;
      return a < b;
    });

    return result;
  }

  /**
   * packet_size 匹を論理的にマージせず、そのまま巣まで送る。
   *
   * flush の本体では packet_size=4 を使う。
   * 同色towerへ着地しても、次の操作で動かすのは上の packet_size 匹だけ。
   *
   * 前方の同色towerが容量不足なら、そのtowerの上4匹を先に flush して
   * 空きを作ってから元のpacketを再開する。
   */
  bool flush_packet_to_nest_core(Board &board, vector<Action> &actions, int color, int source, int packet_size) const {
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
    if (dist_to_nest[color][source] == INT_MAX)
      return false;

    int destination = find_flush_jump_destination(board, color, source, packet_size);

    if (destination != -1) {
      if (!execute_action(board, actions, source, packet_size, destination)) {
        return false;
      }

      if (destination == nest[color])
        return true;

      return flush_packet_to_nest_core(board, actions, color, destination, packet_size);
    }

    // 直線ジャンプができない場合も、Steiner treeには戻らず、
    // 最短距離が1減る隣接マスのどれかへ進む。
    vector<int> nexts = shortest_next_cells(board, color, source, packet_size);

    // そのままpacket全体を受けられる最短路隣接点を優先。
    for (int destination2 : nexts) {
      int capacity = 8 - (int)board[destination2].size();
      if (capacity < packet_size)
        continue;

      if (!execute_action(board, actions, source, packet_size, destination2)) {
        continue;
      }

      if (destination2 == nest[color])
        return true;

      return flush_packet_to_nest_core(board, actions, color, destination2, packet_size);
    }

    // すべて容量不足なら、最短路方向にある同色towerを先にflushして
    // 空きを作れる経路を試す。
    for (int destination2 : nexts) {
      if (board[destination2].empty())
        continue;
      if (top_color(board, destination2) != color)
        continue;

      int front_block = top_block_size(board, destination2);
      if (front_block <= 0)
        continue;

      int front_flush = min(strategy_flush_size, front_block);
      Board board_backup = board;
      size_t action_size_backup = actions.size();

      if (!flush_packet_to_nest_core(board, actions, color, destination2, front_flush)) {
        board = std::move(board_backup);
        actions.resize(action_size_backup);
        continue;
      }

      return flush_packet_to_nest_core(board, actions, color, source, packet_size);
    }

    return false;
  }

  enum class LocalMoveType : uint8_t {
    Flush,
    Setup,
  };

  struct LocalMove {
    LocalMoveType type = LocalMoveType::Setup;
    int source = -1;
    int destination = -1;
    int moved_size = 0;
  };

  struct LocalState {
    Board board{};
    vector<LocalMove> moves;

    // 論理的なflushの現在位置。
    int flush_pos = -1;
    bool flush_finished = false;

    // ここから最短路ベースの決定的flushを行った場合の残り手数。
    int finish_cost = INT_MAX;

    // moves.size() + finish_cost。
    int total_cost = INT_MAX;

    // 後続flushで再利用できるplatformの将来価値。
    int reuse_credit = 0;

    // total_cost - (初期盤面から増えたreuse_credit)。
    // 局所探索ではこの値も比較に使う。
    int effective_cost = INT_MAX;

    // beamの多様性維持用。大きいほど「将来platformになりそう」。
    int potential = 0;
  };

  /**
   * flush が今後通り得る「最短路領域」を列挙する。
   *
   * Steiner tree上の1本の経路ではなく、sourceから巣までの
   * いずれかの最短路に含まれ、かつsourceからHORIZON以内のセルを全て含む。
   */
  void build_flush_route_cache() {
    flush_route_cache.assign(P.K, vector<vector<int>>(V));
    flush_route_mask_cache.assign(P.K, vector<array<uint8_t, MAX_CELLS>>(V));

    for (int color = 0; color < P.K; ++color) {
      for (int source = 0; source < V; ++source) {
        auto &route = flush_route_cache[color][source];
        auto &mask = flush_route_mask_cache[color][source];
        mask.fill(0);

        if (dist_to_nest[color][source] == INT_MAX)
          continue;

        for (int v = 0; v < V; ++v) {
          if (v == source)
            continue;
          if (floor_dist[source][v] == INT_MAX)
            continue;
          if (floor_dist[source][v] > LOCAL_ROUTE_HORIZON)
            continue;
          if (!on_some_shortest_path(color, source, v))
            continue;

          route.push_back(v);
          mask[v] = 1;
        }

        sort(route.begin(), route.end(), [&](int a, int b) {
          int da = floor_dist[source][a];
          int db = floor_dist[source][b];
          if (da != db)
            return da < db;
          if (dist_to_nest[color][a] != dist_to_nest[color][b]) {
            return dist_to_nest[color][a] > dist_to_nest[color][b];
          }
          return a < b;
        });
      }
    }
  }

  const vector<int> &flush_route_cells(int color, int source) const {
    return flush_route_cache[color][source];
  }

  const array<uint8_t, MAX_CELLS> &flush_route_mask(int color, int source) const {
    return flush_route_mask_cache[color][source];
  }

  /**
   * setup対象色のGreedy Steiner treeで v からparent方向へ進んだとき、
   * flushの最短路領域へ初めて到達するまでの辺数。
   */
  int steps_to_route(int color, int v, const array<uint8_t, MAX_CELLS> &on_route, int max_steps) const {
    if (!trees[color].used[v])
      return -1;

    int cur = v;
    for (int step = 0; step <= max_steps; ++step) {
      if (on_route[cur])
        return step;

      int p = trees[color].parent[cur];
      if (p < 0)
        break;
      cur = p;
    }

    return -1;
  }

  /**
   * 現在の盤面で、platformが後続flushにも使われる見込みを概算する。
   *
   * platformが、残っているactive colorスライムの位置から巣への
   * いずれかの最短路上にあるなら、そのスライム数を需要として数える。
   * 4匹につきflushを1回とみなし、現在のflush1回分を除いた回数を
   * 「再利用回数」とする。
   */
  int estimated_extra_platform_uses(const Board &board, int active_color, int platform) const {
    (void)board;
    if (platform < 0 || platform >= V)
      return 0;

    int demand = shortest_path_demand[active_color][platform];
    return max(0, demand / strategy_flush_size - 1);
  }

  /**
   * 盤面上に残る再利用可能platformの将来価値。
   *
   * 他色の純粋tower(高さ1..4)だけをplatformとして数える。
   * 高いほど次のflushが大きく跳べるので、heightと再利用回数の積を
   * 「将来何手か節約できる可能性」の粗いcreditとして使う。
   */
  int reusable_platform_credit(const Board &board, int active_color) const {
    int credit = 0;

    // 今後発生し得るflushの総数で静的hintを上限する。
    int remaining = count_color(board, active_color);
    int future_use_cap = max(0, remaining / strategy_flush_size - 1);
    int platform_max_height = 8 - strategy_flush_size;
    if (future_use_cap == 0)
      return 0;

    for (int v = 0; v < V; ++v) {
      if (board[v].empty())
        continue;

      int color = top_color(board, v);
      if (color < 0 || color == active_color)
        continue;
      if (top_block_size(board, v) != (int)board[v].size())
        continue;

      int h = board[v].size();
      if (!(1 <= h && h <= platform_max_height))
        continue;

      int extra_uses = min(estimated_extra_platform_uses(board, active_color, v), future_use_cap);
      credit += extra_uses * h * REUSE_CREDIT_PER_LEVEL;
    }

    return credit;
  }

  /**
   * flush自身が次に行える操作を全て列挙する。
   * packet_size=4は論理的に固定し、同色towerへ着地しても
   * packetを大きくしない。
   */
  vector<LocalMove> generate_flush_moves(const Board &board, int color, int flush_pos) const {
    vector<LocalMove> result;

    if (flush_pos == nest[color])
      return result;
    if (board[flush_pos].empty())
      return result;
    if (top_color(board, flush_pos) != color)
      return result;
    if (top_block_size(board, flush_pos) < strategy_flush_size)
      return result;
    if (dist_to_nest[color][flush_pos] == INT_MAX)
      return result;

    int h = board[flush_pos].size();
    int k = h - strategy_flush_size;
    int max_jump = k + 1;

    struct Candidate {
      LocalMove move;
      int length = 0;
      int base_height = 0;
      int reuse = 0;
    };
    vector<Candidate> candidates;

    for (int d = 0; d < 4; ++d) {
      int v = flush_pos;

      for (int l = 1; l <= max_jump; ++l) {
        int nv = next_cell(v, d);
        if (nv == -1)
          break;
        if (!is_shortest_step(color, v, nv))
          break;

        v = nv;

        if ((int)board[v].size() + strategy_flush_size > 8)
          continue;

        int reuse = 0;
        if (!board[v].empty() && top_color(board, v) != color && top_block_size(board, v) == (int)board[v].size() &&
            (int)board[v].size() <= 8 - strategy_flush_size) {
          reuse = estimated_extra_platform_uses(board, color, v);
        }

        candidates.push_back({
            {LocalMoveType::Flush, flush_pos, v, strategy_flush_size},
            l,
            (int)board[v].size(),
            reuse,
        });
      }
    }

    // 直接の進行量を最優先し、同程度なら再利用価値のあるplatform着地を優先。
    sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
      if (a.length != b.length)
        return a.length > b.length;
      if (a.reuse != b.reuse)
        return a.reuse > b.reuse;
      if (a.base_height != b.base_height)
        return a.base_height > b.base_height;
      return a.move.destination < b.move.destination;
    });

    result.reserve(candidates.size());
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }

    return result;
  }

  /**
   * 他色をplatform候補へ運ぶ1手を列挙する。
   *
   * 以前は「flush最短路領域に隣接したスライムを1手で直接載せる」だけだった。
   * 今回は最大5手程度先読みするため、他色自身のGreedy Steiner treeを
   * parent方向へ1辺ずつ進める中間操作も探索対象にする。
   *
   * 条件:
   * ・active colorは動かさない
   * ・移動元は純粋な同色tower
   * ・移動先も空、または同じ色の純粋tower
   * ・setup中のtower高さは4以下
   * ・その色のparent列をたどると、残り探索手数以内にflush最短路領域へ到達する
   *
   * これにより、例えば経路から4マス離れた他色スライムも
   * 4手使ってplatform位置まで運べる。
   */
  vector<LocalMove> generate_setup_moves(const Board &board, int active_color, int flush_pos,
                                         int remaining_depth) const {
    vector<LocalMove> result;
    if (remaining_depth <= 0)
      return result;

    const auto &on_route = flush_route_mask(active_color, flush_pos);

    struct CandidateMove {
      LocalMove move;
      int route_dist = INT_MAX;
      int reaches_route = 0;
      int moved_size = 0;
    };
    vector<CandidateMove> candidates;

    for (int source = 0; source < V; ++source) {
      if (source == flush_pos)
        continue;
      if (board[source].empty())
        continue;

      int color = top_color(board, source);
      if (color < 0 || color == active_color)
        continue;

      // setupで扱うtowerは純粋towerだけ。
      if (top_block_size(board, source) != (int)board[source].size()) {
        continue;
      }

      if (!trees[color].used[source])
        continue;

      int destination = trees[color].parent[source];
      if (destination < 0)
        continue;
      if (destination == flush_pos)
        continue;

      // 自身の巣へ入れると消えてplatform材料にならない。
      if (destination == nest[color])
        continue;

      // sourceからrouteへ向かうparent列が、残り手数内に存在するものだけ。
      int source_dist = steps_to_route(color, source, on_route, remaining_depth);
      if (source_dist < 0)
        continue;

      int destination_dist = steps_to_route(color, destination, on_route, remaining_depth);

      // route上のtowerをroute外へ持ち出さない。
      // route外なら、必ず1歩routeへ近づく操作だけを許す。
      if (source_dist == 0) {
        if (destination_dist != 0)
          continue;
      } else {
        if (destination_dist != source_dist - 1)
          continue;
      }

      int destination_color = top_color(board, destination);

      // active colorを他色で覆わない。
      if (destination_color == active_color)
        continue;

      // 空マス、または同色の純粋towerにだけ積む。
      if (!board[destination].empty()) {
        if (destination_color != color)
          continue;
        if (top_block_size(board, destination) != (int)board[destination].size()) {
          continue;
        }
      }

      // flush packetを載せたとき高さ8以下になるよう、
      // setup towerは高さ (8 - flush_size) 以下に抑える。
      int capacity = (8 - strategy_flush_size) - (int)board[destination].size();
      if (capacity <= 0)
        continue;

      int moved_size = min((int)board[source].size(), capacity);
      if (moved_size <= 0)
        continue;

      CandidateMove candidate;
      candidate.move = {
          LocalMoveType::Setup,
          source,
          destination,
          moved_size,
      };
      candidate.route_dist = destination_dist;
      candidate.reaches_route = (destination_dist == 0);
      candidate.moved_size = moved_size;
      candidates.push_back(candidate);
    }

    // 分岐数を抑える。
    // まずrouteへ実際に到達する手、次にrouteまで近い手を優先。
    sort(candidates.begin(), candidates.end(), [](const CandidateMove &a, const CandidateMove &b) {
      if (a.reaches_route != b.reaches_route) {
        return a.reaches_route > b.reaches_route;
      }
      if (a.route_dist != b.route_dist) {
        return a.route_dist < b.route_dist;
      }
      if (a.moved_size != b.moved_size) {
        return a.moved_size > b.moved_size;
      }
      if (a.move.destination != b.move.destination) {
        return a.move.destination < b.move.destination;
      }
      return a.move.source < b.move.source;
    });

    if ((int)candidates.size() > LOCAL_SETUP_CANDIDATE_LIMIT) {
      candidates.resize(LOCAL_SETUP_CANDIDATE_LIMIT);
    }

    result.reserve(candidates.size());
    for (const auto &candidate : candidates) {
      result.push_back(candidate.move);
    }
    return result;
  }

  int evaluate_flush_cost(const Board &board, int color, int source, int packet_size) const {
    if (source == nest[color])
      return 0;

    Board copied = board;
    vector<Action> dummy;

    if (!flush_packet_to_nest_core(copied, dummy, color, source, packet_size)) {
      return INT_MAX;
    }

    return (int)dummy.size();
  }

  /**
   * beamのtie-break用。
   *
   * ・flush経路上に既に存在する高さ1..4の他色tower
   * ・数手以内にflush経路へ到達できる他色tower
   *
   * を高く評価する。
   * 「最初の数手ではまだflush手数が減らないが、数手後にplatformが完成する」
   * 状態をbeamから落としにくくするための値。
   */
  int platform_potential(const Board &board, int active_color, int flush_pos, int remaining_depth) const {
    if (flush_pos == nest[active_color])
      return 0;

    const auto &route = flush_route_cells(active_color, flush_pos);
    const auto &on_route = flush_route_mask(active_color, flush_pos);

    int score = 0;

    // すでに最短路領域上にあるplatform。
    for (int v : route) {
      if (board[v].empty())
        continue;

      int color = top_color(board, v);
      if (color == active_color)
        continue;
      if (top_block_size(board, v) != (int)board[v].size())
        continue;
      if ((int)board[v].size() > 8 - strategy_flush_size)
        continue;

      int h = board[v].size();
      int progress = floor_dist[flush_pos][v];
      int extra_uses = estimated_extra_platform_uses(board, active_color, v);

      score += 100 * h;
      score += max(0, 30 - progress);

      // 後続flushでも使えそうな台は強く残す。
      score += 160 * extra_uses * h;
    }

    if (remaining_depth > 0) {
      // まだrouteに到達していないが、数手でplatformになれるtower。
      for (int v = 0; v < V; ++v) {
        if (board[v].empty())
          continue;
        int color = top_color(board, v);
        if (color < 0 || color == active_color)
          continue;
        if (top_block_size(board, v) != (int)board[v].size())
          continue;
        if ((int)board[v].size() > 8 - strategy_flush_size)
          continue;
        if (on_route[v])
          continue;

        int dist = steps_to_route(color, v, on_route, remaining_depth);
        if (dist <= 0)
          continue;

        score += max(0, remaining_depth - dist + 1) * (int)board[v].size() * 4;
      }
    }

    // route以外も含め、後続flushで再利用されるplatform全体の価値を加える。
    score += 80 * reusable_platform_credit(board, active_color);

    return score;
  }

  uint64_t hash_local_state(const LocalState &state) const {
    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t x) {
      h ^= x + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
      h *= 1099511628211ULL;
    };

    mix((uint64_t)(state.flush_pos + 1));
    mix((uint64_t)state.flush_finished);

    for (int v = 0; v < V; ++v) {
      if (state.board[v].empty())
        continue;
      mix((uint64_t)(v + 1));
      mix((uint64_t)state.board[v].size());
      for (int i = 0; i < state.board[v].size(); ++i) {
        mix((uint64_t)(state.board[v][i] + 1));
      }
    }

    return h;
  }

  /**
   * 「setupを先に全部行ってからflush」ではなく、
   * flush操作と他色setup操作を同じ探索木に入れて最大5手程度先読みする。
   *
   * 例えば
   *   setup -> setup -> flush -> setup -> flush -> ...
   * のような途中挿入も探索できる。
   *
   * 各探索状態からは、最後に最短路ベースの決定的flushを行った場合の
   * 残り手数を正確に計算する。
   * したがって評価値は
   *
   *   ここまでの局所操作数 + 残りflush手数
   *
   * となる。
   */
  vector<LocalMove> search_platform_plan(const Board &board, int color, int source) const {
    int baseline = evaluate_flush_cost(board, color, source, strategy_flush_size);
    if (baseline == INT_MAX)
      return {};

    vector<LocalMove> best_plan;

    const int baseline_reuse_credit = reusable_platform_credit(board, color);
    int best_effective = baseline;
    int best_total = baseline;

    LocalState initial;
    initial.board = board;
    initial.flush_pos = source;
    initial.flush_finished = false;
    initial.finish_cost = baseline;
    initial.total_cost = baseline;
    initial.reuse_credit = baseline_reuse_credit;
    initial.effective_cost = baseline;
    initial.potential = platform_potential(board, color, source, LOCAL_SEARCH_DEPTH);

    vector<LocalState> beam;
    beam.push_back(std::move(initial));

    for (int depth = 0; depth < LOCAL_SEARCH_DEPTH; ++depth) {
      vector<LocalState> next;
      next.reserve(LOCAL_BEAM_WIDTH * 12);
      unordered_set<uint64_t> seen;
      seen.reserve(LOCAL_BEAM_WIDTH * 32);

      for (const auto &state : beam) {
        if (state.flush_finished)
          continue;

        vector<LocalMove> moves = generate_flush_moves(state.board, color, state.flush_pos);

        int remaining_after_this = LOCAL_SEARCH_DEPTH - (depth + 1);

        auto setup_moves = generate_setup_moves(state.board, color, state.flush_pos, remaining_after_this + 1);

        moves.insert(moves.end(), setup_moves.begin(), setup_moves.end());

        for (const auto &move : moves) {
          LocalState child;
          child.board = state.board;
          child.moves = state.moves;
          child.flush_pos = state.flush_pos;
          child.flush_finished = state.flush_finished;

          vector<Action> dummy;
          if (!execute_action(child.board, dummy, move.source, move.moved_size, move.destination)) {
            continue;
          }

          child.moves.push_back(move);

          if (move.type == LocalMoveType::Flush) {
            child.flush_pos = move.destination;
            if (move.destination == nest[color]) {
              child.flush_finished = true;
            }
          }

          // 同一depthでは moves.size() は全状態で同じ。
          // board / flush_pos / flush_finished が同じなら、以降の評価値と
          // 探索可能な遷移も同じなので、重い評価より先に重複排除する。
          uint64_t hash = hash_local_state(child);
          if (!seen.insert(hash).second)
            continue;

          if (child.flush_finished) {
            child.finish_cost = 0;
          } else {
            child.finish_cost = evaluate_flush_cost(child.board, color, child.flush_pos, strategy_flush_size);
          }

          if (child.finish_cost == INT_MAX) {
            child.total_cost = INT_MAX;
            child.reuse_credit = reusable_platform_credit(child.board, color);
            child.effective_cost = INT_MAX;
          } else {
            child.total_cost = (int)child.moves.size() + child.finish_cost;

            child.reuse_credit = reusable_platform_credit(child.board, color);

            int extra_reuse_credit = max(0, child.reuse_credit - baseline_reuse_credit);

            child.effective_cost = child.total_cost - extra_reuse_credit;

            // 現在のflushだけなら少し損でも、後続flushで再利用できる
            // platformを作れるなら採用候補にする。
            if (child.effective_cost < best_effective ||
                (child.effective_cost == best_effective && child.total_cost < best_total)) {
              best_effective = child.effective_cost;
              best_total = child.total_cost;
              best_plan = child.moves;
            }
          }

          child.potential = platform_potential(child.board, color, child.flush_pos, remaining_after_this);

          next.push_back(std::move(child));
        }
      }

      if (next.empty())
        break;

      // まず「現在すでに良い」状態を半分残す。
      sort(next.begin(), next.end(), [](const LocalState &a, const LocalState &b) {
        if (a.effective_cost != b.effective_cost) {
          return a.effective_cost < b.effective_cost;
        }
        if (a.total_cost != b.total_cost)
          return a.total_cost < b.total_cost;
        if (a.finish_cost != b.finish_cost)
          return a.finish_cost < b.finish_cost;
        if (a.reuse_credit != b.reuse_credit)
          return a.reuse_credit > b.reuse_credit;
        if (a.potential != b.potential)
          return a.potential > b.potential;
        return a.moves.size() < b.moves.size();
      });

      vector<LocalState> selected;
      selected.reserve(LOCAL_BEAM_WIDTH);
      unordered_set<uint64_t> selected_hash;
      selected_hash.reserve(LOCAL_BEAM_WIDTH * 2);

      int cost_quota = LOCAL_BEAM_WIDTH / 2;
      for (const auto &state : next) {
        if ((int)selected.size() >= cost_quota)
          break;
        uint64_t hash = hash_local_state(state);
        if (!selected_hash.insert(hash).second)
          continue;
        selected.push_back(state);
      }

      // 残り半分は、まだ直接得になっていなくてもplatform形成の
      // 見込みが大きい状態を残す。
      sort(next.begin(), next.end(), [](const LocalState &a, const LocalState &b) {
        if (a.potential != b.potential)
          return a.potential > b.potential;
        if (a.reuse_credit != b.reuse_credit)
          return a.reuse_credit > b.reuse_credit;
        if (a.effective_cost != b.effective_cost) {
          return a.effective_cost < b.effective_cost;
        }
        if (a.total_cost != b.total_cost)
          return a.total_cost < b.total_cost;
        if (a.finish_cost != b.finish_cost)
          return a.finish_cost < b.finish_cost;
        return a.moves.size() < b.moves.size();
      });

      for (const auto &state : next) {
        if ((int)selected.size() >= LOCAL_BEAM_WIDTH)
          break;
        uint64_t hash = hash_local_state(state);
        if (!selected_hash.insert(hash).second)
          continue;
        selected.push_back(state);
      }

      beam = std::move(selected);
    }

    return best_plan;
  }

  /**
   * flush packetをflushする。
   *
   * 最大5手程度、flush自身の移動と他色のsetupを混ぜて局所探索する。
   * 探索で得なprefixが見つかった場合はその操作列を実行し、
   * 残りは最短路ベースの決定的flushで巣まで送る。
   */
  bool flush_with_platform_search(Board &board, vector<Action> &actions, int color, int source) const {
    if (top_block_size(board, source) < strategy_flush_size)
      return false;

    vector<LocalMove> plan = search_platform_plan(board, color, source);
    int flush_pos = source;

    for (const auto &move : plan) {
      if (!execute_action(board, actions, move.source, move.moved_size, move.destination)) {
        return false;
      }

      if (move.type == LocalMoveType::Flush) {
        flush_pos = move.destination;
        if (flush_pos == nest[color]) {
          return true;
        }
      }
    }

    return flush_packet_to_nest_core(board, actions, color, flush_pos, strategy_flush_size);
  }

  /**
   * active color のtop blockを1段階進める。
   *
   * 基本はGreedy Steiner tree上を通常マージ。
   * tower全体が高さ8になったら、上X段だけをflushして
   * 下側をfill用の集積blockとして残す。
   *
   * また、前方同色towerへfull blockを載せられない場合は、
   * currentの一部をparentへ入れて高さ8までfillした後、
   * parentの上X段をflushする。
   */
  bool advance_one(Board &board, vector<Action> &actions, int color, int source) const {
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

    // 高さ8のtowerができたら、上X段を先に流す。
    // 下側（または下側の残り）は後続blockを受けるfill地点として残す。
    if ((int)board[source].size() == 8 && block_size >= strategy_flush_size) {
      return flush_with_platform_search(board, actions, color, source);
    }

    // full blockのまま進めるなら通常のSteiner集約を優先する。
    int jump_destination = find_jump_destination(board, color, source, block_size);

    if (jump_destination != -1) {
      return execute_action(board, actions, source, block_size, jump_destination);
    }

    int parent = trees[color].parent[source];
    if (parent < 0)
      return false;

    int capacity = 8 - (int)board[parent].size();

    // 同色の前方towerへfull blockが入らない。
    // currentから必要分をfillして高さ8を作り、上X段をflushする。
    if (!board[parent].empty() && top_color(board, parent) == color && capacity < block_size) {
      int need = capacity;

      if (need > 0) {
        if (!execute_action(board, actions, source, need, parent)) {
          return false;
        }
      }

      if ((int)board[parent].size() != 8)
        return false;
      if (top_block_size(board, parent) < strategy_flush_size)
        return false;

      return flush_with_platform_search(board, actions, color, parent);
    }

    if (capacity <= 0)
      return false;

    // 他色tower上では入る分だけ載せる。
    // 局所探索で作るplatformは高さ4以下なので、
    // 4匹blockなら最大高さ8のジャンプ台になる。
    int moved_size = min(block_size, capacity);
    if (moved_size <= 0)
      return false;

    return execute_action(board, actions, source, moved_size, parent);
  }

  // =========================================================
  // One color
  // =========================================================

  int deepest_active_cell(const Board &board, int color) const {
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

  bool solve_one_color(Board &board, vector<Action> &actions, int color) const {
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
    Board board = initial_board();
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

  void try_order(const vector<int> &order, int flush_size, set<vector<int>> &seen, long long &trial_count,
                 SimulationResult &strategy_best, SimulationResult &global_best) {
    if (!seen.insert(order).second)
      return;
    ++trial_count;

    strategy_flush_size = flush_size;
    auto result = simulate(order);
    if (!result.success)
      return;

    if (!strategy_best.success || result.actions.size() < strategy_best.actions.size()) {
      strategy_best = result;
    }

    if (!global_best.success || result.actions.size() < global_best.actions.size()) {
      global_best = std::move(result);
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
    constexpr array<int, 4> FLUSH_SIZES = {7, 6, 5, 4};

    SimulationResult best;
    array<SimulationResult, 4> strategy_best;
    array<set<vector<int>>, 4> seen;
    array<long long, 4> trial_count{};

    vector<vector<int>> seed_orders;

    vector<int> order(P.K);
    iota(order.begin(), order.end(), 0);
    seed_orders.push_back(order);

    reverse(order.begin(), order.end());
    seed_orders.push_back(order);

    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
      if (terminals[a].size() != terminals[b].size()) {
        return terminals[a].size() > terminals[b].size();
      }
      return a < b;
    });
    seed_orders.push_back(order);

    reverse(order.begin(), order.end());
    seed_orders.push_back(order);

    /**
     * まず4戦略すべてを少なくとも1回は試す。
     * これにより fill8/flush7,6,5,4 のどれかが未評価のまま
     * 時間切れになることを避ける。
     */
    for (int i = 0; i < 4; ++i) {
      try_order(seed_orders[0], FLUSH_SIZES[i], seen[i], trial_count[i], strategy_best[i], best);
    }

    // 残りの代表的な色順も、戦略間で公平にround-robinする。
    for (int seed = 1; seed < (int)seed_orders.size(); ++seed) {
      for (int i = 0; i < 4; ++i) {
        if (time_over())
          break;
        try_order(seed_orders[seed], FLUSH_SIZES[i], seen[i], trial_count[i], strategy_best[i], best);
      }
      if (time_over())
        break;
    }

    if (P.K >= 2) {
      const long long all_orders = permutation_count_cap(P.K, 1000000000LL);

      int strategy_index = 0;

      // 残り時間は4戦略をround-robinしながら色順探索に使う。
      while (!time_over()) {
        bool all_finished = true;
        for (int i = 0; i < 4; ++i) {
          if ((long long)seen[i].size() < all_orders) {
            all_finished = false;
            break;
          }
        }
        if (all_finished)
          break;

        int i = strategy_index;
        strategy_index = (strategy_index + 1) % 4;

        if ((long long)seen[i].size() == all_orders)
          continue;

        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end(), mt);

        try_order(order, FLUSH_SIZES[i], seen[i], trial_count[i], strategy_best[i], best);
      }
    }

    long long total_trials = 0;
    for (int i = 0; i < 4; ++i) {
      total_trials += trial_count[i];
      cerr << "fill8/flush" << FLUSH_SIZES[i] << ": ";
      if (strategy_best[i].success) {
        cerr << strategy_best[i].actions.size() << " actions";
      } else {
        cerr << "no complete solution";
      }
      cerr << ", trials=" << trial_count[i] << '\n';
    }
    cerr << "total trials=" << total_trials << '\n';

    if (!best.success) {
      // 念のため時間制限とは無関係に、flush4 + 基本色順を最後に試す。
      strategy_flush_size = 4;
      best = simulate(seed_orders[0]);
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
